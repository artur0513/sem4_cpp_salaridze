#include <iostream>
#include <chrono>
#include <cmath>

template <typename T>
class Timer {
private:
	std::chrono::steady_clock::time_point start;
	T time{ 0 };
	bool running = true;

	void update() {
		time += std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - start) * running;
		start = std::chrono::steady_clock::now();
	}
public:
	Timer() : start(std::chrono::steady_clock::now()) {}

	void pause() {
		update();
		running = false;
	}

	void resume() {
		update();
		running = true;
	}

	void print() {
		update();
		std::cout << time.count() << '\n';
	}

	~Timer() {
		print();
	}
};

void test() { // Для тестов
	double count = 0.0;
	for (auto i = 0; i < 1'000'000; i++) {
		count += std::sin(i) + std::cos(i);
	}
	std::cout << "test() complete \n";
};

int main() {
	{
		std::cout << "Simple timer test: \n \n";
		Timer<std::chrono::microseconds> t;
		test();
		std::cout << "timer was going, time: \n";
		t.print();
		std::cout << '\n';

		t.pause();
		std::cout << "than we pause timer \n";
		test();
		t.print();
		std::cout << '\n';

		std::cout << "than we resume timer again and do one more test \n";
		t.resume();
		test();
	}
	std::cout << "\n \n";
	{
		std::cout << "Test #2, it shows that calling timer::pause() multiple times in a row works fine: \n \n";
		Timer<std::chrono::microseconds> t;
		t.pause();
		test();
		t.print();

		t.pause();
		test();
		t.print();

		t.pause();
		test();
	}
	std::cout << "\n \n";
	{
		std::cout << "Test #3, with milliseconds: \n \n";
		Timer<std::chrono::milliseconds> t;
		test();
	}
}
