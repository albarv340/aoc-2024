#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int safe_count{0};
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        bool is_safe{true};
        int prev_num;
        lineStream >> prev_num;
        int number;
        lineStream >> number;
        bool is_ascending{number > prev_num};
        if (abs(number - prev_num) > 3 || number == prev_num) {
            is_safe = false;
            continue;
        }
        prev_num = number;
        while (lineStream >> number) {
            if (abs(number - prev_num) > 3 || number == prev_num) {
                is_safe = false;
                break;
            }
            if (is_ascending && number < prev_num || !is_ascending && number > prev_num) {
                is_safe = false;
                break;
            }
            prev_num = number;
        }
        if (is_safe) {
            safe_count++;
        }
    }
    std::cout << safe_count << std::endl;

    inputFile.close();
    return 0;
}
