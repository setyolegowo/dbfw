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
    
    // CHECK if risk engine configuration said use checking
    if(!conf->re_active) return true;
    
    removeUnreadableChar(query);
    std::string original_query = query;
    std::string reason = "";

    // convert query to lower case
    str_lowercase(query);
    // perform normalization - make a pattern of the query
    normalizeQuery( (DBProxyType) dbType, query);
    logSQL  (CRIT, "[%d] %s\n", iProxyId, original_query.c_str());
    logEvent(SQL_DEBUG, "[%d] Check Query: AFTER NORM   : %s\n", iProxyId, query.c_str());

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
    if(sql_action.size() == 0) {
        sql_action.assign("unknown");
    }

    if(conf->re_perm_engine) {
        DBPerm perm;
        std::string uri_resource = itoa(iProxyId);
        if(sql_tabel.size() > 0) {
            perm.addAttr(sql_tabel);
            uri_resource.append("/");
            uri_resource.append(db_name);
        } else {
            perm.addAttr(db_name);
        }
        perm.checkout(db_user, sql_action, uri_resource);
        if(perm.error_result) {
            logRisk(ERR, "[%d] Permission ERROR.  DB:%s, ACTION:%s, RESOURCE:%s\n",
                iProxyId, db_user.c_str(), sql_action.c_str(), uri_resource.c_str());
        } else {
            int get_result = perm.getResult();
            if(get_result > 0) {
                if(get_result == 1) {
                    logRisk(INFO, "[%d] Permission DENIED. DB:%s, ACTION:%s, RESOURCE:%s\n",
                        iProxyId, db_user.c_str(), sql_action.c_str(), uri_resource.c_str());
                    return false;
                } else {
                    logRisk(INFO, "[%d] ELSE Permission.   DB:%s, ACTION:%s, RESOURCE:%s\n",
                        iProxyId, db_user.c_str(), sql_action.c_str(), uri_resource.c_str());
                }
            } else {
                logRisk(DEBUG, "[%d] Permission PERMIT. DB:%s, ACTION:%s, RESOURCE:%s\n",
                    iProxyId, db_user.c_str(), sql_action.c_str(), uri_resource.c_str());
            }
        }
    }

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

    if (conf->re_sql_comments > 0 && risk.has_comment > 0) {
        reason += "Query has comments\n";
        logEvent(V_DEBUG, "[*][%d] Query has comments (%d)\n", iProxyId, risk.has_comment);
        ret += (conf->re_sql_comments*risk.has_comment);
    }
    if (conf->re_s_tables > 0 && risk.has_s_table > 0) {
        reason += "Query uses sensitive tables\n";
        logEvent(V_DEBUG, "[*][%d] Query uses sensitive tables (%d)\n", iProxyId, risk.has_s_table);
        ret += (conf->re_s_tables*risk.has_s_table);
    }
    if (conf->re_multiple_queries > 0 && risk.has_separator > 0) {
        reason += "Multiple queries found\n";
        logEvent(V_DEBUG, "[*][%d] Multiple queries found (%d)\n", iProxyId, risk.has_separator);
        ret += (conf->re_multiple_queries*risk.has_separator);
    }
    if (conf->re_or_token > 0 && risk.has_or > 0) {
        reason += "Query has 'or' token\n";
        logEvent(V_DEBUG, "[*][%d] Query has 'or' token (%d)\n", iProxyId, risk.has_or);
        ret += (conf->re_or_token*risk.has_or);
    }
    if (conf->re_union_token > 0 && risk.has_union > 0) {
        reason += "Query has 'union' token\n";
        logEvent(V_DEBUG, "[*][%d] Query has 'union' token (%d)\n", iProxyId, risk.has_union);
        ret += (conf->re_union_token*risk.has_union);
    }
    if (conf->re_var_cmp_var > 0 && risk.has_tautology > 0) {
        reason += "True expression detected (SQL tautology)\n";
        logEvent(V_DEBUG, "[*][%d] True expression detected (SQL tautology) (%d)\n", iProxyId,
            risk.has_tautology);
        ret += (conf->re_var_cmp_var*risk.has_tautology);
    }
    if (conf->re_empty_password > 0 && risk.has_empty_pwd > 0) {
        reason += "Query has empty password expression\n";
        logEvent(V_DEBUG, "[*][%d] Query has empty password expression (%d)\n", iProxyId,
            risk.has_empty_pwd);
        ret += (conf->re_empty_password*risk.has_empty_pwd);
    }
    if (conf->re_bruteforce > 0 && risk.has_bruteforce_function > 0) {
        reason += "Query has SQL fuction that can be used to bruteforce db contents\n";
        logEvent(V_DEBUG, "[*][%d] Query has SQL fuction that can be used to bruteforce db contents (%d)\n",
            iProxyId, risk.has_bruteforce_function);
        ret += (conf->re_bruteforce*risk.has_bruteforce_function);
    }

    return ret;
}
