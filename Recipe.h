#pragma once

#include <map>
#include <vector>
#include <iostream>


const std::map<std::string, std::vector<int>> RECIPE = {
	{"espresso", {7, 25, 0, 1500}},	//coffee, water, milk, price
	{"americano", {7, 225, 0, 2000}}, 
	{"latte", {7, 25, 200, 2300}}, 
	{"cappuccino", {14, 50, 200, 2500}}
};



// 단순 가격조회. (매개변수 user_menu가 없다면, 모두출력.)
void printPrice(std::vector<std::string> user_menu = {{"none"}}) {
	if (user_menu == (std::vector<std::string>) {{"none"}}) {
		user_menu.clear();
		for (const auto& pair : RECIPE) {
			user_menu.push_back(pair.first);
		}
	}
	std::cout << "가격표:  "; 
	for (const std::string& menu : user_menu) {
		// 여기에 바로 출력문이 있었고 함수 종료됐으나, 없는 메뉴 입력시 코어덤프 생기는 관계로 대충 디버그함.
		for (const auto& p : RECIPE) {
			if (p.first == menu) std::cout << menu << ": " << RECIPE.at(menu)[3] << "원   ";
		}
	}
}

