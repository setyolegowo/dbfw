//
// Database Firewall event logging API implementation.
//
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include "log.hpp"
#include "config.hpp"

#include <ctype.h>     // for isascii
#include <stdarg.h>    // for va_list
#include <stdio.h>     // for STDOUT
#include <string.h>
#include <sys/types.h> // for fstat
#include <sys/stat.h>  // for fstat
#include <time.h>

static bool logReload (int type);
static bool logClose2(int type);
static bool logInit   (std::string & file, int type);
static void printLine ( const unsigned char *, int );
static FILE * log_file = stdout;
static FILE * log_sql = stdout;
static FILE * log_risk = stderr;
static FILE * log_alert = stderr;
static int log_level = 3;
static char month_str[][4] = { {"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"Jun"}, {"Jul"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"} };

bool _logInit ()
{
    bool retval = true;
    DBFWConfig * cfg = DBFWConfig::getInstance();

    log_level = cfg->log_level;
    retval = logInit(cfg->log_file, 1);
    if(!retval) return retval;
    retval = logInit(cfg->log_sql, 2);
    if(!retval) return retval;
    retval = logInit(cfg->log_risk, 3);
    if(!retval) return retval;
    return logInit(cfg->log_alert, 4);
}

bool logInit (std::string & file, int type)
{
    if(file.length() == 0)
        return true;

    FILE * fp = fopen(file.c_str(), "a+");

    if (fp == NULL) {
        fprintf(stderr, "[*] Failed to open log file %s\n", file.c_str());
        return false;
    }
    
    switch(type) {
        case 1: log_file = fp;  break;
        case 2: log_sql = fp;   break;
        case 3: log_risk = fp;  break;
        case 4: log_alert = fp; break;
        default: fclose(fp);    break;
    }

    return true;
}

bool logClose ()
{
    if (log_file != stdout) {
        fclose(log_file);
    }
    if (log_sql != stdout) {
        fclose(log_sql);
    }
    if (log_risk != stderr) {
        fclose(log_risk);
    }
    if (log_alert != stderr) {
        fclose(log_alert);
    }
    log_file = stdout;
    log_sql = stdout;
    log_risk = stderr;
    log_alert = stderr;
    return true;
}

bool logClose2(int type)
{
    switch(type) {
        case 2:
            if(log_sql != stdout) fclose(log_sql);
            log_sql = stdout;
            break;
        case 3:
            if(log_risk != stderr) fclose(log_risk);
            log_risk = stderr;
            break;
        case 4:
            if(log_alert != stderr) fclose(log_alert);
            log_alert = stderr;
            break;
        default:
            if(log_file != stdout) fclose(log_file);
            log_file = stdout;
            break;
    }
    return true;
}

void logEvent (ErrorType type, const char * fmt, ...)
{
    va_list ap;
    const char * error;
    struct tm *now;
    time_t tval;

    if (log_level < (int) type) {
        va_end(ap);
        return;
    }

    va_start(ap, fmt);
    tval = time(NULL);
    now = localtime(&tval);

    switch(type) {
        case CRIT:
            error = "CRIT      ";
            break;
        case ERR:
            error = "ERROR     ";
            break;
        case INFO:
            error = "INFO      ";
            break;
        case SQL_DEBUG:
            error = "SQL_DEBUG ";
            break;
        case NET_DEBUG:
            error = "NET_DEBUG ";
            break;
        case VV_DEBUG:
        case V_DEBUG:
        case DEBUG:
            error = "DEBUG     ";
            break;
        case STORAGE:
            error = "STORAGE   ";
            break;
        default:
            error = "UNKNOWN   ";
            break;
    }

    logReload(1);
    fprintf ( log_file, "[%02d/%s/%02d %d:%02d:%02d] %s",
              now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,
              now->tm_hour, now->tm_min, now->tm_sec, error );

    vfprintf ( log_file, fmt, ap );
    va_end (ap);
    fflush (log_file);
}

void logSQL (ErrorType type, const char * fmt, ...)
{
    va_list ap;
    struct tm *now;
    time_t tval;

    if (log_level < (int) type) {
        va_end(ap);
        return;
    }

    va_start(ap, fmt);
    tval = time(NULL);
    now = localtime(&tval);

    logReload(2);
    fprintf ( log_sql, "[%02d/%s/%02d %d:%02d:%02d] ",
              now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,
              now->tm_hour, now->tm_min, now->tm_sec);

    vfprintf ( log_sql, fmt, ap );
    va_end (ap);
    fflush (log_sql);
}

