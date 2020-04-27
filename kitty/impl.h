#pragma once

#include <string>
#include <chrono>
#include <atomic>
#include <sstream>
#include <string>
#include <thread>

#ifndef __LAB2_IMPL_H__
#define __LAB2_IMPL_H__

void lab1(size_t vector_size);
void lab2();
void lab3(size_t element_count, unsigned threads_count);
void lab6();
void lab4();
void lab5();
void lab7();

//---------------------------------------------------------------

struct time_counter {
public:

	time_counter() :
		m_begin{ std::chrono::high_resolution_clock::now() }
	{}

	time_counter(const std::string& msg) :
		m_message(msg),
		m_begin{ std::chrono::high_resolution_clock::now() }
	{}

	~time_counter() noexcept;
private:
	std::chrono::high_resolution_clock::time_point m_begin;
	std::string m_message;
};

//---------------------------------------------------------------

class obj_counter {
public:
	obj_counter();
	static size_t max_count();
private:
	thread_local static std::atomic<int> m_instances;
};

//---------------------------------------------------------------

class GlobalSingleton {
public:
	static GlobalSingleton& get() {
		if (thread_id == 0) {
			std::stringstream ss;
			ss << std::this_thread::get_id();
			thread_id = std::stol(ss.str());
		}	
		static GlobalSingleton instance;
		return instance;
	}
	static std::string moo() {
		return "Moooo!";
	}
	static long id() {
		return thread_id;
	}
private:
	GlobalSingleton() {}
	GlobalSingleton(const GlobalSingleton&) = delete;
	GlobalSingleton(const GlobalSingleton&&) = delete;
	~GlobalSingleton() {}
	//-why not?
	GlobalSingleton& operator=(const GlobalSingleton&) = delete;
	GlobalSingleton& operator=(const GlobalSingleton&&) = delete;
	static std::atomic<long> thread_id;
};

//---------------------------------------------------------------

class ThreadSingleton {
public:
	static ThreadSingleton& get() {
		if (thread_id == 0) {
			std::stringstream ss;
			ss << std::this_thread::get_id();
			thread_id = std::stol(ss.str());
		}
		thread_local static ThreadSingleton instance;
		return instance;
	}
	static std::string meow() {
		return "Meow!";
	}
	static long id() {
		return thread_id;
	}
private:
	ThreadSingleton() {}
	ThreadSingleton(const ThreadSingleton&) = delete;
	ThreadSingleton(const ThreadSingleton&&) = delete;
	~ThreadSingleton() {}
	//-one more time: why not?
	ThreadSingleton& operator=(const ThreadSingleton&) = delete;
	ThreadSingleton& operator=(const ThreadSingleton&&) = delete;
	thread_local static std::atomic<long> thread_id;
};

//---------------------------------------------------------------

#endif // !__LAB2_IMPL_H__