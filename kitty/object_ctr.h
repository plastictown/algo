#pragma once
#ifndef __OBJECT_CTR_H__
#define __OBJECT_CTR_H__

#include <atomic>
#include <mutex>

template<typename value_type, typename mutex_type>
class object_counter {
public:
  object_counter(){
    _count++;
    std::unique_lock<mutex_type> lock(_m);
    _mutex_count++;
  }

  static const unsigned getCount() { //unsafe
    return _count;
  }

  static const unsigned getMutexCount() { //unsafe
    return _mutex_count;
  }


  ~object_counter() {

  }
private:
  inline static std::atomic<unsigned> _count = 0;
  inline static unsigned _mutex_count = 0;
  value_type _data;
  inline static mutex_type _m;
};




#endif // !__OBJECT_CTR_H__
