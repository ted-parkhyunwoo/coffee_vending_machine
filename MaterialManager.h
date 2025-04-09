#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include "Recipe.h"


class MaterialManager {
protected:
	int coffee;
	int water;
	int milk;

public:
	MaterialManager() {};
	MaterialManager(int amount): coffee{amount}, water{amount}, milk{amount} {};

	// 그냥 재료를 차감하는 함수.
	void useMaterials(int coffee = 0, int water = 0, int milk = 0) {
		// 재료부족시 경고 후 리턴.
		if (coffee > this->coffee || water > this->water || milk > this->milk) {
			std::cout << "[ERROR] Not Enough Materials.";
			return;
		}
		this->coffee -= coffee;
		this->water -= water;
		this->milk -= milk;
	}

	// 재료 상황 표시기.
	void report_material() {
		std::cout << "Material Report: [" ;
		std::cout << "Coffee: " << coffee << "   Water: " << water 
		<< "   Milk: " << milk << "]" << std::endl;
	}

	// 재료 추가. (커피,물,우유 순으로 매개변수 입력. 기본값 = 0.)
	void addMaterials(int coffee = 0, int water = 0, int milk = 0) {
		this->coffee += coffee;
		this->water += water;
		this->milk += milk;
		std::cout << "ADD : [";
		std::cout << "Coffee: " << coffee << "  Water: " << water 
		<< "  Milk: " << milk << "]" << std::endl;
	}

	// 메뉴 제작 가능 검토함수.
	bool isEnough(std::string menu_name, bool noPrint = false) {
		bool result = true;
		std::vector<std::string> notEnoughMaterials;
		// check
		if (getRecipeInfo(menu_name, Material::coffee) > coffee) {
			notEnoughMaterials.push_back("Coffee:" + std::to_string(getRecipeInfo(menu_name, Material::coffee) - coffee) + "g");
			result = false;
		}
		if (getRecipeInfo(menu_name, Material::water) > water) {
			notEnoughMaterials.push_back("Water:" + std::to_string(getRecipeInfo(menu_name, Material::water) - water) + "ml");
			result = false;
		}
		if (getRecipeInfo(menu_name, Material::milk) > milk) {
			notEnoughMaterials.push_back("Milk:" + std::to_string(getRecipeInfo(menu_name, Material::milk) - milk) + "ml");
			result = false;
		}


		if (!result && !noPrint) {
			std::cout << "[MaterialManager::isEnough 알림] " << menu_name << "제작에 부족한 재료: [";
			for (const auto& s : notEnoughMaterials) {
				std::cout << s;
				if (s != notEnoughMaterials.back()) std::cout << ", ";
			}
			std::cout << "]" <<std::endl;
		}
		return result;
	}

	// 재료에 따라 가능한 음료, 불가능한 음료 벡터 리턴 함수. (MachineController 에서 관리.)
	std::vector<std::string> validMaterialMenu(bool isEnoughMaterial = true) {
		std::vector<std::string> result;
		std::vector<std::string> possibleMenu;	//제작가능	
		std::vector<std::string> impossibleMenu;	//불가능
		for (const auto& menu : MENUS) {
			if (isEnough(menu.getName(), true)) possibleMenu.push_back(menu.getName());
			else impossibleMenu.push_back(menu.getName());
		}
		// isEnoughMaterial 이 true면 제작가능한 메뉴, false면 불가능한 메뉴 리턴.
		if (isEnoughMaterial) return possibleMenu;
		else return impossibleMenu;
	}

};

#endif

