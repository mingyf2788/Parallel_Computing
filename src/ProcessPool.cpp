#include <unistd.h>
#include <iostream>
#include "../include/ProcessPool.h"

ProcessPool* ProcessPool::instance = nullptr;

ProcessPool::ProcessPool(int number) :process_number(number), index(-1) {
  sub_process = new Process[number];
  for (int i = 0; i < number; i++) {
    //为每一个进程创建一个管道
    if (pipe(sub_process[i].getfds()) < 0) {
      std::cerr << "pipe error";
    }
    pid_t pid;
    pid = fork();
    if (pid < 0) {
      std::cerr << "fork err\n";
    } else if (pid > 0) {
      //父亲进程只能写，不能读
      sub_process[i].setId(pid);
      close(sub_process[i].getfds()[0]);
    } else {
      //子进程
      index = i;
      close(sub_process[i].getfds()[1]);
      //关于这里的break其实还是有疑问的
      break;
    }
  }
}

ProcessPool::~ProcessPool() {
  delete []sub_process;
}

ProcessPool* ProcessPool::creatPool(int number) {
  if (instance == nullptr) {
    instance = new ProcessPool(number);
  }
  return instance;
}
