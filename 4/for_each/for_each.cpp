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
#include <future>
#include <cmath>
#include <execution>
// Тут миллион библиотек, мне лень проверять какие реально нужны

using steady_clock_t = std::chrono::steady_clock;

template <typename T>
class Timer {
public:
	Timer(const std::string& message) : message(message), begin(steady_clock_t::now()) {}

	unsigned get_time() {
		return std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count();
	}

	~Timer() {
		std::cout << message << ": " << std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count() << std::endl;
	}
private:
	steady_clock_t::time_point begin;
	std::string message;
};

namespace my {
	// originally copied from https://en.cppreference.com/w/cpp/algorithm/for_each
	template<class InputIt, class UnaryFunction>
	constexpr UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
	{
		unsigned num_threads = std::thread::hardware_concurrency(), per_thread = (last - first)/num_threads;
		std::vector<std::shared_future<UnaryFunction>> futures;

		for (unsigned i = 0; i < num_threads - 1; i++) {
			futures.push_back(std::async(std::launch::async, std::for_each<InputIt, UnaryFunction>, first + i * per_thread, first + (i + 1) * per_thread, f));
		}
		futures.push_back(std::async(std::launch::async, std::for_each<InputIt, UnaryFunction>, first + (num_threads - 1)* per_thread, last, f));

		return f; // implicit move since C++11
	}
}

int main() {
	unsigned size = 10000000;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d(1, 10);

	std::vector<int> randoms;

	std::generate_n(std::back_inserter(randoms), size, [&gen, &d]() {return d(gen); });
	std::cout << "numbers generated, doing for_each" << std::endl;

	auto test_lambda = [](int a) {
		long ans = 0;
		for (int i = 0; i < 20; i++)
			ans += a * cos(a) * sin(a) + exp(a);
	};

	{
		Timer<std::chrono::milliseconds> t("Parallel, my function");
		my::for_each(randoms.begin(), randoms.end(), test_lambda);
	}

	{
		Timer<std::chrono::milliseconds> t("Sequential, std function");
		std::for_each(randoms.begin(), randoms.end(), test_lambda);
	}

	{
		Timer<std::chrono::milliseconds> t("Parallel, std function");
		std::for_each(std::execution::par, randoms.begin(), randoms.end(), test_lambda);
	}
	
}
