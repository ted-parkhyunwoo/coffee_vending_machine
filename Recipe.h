#pragma once

#include <map>
#include <vector>
#include <iostream>

enum class mName { coffee, water, milk, price} ;

class Recipe {
protected:
	std::string name;
	int info[4];

public:
	Recipe(const std::string& name, int coffee, int water, int milk, int price) {
		this->name = name;
		info[static_cast<int>(mName::coffee)] = coffee;
		info[static_cast<int>(mName::water)] = water;
		info[static_cast<int>(mName::milk)] = milk;
		info[static_cast<int>(mName::price)] = price;
	}

	const std::string& getName() const {
		return name;
	}
	
	const int& getInfo(mName material) const {
		return info[static_cast<int>(material)];
	}
};


// 음료 레시피, 가격 초기화. (새로운메뉴는 여기에 넣으세요.)
const Recipe MENUS[] = {
	//! name, coffee, water, milk, price
	Recipe("espresso", 7, 25, 0, 1500),
	Recipe("americano", 7, 225, 0, 2000),
	Recipe("latte", 7, 25, 200, 2300),
	Recipe("cappuccino", 14, 50, 200, 2500)
};


// 메뉴 이름으로 재료정보를 가져오는 함수.
const int& getRecipeInfo(const std::string& menu, mName material) {
	for (auto& recipe : MENUS) {
		if (recipe.getName() == menu) {
			return recipe.getInfo(material);
		}
	}
	std::cerr << "[ERROR] 메뉴를 찾을 수 없습니다: " << menu << std::endl;
	std::exit(1);
}


// 단순 가격조회. (매개변수 user_menu가 없다면, 모두출력.)
void printPrice(std::vector<std::string> user_menu = {}) {
    if (user_menu.empty()) {
        for (const auto& recipe : MENUS) {
            user_menu.push_back(recipe.getName());
        }
    }

    std::cout << "[INFO] 가격표: ";
    for (const std::string& menu : user_menu) {
        std::cout << menu << ": " << getRecipeInfo(menu, mName::price) << "원   ";
    }
    std::cout << std::endl;
}