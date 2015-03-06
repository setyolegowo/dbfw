//
// Database Firewall functions used to manage a number of proxy objects.
//
// Copyright (c) 2015 Setyo Legowo <13511071@std.stei.itb.ac.id>
// License: GPL v2 (http://www.gnu.org/licenses/gpl.html)
//

#include "proxymap.hpp"

#include <fstream>
#include <stdio.h>
#include <map>

#include "dbfw.hpp"
#include "misc.hpp"
// #include "mysql/mysql_con.hpp"
// #include "pgsql/pgsql_con.hpp"

static bool __proxyMapKeyValue(int &, std::string &, std::string &, std::string &,
    int &, std::string &, int &, std::string &, std::string &);

typedef std::map<int, DBFW *> proxy_t;
static proxy_t proxies;

bool proxyMapInit()
{
    proxyMapReload();
    if (proxies.size() == 0)
        return false;
    
    return true;
}
bool proxyMapReload()
{
    proxy_t::iterator itr;
    DBFWConfig * cfg        = DBFWConfig::getInstance();
    DBFW * cls              = NULL;
    bool ret                = true;
    std::string cfg_file;
    cfg_file.reserve(1024);
    cfg_file                = cfg->conf_path + "proxy.conf";

    /* Temporary variable and default value */
    int proxy_id            = 999;
    std::string name        = "database";
    std::string db_type     = "mysql";
    std::string backend_ip  = "127.0.0.1";
    int backend_port        = 3306;
    std::string proxy_ip    = "127.0.0.1";
    int proxy_port          = 3305;

    std::ifstream file(cfg_file.c_str());
    std::string line;
    std::string key="";
    std::string value= "";
    std::string section = "";
    line.reserve(1024);

    logEvent(DEBUG, "Loading proxy configuration file: %s\n", cfg_file.c_str());
    if (!file.is_open()) {
        logEvent(CRIT, "Failed to load configuration file: %s", cfg_file.c_str());
        return false;
    }

    bool new_section =false, first_iteration = true;
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
            new_section = true;
            continue;
        }

        if (ParseConfLine(line, key, value) == false)
        {
            logEvent(CRIT, "Failed to parse following configuration line: %s", line.c_str());
            return false;
        }
        str_lowercase(key);

        if(section == "database") {
            if(new_section == true) {
                if(first_iteration == false) {
                    // saving
                    itr = proxies.find(proxy_id);
                    if (itr == proxies.end()) {
                        cls = new DBFW();
                        ret = cls->proxyInit(proxy_id, proxy_ip, proxy_port,
                            name, backend_ip, backend_port, db_type);
                        if (ret == false) {
                            logEvent(ERR, "Failed initialize firewall id [%d]",
                                proxy_id);
                            cls->closeConnection();
                            delete cls;
                        } else {
                            logEvent(DEBUG, "Firewall id [%d] initialization successed",
                                proxy_id);
                            proxies[proxy_id] = cls;
                        }
                    }
                } else {
                    first_iteration = false;
                }
                proxy_id     = 999;
                name         = "database";
                db_type      = "mysql";
                backend_ip   = "127.0.0.1";
                backend_port = 3306;
                proxy_ip     = "127.0.0.1";
                proxy_port   = 3305;
            }
            new_section = false;
            __proxyMapKeyValue(proxy_id, name, db_type, backend_ip, backend_port,
                proxy_ip, proxy_port, key, value);
        } else
            logEvent(DEBUG, "No section for this key-value configuration: \"%s\"", line.c_str());
    }

    return true;
}

bool proxyMapClose()
{
    proxy_t::iterator iter;
    DBFW * cls = NULL;

    while (proxies.size() > 0)
    {
        iter = proxies.begin();
        cls = iter->second;
        cls->closeConnection();
        delete cls;
        proxies.erase(iter);
    }

    return true;
}
// void wrapServer(struct ev_loop *loop, ev_io *w_, int revents)
// {}
// void wrapProxy(struct ev_loop *loop, ev_io *w_, int revents)
// {}
// void wrapBackend(struct ev_loop *loop, ev_io *w_, int revents)
// {}
bool __proxyMapKeyValue(int & proxy_id, std::string & name, std::string & db_type, 
    std::string & backend_ip, int & backend_port, std::string & proxy_ip, 
    int & proxy_port, std::string & key, std::string & value)
{
    str_lowercase(key);
    if(key == "id")                proxy_id = atoi(value.c_str());
    else if(key == "name")         name = value;
    else if(key == "type")         db_type = value;
    else if(key == "backend_ip")   backend_ip = value;
    else if(key == "backend_port") backend_port = atoi(value.c_str());
    else if(key == "proxy_ip")     proxy_ip = value;
    else if(key == "proxy_port")   proxy_port = atoi(value.c_str());
    else
        logEvent(DEBUG, "[Parse ENGINE] No section for this key-value configuration: \"%s=%s\"", key.c_str(), value.c_str());

    return true;
}
