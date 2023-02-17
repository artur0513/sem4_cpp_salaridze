#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>
#include <unordered_set>

#define range(container) std::begin(container), std::end(container)

std::ostream& operator<<(std::ostream& os, std::vector<int> v) {
	std::copy(range(v), std::ostream_iterator<int>(os, " "));
	return os;
}

std::istream& operator>>(std::istream& is, std::vector<int>& v) {
	std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(), std::back_inserter(v));
	return is;
}

int main() {
	std::vector<int> p1(10);


	std::generate(range(p1), []() {static int i = 1; return i++; }); // Задание 1 Создайте последовательность П1 целых чисел от 1 до 10;
	std::cout << "Task 1:   " << p1 << '\n';


	std::cin >> p1; // Задание 2 Добавьте еще несколько чисел в конец П1 из cin;
	std::cout << "Task 2:   " << p1 << '\n';


	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(range(p1), rd); 
	std::cout << "Task 3:   " << p1 << '\n'; // Задание 3  Перемешайте П1 случайным образом;


	std::unordered_set<int> temp_set(range(p1));
	p1.assign(range(temp_set)); //!!!!!! Порядок слегка теряется, не знаю насколько это легально !!!!!!
	std::cout << "Task 4:   " << p1 << '\n'; // Задание 4 Удалите дубликаты из П1;


	std::cout << "Task 5:   " << "number of odd numbers: " << std::count_if(range(p1), [](int i) {return i % 2; }) << '\n'; // Задание 5 Подсчитайте количество нечетных чисел в П1;


	std::cout << "Task 6:   " << "max number is:" << *std::max_element(range(p1)) << '\n';
	std::cout << "min number is:" << *std::min_element(range(p1)) << '\n';// Задание 6 Определите минимальное и максимальное значения в П1;


	auto is_prime = [](int num) {
		if (num <= 1)
			return false;

		for (int i = 2; i <= sqrt(num); i++) {
			if (num % i == 0)
				return false;
		}
		return true;
	};
	std::cout << "Task 7:   ";
	auto first_prime = std::find_if(range(p1), is_prime);
	if (first_prime != p1.end())
		std::cout << "Prime number found " << *first_prime << '\n';
	else
		std::cout << "No prime numbers found" << '\n';// Задание 7 Попробуйте найти хотя бы одно простое число в П1;


	std::transform(range(p1), p1.begin(), [](int i) {return i * i; });
	std::cout << "Task 8:   " << p1 << '\n'; // Задание 8 Замените все числа в П1 их квадратами;


	std::vector<int> p2(p1.size());
	auto random_num = []() {
		return rand() % 100 - 50;
	};
	std::generate(range(p2), random_num); // Задание 9 Создайте последовательность П2 из N случайных чисел, где N - длина П1;
	std::cout << "Task 9:   " << p2 << '\n';


	struct Sum
	{
		void operator()(int n) { sum += n; }
		int sum{ 0 };
	};
	Sum s = std::for_each(range(p2), Sum());
	std::cout << "Task 10:   " << "sum of elements in p2: " << s.sum << '\n'; // Задание 10 Вычислите сумму чисел в П2


	std::fill_n(p2.begin(), 3, 1); // Меняем на 1 первые 3 элемента
	std::cout << "Task 11:   " << p2 << '\n'; // Задание 11 Замените первые несколько чисел в П2 числом 1;


	std::vector<int> p3(p1.size());
	for (size_t i = 0; i < p3.size(); i++)
		p3[i] = p1[i] - p2[i];
	std::cout << "Task 12:   " << p3 << '\n'; // Задание 12 Создайте последовательность ПЗ как разность П1 и П2;


	std::replace_if(range(p3), [](int i) {return i < 0; }, 0);
	std::cout << "Task 13:   " << p3 << '\n'; // Задание 13 Заменить каждый отрицательный элемент в ПЗ нулем;


	auto last2 = std::remove_if(range(p3), [](int i) {return i == 0; });
	p3.erase(last2, p3.end());
	std::cout << "Task 14:   " << p3 << '\n'; // Задание 14 Удалите полностью все нулевые элементы из ПЗ;


	std::reverse(range(p3));
	std::cout << "Task 15:   " << p3 << '\n'; // Задание 15 Измените порядок следования элементов в ПЗ на обратный;


	std::partial_sort(p3.begin(), p3.begin() + 3, p3.end(), [](int x, int y) {return x > y; });
	std::cout << "Task 16:   " << "3 max elements are: " << p3[0] << " " << p3[1] << " " << p3[2] << '\n';// Задание 16 Определите быстро топ-3 наибольших элемента в ПЗ;


	std::sort(range(p1));
	std::sort(range(p2));
	std::cout << "Task 17:   " << p1 << '\n';
	std::cout << p2 << '\n'; // Задание 17 Отсортируйте полностью П1 и П2 по возрастанию;


	std::vector<int> p4;
	std::merge(range(p1), range(p2), std::back_inserter(p4));
	std::cout << "Task 18:   " << p4 << '\n'; // Задание 18 Создайте последовательность П4 как слияние П1 и П2;


	auto first_insertion_pos = std::find_if(range(p4), [](int i) {return i >= 1; }) - p4.begin();
	auto last_insertion_pos = std::find_if(range(p4), [](int i) {return i > 1; }) - p4.begin();
	std::cout << "Task 19:   " << "positions where we can insert number 1: [" << first_insertion_pos << ", " << last_insertion_pos << ")" << '\n'; // Задание 19  Определите диапазон для упорядоченной вставки числа 1 в П4;


	std::cout << "Task 20:   " << "All sequences output: \n";// Задание 20 Выведите все последовательности в cout.
	std::cout << p1 << '\n';
	std::cout << p2 << '\n';
	std::cout << p3 << '\n';
	std::cout << p4 << '\n';
}
