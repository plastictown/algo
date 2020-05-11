#pragma once

#ifndef __TRACER_HPP__
#define __TRACER_HPP__

#include <iostream>
#include <string>
#include <chrono>

class Tracer
{
public:
  Tracer(const std::string& s, bool header = true, bool footer = true) :
    _s{ s },
    _start{ std::chrono::high_resolution_clock::now() },
    _print_header{ header },
    _print_footer{ footer }
  {
    if (_print_header)
      std::cout << _s << ": [start]" << std::endl;
  }
  ~Tracer()
  {
    if (!_print_footer)
      return;
    decltype(_start) _end{ std::chrono::high_resolution_clock::now() };
    auto interval =
      std::chrono::duration_cast
      <std::chrono::milliseconds>
      (_end - _start);
    std::cout << _s << ": [end] - " << interval.count() << "ms" << std::endl;
  }
private:
  std::string _s;
  std::chrono::high_resolution_clock::time_point _start;
  bool _print_header;
  bool _print_footer;
};

#ifdef DEBUG_TRACE
#define TRACE Tracer __t__ (__FUNCTION__)
#define TRACEP(h,f) Tracer __t__ (__FUNCTION__,(h),(f))
#else
#define TRACE
#define TRACEP(h,f)
#endif // !DEBUG_TRACE

#endif // !__TRACER_HPP__



