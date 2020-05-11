#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <execution>
#include<memory>
#include <utility>
#include <future>
#include <atomic>

#include "impl.h"
#include "kitty.h"
#include "pool.h"

//---------------------------------------------------------------

void lab1(size_t vector_size)
{
  std::vector<int> v(vector_size);

  std::transform(
    v.begin(),
    v.end(),
    v.begin(),
    [](const int&)
  {
    return rand() % 100;
  }
  );

  {
    //-sequenced_policy-//
    time_counter tc("sequenced_policy : ");

    std::for_each(
      std::execution::seq,
      v.begin(),
      v.end(),
      [](const int& n)
    {
      return n + 1;
    }
    );
  }

  {
    //-parallel_policy-//
    time_counter tc("parallel_policy : ");

    std::for_each(
      std::execution::par,
      v.begin(),
      v.end(),
      [](const int& n)
    {
      return n + 1;
    }
    );
  }

  {
    //-parallel_unsequenced_policy-//
    time_counter tc("parallel_unsequenced_policy : ");

    std::for_each(
      std::execution::par_unseq,
      v.begin(),
      v.end(),
      [](const int& n)
    {
      return n + 1;
    }
    );
  }
}

//---------------------------------------------------------------

void lab2()
{

  std::atomic<int> order_ctr{ 0 };

  struct ret_obj
  {
    std::string name;
    size_t order;
    size_t count;
  };

  auto func =
    [&order_ctr](const std::string & name, int n_obj)-> ret_obj
  {

    size_t order = order_ctr++;

    std::unique_ptr<obj_counter[]> objects
    = std::make_unique<obj_counter[]>(n_obj);

    return { name, order, obj_counter::max_count() };
  };

  std::vector<std::string> names =
  {
    "first",
    "second",
    "third",
    "fourth",
    "fiveth"
  };

  std::vector<std::future<ret_obj>> fut(names.size());

  for (size_t i = 0; i < names.size(); ++i)
  {
    fut[i] = std::async(
               std::launch::async,
               func,
               std::ref(names[i]),
               rand() % 100
             );
  }

  std::cout
      << "name: \t"
      << "order:\t"
      << "count:"
      << std::endl
      << std::endl;

  for (auto& f : fut)
  {
    auto obj = f.get();
    std::cout
        << obj.name << "\t"
        << obj.order << "\t"
        << obj.count
        << std::endl;
  }
}

//---------------------------------------------------------------

time_counter::~time_counter() noexcept
{

  auto time_end =
    std::chrono::high_resolution_clock::now();
  auto interval =
    std::chrono::duration_cast
    <std::chrono::milliseconds>
    (time_end - m_begin);

  std::cout
      << m_message
      << " time: "
      << interval.count()
      << "ms"
      << std::endl;
  std::cout.flush();
}// ! ~time_counter()

//---------------------------------------------------------------

thread_local std::atomic<int> obj_counter::m_instances{ 0 };

size_t obj_counter::max_count()
{
  return m_instances;
}

obj_counter::obj_counter()
{
  m_instances++;
}

//---------------------------------------------------------------

void lab3(size_t elements_count, unsigned threads_count)
{
  // assume correct input

  std::vector<int> v(elements_count);

  std::transform(
    v.begin(),
    v.end(),
    v.begin(),
    [](const int&)
  {
    return rand() % 25;
  }
  );

  {
    //-full sum-//
    int res = 0;
    {
      time_counter tc("full sum");
      res = std::accumulate(v.cbegin(), v.cend(), 0);
    }
    std::cout << " result: " << res << std::endl;
  }

  size_t elements_on_thread =
    elements_count / threads_count;

  size_t elements_rest =
    elements_count % threads_count;

  auto ps = [](
              std::vector<int>::iterator begin,
              size_t f,
              size_t l
            )->int
  {
    return std::accumulate(begin + f, begin + l, 0);
  };

  int sum = 0;

  {
    //-partial sum-//

    std::vector<std::future<int>> F(threads_count);

    if (elements_rest > 0)
      F.reserve(F.size() + 1);

    {
      //-inner scope-//

      time_counter tc("partial sum");

      for (size_t i = 0; i < F.size(); ++i)
      {
        F[i] = std::async(
                 ps,
                 v.begin(),
                 i * elements_on_thread,
                 i * elements_on_thread + elements_on_thread
               );
      }

      if (elements_rest > 0)
        F[F.size() - 1] = std::async(
                            ps,
                            v.begin(),
                            (F.size() - 1) * elements_on_thread,
                            elements_count
                          );

      for (auto& f : F)
      {
        sum += f.get();
      }

    } //-!inner_scope-//

    std::cout << " result: " << sum << std::endl;
  } //-!partial sum-//

}//-!lab3--//

