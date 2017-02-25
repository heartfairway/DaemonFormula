/******************************
*   Daemon Formula
*
*      by. Cory Chiang
*
*******************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>

#include "daemon.h"

int (*_t_proc)(void);  // custom termination procedure

void daemon_init(const char* logname, char* pidfpath, int (*exit_proc)(void)) 
{
    pid_t pid;
    FILE *pidf;
    char buffer[32];
	int i;

    // Fork off, stage 1
    pid=fork();

    if(pid<0) exit(EXIT_FAILURE); // error
    if(pid>0) exit(EXIT_SUCCESS); // parent, terminating

    // Create session and set the process group ID 
    if(setsid()<0) exit(EXIT_FAILURE);

    // Signal handling stage 1, ignore signals ... 
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork off, stage 2 
    pid=fork();

    if(pid<0) exit(EXIT_FAILURE); // error

	// Parent, log PID file and terminate 
    if(pid>0) {
		if(pidfpath) {
			sprintf(buffer, "%d", pid);
			pidf=fopen(pidfpath, "w");
			fwrite(buffer,strlen(buffer), 1, pidf); //fputs(buffer,pidf);
			fclose(pidf);
		}

        exit(EXIT_SUCCESS);
    }

    // Set new file permissions
    umask(0);

    // Change working directory
    chdir("/");

    // Close all open file descriptors 
    for(i=sysconf(_SC_OPEN_MAX); i>0; i--) close(i);

	// Signal handling stage 2, catch signals for termination process 
	signal(SIGINT, daemon_exit);
	signal(SIGQUIT, daemon_exit);
	signal(SIGTERM, daemon_exit);
	
	// set termination procedure, which can be null
	_t_proc=exit_proc;
	
    // Open log file 
    openlog(logname, LOG_PID, LOG_DAEMON);
	syslog(LOG_NOTICE,"Daemon started.");
}

void daemon_exit(int s) 
{
	syslog(LOG_NOTICE, "Received terminating signal: %d\n", s);
	
	// Just return if termination procedure registered and returns a positive value,
	// otherwise, exit the process automatically.
	if(_t_proc) {
		if((*_t_proc)()) return;
	}
	
	syslog(LOG_NOTICE,"Stopping daemon.");
	exit(EXIT_SUCCESS);
}
