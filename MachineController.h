#pragma once

#include "CashManager.h"
#include "MaterialManager.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>
#include "Recipe.h"

// 동전관리, 재료관리, 레시피관리, (앞으로 추가할 커피머신) 등을 통합관리하는 매니저.
class MachineController : public CashManager, public MaterialManager {

private:	
	//! 화폐 추가시 CashManager.h 에서 전체적인 로직수정이 필요함.
	const std::vector<int> VALID_COINS = {10, 50, 100, 500};			//1000원 지폐 있었으나, 기능 삭제.
	std::vector<std::string> possible_order = validMaterialMenu();		//초기화는 재료 유효성검사된 벡터
	std::map<std::string, int> sales_record;							//판매기록.

public:
	// 기본생성자에서 바로 기본클래스(Base class: 상속받은 두 클래스)로 넘김.
	MachineController(): CashManager(), MaterialManager() {}
	MachineController(int first_run_amount, int amount): CashManager(first_run_amount), MaterialManager(amount) {}

	// Update possible_order
	void updatePossible_order() {
		possible_order = validMaterialMenu();
	}

	// Print sales record
	void report_sales() {
		std::cout << "Sales Report: ";
		if (sales_record.empty()) {
			std::cout << "[없음]";
		}
		else {
			std::cout << "[";
			for (const auto& pair : sales_record) {
				std::cout << pair.first << ": " << pair.second << "잔 ";
			}
			std::cout << "]";
		}
		std::cout << std::endl;
	}

	// 단순 메뉴 가격 리턴.	menu_name 는 RECIPE.h의 key를 따름.
	int priceCoffee (std::string menu_name) {
		int price;
		price = getRecipeInfo(menu_name, Material::price);
		return price;
	}

	// 커피 제작. (레시피대로 재료차감, + 출력문.)
    void makeCoffee(std::string menu_name) {
        int coffee = getRecipeInfo(menu_name, Material::coffee);
        int water = getRecipeInfo(menu_name, Material::water);
        int milk = getRecipeInfo(menu_name, Material::milk);
        // 재료 차감.
        useMaterials(coffee, water, milk);
        // 출력문
        std::cout <<"[주문완료] " << menu_name << " 커피가 완성되었습니다." << std::endl;
        // update sales_record
        sales_record[menu_name]++;
    }

    // 주문목록 vector에 입력한 string이 포함됐는지. 포함되면 string return. 
    std::string getValidOrder(std::vector<std::string> menu) {
    	while (true) {
    		std::string order;
    		std::cout << "[INPUT:STRING] Order: ";
    		std::cin >> order;
    		if (find(menu.begin(), menu.end(), order) == menu.end()) {
    			std::cout << "잘못된 주문입니다." << std::endl;
    		} else return order;
    	}
    }

