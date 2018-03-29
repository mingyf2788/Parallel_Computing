#ifndef PROGRESSPOOL_H
#define PROGRESSPOOL_H
#include "./Progress.h"
#include <unistd.h>

class ProgressPool {
  public:
    static ProgressPool* createPool(int number);
    ~ProgressPool();
    int getIndex();
    Progress* getSubProgress();

  private:
    ProgressPool(int number);
    Progress* sub_progress;
    int process_number;
    static ProgressPool* instance;
    int index;
};
#endif
