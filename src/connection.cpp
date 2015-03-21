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
#include "normalization.hpp"
// #include "riskengine.hpp"
// #include "alert.hpp"
// #include "parser/parser.hpp"     // for query_risk struct
// #include "dbmap.hpp"

Connection::Connection(int proxy_id)
{
	logEvent(NET_DEBUG, "[%d] Connection init\n", proxy_id);

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
    logEvent(NET_DEBUG, "[%d] Connection close(), proxy socket %d, backend socket %d\n", 
    	iProxyId, proxy_event.fd, backend_event.fd);
    
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

bool Connection::check_query(std::string & query)
{
    //return true;
    // DBFWConfig * conf = DBFWConfig::getInstance();
    std::string original_query = query;
    // std::string reason = "";

    // convert query to lower case
    str_lowercase(query);
    // perform normalization - make a pattern of the query
    normalizeQuery( (DBProxyType) dbType, query);
    // we will make the reference out of query
    // std::string & pattern = query;

    logEvent(SQL_DEBUG, "AFTER NORM   : %s\n", query.c_str());
    logEvent(VV_DEBUG, "NOTHING TODO\n");

    // bool ret = false;
    // bool privileged_operation = false;
    // int risk = 0;

    // Optimization Changes
    // In case we are in automatic block of the new commands
    // we do not need to calculate query risk if the query
    // pattern is known and it is in the whitelist.
    // This must be 99% of the cases (after learning mode is over).

    // int in_whitelist = 0;

 //    in_whitelist = db->CheckWhitelist(pattern);
 //    if ( in_whitelist )
 //    {
 //        logEvent(SQL_DEBUG, "Found in Exception List.\n");
 //        return true;
 //    }

 //    if ( (ret = checkBlacklist(pattern, reason)) == true)
 //    {
 //         privileged_operation = true;
	//  logEvent(SQL_DEBUG, "FORBIDEN     : %s\n", pattern.c_str());
 //    }
 //    // check if we find anything interesting
 //    risk = calculateRisk(original_query, reason);
 //    logEvent(SQL_DEBUG, "RISK         : %d\n", risk);

 //    DBBlockStatus block_status = db->GetBlockStatus();

 //    if ( block_status == ALWAYS_BLOCK_NEW )
 //    {
 //        reason += "Query blocked because it is not in whitelist.\n";
 //        logEvent(DEBUG, "Query blocked because it is not in whitelist.\n");
 //        logalert(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                 pattern, reason, risk, (int)BLOCKED);
 //        // block this query
 //        return false;
 //    }

 //    if (privileged_operation)
 //    {
	// risk = conf->re_block_level+1;
 //        if (block_status == PRIVILEGE_BLOCK || block_status == RISK_BLOCK)
 //        {
 //            logalert(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                     pattern, reason, risk, (int)BLOCKED);
 //            // block this query
 //            return false;
	// } else if (block_status == LEARNING_MODE ||
	// 	   block_status == LEARNING_MODE_3DAYS ||
	// 	   block_status == LEARNING_MODE_7DAYS)
	// {
	//     db->AddToWhitelist(db_user, pattern);
 //            logwhitelist(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                         pattern, reason, risk, (int)HIGH_RISK);
	//     return true;
 //        } else {
 //            // block_status == RISK_SIMULATION 
 //            // block_status == LEARNING_MODE
 //            logalert(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                     pattern, reason, risk, (int)HIGH_RISK);
 //            return true;
 //        }
 //    }

 //    if (block_status == LEARNING_MODE ||
 //        block_status == LEARNING_MODE_3DAYS ||
 //        block_status == LEARNING_MODE_7DAYS)
 //    {
 //        db->AddToWhitelist(db_user, pattern);
 //        if (risk >= conf->re_block_level)
 //        {
 //            logwhitelist(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                     pattern, reason, risk, (int)HIGH_RISK);
 //        } else if (risk >= conf->re_warn_level)
 //        {
 //            logwhitelist(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                     pattern, reason, risk, (int)WARN);
 //        } else {
 //            logwhitelist(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                     pattern, reason, risk, (int)LOW);
	// }

 //        return true;
 //    }

 //    DBBlockLevel risk_block_level = HIGH_RISK;
 //    if (block_status == RISK_BLOCK)
 //    {
 //        risk_block_level = BLOCKED;
 //    }

 //    if (risk >= conf->re_block_level) 
 //    {
 //        logalert(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                 pattern, reason, risk, (int)risk_block_level);
 //        if (risk_block_level == BLOCKED)
 //            return false;
 //    } 
 //    else if (risk >= conf->re_warn_level)
 //    {
 //        //warn level
 //        logalert(iProxyId, db_name, db_user, db_user_ip, original_query,
 //                 pattern, reason, risk, (int)WARN);
 //    }

    return true;
}
