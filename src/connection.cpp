//
// GreenSQL DB Connection class implementation.
//
// Copyright (c) 2007 GreenSQL.NET <stremovsky@gmail.com>
// License: GPL v2 (http://www.gnu.org/licenses/gpl.html)
//

#include "connection.hpp"

#include "dbfw.hpp"
#include "log.hpp"
#include "misc.hpp"
// #include "normalization.hpp"
// #include "riskengine.hpp"
// #include "alert.hpp"
// #include "parser/parser.hpp"     // for query_risk struct
// #include "dbmap.hpp"

Connection::Connection(int proxy_id)
{
	logEvent(NET_DEBUG, "connection init()\n");

    iProxyId       = proxy_id;
    first_request  = true;
    // db             = NULL;
    db_name        = "";
    db_new_name    = "";
    db_user        = "";
    db_type        = "";
    db_user_ip     = "";
	SecondPacket   = false;
	first_response = true;
}

bool Connection::close()
{
    logEvent(NET_DEBUG, "connection close(), proxy socket %d, backend socket %d\n", 
    	proxy_event.fd, backend_event.fd);
    
    DBFW::socketClose(proxy_event.fd);
    DBFW::socketClose(backend_event.fd);

    if (proxy_event.fd >0)
        proxy_event.stop();
    if (proxy_event_writer.fd > 0)
        proxy_event_writer.stop();

    if (backend_event.fd > 0)
        backend_event.stop();
    if (backend_event_writer.fd > 0)
        backend_event_writer.stop();
    connections->erase(location);

    return true;
}
