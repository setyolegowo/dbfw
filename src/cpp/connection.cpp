//
// DBFW Connection class implementation
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "connection.hpp"

#include "dbfw.hpp"
#include "log.hpp"
#include "misc.hpp"
#include "normalization.hpp"
// #include "alert.hpp"
#include "parser/parser.hpp"     // for query_risk struct
#include "dbperm.hpp"
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
    sql_tabel      = "";
    sql_action     = "";
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
    DBFWConfig * conf = DBFWConfig::getInstance();
    std::string original_query = query;
    std::string reason = "";

    // convert query to lower case
    str_lowercase(query);
    // perform normalization - make a pattern of the query
    normalizeQuery( (DBProxyType) dbType, query);
    logEvent(SQL_DEBUG, "AFTER NORM   : %s\n", query.c_str());

    // bool ret = false;
    int risk = 0;
    
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
 //  logEvent(SQL_DEBUG, "FORBIDEN     : %s\n", pattern.c_str());
 //    }
 //    // check if we find anything interesting
    risk = calculateRisk(original_query, reason);

    if(sql_action == "select") {
        DBPerm perm;
        perm.addAttr(sql_tabel);
        std::string uri_resource = itoa(iProxyId);
        uri_resource.append("/");
        uri_resource.append(db_name);
        perm.checkout(db_user, sql_action, uri_resource);
        if(perm.error_result) {
            logEvent(DEBUG, "Permission error\n");
        } else {
            if(perm.getResult() > 0) {
                if(perm.mask_map.size() == 1) {
                    if(perm.getResult() == 1) {
                        logEvent(VV_DEBUG, "Permission denied\n");
                        return false;
                    } else {
                        logEvent(VV_DEBUG, "Else permission\n");
                    }
                } else
                    logEvent(VV_DEBUG, "Unknown permission\n");
            } else {
                logEvent(VV_DEBUG, "Permission permit\n");
            }
        }
    }

    logEvent(SQL_DEBUG, "RISK         : %d\n", risk);
    if (risk >= conf->re_block_level) {
        logAlert(iProxyId, db_name, db_user, db_user_ip, original_query,
            query, reason, risk, 2);
        return false;
    } else if (risk >= conf->re_warn_level) {
        //warn level
        logAlert(iProxyId, db_name, db_user, db_user_ip, original_query,
            query, reason, risk, 1);
    }

    return true;
}

unsigned int Connection::calculateRisk(std::string & query, std::string & reason)
{
    DBFWConfig * conf = DBFWConfig::getInstance();
    unsigned int ret = 0;

    struct query_risk risk;
    query_parse(&risk, getSQLPatterns(), query.c_str(), sql_action, sql_tabel);

    if (conf->re_sql_comments > 0 && risk.has_comment == 1) {
        reason += "Query has comments\n";
        logEvent(DEBUG, "Query has comments\n");
        ret += conf->re_sql_comments;
    }
    if (conf->re_s_tables > 0 && risk.has_s_table == 1) {
        reason += "Query uses sensitive tables\n";
        logEvent(DEBUG, "Query uses sensitive tables\n");
        ret += conf->re_s_tables;
    }
    if (conf->re_multiple_queries > 0 && risk.has_separator == 1) {
        reason += "Multiple queries found\n";
        logEvent(DEBUG, "Multiple queries found\n");
        ret += conf->re_multiple_queries;
    }
    if (conf->re_or_token > 0 && risk.has_or == 1) {
        reason += "Query has 'or' token\n";
        logEvent(DEBUG, "Query has 'or' token\n");
        ret += conf->re_or_token;
    }
    if (conf->re_union_token > 0 && risk.has_union == 1) {
        reason += "Query has 'union' token\n";
        logEvent(DEBUG, "Query has 'union' token\n");
        ret += conf->re_union_token;
    }
    if (conf->re_var_cmp_var > 0 && risk.has_tautology == 1) {
        reason += "True expression detected (SQL tautology)\n";
        logEvent(DEBUG, "True expression detected (SQL tautology)\n");
        ret += conf->re_var_cmp_var;
    }
    if (conf->re_empty_password > 0 && risk.has_empty_pwd == 1) {
        reason += "Query has empty password expression\n";
        logEvent(DEBUG, "Query has empty password expression\n");
        ret += conf->re_empty_password;
    }
    if (conf->re_bruteforce > 0 && risk.has_bruteforce_function == 1) {
        reason += "Query has SQL fuction that can be used to bruteforce db contents\n";
        logEvent(DEBUG, "Query has SQL fuction that can be used to bruteforce db contents\n");
        ret += conf->re_bruteforce;
    }

    return ret;
}
