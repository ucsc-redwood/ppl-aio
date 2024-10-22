#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <thread>

#include "host/dispatcher.hpp"
#include "shared/structures.h"
#include "third-party/BS_thread_pool_utils.hpp"
#include "third-party/CLI11.hpp"

// Problem size
constexpr auto n = 640 * 480;  // ~300k
// constexpr auto n = 1920 * 1080;  // ~2M
constexpr auto min_coord = 0.0f;
constexpr auto range = 1024.0f;
constexpr auto seed = 114514;

// demo config
constexpr auto n_iterations = 30;

void gen_data(const std::shared_ptr<struct pipe>& p) {
  std::mt19937 gen(seed);  // NOLINT(cert-msc51-cpp)
  std::uniform_real_distribution dis(min_coord, min_coord + range);
  std::generate_n(p->u_points, n, [&dis, &gen] {
    return glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);
  });
}

void demo_cpu_only(BS::thread_pool& pool, const int n_threads) {
  auto p = std::make_shared<struct pipe>(n, min_coord, range, seed);

  gen_data(p);

  BS::timer t;

  t.start();

  std::cout << "staring CPU only demo" << '\n';

  for (auto i = 0; i < n_iterations; ++i) {
    cpu::dispatch_ComputeMorton(pool, n_threads, p.get());
    cpu::dispatch_RadixSort(pool, n_threads, p.get());
    cpu::dispatch_RemoveDuplicates(pool, n_threads, p.get());
    cpu::dispatch_BuildRadixTree(pool, n_threads, p.get());
    cpu::dispatch_EdgeCount(pool, n_threads, p.get());
    cpu::dispatch_EdgeOffset(pool, n_threads, p.get());
    cpu::dispatch_BuildOctree(pool, n_threads, p.get());

    std::cout << '.' << std::flush;
  }
  std::cout << '\n';

  t.stop();

  // print total time and average time, 't.ms()'
  std::cout << "CPU only Total: " << t.ms() << "ms" << '\n';
  std::cout << "Average: " << t.ms() / n_iterations << "ms" << '\n';
}

int main(int argc, char** argv) {
  int n_threads = 1;
  int demo_id = 0;

  const auto max_threads =
      static_cast<int>(std::thread::hardware_concurrency());

  CLI::App app{"Demo for new hybrid version"};

  app.add_option("-t,--threads", n_threads, "Number of threads")
      ->check(CLI::Range(1, max_threads));
  app.add_option("-d,--demo", demo_id, "Demo id");

  CLI11_PARSE(app, argc, argv)

  std::cout << "Threads: " << n_threads << '\n';

  BS::thread_pool pool{static_cast<BS::concurrency_t>(n_threads)};

  demo_cpu_only(pool, n_threads);

  return 0;
}