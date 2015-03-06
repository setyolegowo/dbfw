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
    // logevent(NET_DEBUG, "connection close(), proxy socket %d, backend socket %d\n", 
    // 	proxy_event.ev_fd, backend_event.ev_fd);
    
    // DBFW::socketClose(proxy_event.ev_fd);
    // DBFW::socketClose(backend_event.ev_fd);

    // if (proxy_event.ev_fd != 0 && proxy_event.ev_fd != -1 && proxy_event.ev_flags & EVLIST_INIT)
    //     event_del(&proxy_event);
    // if (proxy_event_writer.ev_fd != 0 && proxy_event_writer.ev_fd != -1 &&
    //             proxy_event_writer.ev_flags & EVLIST_INIT && proxy_event_writer.ev_flags & EVLIST_INSERTED)
    //     event_del(&proxy_event_writer);

    // if (backend_event.ev_fd != 0 && backend_event.ev_fd != -1 && backend_event.ev_flags & EVLIST_INIT)
    //     event_del(&backend_event);
    // if (backend_event_writer.ev_fd != 0 && backend_event_writer.ev_fd != -1 &&
    //             backend_event_writer.ev_flags & EVLIST_INIT && backend_event_writer.ev_flags & EVLIST_INSERTED )
    //     event_del(&backend_event_writer);
    // connections->erase(location);

    return true;
}
