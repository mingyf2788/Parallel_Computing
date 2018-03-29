#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cmath>
using namespace std;

void writetoFile(string path, vector<bool> num, int low, int high) {
  int count = 0;
  ofstream file;
  //在这种打开方式下，如果文件存在，那么设置文件的长度为0
  file.open(path.c_str(), ios::trunc);
  if (!file) {
    std::cerr << "file open error\n";
  }
  int size = num.size();
  for (int i = 0; i < size; i++) {
    if (num[i]) {
      file << i << endl;
      count++;
    }
  }
  file << "Runtime: " << endl;
  file << "count： " << count << endl;
  file.close();
}

int main() {
  int number = 5;
  string str = "./result/" + to_string(number) + "/sub_progress_0.txt";
  vector<bool> vec(15, true);
  int father_range = 15;
  writetoFile(str, vec, 0, father_range + 1);
  cout << str << endl;
  return 0;
}
