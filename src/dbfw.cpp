//
// Database Firewall Proxy implementation
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "dbfw.hpp"
#include "connection.hpp"
#include "mysql/mysql_con.hpp"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>

static const int min_buf_size = 10240;
static void _disableEventWriter(Connection * conn, bool proxy);
static void _enableEventWriter(Connection * conn, bool proxy);
static bool _socketRead(int fd, char * data, int & size);
static bool _socketWrite(int fd, const char* data, int & size);

DBFW::DBFW()
{}

DBFW::~DBFW()
{}

bool DBFW::closeConnection()
{
    Connection * conn;
    while ( v_conn.size() != 0)
    {
        conn = v_conn.front();
        // we remove pointer to the conn object inside the close() function
        conn->close();
        delete conn;
    }
    v_conn.clear();

    _closeServer();
    return true;
}

void DBFW::_closeServer()
{
    return;
}

bool DBFW::proxyInit(int _proxy_id, std::string& _proxy_ip, int _proxy_port,
            std::string& _backend_name, std::string& _backend_ip,
            int _backend_port, std::string& _db_type)
{
    proxy_id     = _proxy_id;
    backend_name = _backend_name;
    backend_ip   = _backend_ip;
    backend_port = _backend_port;
    proxy_ip     = _proxy_ip;
    proxy_port   = _proxy_port;
    if (strcasecmp(_db_type.c_str(), "pgsql") == 0)
        db_type = DBTypePGSQL;
    else
        db_type = DBTypeMySQL;

    int sfd = _serverSocket(proxy_ip, proxy_port);

    if (sfd == -1)
        return false;

    // event_set(&serverEvent, sfd, EV_READ | EV_WRITE | EV_PERSIST,
    //           wrap_Server, (void *)iProxyId);
    // event_add(&serverEvent, 0);
    logEvent(V_DEBUG, "[%d] Server file descriptor socket initialized: sfd=%d\n", proxy_id, sfd);
    io.set<DBFW, &DBFW::ioAccept>(this);
    io.start(sfd, ev::READ|ev::WRITE);

    return true;
}

void DBFW::ioAccept(ev::io &watcher, int revents)
{
    if (EV_ERROR & revents) {
        logEvent(ERR, "Got invalid event. Firewall id [%d]\n", proxy_id);
        return;
    }

    DBFWConfig * conf = DBFWConfig::getInstance();
    Connection* conn = NULL;
    if (conf->server_running == false)
        closeConnection();

    int sfd = _socketAccept(watcher.fd);
    if (sfd == -1)
        return;

    int cfd = _clientSocket(backend_ip.empty() ? backend_name : backend_ip, backend_port);
    if (cfd == -1)
    {
        socketClose(sfd);
        logEvent(NET_DEBUG, "Failed to connect to backend db server (%s:%d)\n", backend_name.c_str(), backend_port);
        return;
    }

    logEvent(V_DEBUG, "client (to backend db server) connection established\n");

    if (db_type == DBTypeMySQL)
    {
        conn = new MySQLConnection(proxy_id);
    } // else if (db_type == DBTypePGSQL)
    // {
    //     conn = new PgSQLConnection(proxy_id);
    // }

    logEvent(V_DEBUG, "Database Firewall socket use: sfd=%d, cfd=%d\n", sfd, cfd);

    conn->proxy_event.set<DBFW, &DBFW::proxyCB>(this);
    conn->proxy_event_writer.set<DBFW, &DBFW::proxyCB>(this);
    conn->proxy_event.start(sfd, ev::READ);

    conn->backend_event.set<DBFW, &DBFW::backendCB>(this);
    conn->backend_event_writer.set<DBFW, &DBFW::backendCB>(this);
    conn->backend_event.start(cfd, ev::READ);

    logEvent(V_DEBUG, "size of the connection queue: %d\n", v_conn.size());

    v_conn.push_front(conn);

    conn->connections = &v_conn;
    conn->location = v_conn.begin();

    // get db user ip address
    struct sockaddr_storage ss;
    size_t len = sizeof(struct sockaddr_storage);

    getpeername(sfd,(struct sockaddr*)&ss, (socklen_t*)&len);
    if (ss.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&ss;
        conn->db_user_ip = inet_ntoa(s->sin_addr);
    } else if (ss.ss_family == AF_INET6) {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&ss;
        char ipstr[INET6_ADDRSTRLEN*2];
        inet_ntop(AF_INET6, (void *)&s->sin6_addr, ipstr, sizeof(ipstr));
        conn->db_user_ip = ipstr;
    }
}

