#include <thread>
#include <vector>

void do_some_job() {
	std::this_thread::sleep_for(std::chrono::seconds{1});
}

// product.conf
//       "numThreads" : 4;

int main() {
	const size_t DEFAULT_NUM_THREADS = 4;

	std::vector<std::thread> threads;

	size_t numThreads = std::thread::hardware_concurrency();
	if (numThreads == 0) {
		numThreads = DEFAULT_NUM_THREADS;
	}

	// 32

	for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
		threads.emplace_back(std::thread{do_some_job});
	}

	for (auto& th : threads)
		th.join();

	return 0;
}