void logRisk (ErrorType type, const char * fmt, ...)
{
    va_list ap;
    struct tm *now;
    time_t tval;

    if (log_level < (int) type) {
        va_end(ap);
        return;
    }

    va_start(ap, fmt);
    tval = time(NULL);
    now = localtime(&tval);

    logReload(3);
    fprintf ( log_risk, "[%02d/%s/%02d %d:%02d:%02d] ",
              now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,
              now->tm_hour, now->tm_min, now->tm_sec);

    vfprintf ( log_risk, fmt, ap );
    va_end (ap);
    fflush (log_risk);
}

void logHex (ErrorType type, const unsigned char * data, int size)
{
    const char * error;
    struct tm *now;
    time_t tval;

    if (size == 0)
        return;
    if (log_level < (int) type)
        return;

    tval = time(NULL);
    now = localtime(&tval);

    switch(type) {
        case CRIT:
            error = "CRIT      ";
            break;
        case ERR:
            error = "ERROR     ";
            break;
        case INFO:
            error = "INFO      ";
            break;
        case SQL_DEBUG:
            error = "SQL_DEBUG ";
            break;
        case NET_DEBUG:
            error = "NET_DEBUG ";
            break;
        case VV_DEBUG:
        case V_DEBUG:
        case DEBUG:
            error = "DEBUG     ";
            break;
        case STORAGE:
            error = "STORAGE   ";
            break;
        default:
            error = "UNKNOWN   ";
            break;
    }

    int lines = size / 16;
    int i = 0;

    logReload(1);

    for ( i = 0; i < lines; i++ ) {
        // fprintf(log_file, error);
        fprintf(log_file,"[%02d/%s/%02d %d:%02d:%02d] %s", now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,now->tm_hour, now->tm_min, now->tm_sec, error );
        printLine(data+i*16, 16);
    }

    // ord(size%16)
    int ord = (((unsigned char) (size<<4)) >> 4);
    if ( ord > 0 ) {
        // fprintf(log_file, error);
        fprintf(log_file, "[%02d/%s/%02d %d:%02d:%02d] %s", now->tm_mday, month_str[now->tm_mon], now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec, error );
        printLine(data+i*16, ord);
    }

    fflush(log_file);
}

bool logAlert(int proxy_id, std::string & dbname, std::string & dbuser, std::string & dbuserip,
        std::string & query, std::string & pattern, 
        std::string & reason, int risk, int block)
{
    struct tm *now;
    time_t tval;

    tval = time(NULL);
    now = localtime(&tval);

    logReload(4);

    fprintf(log_alert, "[%02d/%s/%02d %d:%02d:%02d] %d:%d:%d\n", now->tm_mday, month_str[now->tm_mon],
        now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec, proxy_id, risk, block);
    fprintf(log_alert, "%s:%s@%s\nq:%s\np:%s\nr:%s", dbuser.c_str(), dbuserip.c_str(), dbname.c_str(), query.c_str(), pattern.c_str(), reason.c_str());

    fflush(log_alert);

    return true;
}

static void printLine (const unsigned char * data, int max)
{

    int j = 0;
    char temp[256];
    memset(temp, ' ', sizeof(temp));
    temp[sizeof(temp)-1] = 0;
    unsigned char b;

    for( j = 0; j < max; j++ ) {
        b = data[j];
        if (isalnum(b) || b == ' ' || ispunct(b))
            temp[j] = data[j];
        else
            temp[j] = '.';
    }

    // print hex
    temp[18] = '|';

    for ( j = 0; j < max; j++ ) {
        b = (data[j]>>4);
        b += ( b > 9) ? 'A'-10 : '0';
        temp[20+j*3] = b;

        b = ((unsigned char)(data[j]<<4)) >>4;
        b += ( b > 9) ? 'A'-10 : '0';
        temp[20+j*3+1] = b;
    }

    temp[20+j*3] = '\n';
    temp[20+j*3+1] = 0;

    fprintf(log_file, "%s", temp);
}

static bool logReload (int type)
{
    DBFWConfig * cfg = DBFWConfig::getInstance();
    FILE * fp;
    bool type_stdout = true;
    struct stat f_stat;
    switch(type) {
        case 2: fp = log_sql;   break;
        case 3: fp = log_risk; type_stdout = false;  break;
        case 4: fp = log_alert; type_stdout = false; break;
        default: fp = log_file; break;
    }

    if ((type_stdout && fp != stdout) || (!type_stdout && fp != stderr)) {
        // check if file was deleted
        if ( fstat(fileno(fp), &f_stat) == 0 ) {
            // check number of hard links, 0 - deleted
            if (f_stat.st_nlink != 0)
                return true;
        }
    }

    logClose2(type);
    switch(type) {
        case 2: logInit(cfg->log_sql, 2);   break;
        case 3: logInit(cfg->log_risk, 3);  break;
        case 4: logInit(cfg->log_alert, 4); break;
        default: logInit(cfg->log_file, 1);  break;
    }

    return true;
}
