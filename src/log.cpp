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

static bool logReload ();
static void printLine ( const unsigned char *, int );
static FILE * log_file = stdout;
static int log_level = 3;
static char month_str[][4] = { {"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"Jun"}, {"Jul"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"} };

bool _logInit ()
{
    DBFWConfig * cfg = DBFWConfig::getInstance();

    return logInit(cfg->log_file, cfg->log_level);
}

bool logInit (std::string & file, int level)
{
    log_level = level;

    FILE * fp = fopen(file.c_str(), "a+");

    if (fp == NULL)
        return false;
    
    log_file = fp;

    return true;
}

bool logClose ()
{
    if (log_file != stdout) {
        fclose(log_file);
    }
    log_file = stdout;
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

    logReload();
    fprintf ( log_file, "[%02d/%s/%02d %d:%02d:%02d] %s",
              now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,
              now->tm_hour, now->tm_min, now->tm_sec, error );

    vfprintf ( log_file, fmt, ap );
    va_end (ap);
    fflush (log_file);
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

    logReload();

    for ( i = 0; i < lines; i++ ) {
        // fprintf(log_file, error);
        fprintf(log_file,"[%02d/%s/%02d %d:%02d:%02d] %s", now->tm_mday, month_str[now->tm_mon], now->tm_year+1900,now->tm_hour, now->tm_min, now->tm_sec, error );
        printLine(data+i*16, 16);
    }

    // ord(size%16)
    int ord = (((unsigned char) (size<<4)) >> 4);
    if ( ord > 0 ) {
        // fprintf(log_file, error);
        fprintf(log_file,"[%02d/%s/%02d %d:%02d:%02d] %s", now->tm_mday, month_str[now->tm_mon], now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec, error );
        printLine(data+i*16, ord);
    }

    fflush(log_file);
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

static bool logReload ()
{
    DBFWConfig * cfg = DBFWConfig::getInstance();
    struct stat f_stat;

    if (log_file != stdout) {
        // check if file was deleted
        if ( fstat(fileno(log_file), &f_stat) == 0 ) {
            // check number of hard links, 0 - deleted
            if (f_stat.st_nlink != 0)
                return true;
        }
    }

    logClose();
    logInit(cfg->log_file, cfg->log_level);

    return true;
}
