#pragma once
#ifndef __LOCKER_H__
#define __LOCKER_H__

#include <atomic>

#include "tracer.hpp"

class locker {
public:

  locker(const locker&) = delete;
  locker& operator=(const locker&) = delete;

  void lock() {
    TRACE;
    while (flag.test_and_set());
  }

  void unlock() {
    TRACE;
    flag.clear();
  };
  bool try_lock() {
    TRACE;
    return !flag.test_and_set();
  };

  locker() noexcept:
    flag{} {
    TRACE;
  }

  ~locker() {
    TRACE;
    flag.clear();
  }

private:
  std::atomic_flag flag;
};

#endif // !__LOCKER_H__