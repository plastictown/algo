#pragma once

#ifndef __POOL_H__
#define __POOL_H__

#include <functional>
#include <thread>
#include <atomic>
#include <tuple>
#include <array>
#include <string>
#include <chrono>
#include "kitty.h"
#include "impl.h"

namespace MikhailDomchenkov{

  template<
    typename rettype, 
    size_t n_threads, 
    size_t kitty_legs, 
    typename... fArgs>
  class pool {

    using func_type = std::function<rettype(fArgs...)>;
    using args_type = std::tuple<fArgs...>;
    using dur_type = std::chrono::milliseconds;

  public:

    pool() :
      _stop{ false },
      _handler{nullptr}
    {}

    pool(func_type f):
      _stop{ false },
      _handler{ f }
    {
      std::fill(_rv.begin(), _rv.end(), 0u);
      std::fill(_t.begin(), _t.end(), std::chrono::milliseconds{ 0 });
      for (size_t i = 0u; i < n_threads; ++i) {
        auto f = [this](size_t idx) {this->worker(idx); };
        _pool[i] = std::thread(f, i);
      }
    }

    ~pool() {
      _kitty.KILL();
      _stop = true;
      for (auto& t : _pool) {
        t.join();
      }

      for (size_t i = 0u; i < n_threads; ++i) {
        std::cout << "[" << i << "] - " << _rv[i] 
          << " - " 
          << _t[i].count()
          << "ms"
          << std::endl;
      }
    };

    //template<typename... T>
    void add(fArgs... args) {
      if (_stop) return;
      _kitty.safe_push(std::tuple(args...));
    }

  private:

    void worker(size_t thread_idx) {
      
      struct timer {
        size_t _idx;
        std::array<dur_type, n_threads>& _t;
        timer(size_t idx, std::array<dur_type, n_threads>& t):
          _idx{ idx }, _t{ t }{}
        std::chrono::high_resolution_clock::time_point 
          time_begin = std::chrono::high_resolution_clock::now();
        ~timer() {
          auto time_end = std::chrono::high_resolution_clock::now();
          this->_t[_idx] = std::chrono::duration_cast
            <std::chrono::milliseconds>(time_end - time_begin);
        }
      } obj(thread_idx, _t);

      auto time_begin = std::chrono::high_resolution_clock::now();
      while (!_stop) {
        args_type task;
        _kitty.safe_pop(task);
        if (_stop) return;
        _rv[thread_idx]++;
      }
      //auto time_end = std::chrono::high_resolution_clock::now();
      //_t[thread_idx] = std::chrono::duration_cast
     //   <std::chrono::milliseconds>(time_end - time_begin);
     // std::cout << _t[thread_idx].count() << std::endl;
    }

    MikhailDomchenkov::kitty<args_type, kitty_legs> _kitty;
    std::atomic<bool> _stop;
    func_type _handler;
    std::array<std::thread, n_threads> _pool;
    std::array<size_t, n_threads> _rv;
    std::array<dur_type, n_threads> _t;
  };

}
#endif // !__POOL_H__
