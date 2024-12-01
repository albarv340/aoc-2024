#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

int main() {
    std::ifstream file("numbers.txt");
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::multiset<int> set1, set2;

    int num1, num2;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        if (ss >> num1 >> num2) {
            set1.insert(num1);
            set2.insert(num2);
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();

    auto it1{set1.begin()};
    auto it2{set2.begin()};
    int sum{0};
    while (it1 != set1.end() && it2 != set2.end()) {
        // std::cout << "Set1: " << *it1 << ", Set2: " << *it2 << std::endl;
        sum += abs(*it1 - *it2);
        ++it1;
        ++it2;
    }

    std::cout << sum << std::endl;

    return 0;
}
