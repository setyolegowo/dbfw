//
// Database Firewall configuration class implementation.
//
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "config.hpp"

#include <iostream>
#include <fstream> //ifstream

#include "log.hpp"
#include "misc.hpp"

DBFWConfig* DBFWConfig::_obj = NULL;

DBFWConfig::DBFWConfig() {
    init();
}

DBFWConfig::~DBFWConfig() {
    // TODO Log destruction
}

DBFWConfig* DBFWConfig::getInstance() {
    if (_obj == NULL) {
        _obj = new DBFWConfig();
    }
    return _obj;
}

void DBFWConfig::free() {
    if(_obj != NULL) {
        // _obj->closeDb();
        delete _obj;
        _obj = NULL;
    }
}

void DBFWConfig::init()
{
    server_running      = true;
    
    re_active           = 1;
    re_perm_engine      = 1;
    re_block_level      = 30;
    re_warn_level       = 20;
    re_sql_comments     = 30;
    re_s_tables         = 10;
    re_or_token         =  5;
    re_union_token      =  5;
    re_var_cmp_var      = 30;
    re_always_true      = 30;
    re_empty_password   = 30;
    re_multiple_queries = 30;
    re_bruteforce       = 15;
    re_return_sql_error = 1;
    
    log_level           = 3;
    log_file            = "/var/log/dbfw.log";
    log_sql             = "";
    log_risk            = "";
    log_alert           = "";

    perm_port           = 5000;
    perm_host           = "127.0.0.1";
}

bool DBFWConfig::load()
{
    std::string cfg_file;
    cfg_file.reserve(1024);

    cfg_file = conf_path;
    cfg_file += "dbfw.conf";
    
    std::ifstream file(cfg_file.c_str());
    std::string line;
    std::string key="";
    std::string value= "";
    std::string section = "";
    line.reserve(1024);

    logEvent(DEBUG, "[*][Config] Loading config file: %s\n", cfg_file.c_str());
    if (!file.is_open()) {
        logEvent(CRIT, "[*][Config] Failed to load configuration file: %s", cfg_file.c_str());
        return false;
    }

    //load default settings
    init();

    while (std::getline(file, line))
    {
        //ignore empty lines
        if (line.length() == 0)
            continue;
        //ignore comments
        if (line[0] == '#' || line[0] == ';')
            continue;
        TrimStr(line);

        if (line[0] =='[') {
            size_t end_section = line.find(']');
            if (end_section == 1 || end_section == std::string::npos) {
                //check if we found end of section
                logEvent(DEBUG, "[*][Config] Failed to parse following config line: %s", line.c_str());
                return false;
            }
            section = line.substr(1, end_section-1);
            str_lowercase(section);
            continue;
        }

        // format:
        // key=value
        if (ParseConfLine(line, key, value) == false)
        {
            logEvent(CRIT, "[*][Config] Failed to parse following configuration line: %s", line.c_str());
            return false;
        }
        str_lowercase(key);
        logEvent(DEBUG, "[*] WHAT is \"%s\":\"%s\"\n", key.c_str(), value.c_str());
        
        if (section == "risk engine")   parse_re_setting  (key, value);
        else if (section == "logging")  parse_log_setting (key, value);
        else if (section == "dbperm")  parse_perm_setting (key, value);
        else 
            logEvent(DEBUG, "[*][Config] No section for this key-value configuration: \"%s\"", line.c_str());
    }

    return true;
}

bool DBFWConfig::parse_re_setting(std::string & key, std::string & value)
{
    str_lowercase(key);
    if (key == "block_level")                 re_block_level = atoi(value.c_str());
    else if (key == "risk_detection")         re_active = atoi(value.c_str());
    else if (key == "permission_engine")      re_perm_engine = atoi(value.c_str());
    else if (key == "warn_level")             re_warn_level = atoi(value.c_str());
    else if (key == "risk_sql_comments")      re_sql_comments = atoi(value.c_str());
    else if (key == "risk_senstivite_tables") re_s_tables = atoi(value.c_str());
    else if (key == "risk_or_token")          re_or_token = atoi(value.c_str());
    else if (key == "risk_union_token")       re_union_token = atoi(value.c_str());
    else if (key == "risk_var_cmp_var")       re_var_cmp_var = atoi(value.c_str());
    else if (key == "risk_always_true")       re_always_true = atoi(value.c_str());
    else if (key == "risk_empty_password")    re_empty_password = atoi(value.c_str());
    else if (key == "risk_multiple_queries")  re_multiple_queries = atoi(value.c_str());
    else if (key == "risk bruteforce")        re_bruteforce = atoi(value.c_str());
    else if (key == "return_sql_error")       re_return_sql_error = atoi(value.c_str());
    else 
        logEvent(DEBUG, "[*][Config] Parse ENGINE: No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

    return true;
}

bool DBFWConfig::parse_log_setting(std::string & key, std::string & value)
{
    if (key == "loglevel")      log_level = atoi(value.c_str());
    else if (key == "logfile")  log_file = value;
    else if (key == "logsql")   log_sql = value;
    else if (key == "logrisk")  log_risk = value;
    else if (key == "logalert")  log_alert = value;
    else 
        logEvent(DEBUG, "[*][Config] Parse LOG: No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

    return true;
}

bool DBFWConfig::parse_perm_setting(std::string & key, std::string & value)
{
    if (key == "port")      perm_port = atoi(value.c_str());
    else if (key == "host") perm_host = value;
    else 
        logEvent(DEBUG, "[*][Config] Parse DBPerm: No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

    return true;
}

// bool DBFWConfig::loadDb()
// {
//     // if(sDbType == DB_MYSQL) {
//     //   if (! db_init("mysql")) {
//     //     return false;
//     //   }
//     // }
//     // else if(sDbType == DB_PGSQL) {
//     //   if (! db_init("pgsql")) {
//     //     return false;
//     //   }
//     // }

//     // if (db_load(sDbHost.c_str(),sDbUser.c_str(),sDbPass.c_str(),sDbName.c_str(),iDbPort) == 0)
//     // {
//     //     logevent(STORAGE, "Failed to connect to backend db, error: %s\n", db_error());
//     //     return false;
//     // }
//     return true;
// }

// bool DBFWConfig::closeDb()
// {
//     // db_close();
//     return true;
// }
