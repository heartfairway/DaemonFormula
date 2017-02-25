#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

#include "daemon.h"

int working;

// In this example, this will be called by daemon_exit(...) if registered in daemon_init(...).
int onExit(void)
{
	working=0;
	syslog(LOG_NOTICE, "Custom exit procedure.\n");
	
	/* You may choose whether to let process terminate daemon_exit(...) according to your implement ...

	 return 0, or values for false, the process will terminate by daemon_exit(...).
	 return 1, or values for true, daemon_exit(...) will just return, without terminating process. */
	return 0;
}

int main(void)
{
	int i;

	// If you don't have any exit procedre, you can simply register the exit procedure as NULL.
	//daemon_init("daemon_example", "/tmp/daemon_example.pid", NULL);
	daemon_init("daemon_example", "/tmp/daemon_example.pid", onExit);

	i=0;
	working=1;
	syslog(LOG_NOTICE, "Running main() works...\n");
	
	while(working) {
		sleep(2);
		
		/* This example will run about 5 minutes and call daemon_exit(...) ...

		Generally, daemon_exit(...) will be called automatically once receiving signal SIGINT, SIGQUIT or SIGTERM, 
		you can also call it somewhere in your implement if needed, 
		the integer parameter will just be a value for syslog. */
		if(i++ > 150) daemon_exit(0);
	}
	
	// This will be executed if onExit() returns 1, or values for true.
	syslog(LOG_NOTICE, "Daemon stopping by returning main().\n");
	
	return 0;
}
