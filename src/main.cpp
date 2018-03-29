#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
#include "errno.h"
#include <time.h>
#include "../include/ProcessPool.h"

using namespace std;
void getPrime(int number);
clock_t start;
int main() {
  int number;
  cout << "Please input the number between 0 and 9: ";
  cin >> number;
  while (!(number >= 0 && number <= 9)) {
    cout << "OOh, Please input the number between 0 and 9: ";
    cin >> number;
  }
  getPrime(number);
  return 0;
}

void getPrime(int number) {
  start = clock();
  int number1 = pow(2, number) * 1000;
  int progress_number = sqrt(number1) / 2;
  static ProcessPool* mypool = ProcessPool::creatPool(progress_number);
  int loc = 0;

  if (mypool -> getIndex() == -1) {
    //这个进程代表的是父进程
    int father_range = number1 / (progress_number + 1) + number1 % (progress_number + 1) + 1;
    vector<bool> num(father_range, true);
    num[0] = num[1] = false;
    for (int i = 2; i < father_range; i++) {
      loc = i;
      if (num[i]) {
        for (int j = 0; j < progress_number; j++) {
          write(mypool->getSubProcess()[j].getfds()[1], &loc, 4);
        }
        loc = i + i;
        while (loc < father_range) {
          num[loc] = false;
          loc += i;
        }
      }
    }
    for (int i = 0; i < progress_number; i++) {
      close(mypool->getSubProcess()[i].getfds()[1]);
    }
    int size = num.size();
    cout << "progress 0: ";
    for (int i = 0; i < size; i++) {
      if (num[i]) {
        cout << i << " ";
      }
    }
    cout << endl;
    clock_t now = clock();
  } else {
    int index = mypool -> getIndex();
    int low_bound = number1 / (progress_number + 1) * (index + 1) + number1 % (progress_number + 1) + 1;
    int sub_range = number1 / (progress_number + 1);
    int* step = new int();
    vector<bool> nums(sub_range, true);
    while (1) {
      //这里是如何实现选择到不同的值的
      int size = read(mypool->getSubProcess()[index].getfds()[0], step, 4);
      if (size > 0) {
        //需要找出的是第一个能被*step整除的数
        int start = 0;
        start = (low_bound / (*step)) * (*step) == low_bound ? low_bound : (low_bound / (*step)) * (*step) + (*step);
        for (int i = start; i < low_bound + sub_range;) {
          nums[i - low_bound] = false;
          i += *step;
        }
      } else {
        break;
      }
    }
    int size = nums.size();
    cout << "progress " << index << ": ";
    for (int i = 0; i < size; i++) {
      if (nums[i]) {
        cout << i + low_bound << " ";
      }
    }
    cout << endl;
  }
}


















//
