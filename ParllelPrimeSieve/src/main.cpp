#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
#include "errno.h"
#include <time.h>
#include "../include/ProgressPool.h"

using namespace std;
void getPrime(int number);
void writetoFile(string path, vector<bool> num, int low, int high);
void writeAlltoFile(int number, int process_number);
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
  //clock_t finish = clock();
  //cout << "time: " << (double)(finish-start)/CLOCKS_PER_SEC << endl;
  return 0;
}

void getPrime(int number) {
  start = clock();
  int number1 = pow(2, number) * 1000;
  int progress_number = sqrt(number1) / 2;
  static ProgressPool* mypool = ProgressPool::createPool(progress_number);
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
          write(mypool->getSubProgress()[j].getfds()[1], &loc, 4);
        }
        loc = i + i;
        while (loc < father_range) {
          num[loc] = false;
          loc += i;
        }
      }
    }
    for (int i = 0; i < progress_number; i++) {
      close(mypool->getSubProgress()[i].getfds()[1]);
    }
    string str = "./result/" + to_string(number) + "/sub_progress_0.txt";
    writetoFile(str, num, 0, father_range + 1);
    int status;
    pid_t wpid;
    //等待所有的子进程全部结束
    while ((wpid = wait(&status)) > 0);
    writeAlltoFile(number, progress_number);
  } else {
    int index = mypool -> getIndex();
    int low_bound = number1 / (progress_number + 1) * (index + 1) + number1 % (progress_number + 1) + 1;
    int sub_range = number1 / (progress_number + 1);
    int* step = new int();
    vector<bool> nums(sub_range, true);
    while (1) {
      //这里是如何实现选择到不同的值的
      int size = read(mypool->getSubProgress()[index].getfds()[0], step, 4);
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
    string path = "./result/" + to_string(number) + "/sub_progress_" + to_string(mypool->getIndex() + 1) +".txt";
    writetoFile(path, nums, low_bound, sub_range + low_bound);
  }
}

void writetoFile(string path, vector<bool> num, int low, int high) {
  ofstream file;
  //在这种打开方式下，如果文件存在，那么设置文件的长度为0
  file.open(path.c_str(), ios::trunc);
  if (!file) {
    std::cerr << "file1 open error, path: " << path.c_str() <<"\n";
    return;
  }
  int count = 0;
  for (int i = low; i < high; i++) {
    if (num[i-low]) {
        file << i << " ";
        count++;
    }
  }
  file << "count: " << count << endl;
  clock_t finish = clock();
  double time = (double)(finish - start) / CLOCKS_PER_SEC;
  file << "Runtime: " << time << endl;
  file.close();
}

void writeAlltoFile(int number, int process_number) {
  //将所有的文件里面的数据汇总到一个文件里面
  int count = 0;
  fstream file;
  ofstream addfile;
  string buffer;
  string pathadd = "./result/" + to_string(number) + "/all_progress.txt";
  string paths = "";
  //addfile.open(pathadd.c_str(), ios::app);
  addfile.open(pathadd.c_str(), ios::app);
  if (!addfile) {
    std::cerr << "add file open fail\n";
  }
  for (int i = 0; i <= process_number; i++) {
    paths = "./result/" + to_string(number) + "/sub_progress_" + to_string(i) + ".txt";
    file.open(paths.c_str(), ios::in);
    if (!file) {
      std::cerr << "open file fail\n";
    }
    while (!file.eof()) {
      buffer = "";
      file >> buffer;
      if (buffer == "count:") {
        break;
      }
      addfile << buffer << endl;
      count++;
    }
    file.close();
  }

  addfile << "count is: " << to_string(count) << "\n";
  addfile.close();
}



















//
