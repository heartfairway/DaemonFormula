/******************************
*   Daemon Formula
*
*      by. Cory Chiang
*
*******************************/
#ifndef __DAEMON_H_
#define __DAEMON_H_

/*
* Fork the process to background executing daemon.
* 
* logname: Identity name for system log.
* pidfpath: The path for PID file after successed, NULL if no PID file needed.
* t_proc: On receiving signal SIGINT, SIGQUIT and SIGTERM, daemon_exit() will be execute automatically,
*         if t_proc is specified, the specified fumction with no parameter would execte before process termination.
*/
void daemon_init(const char* logname, char* pidfpath, int (*exit_proc)(void));

/*
* Terminate the daemon process
*
* s: Signal received, just for log.
* This function will execute automatically when receiving signal SIGINT, SIGQUIT or SIGTERM, 
* together with function exit_proc specified in daemon_init(). It is also available for call manually.
*/
void daemon_exit(int s);

#endif
