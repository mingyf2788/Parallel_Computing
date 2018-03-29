#ifndef PROCESS_H
#define PROCESS_H
#include <sys/types.h>
#include <time.h>

class Process {
public:
	Process() {
		m_pid = -1;
		beginTime = clock();
	}
	void setId(pid_t id) {
		m_pid = id;
	}
	void setBeginTime(clock_t time) {
		beginTime = time;
	}
	int * getfds() {
		return fds;	
	}
	clock_t getBeginTime() {
		return clock();
	}

private:
	pid_t m_pid;
	// filedes[0] is to read pipe, filedes[1] is to write pipe
	int fds[2];
	int beginTime;
};
#endif // !PROCESS_H

