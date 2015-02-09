//
// Database Firewall event logging API.
// 
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#ifndef DBFW_LOG_HPP
#define DBFW_LOG_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <string>

enum ErrorType { CRIT, ERR, INFO, SQL_DEBUG, DEBUG, STORAGE, NET_DEBUG};

bool log_init  (std::string & file, int level);
bool log_close ();
void logevent  (ErrorType type, const char * fmt, ...);
void loghex    (ErrorType type, const unsigned char * data, int size);

#ifdef __cplusplus
}
#endif

#endif /* DBFW_LOG_HPP */

