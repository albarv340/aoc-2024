#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>
#include <thread>
#include <mutex>

std::mutex mtx;
long long result;
long long const iters{2000};
long long const num_threads = 12;
long long const its_per_thread = iters / num_threads;
std::vector<std::vector<long long>> prices;
std::vector<std::vector<long long>> diffs;

void task(int thread_id)
{
    long long const its_per_thread = iters / num_threads;
    long long max_price{-1};
    int a = thread_id - 3;
    for (int b = -9; b < 10; b++)
        for (int c = -9; c < 10; c++)
            for (int d = -9; d < 10; d++)
            {
                long long sum{0};
                for (int i = 0; i < diffs.size(); i++)
                {
                    for (int j = 0; j < iters - 3; j++)
                    {
                        if (diffs[i][j] == a && diffs[i][j + 1] == b && diffs[i][j + 2] == c && diffs[i][j + 3] == d)
                        {
                            sum += prices[i][j + 3];
                            break;
                        }
                    }
                }
                if (sum > max_price)
                {
                    max_price = sum;
                }
            }
    std::lock_guard<std::mutex> lock(mtx);
    if (max_price > result) {
        result = max_price;
    }
}

int main()
{
    std::ifstream file("input.txt");
    std::string line;
    long long number;

    if (!file)
    {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }
    long long tot{0};
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::vector<long long> price;
        std::vector<long long> diff;
        ss >> number;
        long long prev{number % 10};
        for (int i = 0; i < iters; i++)
        {
            long long op1 = number << 6;
            number ^= op1;
            number %= 16777216;
            long long op2 = number >> 5;
            number ^= op2;
            number %= 16777216;
            long long op3 = number << 11;
            number ^= op3;
            number %= 16777216;
            long long one_digit = number % 10;
            price.push_back(one_digit);
            diff.push_back(one_digit - prev);
            prev = one_digit;
        }
        prices.push_back(price);
        diffs.push_back(diff);
    }

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.push_back(std::thread(task, i));
    }

    for (auto &t : threads)
    {
        t.join();
    }
    std::cout << result << std::endl;

    file.close();

    return 0;
}