void DBFW::proxyCB(ev::io &watcher, int revents)
{
    logEvent(VV_DEBUG, "Proxy callback\n");
    if (EV_ERROR & revents) {
        logEvent(ERR, "Got invalid event. Firewall id [%d]\n", proxy_id);
        return;
    }

    GreenSQLConfig * conf = GreenSQLConfig::getInstance();
    if (conf->bRunning == false)
        closeConnection();

    Connection * conn = _connSearchById(watcher.fd, true);
    if (revents & EV_WRITE) {
        logEvent(V_DEBUG, "Proxy callback event: WRITE\n");
        if (_proxyWriteCB(watcher, conn) == false) {
            conn->close();
            delete conn;
            return;
        }
    }

    if (!(revents & EV_READ))
        return;

    logEvent(V_DEBUG, "Proxy callback event: READ\n");

    char data[min_buf_size];
    int len = sizeof(data) - 1;

    if (_socketRead(watcher.fd, data, len) == false) {
        logEvent(NET_DEBUG, "[%d] Failed to read socket, closing socket\n", watcher.fd);
        conn->close();
        delete conn;
        return;
    }

    logEvent(V_DEBUG, "[%d] proxy socket read %d bytes\n", watcher.fd, len);
    if ( len > 0 ) {
        data[len] = '\0';
        //printf("received data\n");
        conn->request_in.append(data,len);
    
        //perform validation of the request
        if (_proxyValidateClientRequest(conn) == false) {
            conn->close();
            delete conn;
        }
    }
}
bool DBFW::_proxyWriteCB(ev::io &watcher, Connection * conn)
{
    int len = conn->response_out.size();
    logEvent(V_DEBUG, "[%d] proxy socket %d write %d byte(s)\n", proxy_id, watcher.fd, len);
    if (len == 0) {
        // we can clear the WRITE event flag
        _disableEventWriter(conn,true);
        // clear_init_event(conn,fd,EV_READ | EV_PERSIST,wrap_Proxy,(void *)conn);
        return true;
    }

    const unsigned char * data = conn->response_out.raw();

    if (_socketWrite(watcher.fd, (const char*) data, len) == true) {
        logEvent(V_DEBUG, "Send data to client, size: %d\n",len);
        conn->response_out.chop_back(len);
    } else {
        logEvent(NET_DEBUG, "[%d] Failed to send, closing socket %d\n", proxy_id, watcher.fd);
        // no need to close socket object here
        // it will be done in the caller function
        return false;
    }
    if (conn->response_out.size() == 0 ) {
        // we can clear the WRITE event flag
        _disableEventWriter(conn,true);
        // clear_init_event(conn,fd,EV_READ|EV_PERSIST,wrap_Proxy,(void *)conn);
    } else { // conn->response_out.size() > 0
        // we need to enable WRITE event flag
        _enableEventWriter(conn,true);
        // clear_init_event(conn,fd,EV_READ|EV_WRITE|EV_PERSIST,wrap_Proxy,(void *)conn);
    }
    return true;
}

void DBFW::backendCB(ev::io &watcher, int revents)
{
    logEvent(VV_DEBUG, "Backend callback");
    if (EV_ERROR & revents) {
        logEvent(ERR, "Got invalid event. Firewall id [%d]\n", proxy_id);
        return;
    }

    GreenSQLConfig * conf = GreenSQLConfig::getInstance();
    if (conf->bRunning == false)
        closeConnection();

    Connection * conn = _connSearchById(watcher.fd, false);

    // check if we can write to this socket
    if ( revents & EV_WRITE ) {
        logEvent(V_DEBUG, "Backend callback event: WRITE\n");
        if ( _backendWriteCB(watcher, conn) == false ) {
            // failed to write, close this socket
            conn->close();
            delete conn;
            return;
        }
    }

    if (!(revents & EV_READ))
        return;

    logEvent(V_DEBUG, "Backend callback event: READ\n");
    
    char data[min_buf_size];
    int len = sizeof(data)-1;

    if (_socketRead(watcher.fd, data, len) == false) {
        conn->close();
        delete conn;
        return;
    }

    if ( len > 0 ) {
        data[len] = '\0';
        //printf("received data\n");
        conn->response_in.append(data,len);

        if (_proxyValidateServerResponse(conn) == false) {
            conn->close();
            delete conn;
        }
    }

    return;
}

