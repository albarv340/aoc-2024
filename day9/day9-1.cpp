#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

int main()
{
    std::ifstream file("input.txt");

    if (!file)
    {
        std::cerr << "Unable to open the file.\n";
        return 1;
    }

    std::string line;
    std::getline(file, line);

    file.close();

    std::vector<long long> file_representation;

    long long num;
    long long index = 0;

    for (long long i = 0; i < line.length(); i++)
    {
        long long value = line[i] - '0';
        if (i % 2 == 0)
        {
            file_representation.insert(file_representation.end(), value, index);
            index++;
        }
        else
        {
            file_representation.insert(file_representation.end(), value, -1);
        }
    }

    for (long long i = 0; i < file_representation.size(); i++)
    {
        if (file_representation[i] != -1) {
            continue;
        }
        while (file_representation.back() == -1)
        {
            file_representation.pop_back();
        }
        
        file_representation[i] = file_representation.back();
        file_representation.pop_back();
    }

    long long checksum{0};

    for (long long i = 0; i < file_representation.size(); i++)
    {
        checksum += file_representation[i] * i;
    }
    std::cout << checksum << std::endl;

    return 0;
}
