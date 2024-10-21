// Include necessary headers
#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <memory>
#include <random>
#include <thread>

#include "host/dispatcher.hpp"
#include "shared/structures.h"
#include "third-party/BS_thread_pool.hpp"

// Constants for data generation
constexpr auto n = 640 * 480;  // ~300k
constexpr auto min_coord = 0.0f;
constexpr auto range = 1024.0f;
constexpr auto seed = 114514;

void gen_data(const std::shared_ptr<struct pipe>& p) {
  std::mt19937 gen(seed);  // NOLINT(cert-msc51-cpp)
  std::uniform_real_distribution dis(min_coord, min_coord + range);
  std::generate_n(p->u_points, n, [&dis, &gen] {
    return glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);
  });
}

// Test fixture class for setting up common resources
class DispatchTest : public ::testing::Test {
 protected:
  void SetUp() override {
    n_threads = std::thread::hardware_concurrency();
    p = std::make_shared<struct pipe>(n, min_coord, range, seed);
    gen_data(p);
  }

  void TearDown() override {
    pool.wait();
    p.reset();
  }

  bool isSorted(const uint32_t* data, size_t size) {
    return std::is_sorted(data, data + size);
  }

  BS::thread_pool pool;
  int n_threads;
  std::shared_ptr<struct pipe> p;
};

// Test for dispatch_ComputeMorton
TEST_F(DispatchTest, ComputeMortonRunsWithoutException) {
  EXPECT_NO_THROW({ cpu::dispatch_ComputeMorton(pool, n_threads, p.get()); });
}

// Test for dispatch_RadixSort
TEST_F(DispatchTest, RadixSortRunsWithoutException) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
  });
}

// Test for dispatch_RadixSort to verify sorting correctness
TEST_F(DispatchTest, RadixSortCorrectness) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
  });

  // Verify that the morton codes are sorted
  bool sorted = isSorted(p->getSortedKeys(), p->n_input());
  EXPECT_TRUE(sorted) << "The morton codes are not sorted correctly.";
}

// Test for dispatch_RemoveDuplicates
TEST_F(DispatchTest, RemoveDuplicatesRunsWithoutException) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
  });
}

// TEST_F(DispatchTest, RemoveDuplicatesCorrectness) {
//   // Run the necessary preceding functions
//   ASSERT_NO_THROW({
//     cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
//     cpu::dispatch_RadixSort(pool, n_threads, p.get());
//   });

//   // Capture the sorted morton codes before removing duplicates
//   std::vector<uint32_t> sorted_morton_codes(p->getSortedKeys(),
//                                             p->getSortedKeys() +
//                                             p->n_input());

//   // Run the RemoveDuplicates function
//   ASSERT_NO_THROW(
//       { cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get()); });

//   // Use std::unique to get the expected unique morton codes
//   auto unique_end =
//       std::unique(sorted_morton_codes.begin(), sorted_morton_codes.end());
//   sorted_morton_codes.erase(unique_end, sorted_morton_codes.end());

//   // Compare sizes
//   EXPECT_EQ(p->getUniqueKeys(), sorted_morton_codes.size())
//       << "The number of unique morton codes does not match.";

//   // Compare the unique morton codes
//   bool match = std::equal(p->getUniqueKeys(),
//                           p->getUniqueKeys() + p->n_unique_mortons(),
//                           sorted_morton_codes.begin());
//   EXPECT_TRUE(match)
//       << "The unique morton codes do not match the expected result.";
// }

// Test for dispatch_BuildRadixTree
TEST_F(DispatchTest, BuildRadixTreeRunsWithoutException) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
  });
}

// Test for dispatch_EdgeCount
TEST_F(DispatchTest, EdgeCountRunsWithoutException) {
  EXPECT_NO_THROW({
    // cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    // cpu::dispatch_RadixSort(pool, n_threads, p.get());
    // cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    // cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
    // cpu::dispatch_EdgeCount(pool, n_threads, p.get());

    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
    cpu::dispatch_EdgeCount(pool, n_threads, p.get());
    // cpu::dispatch_EdgeOffset(pool, n_threads, p.get());
    // cpu::dispatch_BuildOctree(pool, n_threads, p.get());
  });
}

// Test for dispatch_EdgeOffset
TEST_F(DispatchTest, EdgeOffsetRunsWithoutException) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
    cpu::dispatch_EdgeCount(pool, n_threads, p.get());
    cpu::dispatch_EdgeOffset(pool, n_threads, p.get());
  });
}

// Test for dispatch_BuildOctree
TEST_F(DispatchTest, BuildOctreeRunsWithoutException) {
  EXPECT_NO_THROW({
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
    cpu::dispatch_EdgeCount(pool, n_threads, p.get());
    cpu::dispatch_EdgeOffset(pool, n_threads, p.get());
    cpu::dispatch_BuildOctree(pool, n_threads, p.get());
  });
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
