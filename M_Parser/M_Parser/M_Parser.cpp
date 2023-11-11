#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Solution {
public:
    unsigned long calculateCRC(std::vector<std::string>& data) {
        unsigned long sum = 0;
        unsigned long decimalValue;
        for (int i = 0; i < data.size(); i++) {
            if (data[i] == "7D" && (data[i + 1] == "5D" || data[i + 1] == "5E")) {
                if (data[i + 1] == "5E") {
                    data[i] = "7E";
                }
                data.erase(data.begin() + i + 1);
                i--;
                continue;
            }
            std::string hexString = data[i];
            std::istringstream(hexString) >> std::hex >> decimalValue;

            if (i > 0 && i < data.size() - 2) {
                sum += decimalValue;
                continue;
            }
        }

        unsigned long crc;
        std::istringstream(data[data.size() - 2]) >> std::hex >> crc;
        crc = (255 - sum % 256) == crc ? 0 : crc;
        return crc;
    }

    unsigned long checkSpeed(std::vector<std::string>& data) {
        unsigned long sum = 0;
        string res = "";
        for (int i = 0; i < data.size(); i++) {
            if (i > 3 && i < 6) {
                std::string hexString = data[i];
                if (i == 5) {
                    hexString = data[i][0];
                }
                res += hexString;
            }
        }
        std::istringstream(res) >> std::hex >> sum;
        return sum;
    }
};


int main() {

    Solution test;
    std::ifstream file("D:/Programming/M_CRC/M_Parser/parser.log");
    if (!file) {
        std::cerr << "Error: Unable to open file 'parser.log'" << std::endl;
        return 1;
    }

    std::string line;
    unsigned int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;
        std::vector<std::string> data;

        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            data.push_back(word);
        }

        // Calculate the CRC for header and data bytes
        unsigned long crc = test.calculateCRC(data);
        unsigned long speed = test.checkSpeed(data);
        std::cout << "Line " << line_number << ": Speed " << (double)speed*0.1 << std::endl;
        if (crc != 0) {
            std::cout << "Line " << line_number << ": FCS Check: Failed. Correct " << crc << std::endl;
        }

    }


    file.close();
    return 0;
}