//---------------------------------------------------------------

void lab6()
{
  auto global_test = []()
  {
    GlobalSingleton& gs1 = GlobalSingleton::get();
    GlobalSingleton& gs2 = GlobalSingleton::get();

    std::cout
//      << "-global-" << std::endl
        << gs1.id() << std::endl
        << gs2.id() << std::endl;
  };

  auto thread_test = []()
  {
    ThreadSingleton& ts1 = ThreadSingleton::get();
    ThreadSingleton& ts2 = ThreadSingleton::get();

    std::cout
//      << "-thread-" << std::endl
        << ts1.id() << std::endl
        << ts2.id() << std::endl;
  };

  //--------------------

  std::cout << "global singleton test..." << std::endl;

  std::vector<std::thread> globals(3);
  for (auto& t : globals)
  {
    t = std::thread(global_test);
  }
  for (auto& t : globals)
  {
    t.join();
  }

  //--------------------

  std::cout << "thread singleton test..." << std::endl;

  std::vector<std::thread> threads(3);
  for (auto& t : threads)
  {
    t = std::thread(thread_test);
  }
  for (auto& t : threads)
  {
    t.join();
  }
}

//---------------------------------------------------------------

std::atomic<long> GlobalSingleton::thread_id = 0;
thread_local std::atomic<long> ThreadSingleton::thread_id = 0;

//---------------------------------------------------------------

void lab4()
{

  auto danger_func = []()->int
  {
    std::string si;
    int i = 0;
    std::cout << "input an integer..." << std::endl;
    std::cin >> si;
    return std::stoi(si);
  };

  auto fut = std::async(std::launch::async, danger_func);

  try
  {
    int result = fut.get();
    std::cout << "result: " << result << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

//---------------------------------------------------------------

void lab5()
{

  MikhailDomchenkov::kitty<MikhailDomchenkov::tabby_color, 15> Tom;

  auto pusher = [&Tom]()
  {
    for (size_t i = 0u; i < 101; ++i)
    {
      Tom.safe_push(i);
    }
  };

  auto popper = [&Tom]()
  {
    for (size_t i = 0u; i < 100; ++i)
    {
      size_t v;
      Tom.safe_pop(v);
    }
  };

  auto wfut = std::async(std::launch::async, pusher);
  auto rfut = std::async(std::launch::async, popper);

  std::atomic_thread_fence(std::memory_order_seq_cst);
  std::cout << "Press any key to stop..." << std::endl;

  std::cin.get();

  //Tom.KILL();

  decltype(Tom) q1(Tom);
  decltype(Tom) q2 = Tom;

  wfut.get();
  rfut.get();

  std::cout << "stop - [ok]" << std::endl;

}

//---------------------------------------------------------------

void lab7()
{
  {
    //- inner scope -//


    using retval_type = int;
    using arg_type = std::vector<int>;

    constexpr size_t n_threads = 20u;
    constexpr size_t n_legs    = 40u;

    auto f = [](arg_type v)->retval_type
    {
      return std::accumulate(v.cbegin(), v.cend(), 0);
    };

    MikhailDomchenkov::pool <
    retval_type,
    n_threads,
    n_legs,
    arg_type
    > p{ f };

    for (int i = 0; i < 100; ++i)
    {
      std::vector<int> v((std::rand() % 10 + 1) * 10);
      std::transform(
        v.begin(),
        v.end(),
        v.begin(),
        [](const int&)
      {
        return rand() % 10;
      }
      );
      p.add(v);
    }
  } //- inner scope --//

  std::cout << "ready" << std::endl;

  std::cin.get();
  //p.add(5, 10.0f);


}

//---------------------------------------------------------------