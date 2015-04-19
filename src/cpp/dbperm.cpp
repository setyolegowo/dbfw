//
// Database Permission class implementation
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "dbperm.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "buffer.hpp"
#include "config.hpp"
#include "log.hpp"

static int _parseToInt(int start, const char * buff, int size, int * use_char);
static void _parseToString(int start, const char * buff, int size, int * use_char, std::string& output);

DBPerm::DBPerm()
{
    error_result = true;
    attr_list.clear();
    mask_map.clear();
}

DBPerm::~DBPerm()
{
    attr_list.clear();
    mask_map.clear();
}

bool DBPerm::addAttr(std::string& attr)
{
    attr_list.push_back(attr);
    return true;
}

bool DBPerm::checkout(std::string& subject, std::string& action, std::string& uri)
{
    Buffer buf;
    std::string temp = " ";
    buf.append(subject);
    buf.append(temp.c_str(), 1);
    buf.append(action);
    buf.append(temp.c_str(), 1);
    buf.append(uri);
    for (std::vector<std::string>::iterator it = attr_list.begin(); it != attr_list.end(); ++it) {
        buf.append(temp.c_str(), 1);
        buf.append(*it);
    }
    return _connect(buf);
}

bool DBPerm::oneCheckPermission(std::string& subject, std::string& action, std::string& uri)
{
    Buffer buf;
    std::string temp = " ";
    buf.append(subject);
    buf.append(temp.c_str(), 1);
    buf.append(action);
    buf.append(temp.c_str(), 1);
    buf.append(uri);
    return _connect(buf);
}

bool DBPerm::_connect(Buffer& buff)
{
    DBFWConfig * cfg = DBFWConfig::getInstance();
    int sockfd = 0, n = 0;
    char _buff[1024];
    struct sockaddr_in serv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logEvent(NET_DEBUG, "Could not create socket in DBPerm\n");
        return false;
    }

    memset(_buff, 0, sizeof(_buff));
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(cfg->perm_port);
    // serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, cfg->perm_host.c_str(), &serv_addr.sin_addr)<=0) {
    // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        logEvent(NET_DEBUG, "inet_pton error occured in DBPerm\n");
        return false;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
            logEvent(NET_DEBUG, "Connect to backend DBPerm %d\n", sockfd);
        } else if (errno == EMFILE) {
            logEvent(NET_DEBUG, "Failed to connect to DBPerm %d, too many open sockets\n", sockfd);
            return false;
        } else {
            logEvent(NET_DEBUG, "Failed to connect to DBPerm (%s:%d), sockfd=%d\n",
                cfg->perm_host.c_str(), cfg->perm_port, sockfd);
            return false;
        }
    }

    if ((n = write(sockfd, buff.raw(), buff.size())) <= 0) {
        logEvent(NET_DEBUG, "[%d] Socket write error %d in DBPerm\n", sockfd, errno);
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS)
            n = 0;
        else
            return false;
    }

    if((n = read(sockfd, _buff, sizeof(_buff)-1)) > 0) {
        _buff[n] = 0;
        logHex(VV_DEBUG, (unsigned char*) _buff, n);
        _parsingResult(_buff, n);
    } else if(n < 0) {
        logEvent(NET_DEBUG, "Socket %d read error, errno=%d in DBPerm\n", sockfd, errno);
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
           return true;
        }
        return false;
    } else {
        logEvent(NET_DEBUG, "Get 0 size in DBPerm\n");
    }

    close(sockfd);

    return true;
}
int DBPerm::getResult()
{
    if(mask_map.size() == 1) {
        return mask_map.begin()->second;
    }
    return 2;
}

bool DBPerm::_parsingResult(const char * buff, int size)
{
    if(buff[0] == '0' || size < 4) {
        error_result = true;
    } else { // buff[0] == '1'
        int it = 2;
        int tmp;
        std::string tmp_sresult = "";
        int tmp_iresult;
        mask_map.clear();
        int total = _parseToInt(it, buff, size, &tmp);
        it += tmp;
        for(int i = 0; i < total; i++) {
            tmp_iresult = _parseToInt(it, buff, size, &tmp);
            it += tmp;
            _parseToString(it, buff, size, &tmp, tmp_sresult);
            it += tmp;
            mask_map.insert(std::pair<std::string, unsigned char>(tmp_sresult, tmp_iresult));
        }
        error_result = false;
    }
    return true;
}

int _parseToInt(int start, const char * buff, int size, int * use_char)
{
    int tmp = 0;
    *use_char = 0;
    for(int i = start; i < size; i++) {
        if(buff[i] >= '0' && buff[i] <= '9')
            tmp = (tmp*10) + (buff[i] - '0');
        else
            break;
        *use_char = *use_char + 1;
    }
    *use_char = *use_char + 1;
    return tmp;
}

void _parseToString(int start, const char * buff, int size, int * use_char, std::string& output)
{
    *use_char = 0;
    output = "";
    for(int i = start; i < size; i++) {
        if((buff[i] >= 'A' && buff[i] <= 'Z') || (buff[i] >= 'a' && buff[i] <= 'z') || buff[i] == '_')
            output.append(1, buff[i]);
        else
            break;
        *use_char = *use_char + 1;
    }
    *use_char = *use_char + 1;
}
