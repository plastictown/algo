#include "object_ctr.h"

#include <atomic>

template<typename value_type, typename mutex_type>
std::atomic<unsigned> object_counter<value_type, mutex_type>::_count = 0u;