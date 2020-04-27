#include <iostream>

#include <atomic>
#include <vector>
#include <cstdint>
#include<algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>

#define DEBUG_TRACE

#include "locker.h"
#include "object_ctr.h"

void task1(uint32_t);
void task2();
void task3();

//----------------------------------------------------------//

int main() {
  //task1(1000000);
  task2();
  std::cin.get();
  return 0;
}

//----------------------------------------------------------//

void task1(uint32_t N) {

  using value_type = uint32_t;
  std::vector<value_type> v;
  std::atomic_flag can_write = ATOMIC_FLAG_INIT;

  std::atomic<bool> rd1{ false };
  std::atomic<bool> rd2{ false };

  //---------------------------//

  auto proc = [N](decltype(v)& _v, decltype(can_write)& _w, decltype(rd1)& _flag) {
    for (size_t i = 0; i < N; ++i) {
      while (std::atomic_flag_test_and_set(&_w));
      _v.push_back(i);
      std::atomic_flag_clear(&_w);
    }
    _flag.store(true);
    std::cout << "exit worker" << std::endl;
  };

  //---------------------------//

  auto final_proc = [](decltype(v)& _v, decltype(rd1)& flag1, decltype(rd2)& flag2) {
    while (!(flag1 && flag2));
    std::for_each(_v.begin(), _v.end(), [](value_type& arg) {arg += 1; });
    std::cout << "exit final_proc" << std::endl;
  };

  //---------------------------//

  std::thread t1(proc, std::ref(v), std::ref(can_write), std::ref(rd1));
  std::thread t2(proc, std::ref(v), std::ref(can_write), std::ref(rd2));
  std::thread t3(final_proc, std::ref(v), std::ref(rd1), std::ref(rd2));

  //---------------------------//

  t1.detach();
  t2.detach();
  t3.join();

}

//----------------------------------------------------------//

void task2() {

  using value_type = int;
  using cont_type = std::vector<value_type>;
  size_t cycles = 10000;

  cont_type ct;
  locker l;
  std::mutex m;

  auto foo = [&]() {
    TRACE;
    for (size_t i = 0; i < cycles; ++i) {
      std::lock_guard<decltype(l)> lock(l);
      ct.push_back(value_type{});
      std::this_thread::yield();
    }
    std::cout << "exit_thread" << std::endl;
  };

  auto bar = [&]() {
    TRACE;
    for (size_t i = 0; i < cycles; ++i) {
      std::lock_guard<decltype(m)> lock(m);
      ct.push_back(value_type{});
      std::this_thread::yield();
    }
    std::cout << "exit_thread" << std::endl;
  };

  std::thread t1{ foo };
  std::thread t2{ foo };

  t1.join();
  t2.join();

  ct.clear();
  std::cout << "-----------------------" << std::endl;

  std::thread t3{ foo };
  std::thread t4{ foo };

  t3.join();
  t4.join();


}

//----------------------------------------------------------//

void task3() {
  object_counter<int, locker> ctr;
}