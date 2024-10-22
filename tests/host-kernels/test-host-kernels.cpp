// Include necessary headers
#include <gtest/gtest.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <random>
#include <thread>

#include "gtest/gtest.h"
#include "host/dispatcher.hpp"
#include "shared/structures.h"
#include "third-party/BS_thread_pool.hpp"

// Constants for data generation
constexpr auto n = 640 * 480;  // ~300k
constexpr auto min_coord = 0.0f;
constexpr auto range = 1024.0f;
constexpr auto seed = 114514;

void gen_data(const std::unique_ptr<struct pipe>& p) {
  std::mt19937 gen(seed);  // NOLINT(cert-msc51-cpp)
  std::uniform_real_distribution dis(min_coord, min_coord + range);
  std::generate_n(p->u_points, n, [&dis, &gen] {
    return glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);
  });
}

// Don't use fixtures, it cause segfaults because of thread pool
#define SETUP_PIPE_AND_POOL                                          \
  auto p = std::make_unique<struct pipe>(n, min_coord, range, seed); \
  gen_data(p);                                                       \
  BS::thread_pool pool(std::thread::hardware_concurrency());

TEST(ComputeMortonRunsWithoutException, ComputeMortonRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get()));
}

TEST(RadixSortRunsWithoutException, RadixSortRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());

  EXPECT_NO_THROW(
      cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get()));

  // Check if the Morton codes are sorted
  auto is_sorted = std::is_sorted(p->getSortedKeys(), p->getSortedKeys() + n);
  EXPECT_TRUE(is_sorted);
}

TEST(RemoveDuplicatesRunsWithoutException,
     RemoveDuplicatesRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get()));
}

// Radix Tree
TEST(BuildRadixTreeRunsWithoutException, BuildRadixTreeRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get());

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_BuildRadixTree(pool, pool.get_thread_count(), p.get()));
}

// edge count
TEST(CountEdgesRunsWithoutException, CountEdgesRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_BuildRadixTree(pool, pool.get_thread_count(), p.get());

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_EdgeCount(pool, pool.get_thread_count(), p.get()));
}

// edge offset
TEST(ComputeEdgeOffsetRunsWithoutException,
     ComputeEdgeOffsetRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_BuildRadixTree(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_EdgeCount(pool, pool.get_thread_count(), p.get());

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_EdgeOffset(pool, pool.get_thread_count(), p.get()));
}

// octree
TEST(BuildOctreeRunsWithoutException, BuildOctreeRunsWithoutException) {
  SETUP_PIPE_AND_POOL

  // Call prerequisite functions
  cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_BuildRadixTree(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_EdgeCount(pool, pool.get_thread_count(), p.get());
  cpu::dispatch_EdgeOffset(pool, pool.get_thread_count(), p.get());

  // Run the function under test and check for exceptions
  EXPECT_NO_THROW(
      cpu::dispatch_BuildOctree(pool, pool.get_thread_count(), p.get()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
