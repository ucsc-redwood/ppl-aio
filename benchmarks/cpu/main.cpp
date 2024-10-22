#include <benchmark/benchmark.h>

#include <glm/glm.hpp>
#include <memory>
#include <random>
#include <thread>

#include "host/dispatcher.hpp"

// Problem size
constexpr auto n = 640 * 480;  // ~300k
// constexpr auto n = 1920 * 1080;  // ~2M
constexpr auto min_coord = 0.0f;
constexpr auto range = 1024.0f;
constexpr auto seed = 114514;

// Benchmark config
constexpr auto n_iterations = 50;

void gen_data(const std::unique_ptr<struct pipe>& p) {
  std::mt19937 gen(seed);  // NOLINT(cert-msc51-cpp)
  std::uniform_real_distribution dis(min_coord, min_coord + range);
  std::generate_n(p->u_points, n, [&dis, &gen] {
    return glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);
  });
}

class CPU : public benchmark::Fixture {
 public:
  explicit CPU()
      : p(std::make_unique<struct pipe>(n, min_coord, range, seed)),
        pool(std::thread::hardware_concurrency()) {
    gen_data(p);

    // basically pregenerate the data
    cpu::dispatch_ComputeMorton(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_RadixSort(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_RemoveDuplicates(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_BuildRadixTree(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_EdgeCount(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_EdgeOffset(pool, pool.get_thread_count(), p.get());
    cpu::dispatch_BuildOctree(pool, pool.get_thread_count(), p.get());
  }

  std::unique_ptr<struct pipe> p;
  BS::thread_pool pool;
};

// ----------------------------------------------------------------------------
// Morton code
// ----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CPU, BM_Morton)(benchmark::State& state) {
  const auto n_threads = state.range(0);

  for (auto _ : state) {
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
  }
}

BENCHMARK_REGISTER_F(CPU, BM_Morton)
    ->DenseRange(1, std::thread::hardware_concurrency(), 1)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(n_iterations);

// ----------------------------------------------------------------------------
// Radix sort
// ----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CPU, BM_Sort)(benchmark::State& state) {
  const auto n_threads = state.range(0);

  for (auto _ : state) {
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
  }
}

BENCHMARK_REGISTER_F(CPU, BM_Sort)
    ->DenseRange(1, std::thread::hardware_concurrency(), 1)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(n_iterations);

// ----------------------------------------------------------------------------
// Remove duplicates
// ----------------------------------------------------------------------------

BENCHMARK_DEFINE_F(CPU, BM_RemoveDuplicates)(benchmark::State& state) {
  const auto n_threads = state.range(0);

  for (auto _ : state) {
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
  }
}

BENCHMARK_REGISTER_F(CPU, BM_RemoveDuplicates)
    ->DenseRange(1, std::thread::hardware_concurrency(), 1)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(n_iterations);

BENCHMARK_MAIN();
