//
// GreenSQL DB Connection class header.
//
// Copyright (c) 2007 GreenSQL.NET <stremovsky@gmail.com>
// License: GPL v2 (http://www.gnu.org/licenses/gpl.html)
//

#ifndef DBFW_CONNECTION_HPP
#define DBFW_CONNECTION_HPP

#include "buffer.hpp"
// #include "dbpermobj.hpp"
// #include "patterns.hpp"   // for SQLPatterns
#include "config.hpp"     //for DBProxyType

#include <list>
#include <string.h>
#include <ev.h>
#include <ev++.h>
// #include <sys/types.h>

class Connection
{
    public:
        Connection(int proxy_id);
        virtual ~Connection() {};
        bool close();
        // bool check_query(std::string & query);
        // virtual bool checkBlacklist(std::string & query, std::string & reason) = 0;
        // virtual bool parseRequest(std::string & req, bool & hasResponse ) = 0;
        // virtual bool parseResponse(std::string & response) = 0;
        // virtual bool blockResponse(std::string & response) = 0;
        // virtual SQLPatterns * getSQLPatterns() = 0;

        std::list<Connection*>::iterator location;
        std::list<Connection*> * connections;
        ev::io proxy_event;
        ev::io proxy_event_writer;
        ev::io backend_event;
        ev::io backend_event_writer;

        int iProxyId;    // the simplest method to transfer proxy id
        std::string db_srv_version;  /* version */
        std::string db_name;
        std::string db_new_name;
        std::string db_user;
        std::string db_type;
        std::string db_user_ip;
        // DBPermObj * db;
        DBProxyType dbType;
        std::string original_query;
        bool first_request;
        bool first_response;
        bool SecondPacket;

        Buffer request_in;
        Buffer request_out;
        Buffer response_in;
        Buffer response_out;

    private:
        // unsigned int calculateRisk(std::string & query, std::string &reason);
    
};

#endif
