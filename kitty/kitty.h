#pragma once

#ifndef _TS_QUEUE__
#define __TS_QUEUE__

#include <iostream>
#include <memory>
#include <array>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <type_traits>

//#define DEBUG_TRACE

#include "tracer.hpp"
#include "locker.h"

namespace MikhailDomchenkov {

	using tabby_color = size_t;
	using one_color = char;

	template<typename color, size_t legs_count = 4>
	class kitty {
		using cv = std::condition_variable;
		using mutex_type = locker; //std::mutex;
		using lock_type = std::unique_lock<mutex_type>;
		using this_type = kitty<color, legs_count>;
		using _ms = std::chrono::milliseconds;
		//- INTERFACE -//
	public:

		kitty() :
			m_h{ 0 },
			m_t{ 0 },
			b_stop{ false } {}
		~kitty() { KILL(); }

		kitty(const this_type& rhs) {
			std::unique_lock<decltype(rhs.m_m)> lock(rhs.m_m);
			m_q = rhs.m_q;
			m_h = rhs.m_h;
			m_t = rhs.m_t;
			b_stop = false;
		}

		this_type& operator = (const this_type& rhs) {
			if (this == &rhs)
				return *this;
			KILL();
			std::unique_lock<decltype(m_m)> lock_me(m_m, std::defer_lock);
			std::unique_lock<decltype(rhs.m_m)> lock_rhs(rhs.m_m, std::defer_lock);

			std:lock(lock_me, lock_rhs);

			m_q = rhs.m_q;
			m_h = rhs.m_h;
			m_t = rhs.m_t;
			b_stop = false;
		}

	private:
		lock_type safe_size(size_t& out) const {
			//TRACE;
			lock_type l{ m_m };
			out = unsafe_size();
			return l;
		}

		lock_type safe_empty(bool& out) const {
			//TRACE;
			lock_type l{ m_m };
			out = unsafe_empty();
			return l;
		}

		lock_type safe_full(bool& out) const {
			//TRACE;
			lock_type l{ m_m };
			out = unsafe_full();
			return l;
		}
	public:
		//- PUSH -//
		template<typename T>
		bool safe_push(T&& val) {
			TRACE;
			if (b_stop) return false;
			bool full = false;
			auto lock = safe_full(full);
			if (full) 
				if (cv_r.wait_for(lock, _ms{ 100 }) == std::cv_status::timeout) 
					return false;
			if (b_stop) return false;
			unsafe_push(std::forward<T>(val));
			cv_r.notify_one();
			return true;
		}

		//- POP -//
		bool safe_pop(color& out) {
			TRACE;
			if (b_stop) return false;
			bool empty = false;
			auto lock = safe_empty(empty);
			if (empty) 
				if (cv_r.wait_for(lock, _ms{100}) == std::cv_status::timeout)
					return false;
			if (b_stop) return false;
			bool rv = unsafe_pop(out);
			cv_w.notify_one();
			return rv;
		}

		//- KILL HIM !!! -//
		void KILL() {
			b_stop = true;
			cv_w.notify_all();
			cv_r.notify_all();
		}

	 //- backend -//
	private:
		size_t next(size_t idx) const { return (idx + 1) % legs_count; }

		size_t unsafe_size() const {
			if (m_t < m_h)
				return legs_count - m_h + m_t;
			return m_t - m_h;
		}
		bool unsafe_empty() const { return m_h == m_t; }
		bool unsafe_full() const { return m_h == next(m_t); }

		template<typename T>
		bool unsafe_push(T&& val) {
			if (unsafe_full())
				return false;
			m_q[m_t] = std::forward<T>(val);
			m_t = next(m_t);
			return true;
		}

		bool unsafe_pop(color& out) {
			if (unsafe_empty())
				return false;
			out = std::move(m_q[m_h]);
			m_h = next(m_h);
			return true;
		}

	//- data fields -//
	private:

		size_t m_h, m_t; // <-- offsets
		std::array<color, legs_count> m_q;
		mutable mutex_type m_m;
		cv cv_r;
		cv cv_w;
		bool b_stop;
	};

}
#endif // !__TS_QUEUE__