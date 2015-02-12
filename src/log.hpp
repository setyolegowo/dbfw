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

enum ErrorType { CRIT, ERR, INFO, SQL_DEBUG, DEBUG, STORAGE, NET_DEBUG };

bool _logInit  ();
bool logInit  (std::string & file, int level);
bool logClose ();
void logEvent  (ErrorType type, const char * fmt, ...);
void logHex    (ErrorType type, const unsigned char * data, int size);

#endif /* DBFW_LOG_HPP */