bool DBFW::_backendWriteCB(ev::io &watcher, Connection * conn)
{
    int len = conn->request_out.size();

    if (len == 0) {
        logEvent(NET_DEBUG, "[%d] backend socket write %d bytes\n", watcher.fd, len);
        _disableEventWriter(conn,false);            
        // clear_init_event(conn,fd,EV_READ | EV_PERSIST,wrap_Backend,(void *)conn,false);
        return true;
    }

    const unsigned char * data = conn->request_out.raw();

    if (_socketWrite(watcher.fd, (const char *)data, len) == true) {
        logEvent(NET_DEBUG, "sending data to backend server\n");
        conn->request_out.chop_back(len);
    } else {
        if(conn->first_response) {
            _enableEventWriter(conn,false);
            return true;
        }
        logEvent(NET_DEBUG, "[%d] Failed to send data to client, closing socket\n", watcher.fd);
        // no need to close connection here
        return false;
    }
    if (conn->request_out.size() == 0) {
        // we can clear the WRITE event flag
        _disableEventWriter(conn,false);
        // clear_init_event(conn,fd,EV_READ | EV_PERSIST,wrap_Backend,(void *)conn,false);
    } else { // conn->request_out.size() > 0
        // we need to enable WRITE event flag
        _enableEventWriter(conn,false);
        // clear_init_event(conn,fd,EV_READ | EV_WRITE | EV_PERSIST,wrap_Backend,(void *)conn,false);
    }

    return true;
}

int DBFW::_newSocket() {
    int sfd;

    if ((sfd = (int)socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        logEvent(NET_DEBUG, "Failed to create socket\n");
        return -1;
    }

    int flags;
    if ((flags = fcntl(sfd, F_GETFL, 0)) < 0 ||
         fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        logEvent(NET_DEBUG, "[%d] Failed to swith socket to non-blocking mode\n", sfd);
        socketClose(sfd);
        return -1;
    }

    return sfd;
}

int DBFW::_serverSocket(std::string & ip, int port)
{
    int sfd;
    
    struct sockaddr_in addr;
    int flags =1;
    struct linger ling = {0, 0};

    if ((sfd = _newSocket()) == -1) {
        return -1;
    }

    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
    setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        logEvent(NET_DEBUG, "Failed to bind server socket on %s:%d\n", ip.c_str(), port);
        socketClose(sfd);
        return -1;
    }
    
    if (listen(sfd, 1024) < 0) {
        logEvent(NET_DEBUG, "Failed to switch socket to listener mode\n");
        socketClose(sfd);
        return -1;
    }

    return sfd;
}

