#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <limits>

#include "game.h" 

	void Game::init() {
		std::ifstream file("./Game/save.txt", std::ios::in | std::ios::ate);
		if (file.is_open())
		{
			if (file.tellg() == 0) {
				current_year = 1;
			}
			else {
				std::cout << "������� ������ ����� ����, ��� ��������� � ���������� ����������?" << std::endl
					<< "1 - ����� ����" << std::endl
					<< "2 - ���������" << std::endl;
				int choice;
				check_valid_input(choice, 1, 2);
				if (choice == 1) current_year = 1;
				else {
					file.seekg(0);
					file >> current_year 
						 >> current_people 
						 >> current_wheat 
						 >> current_acres 
						 >> dead_people 
						 >> planted_acres
						 >> harvested_wheat_per_acre
						 >> rats_eaten_wheat
						 >> new_people
						 >> is_plague
						 >> acre_price;
					for (int i = 0; i < current_year - 1; ++i) {
						file >> dead_percent[i];
					}
					is_game_loaded = true;
				}
			}
		}
		file.close();
	}

	void Game::run() {

		while (!is_game_over) {
			next_year();
		}
		game_over();

	};

	void Game::next_year() {

		// Check if game is over
		if (current_year > total_years) {
			is_game_over = true;
			return;
		}

		//New Game Initialization

		if (current_year == 1) {
			current_people = 95;
			current_wheat = 0;
			current_acres = 1000;

			dead_people = 0;
			planted_acres = 1000;

			harvested_wheat_per_acre = 3;
			rats_eaten_wheat = 200;
			new_people = 5;
			is_plague = false;
			acre_price = 26;
		}
		// Setting random values

		else if (!is_game_loaded){
			harvested_wheat_per_acre = random_in_range(harvested_wheat_min, harvested_wheat_max);
			rats_eaten_wheat = random_in_range(0, static_cast<int>(rats_eat_factor * current_wheat));
			new_people = std::max(0, std::min(50, dead_people / 2 + (5 - harvested_wheat_per_acre) * current_wheat / 600 + 1));
			is_plague = random_in_range(0, 100) < plague_chance;
			acre_price = random_in_range(acre_price_min, acre_price_max);
		}

		//Saving game

		std::ofstream file("./Game/save.txt", std::ios::out | std::ios::trunc);
		if (file.is_open())
		{
			file << current_year << " " 
				 << current_people << " " 
				 << current_wheat << " " 
				 << current_acres << " " 
				 << dead_people << " " 
				 << planted_acres << " " 
				 << harvested_wheat_per_acre << " "
				 << rats_eaten_wheat << " "
				 << new_people << " "
				 << is_plague << " "
				 << acre_price << " ";
			for (int i = 0; i < current_year - 1; ++i) {
				file << dead_percent[i] << " ";
			}
		}
		file.close();

		// Making new year's calculations

		current_people -= dead_people;
		current_people += new_people;
		if (is_plague)
			current_people /= 2;

		harvested_wheat_total = std::min(planted_acres, current_people * 10) * harvested_wheat_per_acre;
		current_wheat += harvested_wheat_total;
		current_wheat = std::max(0, current_wheat - rats_eaten_wheat);


		print_game_state();
		user_input();

		dead_people = std::max(0, current_people - eaten_wheat / wheat_person_eats);
		if (dead_people > dead_people_limit * current_people) {
			is_game_over = true;
			is_game_lost = true;
		}
		else {
			dead_percent[current_year - 1] = static_cast<int>(std::round(static_cast<double>(dead_people) / current_people * 100));
			++current_year;
		}
		
	}

	void Game::game_over() {
		if (is_game_lost) {
			std::cout << "�� �������� ������� " << dead_people << " ������� �� ���� ���!" << std::endl
					  << "��-�� ������ ������ ��������� ����������� �� �� ������ �����������" << std::endl
					  << "���������� � ���� ���������� �� ���������," << std::endl
					  << "�� � ���� ��������� ������������� ������������!" << std::endl;
		}
		else {
			acre_per_person = current_acres / current_people;
			int dead_percent_average = 0, grade_l, grade_p, grade;
			for (int i = 0; i < total_years; ++i) {
				dead_percent_average += dead_percent[i];
			}
			dead_percent_average /= 10;
			if (dead_percent_average > 33) grade_p = 2;
			else if (dead_percent_average > 10) grade_p = 3;
				 else if (dead_percent_average > 3) grade_p = 4;
					  else grade_p = 5;

			if (acre_per_person < 7) grade_l = 2;
			else if (acre_per_person < 9) grade_l = 3;
			else if (acre_per_person < 10) grade_l = 4;
			else grade_l = 5;

			grade = std::min(grade_l, grade_p);

			switch (grade) {
				case 2: 
					std::cout << "��-�� ����� ���������������� � ����������," << std::endl
						<< "����� ������� ����, � ������ ��� �� ������." << std::endl
						<< "������ �� ��������� ������� ������ ������������� � ��������" << std::endl;
					break;
				case 3: 
					std::cout << "�� ������� �������� �����, ������� ������ � ����� ��������" << std::endl
						<< " ����� �������� � �����������, � ����� ������ �� ������ ������ ��� ����������" << std::endl;
					break;
				case 4:
					std::cout << "�� ���������� ������ �������, � ���, �������, ���� ���������������," << std::endl
						<< "�� ������ ������ �� ������� ��� �� ����� ������ �����" << std::endl;
					break;
				case 5:
					std::cout << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����" << std::endl;
					break;
			}	
		}
		


	}

	void Game::print_game_state() {

		std::cout << "��� ����������, �������� �������� ����:" << std::endl;
		std::cout << "� ���� " << current_year << " ������ ����������� ���������" << std::endl;
		if (dead_people != 0)
			std::cout << dead_people << " ������� ������ � ������" << std::endl;
		if (new_people != 0)
			std::cout << new_people << " ������� ������� � ��� ������� �����" << std::endl;
		if (is_plague) 
			std::cout << "���� ���������� �������� ���������" << std::endl;
		std::cout << "��������� ������ ������ ���������� " << current_people << " �������" << std::endl;
		std::cout << "�� ������� " << harvested_wheat_total << " ������� �������, �� " << harvested_wheat_per_acre << " ������ � ����" << std::endl;
		std::cout << "����� ��������� " << rats_eaten_wheat << " ������� �������, ������� " << current_wheat << " ������ � �������" << std::endl;
		std::cout << "����� ������ �������� " << current_acres << " �����" << std::endl;
		std::cout << "1 ��� ����� ����� ������ " << acre_price << " ������." << std::endl;
	}

	void Game::user_input() {
		
		std::cout << "��� ���������, ����������? " << std::endl;

		int bought_acres = 0;
		check_valid_input(bought_acres, "������� ����� ����� ����������� ������? ", current_wheat / acre_price);
		current_acres += bought_acres;
		current_wheat -= bought_acres * acre_price;

		if (bought_acres == 0) {
			int sold_acres = 0;
			check_valid_input(sold_acres, "������� ����� ����� ����������� �������? ", current_acres);
			current_acres -= sold_acres;
			current_wheat += sold_acres * acre_price;
		}

		check_valid_input(eaten_wheat, "������� ������� ������� ����������� ������? ", current_wheat);
		current_wheat -= eaten_wheat;
		
		check_valid_input(planted_acres, "������� ����� ����� ����������� �������? ", std::min(current_acres, static_cast<int>(current_wheat / wheat_acre_need)));
		current_wheat -= static_cast<int>(planted_acres * wheat_acre_need);
	}

	void Game::check_valid_input(int & input, std::string mes, int max) {

		bool isValid = false;

		while (!isValid) {
			std::cout << mes;
			std::cin >> input;

			if (std::cin.fail()) {
				std::cout << "����������, �����, ����� ���������� �����!" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			} 
			else {
				std::string remain;
				std::getline(std::cin, remain);
				if (remain.find_first_not_of(' ') != std::string::npos) {
					std::cout << "����� �����������, ����������!" << std::endl;
				}
				else if (input < 0) {
					std::cout << "�����, ����������, ����� ������������� �����!" << std::endl;
				}
				else if (input > max) {
					std::cout << "�, ����������, ������ ���! � ��� ������ " << current_people << " �������, " << current_wheat << " ������� ������� � " << current_acres << " ����� �����!" << std::endl;
				}
				else {
					isValid = true;
				}
			}
		}
	}

	void Game::check_valid_input(int& input, int min, int max) {

		bool isValid = false;

		while (!isValid) {
			std::cin >> input;

			if (std::cin.fail()) {
				std::cout << "������� ���������� �����!" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else {
				std::string remain;
				std::getline(std::cin, remain);
				if (remain.find_first_not_of(' ') != std::string::npos) {
					std::cout << "������� ���������� �����!" << std::endl;
				}
				else if (input != 1 && input != 2) {
					std::cout << "������� ���������� �����!" << std::endl;
				}
				else {
					isValid = true;
				}
			}
		}
	}

	int Game::random_in_range(int min, int max) {
		if (min > max) std::swap(min, max);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);

		return dist(gen);
	}
