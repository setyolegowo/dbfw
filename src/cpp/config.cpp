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
    
    log_level           = 3;
    log_file            = "/var/log/dbfw.log";
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

    logEvent(DEBUG, "Loading config file: %s\n", cfg_file.c_str());
    if (!file.is_open()) {
        logEvent(CRIT, "Failed to load configuration file: %s", cfg_file.c_str());
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
                logEvent(DEBUG, "Failed to parse following config line: %s", line.c_str());
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
            logEvent(CRIT, "Failed to parse following configuration line: %s", line.c_str());
            return false;
        }
        str_lowercase(key);
        
        if (section == "risk engine")   parse_re_setting  (key, value);
        else if (section == "logging")  parse_log_setting (key, value);
        else 
            logEvent(DEBUG, "No section for this key-value configuration: \"%s\"", line.c_str());
    }

    return true;
}

bool DBFWConfig::parse_re_setting(std::string & key, std::string & value)
{
    str_lowercase(key);
    if (key == "block_level")                 re_block_level = atoi(value.c_str());
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
    else 
        logEvent(DEBUG, "[Parse ENGINE] No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

    return true;
}

bool DBFWConfig::parse_log_setting(std::string & key, std::string & value)
{
    if (key == "loglevel")      log_level = atoi(value.c_str());
    else if (key == "logfile")  log_file = value;
    else 
        logEvent(DEBUG, "[Parse LOG] No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

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
