#ifndef CASHMANAGER_H
#define CASHMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Recipe.h"

class CashManager {
protected:
	std::map<int, int> coins;
	int income_total = 0;	//makeChange, insertCoins로만 작동.

public:
	//CashManager; 기본생성자. 각 동전 0으로 초기화.
	CashManager(): coins{{10,0},{50,0},{100,0},{500,0}} {}

	//CashManager test(100); 매개변수를 받는 생성자. first_run_amount 만큼 초기화.
	CashManager(int first_run_amount) {	// You can init first amount.
		coins[10] = first_run_amount;
		coins[50] = first_run_amount;
		coins[100] = first_run_amount;
		coins[500] = first_run_amount;
	}


	// 관리자용 동전별 잔고 표시기.
	// RESULT(print): Cash Report: [10원: 2개   50원: 2개   100원: 2개   500원: 2개   총액: 1320원   수익: 0원]
	void report_cash() {
		int all = 0;
		std::cout << "Cash Report: [";
		for (const auto& c : coins) {
			all += c.first * c.second;
			std::cout << c.first << "원: " << c.second << "개   ";
		}
		std::cout << "총액: " << all << "원" << "   ";
		std::cout << "수익: " << income_total << "원]" << std::endl;
	}


	// 동전 * 갯수로 직접 추가하는 함수. (총 수익까지 올라감.)
	// USE: addCoins(50, 1(optional))
	// RESULT: coins[50] += 1, income_total += 50 * 1
	void addCoins(int price, int amount = 1)  {
		coins[price] += amount;
		income_total += price * amount;
	}


	// map으로 직접 동전:갯수를 차감하고, 총 수익을 반환. (자판기 주문 취소시 반환)
	// USE : returnCash({500, 1}, {100, 2})
	// RESULT(calc) : RESULT(calc) : coins에서 {{500, 1}, {100, 2}} 만큼 계산해서 차감.(없으면 100 * 7 등 알아서 계산.) 
	// RESULT(print) : [반환] 금액: 100원: 2개   500원: 1개   [줄바꿈]총 700원이 반환되었습니다.
	void returnCash(std::map<int, int> user_inserted) {
		int total_incomed = 0;
		std::cout << "[반환] 금액: ";
		for (const auto& pair : user_inserted) {
			coins[pair.first] -= pair.second;
			total_incomed += pair.first * pair.second;
			std::cout << pair.first << "원: " << pair.second <<"개   ";
		}
		std::cout << "			";
		std::cout << "총 " << total_incomed << "원이 반환되었습니다." << std::endl;
		income_total -= total_incomed;
	}


	// 거스름돈 반환 함수. amount= 목표 거스름돈 process = true 시 실제 진행(returnCash함수 연동으로 출력문도 포함),
	// USE : makeChange(700, true) -false 시는 가능여부 bool타입만 리턴.
	// RESULT(calc) : coins에서 {{500, 1}, {100, 2}} 만큼 계산해서 차감.(없으면 100 * 7 등 알아서 계산.) 
	// RESULT(return) : 성공여부. true, or false.
	bool makeChange(int amount, bool process = false) {

	// 재고가 0이 아닌 동전들만 possibleReturnCoins 에 맵핑.
		std::map<int, int, std::greater<int>> possibleReturnCoins;
		for (const auto& pair : coins) {

			//!!!중요. 500이하 동전만 possibleReturnCoins 목록에 추가했음.
			if (pair.second != 0 && pair.first <= 500) possibleReturnCoins[pair.first] = pair.second;
		}


	// possibleReturnCoins으로 amount를 맞출수 있는지 검토하는 코드, change 맵핑.
		int tmp_price = amount;	// 임시 변수.(보유동전으로 0을 만들 수 있는지 체크)
		std::map<int, int, std::greater<int>> change;	//거스름돈 맵.

		for (auto& pair : possibleReturnCoins) {
			while (pair.second != 0 && tmp_price >= pair.first) {
				pair.second--;
				tmp_price -= pair.first;
				change[pair.first]++;
				if (tmp_price == 0) {
					break;
				}
			}
		}

	// process= true시. 거스름돈을 맞춰 반환할 수 있을 때 returnCash()함수(동전반환 프린트와 차감이 진행됨.) 연결. 
		if ((tmp_price == 0) && process) {
			std::map<int, int> tmp(change.begin(), change.end());
			returnCash(tmp);
		}

		return (tmp_price == 0);
	}


	// 머신 동전 추가. (add_amount = 추가동전. ,admin = true시 관리자란 뜻으로, 수익에 합산 안함.)
	// USE : insertCoins({{500, 20}, {100, 50}}, true);	500원 20개, 100원 50개 추가.(수익합산x)
	// RESULT : coins, income_total(optional) 합 계산.
	void insertCoins(std::map<int, int> add_amount, bool admin = false) {
		for (const auto& pair : add_amount) {
			coins[pair.first] += pair.second;
			//수익 추가.
			if (!admin) income_total += pair.first * pair.second; 
		}
	}


	// 부족한 거스름돈 출력 (minimum = 최소. 기본값 0)
	// USE : showEmptyCoins(0);
	// RESULT(print) : 0개 이하의 동전 : [10, 50, 100, 500]
	void showEmptyCoins(int minimum = 0) {
		//정의.
		std::vector<int> emptyCoins;
		for (const auto& coin : coins) {
			if (coin.first > 500) continue;
			if (coin.second <= minimum) emptyCoins.push_back(coin.first);
		}

		//출력.
		int len = emptyCoins.size();
		if (emptyCoins.empty()) return;

		std::cout << minimum <<"개 이하의 동전 : [";
		for (int i = 0; i < len; i++) {
			std::cout << emptyCoins[i];
			if (i != len - 1) std::cout << ", "; 
		}
		std::cout << "]" << std::endl;
	}
};

#endif