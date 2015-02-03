//
// Database Firewall
// 
// Created by: Setyo Legowo (13511071@std.stei.itb.ac.id)
// Institution: Institut Teknologi Bandung
// License: GPL v2 (http://www.gnu.org/licenses/gpl.html)
//

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <ev.h>

static bool fix_dir_name(std::string & conf_dir);
int initLinux();
void clb_timeout(int fd, short which, void * arg);
void killer(int);

int main(int argc, char** argv)
{
    DbFWConfig * cfg = DbFW::getInstance();
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
}

void killer(int)
{
    logevent(CRIT, "Killer\n");
    GreenSQLConfig * cfg = GreenSQLConfig::getInstance();
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
static bool fix_dir_name(std::string & conf_dir)
{
  int len = conf_dir.length();
  
  if (conf_dir[len-1] != '/')
  {
    conf_dir += "/";
  }
  return true; 
}

