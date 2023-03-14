#include <thread>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <fstream>
#include <string>

using steady_clock_t = std::chrono::steady_clock;

template <typename T>
class Timer {
public:
	Timer(const std::string& message) : message(message), begin(steady_clock_t::now()) {}

	unsigned get_time() {
		return std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count();
	}

	~Timer() {
		//std::cout << message << ": " << std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count() << std::endl;
	}
private:
	steady_clock_t::time_point begin;
	std::string message;
};

template<typename Iter, typename T>
void accumulate_block(Iter begin, Iter end, T init, T& result) {
	result = std::accumulate(begin, end, init);
}

template<typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init, unsigned num_threads) {
	//auto num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	std::vector<T> results(num_threads - 1);
	auto block_size = std::distance(begin, end) / num_threads;
	for (auto i = 0u; i + 1 < num_threads; i++) {
		threads.push_back(std::thread(
			accumulate_block<Iter, T>,
			std::next(begin, i * block_size),
			std::next(begin, (i + 1) * block_size),
			0,
			std::ref(results[i]))
		);
	}
	T last_result;
	accumulate_block(std::next(begin, (num_threads - 1) * block_size),
		end, init, last_result);
	std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
	return std::accumulate(std::begin(results), std::end(results), last_result);
}

int main() {
	unsigned size = 30000000, repeat = 100;
	std::ofstream output("output.txt");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d(1, 10);

	std::vector<int> randoms;
	
	std::generate_n(std::back_inserter(randoms), size, [&gen, &d]() {return d(gen); });

	auto parallel_test = [&](unsigned num_threads) {
		Timer<std::chrono::milliseconds> t("Parallel");
		auto result = parallel_accumulate(randoms.begin(), randoms.end(), 0, num_threads);
		return t.get_time();
	};

	for (unsigned i = 1; i < std::thread::hardware_concurrency() * 3; i++) {
		unsigned sum_time = 0u;
		for (unsigned j = 0; j < repeat; j++) {
			sum_time += parallel_test(i);
		}
		double avg_time = double(sum_time) / double(repeat);
		std::cout << "Avg time parallel " << i << " threads: " << avg_time << std::endl;
		output << i << " " << avg_time << std::endl;
	}

	output.close();
}
