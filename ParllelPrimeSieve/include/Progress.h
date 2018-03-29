#ifndef PROGRESS_H
#define PROGRESS_H
#include <sys/types.h>

class Progress {
  public:
    Progress() :m_pid(-1) {}
    int setId(int id) {
      m_pid = id;
    }
    int* getfds() {
      return fds;
    }
  private:
    pid_t m_pid;
    //0表示读，1表示写
    int fds[2];
};

#endif
