#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <utility>

#include "json.hpp"
// В json.hpp удалены все вхождения #include <ciso646>, иначе проблемы с с++20

using json = nlohmann::json;

struct Human {
	std::string name, surname, email;
	int age;
	int passport_id;

	friend std::istream& operator>>(std::istream& in, Human& human)
	{
		in >> human.name >> human.surname >> human.email >> human.age >> human.passport_id;
		return in;
	}
};

void to_json(json& j, const Human& human) {
	j = json{ {"name", human.name}, {"surname", human.surname}, {"email", human.email}, {"age", human.age}, {"passport_id", human.passport_id} };
}

int main()
{
	/* input example :
	3
	vasya pupkin abc@gmail.com 30 123145
	saul goodman asfaf@asdasd.asd 48 3121241
	ryan gosling ne_umer@v_drive.com 42 124982
	*/
	int number_of_humans;
	std::cin >> number_of_humans;

	std::string save_path = "people";
	std::filesystem::create_directory(save_path);

	std::vector<std::pair<Human, json>> people(number_of_humans);
	for (auto& human : people) {
		std::cin >> human.first;
		human.second = human.first;// автоматически вызывает to_json

		std::ofstream out(save_path + "/" + human.first.name + ".json");
		out << human.second;
		out.close();
	}

}