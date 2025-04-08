#include <iostream>
#include "MachineController.h"

//TODO. Run 에 status 를 찍고 report를 대체할 것. (report도 함께 출력.)
// 몇잔 팔았는지 map 찍을 것.
// showEmptyCoins(0) 처럼 재료도 만들 것.


void startLoop(MachineController& source) {
	
	// 가격표 메서드
	printPrice();
	// printPrice({{"espresso"}, {"americano"}});

	// 동전 삽입 루프
	while (1){
		source.startInsert();
		char user_input;
		std::cout << "[FINISH] New order? (y or n): ";
		std::cin >> user_input;
		if (user_input == 'y') continue;
		else if (user_input == 'n') break;
		else break;
	}
};

int main() {

	//초기화.
	MachineController manager(5, 550);

	// 재료별 주문가능 메뉴 출력.
	// manager.printValidMaterialMenu();

	// 불가능 메뉴 
	// manager.printMenu(false);
	// 부족한 동전 출력 (환경변수는 그 갯수 이하 동전 출력)
	// manager.showEmptyCoins(2);


	while (1) {
		std::cout << "[START] 1:sales  2:report  3:add Material  4:add Coins  0:turnOff\nRUN : ";
		int mode_switch;
		std::cin >> mode_switch;
		puts("");
		switch (mode_switch) {

		case 1:
			std::cout << "[SALES MODE START]" << std::endl;
			startLoop(manager); 
			puts("");
			break;
		case 2:
			std::cout << "[REPORT MODE]" << std::endl;
			manager.report_material();
			manager.report_cash();
			manager.report_sales();
			puts("");
			break;
		case 3:
			std::cout << "[ADD MATERIALS]" << std::endl;
			std::cout << "[INFO] 커피, 물, 우유 순으로 띄어쓰기 구분합니다. 음수 사용 가능합니다." << std::endl;
			std::cout << "coffee, water, milk: ";
			int cf, wt, mk;
			std::cin >> cf >> wt >> mk;
			manager.addMaterials(cf, wt, mk);
			manager.report_material();
			puts("");
			break;
		case 4:
			std::cout << "[ADD COINS]" << std::endl;
			std::cout << "[WARNING] 지폐 혹은 실제로 없는 단위 화폐를 추가 하여도, 머신은 유효한 동전 외 반환하지 않습니다." << std::endl;
			std::cout << "[INFO] 단위 갯수 (예 : 500 10) 처럼 띄어쓰기 구분합니다. 음수 사용 가능합니다." << std::endl;
			int coin_price, coin_amount;
			std::cout << "price, amount: ";
			std::cin >> coin_price >> coin_amount;
			manager.insertCoins({{coin_price, coin_amount}}, true);
			manager.report_cash();
			puts("");
			break;

		case 0:
			return 0;
		default:
			continue;
		}
	}

	return 0;
}


