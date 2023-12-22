// day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct DigitPosition {
    int mValue;
    size_t mStartPos;
    DigitPosition(int value, size_t startPos)
        : mValue(value)
        , mStartPos(startPos) {}
};

std::vector<DigitPosition> getNormalDigits(const std::string& input) {
    
    std::vector<DigitPosition> digitPositions;

    int position = -1;

    for (auto&& c : input) {
        position++;
        if (!std::isdigit(c)) {
            continue;
        }

        digitPositions.emplace_back(c - '0', position);
    }

    return digitPositions;
}

int _spelledDigitToInt(const std::string& spelledDigit) {
    if (spelledDigit == "one") { return 1; }
    if (spelledDigit == "two") { return 2; }
    if (spelledDigit == "three") { return 3; }
    if (spelledDigit == "four") { return 4; }
    if (spelledDigit == "five") { return 5; }
    if (spelledDigit == "six") { return 6; }
    if (spelledDigit == "seven") { return 7; }
    if (spelledDigit == "eight") { return 8; }
    if (spelledDigit == "nine") { return 9; }
    
    throw("Spelled digit was invalid");
}

std::vector<DigitPosition> _getSingleSpelledDigit(const std::string& input, std::string digitToFind) {

    std::vector<DigitPosition> digitPositions;

    size_t pos = input.find(digitToFind);
    while (pos != std::string::npos) {
        digitPositions.emplace_back(_spelledDigitToInt(digitToFind), pos);
        pos = input.find(digitToFind, pos + 1);
    }

    return digitPositions;
}

std::vector<DigitPosition> getSpelledDigits(const std::string& input) {
    
    std::vector<DigitPosition> digitPositions;
    const std::string numberNames[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    for (auto&& numberName : numberNames) {
        std::vector<DigitPosition> singleNumberDigitPositions = _getSingleSpelledDigit(input, numberName);
        digitPositions.insert(digitPositions.end(), singleNumberDigitPositions.begin(), singleNumberDigitPositions.end());
    }
    
    return digitPositions;
}

int getCalibrationValueFromLine(std::string inputLine) {
    std::vector<DigitPosition> digitPositions = getNormalDigits(inputLine);
    std::vector<DigitPosition> spelledDigitPositions = getSpelledDigits(inputLine);
    digitPositions.insert(digitPositions.end(), spelledDigitPositions.begin(), spelledDigitPositions.end());

    std::sort(digitPositions.begin(), digitPositions.end(), [](const DigitPosition& a, const DigitPosition& b) { return a.mStartPos < b.mStartPos; });

    return (digitPositions.front().mValue * 10) + digitPositions.back().mValue;
}

std::vector<std::string> getLines(std::ifstream& inputFile) {
    std::vector<std::string> lines;
    std::string singleLine;

    while (std::getline(inputFile, singleLine)) {
        lines.emplace_back(singleLine);
        
        std::cout << "Got the line: " << singleLine << " which evaluates to " << getCalibrationValueFromLine(singleLine) << "\r\n";
    }

    return lines;
}

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    if (!inputFile) {
        std::cerr << "Unable to open input file";
        exit(1);
    }

    const std::vector<std::string> lines = getLines(inputFile);

    int tally = 0;

    for (auto&& line : lines) {
        tally += getCalibrationValueFromLine(line);
    }

    std::cout << "Final tally: " << tally << "\r\n";
}