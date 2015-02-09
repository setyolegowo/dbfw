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

#include "dbfw.hpp"

/* ---------------------------------------------------------------- */
/*                   INTERNAL FUNCTION DEFINITION                   */
/* ---------------------------------------------------------------- */

static bool fix_dir_name (std::string &);
int initLinux ();
// void clb_timeout (int, short, void *);
int parameterCheck (int, char **);
void killer (int);

/* ---------------------------------------------------------------- */
/*                           MAIN PROGRAM                           */
/* ---------------------------------------------------------------- */

int main (int argc, char** argv)
{
    DBFWConfig * cfg = DBFWConfig::getInstance();
    std::string conf_path = "";
    if(parameterCheck(argc, argv) < 0)
        return -1;
    
    fix_dir_name(conf_path);
    if (cfg->load(conf_path) == false)
	{
        fprintf(stderr, "Failed to load config file: %s/dbfw.conf\n\n", 
			    conf_path.c_str());
	    fprintf(stderr, "Specify location of the conf. file using \"-p\" parameter.\n\n");
	    fprintf(stderr, "Usage: %s -p DIRECTORY\n\n", argv[0]);
	    fprintf(stderr, "DIRECTORY is a location of the config files\n");
	    return -1;
	}
	if (cfg->load_db() == false) {
	    fprintf(stderr, "Failed to connect to db storage.\n");
	    return -1;
	}
	
	/* START OF RULES INIT
	if (mysql_rules_init(conf_path) == false) {
	    fprintf(stderr, "Failed to load MySQL list of rules.\n");
	    return -1;
	}
	if (pgsql_rules_init(conf_path) == false) {
	    fprintf(stderr, "Failed to load PGSQL list of rules.\n");
	    return -1;
	} /* END OF RULES INIT */
	
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
    signal(SIGQUIT, Killer);
    signal(SIGHUP, Killer);
    signal(SIGINT, Killer);
    signal(SIGTERM, Killer);
    return 1;
}

/*
void clb_timeout(int fd, short which, void * arg)
{
    struct timeval delay;
    delay.tv_sec=1;
    delay.tv_usec=0;

    struct event * tEvent = (struct event*) arg;

    GreenSQLConfig * cfg = GreenSQLConfig::getInstance();

    //logevent(INFO, "timer fired\n");
    if (cfg->bRunning == false) {
        proxymap_close();
    } else {
        counter++;
        if (counter == 5) {
            //logevent(INFO, "Timer fired 1000 times\n");
            counter = 0;
            proxymap_reload();
            dbmap_reload();
            agroupmap_reload();
        }
    }
} */

void killer(int)
{
    logevent(CRIT, "Killer\n");
    DBFWConfig * cfg = DBFWConfig::getInstance();
    cfg->bRunning = false;
}

int parameterCheck(int argc, char** argv)
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

