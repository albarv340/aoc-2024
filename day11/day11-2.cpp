#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>

std::map<std::pair<long long, long long>, long long> cache;

long long blink_and_cache(long long num, long long blinks)
{
    if (blinks == 0)
    {
        return 1;
    }

    if (num == 0)
    {
        std::pair<long long, long long> key = {1, blinks - 1};
        if (cache.count(key))
        {
            return cache[key];
        }
        long long res = blink_and_cache(1, blinks - 1);
        cache[key] = res;
        return res;
    }
    int num_digits = (int)floor(log10(num)) + 1;
    if (num_digits % 2 == 0)
    {
        long long divisor = 1;
        for (int j = 0; j < num_digits / 2; ++j)
        {
            divisor *= 10;
        }
        long long first_half = num / divisor;

        long long second_half = num % divisor;

        std::pair<long long, long long> first_key = {first_half, blinks - 1};
        std::pair<long long, long long> second_key = {second_half, blinks - 1};
        long long sum{0};
        if (cache.count(first_key))
        {
            sum += cache[first_key];
        }
        else
        {
            long long res = blink_and_cache(first_half, blinks - 1);
            sum += res;
            cache[first_key] = res;
        }
        if (cache.count(second_key))
        {
            sum += cache[second_key];
        }
        else
        {
            long long res = blink_and_cache(second_half, blinks - 1);
            sum += res;
            cache[second_key] = res;
        }
        return sum;
    }
    std::pair<long long, long long> key = {num * 2024, blinks - 1};
    if (cache.count(key))
    {
        return cache[key];
    }
    long long res = blink_and_cache(num * 2024, blinks - 1);
    cache[key] = res;
    return res;
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<long long> numbers;
    long long num;

    if (file.is_open())
    {
        while (file >> num)
        {
            numbers.push_back(num);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open the file!" << std::endl;
    }

    long long sum{0};
    for (const auto &n : numbers)
    {
        sum += blink_and_cache(n, 75);
    }

    std::cout << sum << std::endl;

    return 0;
}
