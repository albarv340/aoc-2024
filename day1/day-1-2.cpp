#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

int main() {
    std::ifstream file("numbers.txt");
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<int> v1;
    std::unordered_map<int, int> h2;

    int num1, num2;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        if (ss >> num1 >> num2) {
            v1.push_back(num1);
            h2[num2] += 1;
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();

    int sum{0};
    for (int v : v1) {
        // std::cout << "Set1: " << v << ", Set2: " << h2[v] << std::endl;
        sum += v * h2[v];
    }

    std::cout << sum << std::endl;

    return 0;
}
