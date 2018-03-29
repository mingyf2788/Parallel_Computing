#include <unistd.h>
#include <iostream>
#include "../include/ProgressPool.h"

ProgressPool* ProgressPool::instance = nullptr;

ProgressPool::ProgressPool(int number) :process_number(number), index(-1) {
  sub_progress = new Progress[number];
  for (int i = 0; i < number; i++) {
    //为每一个进程创建一个管道
    if (pipe(sub_progress[i].getfds()) < 0) {
      std::cerr << "pipe error";
    }
    pid_t pid;
    pid = fork();
    if (pid < 0) {
      std::cerr << "fork err\n";
    } else if (pid > 0) {
      //父亲进程只能写，不能读
      sub_progress[i].setId(pid);
      close(sub_progress[i].getfds()[0]);
    } else {
      //子进程
      index = i;
      close(sub_progress[i].getfds()[1]);
      //关于这里的break其实还是有疑问的
      break;
    }
  }
}

int ProgressPool::getIndex() {
  return index;
}

Progress* ProgressPool::getSubProgress() {
  return sub_progress;
}

ProgressPool::~ProgressPool() {
  delete []sub_progress;
}

ProgressPool* ProgressPool::createPool(int number) {
  if (instance == nullptr) {
    instance = new ProgressPool(number);
  }
  return instance;
}
