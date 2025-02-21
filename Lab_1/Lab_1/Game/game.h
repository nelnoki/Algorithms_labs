#pragma once
#include <string>

struct Game {

	// Functions:
	void init();
	void run();

private:

	// Player state params:
	int current_year;
	int current_people;
	int current_wheat;
	int current_acres;

	//Game params:
	int fed_people;
	int dead_people;
	int new_people;
	bool is_plague;
	int planted_acres;
	int eaten_wheat;
	int harvested_wheat_total;
	int harvested_wheat_per_acre;
	int rats_eaten_wheat;
	int acre_price;

	// Consts:
	const int wheat_person_eats = 20;
	const int acres_person_harvest = 10;
	const double wheat_acre_need = 0.5;
	const int harvested_wheat_min = 1;
	const int harvested_wheat_max = 6;
	const int acre_price_min = 17;
	const int acre_price_max = 26;
	const double rats_eat_factor = 0.07;
	const double dead_people_limit = 0.45;
	const int plague_chance = 15;
	static const int total_years = 10;

	// Game statistics:
	int dead_percent[total_years] = {0};
	int acre_per_person;

	//Game state:
	bool is_game_over = false;
	bool is_game_lost = false;
	bool is_game_loaded = false;

	// Functions:
	void next_year();
	void game_over();
	void user_input();
	void print_game_state();
	void check_valid_input(int& input, std::string mes, int max);
	void check_valid_input(int& input, int min, int max);
	int random_in_range(int min, int max);
};