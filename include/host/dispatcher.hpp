#pragma once

#include "shared/structures.h"
#include "third-party/BS_thread_pool.hpp"

namespace cpu {

void dispatch_ComputeMorton(BS::thread_pool& pool,
                            int n_threads,
                            struct pipe* p);

void dispatch_RadixSort(BS::thread_pool& pool, int n_threads, struct pipe* p);

void dispatch_RemoveDuplicates(BS::thread_pool& pool,
                               int n_threads,
                               struct pipe* p);

void dispatch_BuildRadixTree(BS::thread_pool& pool,
                             int n_threads,
                             struct pipe* p);

void dispatch_EdgeCount(BS::thread_pool& pool, int n_threads, struct pipe* p);

void dispatch_EdgeOffset(BS::thread_pool& pool, int n_threads, struct pipe* p);

void dispatch_BuildOctree(BS::thread_pool& pool, int n_threads, struct pipe* p);

}  // namespace cpu