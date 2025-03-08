#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <map>
#include <vector>


struct Dice {
    int count;
    int sides;
    int modification;
    Dice(int c, int s, int m) : count(c), sides(s), modification(m) {};
};

Dice parse(const std::string& s) {
    Dice dice = Dice(0, 0, 0);
    char d;

    std::stringstream ss(s);

    ss >> dice.count >> d >> dice.sides;
    if (ss.peek() == '+' || ss.peek() == '-')
        ss >> dice.modification;
    return dice;
};

int roll(int sides) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> res(1, sides);
    return res(gen);
};

int dice(const std::string& s) {
    std::stringstream ss(s);
    std::string part;
    int result = 0;

    while (std::getline(ss, part, ',')) {
        Dice dice = parse(part);
        for (int i = 0; i < dice.count; ++i) {
            result += roll(dice.sides);
        }
        result += dice.modification;
    }
    return result;
};

std::map<int, int> test(const std::string& input, int trials) {
    std::map<int, int> frequency;

    for (int i = 0; i < trials; ++i) {
        int result = dice(input);
        frequency[result]++;
    }

    return frequency;
}

int main()
{
    std::string s = "2d6 + 2, 3d10";
    int result = dice(s);
    std::cout << "\Results for " << s << ": " << result << std::endl;

    std::vector<std::string> tests = { "1d6", "2d6", "3d6", "1d10", "2d10", "3d10" };
    int trials = 10000;

    for (const auto& str : tests) {
        std::cout << "\Results for " << str << ":\n";
        auto freq = test(str, trials);
        for (std::map<int, int>::const_iterator it = freq.begin(); it != freq.end(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }
    }

    return 0;
}

