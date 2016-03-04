#include <iostream>
#include <functional>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "ThreadPool.h"

void func(int i) {
  std::cout << i << std::endl;
}

int main()
{
  ThreadPool tp(10, 10);  // queueSize, poolSize

  tp.pool_init();
  srand((unsigned)time(NULL));
  for(long long i = 0; i < 10000; ++i) {
    tp.addTask(std::bind(&func, rand() % 100));
    //sleep(1);
  }
  tp.pool_destroy();

  return 0;
}