int DBFW::_clientSocket(std::string & server, int port)
{
    int sfd;
    struct sockaddr_in addr;
    int flags =1;
    struct linger ling = {0, 0};

    if ((sfd = _newSocket()) == -1) {
        return -1;
    }
    setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(server.c_str());
      
    if (connect(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        logEvent(NET_DEBUG, "[%d] Failed to connect to backend server\n", sfd);
        socketClose(sfd);
        return -1;
    }
    return sfd;
}

int DBFW::_socketAccept(int serverfd)
{
    socklen_t addrlen;
    struct sockaddr addr;
    int sfd;
    int flags = 1;
    struct linger ling = {0, 0};

    memset(&addr, 0, sizeof(addr));
    addrlen = sizeof(addr);

    if ((sfd = (int)accept(serverfd, &addr, &addrlen)) == -1) {
        logEvent(NET_DEBUG, "[%d] Failed to accept client socket\n", serverfd);
        socketClose(sfd);
        return -1;
    }

    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
    setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
  
    if ((flags = fcntl(sfd, F_GETFL, 0)) < 0 ||
        fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        logEvent(NET_DEBUG, "[%d] Failed to swith socket to non-blocking mode\n", sfd);
        socketClose(sfd);
        return -1;
    }

    return sfd;
}

bool DBFW::_proxyValidateClientRequest(Connection * conn)
{
    // std::string request = "";
    // bool hasResponse = false;
    // request.reserve(min_buf_size);
    // int len = 0;

    // // mysql_validate(request);
    // if (conn->parseRequest(request, hasResponse) == false)
    // {
    //     logevent(NET_DEBUG, "Failed to parse request, closing socket.\n");
    //     return false;
    // }

    // len = (int)request.size();

    // if (hasResponse == true)
    // {
    //     // we can push response to response_in or response_out queues
    //     if (conn->response_in.size() != 0)
    //     {
    //         if (ProxyValidateServerResponse(conn) == false)
    //         {
    //             return false;
    //         }
    //     } else if (conn->response_out.size() != 0)
    //     {
    //         Proxy_write_cb( conn->proxy_event.ev_fd, conn);
    //     }
    // }
    // if (len <= 0)
    //     return true;

    //push request
    // conn->request_out.append(request.c_str(), (int)request.size());
    conn->request_out.append((const char *) conn->request_in.raw(),
        (int) conn->request_in.size());

    return _backendWriteCB(conn->backend_event, conn);
}

bool DBFW::_proxyValidateServerResponse(Connection * conn)
{
    // std::string response;
    // response.reserve(min_buf_size);

    // conn->parseResponse(response);
    
    // //push response
    // if (response.size() == 0)
    // {
    //     return true;
    // }
    conn->response_out.append((const char *) conn->response_in.raw(), (int)conn->response_in.size());
    // if an error occurred while sending data, this socket will be closed.
    return _proxyWriteCB(conn->proxy_event, conn);
}

bool _socketRead(int fd, char * data, int & size)
{
    if ((size = recv(fd, data, size, 0)) < 0)
    {
        size = 0;
        logEvent(NET_DEBUG, "[%d] Socket read error\n", fd);
// #ifndef WIN32
//         if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
//            return true;
//         }
// #endif
        return false;
    }
    if (size == 0)
    {
       logEvent(NET_DEBUG, "[%d] Socket read error\n", fd);
       return false;
    }
                   
    return true; 
}

bool _socketWrite(int fd, const char* data, int & size)
{
    logEvent(NET_DEBUG, "Socket_write\n");
    if ((size = send(fd, data, size, 0))  <= 0)
    {
        logEvent(NET_DEBUG, "[%d] Socket write error\n", fd);
        return false;
    }

    return true;  
}

void _disableEventWriter(Connection * conn, bool proxy)
{
    ev::io *writer = proxy ? &conn->proxy_event_writer : &conn->backend_event_writer;
    bool del_event = writer->fd != 0 && writer->fd != -1;
    
    if (del_event) {
        writer->stop();
        logEvent(NET_DEBUG, "No data clear write event, fd: %d\n", writer->fd);
    }
}

void _enableEventWriter(Connection * conn, bool proxy)
{
    ev::io *writer = proxy ? &conn->proxy_event_writer : &conn->backend_event_writer;
    bool add_event = writer->fd != 0 && writer->fd != -1;

    if (add_event) {
        writer->start();
        logEvent(NET_DEBUG, "Try again add write event, fd: %d\n", writer->fd);
    }
}

int DBFW::socketClose(int socketfd)
{
    close(socketfd);

    return 0;
}

Connection * DBFW::_connSearchById(int fd, bool is_server)
{
    Connection * ret_val = NULL;

    std::list<Connection*>::iterator iter = v_conn.begin();
    while(iter != v_conn.end()) {
        ret_val = *iter;
        if(is_server == true) {
            if (ret_val->proxy_event.fd == fd)
                break;
            else
                ret_val = NULL;
        } else {
            if (ret_val->backend_event.fd == fd)
                break;
            else
                ret_val = NULL;
        }
        ++iter;
    }

    return ret_val;
}
