//
// Database Firewall Proxy class header
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#ifndef DBFW_FIREWALL_HPP
#define DBFW_FIREWALL_HPP

#include <ev.h>
#include <ev++.h>

#include "connection.hpp"
#include "log.hpp"
#include "config.hpp"

class DBFW {
    public:
        DBFW();
        virtual ~DBFW();

        static void signal_cb(ev::sig &signal, int revents) { signal.loop.break_loop(); }

        bool proxyInit(int /*_proxy_id*/, std::string& /*_proxy_ip*/, int /*_proxy_port*/,
            std::string& /*_backend_name*/, std::string& /*_backend_ip*/,
            int /*_backend_port*/, std::string& /*_db_type*/);
        void proxyCB(ev::io& /*watcher*/, int /*revents*/);
        void backendCB(ev::io& /*watcher*/, int /*revents*/);

        bool closeConnection();
        static int socketClose(int /*socketfd*/);
        void ioAccept(ev::io& /*watcher*/, int /*revents*/);

        ev::io      io;
        ev::sig     sio;
        std::list<Connection*> v_conn;

        int         proxy_id;
        std::string backend_name;
        std::string backend_ip;
        int         backend_port;
        std::string proxy_ip;
        int         proxy_port;
        DBProxyType db_type;
        
    private:
        Connection * _connSearchById(int /*fd*/, bool /*is_server*/);
        void _closeServer();
        // bool _serverInitialized();
        bool _proxyWriteCB(ev::io& /*watcher*/, Connection* /*conn*/);
        bool _backendWriteCB(ev::io& /*watcher*/, Connection* /*conn*/);
        bool _proxyValidateClientRequest(Connection * conn);
        bool _proxyValidateServerResponse(Connection * conn);

        //socket
        int _serverSocket(std::string& /*ip*/, int /*port*/);
        int _clientSocket(std::string& /*server*/, int /*port*/);
        int _socketAccept(int /*serverfd*/);
        int _newSocket();
};

#endif /* DBFW_FIREWALL_HPP */
