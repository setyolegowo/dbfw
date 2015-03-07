//
// Database Firewall event logging API.
//
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#ifndef DBFW_LOG_HPP
#define DBFW_LOG_HPP

#include <string>

enum ErrorType { CRIT = -1, ERR = 0, INFO = 1, SQL_DEBUG = 2, NET_DEBUG = 3, DEBUG = 4, STORAGE = 5, V_DEBUG = 6, VV_DEBUG = 7 };

bool _logInit  ();
bool logInit   (std::string & file, int level);
bool logClose  ();
void logEvent  (ErrorType type, const char * fmt, ...);
void logHex    (ErrorType type, const unsigned char * data, int size);

#endif /* DBFW_LOG_HPP */
