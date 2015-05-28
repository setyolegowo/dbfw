//
// Database Firewall
//
// Created by  : Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution : Institut Teknologi Bandung
// License     : GPL v2 (http://www.gnu.org/licenses/gpl-2.0.html)
//

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <ev.h>
#include <ev++.h>

#include "config.hpp"
#include "dbfw.hpp"
#include "mysql/mysql_con.hpp"
#include "pgsql/pgsql_con.hpp"
#include "proxymap.hpp"

/* ---------------------------------------------------------------- */
/*                   INTERNAL FUNCTION DEFINITION                   */
/* ---------------------------------------------------------------- */

static bool fix_dir_name (std::string &);
// static void timeout_cb (EV_P_ struct ev_timer *, int);
int initLinux ();
int parameterCheck (int, char **, std::string &);
void killer (int);

/* ---------------------------------------------------------------- */
/*                           MAIN PROGRAM                           */
/* ---------------------------------------------------------------- */

int main (int argc, char** argv)
{
    DBFWConfig * cfg = DBFWConfig::getInstance();
    if(parameterCheck(argc, argv, cfg->conf_path) < 0)
        return -1;
    fix_dir_name(cfg->conf_path);
    if (cfg->load() == false) {
        fprintf(stderr, "Specify location of the configuration file using \"-p\" parameter.\n\n");
        fprintf(stderr, "Usage: %s -p DIRECTORY\n\n", argv[0]);
        fprintf(stderr, "DIRECTORY is a location of the config files\n");
        return -1;
    }
    if(_logInit() == false) {
        fprintf(stderr, "Failed to open log file\n");
        return -1;
    }

    logEvent(INFO, "Application started\n");

    /* DBMS RULES INIT */
    if (mysqlPatternsInit(cfg->conf_path) == false) {
        fprintf(stderr, "Failed to load MySQL list of patterns.\n");
        return -1;
    }
    if (pgsql_patterns_init(cfg->conf_path) == false) {
        fprintf(stderr, "Failed to load PGSQL list of patterns.\n");
        return -1;
    }

    initLinux();

    ev::default_loop loop;

    if (proxyMapInit() == false) {
        logEvent(DEBUG, "[*] Failed to open all server sockets, closing application\n");
        exit(0);
    }

    loop.run(0);

    /* =================== AFTER RUN =================== */

    proxyMapClose();
    DBFWConfig::free();

    logEvent(DEBUG, "[*] Application closed\n\n");
    logClose();

    return 0;
}

/* ---------------------------------------------------------------- */
/*                 INTERNAL FUNCTION IMPLEMENTATION                 */
/* ---------------------------------------------------------------- */

static bool fix_dir_name (std::string & conf_dir)
{
    int len = conf_dir.length();
      
    if (conf_dir[len-1] != '/') {
        conf_dir += "/";
    }
    
    return true; 
}

int initLinux()
{
    signal(SIGPIPE, SIG_IGN);
    // signal(SIGQUIT, killer);
    // signal(SIGHUP, killer);
    // signal(SIGINT, killer);
    // signal(SIGTERM, killer);
    return 1;
}

/* void timeout_cb (EV_P_ struct ev_timer *w, int revents)
{
    puts ("timeout");
    // this causes the innermost ev_run to stop iterating
    ev_break (EV_A_ EVBREAK_ONE);
} */

void killer(int)
{
    logEvent(CRIT, "[*] Killer fired\n");
    DBFWConfig * cfg = DBFWConfig::getInstance();
    cfg->server_running = false;
}

int parameterCheck(int argc, char** argv, std::string & conf_path)
{
    if (argc > 2) {
        if (strcmp(argv[1], "-p") == 0) {
            conf_path = argv[2];
            return 0;
        } else {
           fprintf(stderr, "Unrecognized parameter\n\n");
           fprintf(stderr, "Usage: %s -p DIRECTORY\n\n", argv[0]); 
           fprintf(stderr, "DIRECTORY is a location of the config files\n");
           return -1;
        }
    } else if (argc > 1) {
        fprintf(stderr, "Unrecognized parameter\n\n");
        fprintf(stderr, "Usage: %s -p DIRECTORY\n\n", argv[0]);
        fprintf(stderr, "DIRECTORY is a location of the config files\n");
        return -1;
    } else
        return 0;
}
