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

class DBFWConfig {
    public:
        static DBFWConfig * getInstance();
        static void free();
        bool load (std::string &);
        bool load_db ();
        
    private:
    	static DBFWConfig * _obj;
        DBFWConfig ();
        ~DBFWConfig ();
        void init ();
        
};

#endif /* DBFW_CONFIG_HPP */
