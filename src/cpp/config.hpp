//
// Database Firewall configuration class header.
//
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#ifndef DBFW_CONFIG_HPP
#define DBFW_CONFIG_HPP

#include <string.h>
#include <stdlib.h>
#include <string>

enum DBProxyType { DBTypeMySQL, DBTypePGSQL };
// enum DBBackEnd { DB_MYSQL = 1, DB_PGSQL = 2 };

class DBFWConfig {
    public:
        static DBFWConfig * getInstance();
        static void free();
        std::string conf_path;
        bool load ();
        // bool loadDb ();
        // bool closeDb ();
        bool server_running;

        // risk engine factors
        int re_block_level;
        int re_warn_level;
        int re_sql_comments;
        int re_s_tables;
        int re_or_token;
        int re_union_token;
        int re_var_cmp_var;
        int re_always_true;
        int re_empty_password;
        int re_multiple_queries;
        int re_bruteforce;

        int log_level;
        std::string log_file;

    private:
    	static DBFWConfig * _obj;
        DBFWConfig ();
        ~DBFWConfig ();
        void init (); // member initialization

        bool parse_re_setting  (std::string & key, std::string & value);
        bool parse_log_setting (std::string & key, std::string & value);

};

#endif /* DBFW_CONFIG_HPP */