	// 금액투입 함수 구현.
	// CashManager 에서 구현하기에는 재료관리, 커피머신, 현금관리에 어려움이 있어서 통합함.
	void startInsert() {

		int totalInsert = 0;											//현재까지 삽입된 총 금액.
		std::map<int, int> currentInsertedCoins;					//현재 투입된 화폐:갯수 맵. - 아래 while문에서 업데이트.

		std::vector<std::string> order_valid_menues;		// 주문 가능 메뉴.
		

		// 재료별 주문가능 메뉴 출력. false 로 재료부족은 출력 안함.
		updatePossible_order();		// 시작 전 관리자 재료 추가를 대비해 possible_order 업데이트.
		std::cout << "주문가능: ";
		printValidMaterialMenu();
		std::cout << "재료부족: ";
		printValidMaterialMenu(false);

		// 동전 투입 반복문.
		std::cout << "[INFO] 화폐는 10, 50, 100, 500원만 가능합니다. 0 입력시 주문 시작, -1 입력시 취소됩니다. -2 : 나가기" << std::endl;
		while (1) {

			std::vector<std::string> tmp_possible_menu = possible_order;	//재료에 따른 가능한 메뉴 초기화.
			std::cout << "[STATUS] 현재 금액 : " << totalInsert << ",   ";
			int inserted; 

			// 정수 이외 입력처리 불가 세팅.
			std::cout << "Insert: ";
		    if (!(std::cin >> inserted)) {
		        std::cout << "[ERROR] 잘못된 입력입니다." << std::endl;
		        std::cin.clear();
		        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		        continue;
    		}

			// 루프종료
			if (inserted == -2) break;
			
			if (inserted != 0 && inserted != -1) { 		// 동전투입 반복 조건문.
				bool isValidCoin = std::find(VALID_COINS.begin(), VALID_COINS.end(), inserted) != VALID_COINS.end();
				if (!isValidCoin) std::cout << "[ERROR]"<< inserted << "원 화폐는 지원하지 않습니다. [반환]." << std::endl;
				else {
					// 이 시점에 투입가능한 금액이 결정됨.
					totalInsert += inserted;
					currentInsertedCoins[inserted] += 1;
					addCoins(inserted);

					// tmp_possible_menu 업데이트. 주문 가능한 메뉴"이름" 만 업데이트됨.
					std::vector<std::string> tmp_valid_price_menu;
					for (const std::string& menu : tmp_possible_menu) {
						int currentMenuPrice = priceCoffee(menu);
						if (currentMenuPrice <= totalInsert) {						// 음료가격, 금액 검증.
							if (makeChange(totalInsert - currentMenuPrice))		// 선택시 잔돈 반환 가능여부 검증.
								// 실제 자판기의 로직상에도 지폐는 반환이 안되어서 지폐는 빼버림.
								// 반환 안되어도 사용하게 하려면 VALID_COINS 에서 추가만 하면 작동함.
								tmp_valid_price_menu.push_back(menu);
						}
					}
					
					// 가능메뉴 표기.(직접 프린트됨.)
					if (!tmp_valid_price_menu.empty()) {
						std::cout << "[UPDATE] 구입가능: ";
						printMenu(tmp_valid_price_menu);
						}						
					order_valid_menues = tmp_valid_price_menu;			// last_updated_valid_menues에 가능메뉴 갱신.
				} 
			} 

			else if (inserted == -1) {									//주문 취소시 모두 다시 반환 (수익도 다시 차감.)
				if (totalInsert == 0) {
					std::cout << "삽입된 금액이 없습니다." << std::endl;
					continue;					
				}
				returnCash(currentInsertedCoins);	
				return;
			}

			// 가능하면 //makeOrder 함수로 만들 것.
			else if (inserted == 0) {									// 동전삽입 완료시
				if (totalInsert == 0) {									// 입력된 돈 0이면 다시 재진입.
					std::cout << "삽입된 금액이 없습니다." << std::endl;
					continue;
				} else if (order_valid_menues.empty()) {				//order_valid_menues 이 없으면 주문가능없다며 다시 루프 재진입. 
					std::cout << "주문 가능한 메뉴가 없습니다." << std::endl;
					continue;
				}

				// 이하 메뉴선택과 주문처리.

				// 가격표 조회.
				printPrice(order_valid_menues);

				// 검증된 메뉴 선택만 가능하도록 getValidOrder 함수 사용.
				std::string your_choice = getValidOrder(order_valid_menues);
				makeCoffee(your_choice);

				// 주문한 your_choice 대비 거스름돈 반환.
				int your_change = totalInsert - getRecipeInfo(your_choice, Material::price);
				if (your_change) makeChange(your_change, true);		//your_change 가 0일땐 거스름돈 진행하지 않도록 됨.

				// possible_order 초기화.
				possible_order = validMaterialMenu();

				break;		// 주문 종료. 끝.
			}
		}
	}

	// 매개변수의 배열(vector<string>)을 입력받아 직접 출력
	void printMenu(std::vector<std::string> menu) {
		int length = menu.size();
		std::cout << "[";
		for (int i = 0; i < length; i++) {
			std::cout << menu[i];
			if (i != length- 1) std::cout << ",  ";
		}
		//비어있는 경우 없음 처리.
		if (menu.empty()) std::cout << "없음";

		std::cout << "]" << std::endl;
	}


	// 재료에 따른 주문 가능 불가 직접프린트.	-자판기의 idle 상태로 미리 표시해야됨.
	void printValidMaterialMenu(bool possOrImposs = true){
		auto menu = validMaterialMenu(possOrImposs);
		printMenu(menu);
	}
};

