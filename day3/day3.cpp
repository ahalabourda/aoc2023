// day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using Schematic = std::vector<std::vector<char>>;
using Coordinate = std::pair<int, int>;

struct PartSymbol {
    PartSymbol(char symbol, Coordinate& coordinate)
        : mSymbol(symbol)
        , mCoordinate(coordinate) {}

    const char mSymbol;
    const Coordinate mCoordinate;

    bool operator==(const PartSymbol& rhs) const {
        return mCoordinate == rhs.mCoordinate;
    }
};

struct FoundNumber {
    FoundNumber(int value, Coordinate coordinate, std::vector<PartSymbol> partSymbols = {})
        : mValue(value)
        , mCoordinate(coordinate)
        , mPartSymbols(partSymbols) {}

    const int mValue;
    const Coordinate mCoordinate;
    std::vector<PartSymbol> mPartSymbols;
    
    void PopulatePartSymbols(const Schematic& schematic) {
        std::vector<Coordinate> legalNeighbouringCoordinates = GetAllLegalNeighbouringCoordinates(schematic);
        
        for (auto&& coordinate : legalNeighbouringCoordinates) {
            if (isCoordinatePunctuation(coordinate, schematic)) {
                mPartSymbols.emplace_back(schematic[coordinate.first][coordinate.second], coordinate);
            }
        }

    }

    bool isPartOfGear(const PartSymbol& gear) const {
        for (auto&& symbol : mPartSymbols) {
            if (symbol == gear) {
                return true;
            }
        }

        return false;
    }

    bool isSchematicPart() const {
        return mPartSymbols.size() > 0;
    }

    std::vector<Coordinate> GetAllLegalNeighbouringCoordinates(const Schematic& schematic) const {
        std::vector<Coordinate> legalNeighbouringCoordinates;

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < GetNumberOfDigits() + 1; j++) {
                Coordinate coordinate{ mCoordinate.first + i, mCoordinate.second + j };
                if (isLegalCoordinate(coordinate, schematic)) {
                    legalNeighbouringCoordinates.emplace_back(coordinate);
                }
            }
        }

        return legalNeighbouringCoordinates;
    }

    bool isLegalCoordinate(const Coordinate& coordinate, const Schematic& schematic) const {
        if (coordinate.first < 0) {
            return false;
        }

        if (coordinate.first >= schematic[0].size()) {
            return false;
        }

        if (coordinate.second < 0) {
            return false;
        }

        if (coordinate.second >= schematic.size()) {
            return false;
        }

        return true;
    }

    bool isCoordinatePunctuation(const Coordinate& coordinate, const Schematic& schematic) const {
        const char value = schematic[coordinate.first][coordinate.second];

        if (value == '.') {
            return false;
        }

        if (std::isdigit(value)) {
            return false;
        }

        return true;
    }

    void AddPartSymbol(const PartSymbol& partSymbol) {
        mPartSymbols.emplace_back(partSymbol);
    }

    bool ContainsCoordinate(const Coordinate& coordinate) const {
        for (int i = 0; i < GetNumberOfDigits(); i++) {
            Coordinate coordinateToCheck(mCoordinate.first, mCoordinate.second + i);
            if (coordinate == coordinateToCheck) {
                return true;
            }
        }

        return false;
    }

    int GetNumberOfDigits() const {
        int value = abs(mValue);
        return  (value < 10 ? 1 :
                (value < 100 ? 2 :
                (value < 1000 ? 3 :
                (value < 10000 ? 4 :
                (value < 100000 ? 5 :
                (value < 1000000 ? 6 :
                (value < 10000000 ? 7 :
                (value < 100000000 ? 8 :
                (value < 1000000000 ? 9 :
                10)))))))));
    }
};

int getIntFromDigits(const std::vector<int>& digits) {
    int result = 0;
    for (auto digit : digits)
    {
        result = result * 10 + digit;
    }

    return result;
}

int extractNumber(const Schematic& schematic, const Coordinate& coordinate) {
    bool doneFindingNumber = false;
    int index = coordinate.second;
    
    std::vector<int> values;
    while (true) {
        
        if (index >= schematic.size()) {
            break;
        }
        
        if (!std::isdigit(schematic[coordinate.first][index])) {
            break;
        }

        values.emplace_back(schematic[coordinate.first][index] - '0');
        index++;
    }

    return getIntFromDigits(values);
}

std::vector<FoundNumber> getAllNumbers(const Schematic& schematic) {
    std::vector<FoundNumber> foundNumbers;

    for (int i = 0; i < schematic.size(); i++) {
        for (int j = 0; j < schematic[i].size(); j++) {
            if (std::isdigit(schematic[i][j])) {
                const Coordinate coordinate = { i, j };
                const int value = extractNumber(schematic, coordinate);
                
                FoundNumber found(value, coordinate);
                found.PopulatePartSymbols(schematic);

                foundNumbers.emplace_back(found);
                
                j += found.GetNumberOfDigits();
            }
            
        }
    }

    return foundNumbers;
}

int getFinalSum(const std::vector<FoundNumber>& numbers, bool isValid = true) {
    int tally = 0;

    for (auto&& number : numbers) {
        if (number.isSchematicPart() == isValid) {
            tally += number.mValue;
        }
    }

    return tally;
}

int getSingleGearValue(const Coordinate& coordinate, const std::vector<FoundNumber>& numbers) {
    std::vector<int> attachedValues;
    attachedValues.reserve(2);
    for (auto&& number : numbers) {
        for (auto&& partSymbol : number.mPartSymbols) {
            if (partSymbol.mCoordinate == coordinate) {
                attachedValues.emplace_back(number.mValue);
            }
        }
    }

    if (attachedValues.size() != 2) {
        return 0;
    }

    // spec states gears will only ever have exactly two values
    return attachedValues[0] * attachedValues[1];
}

int getGearSumTotal(const Schematic& schematic, const std::vector<FoundNumber>& numbers) {
    int tally = 0;
    
    for (int i = 0; i < schematic.size(); i++) {
        for (int j = 0; j < schematic[i].size(); j++) {
            if (schematic[i][j] == '*') {
                tally += getSingleGearValue({ i, j }, numbers);
            }
        }
    }

    return tally;
}

Schematic getSchematic(std::ifstream& inputFile) {
    Schematic schematic;
    std::string singleLine;
    
    while (std::getline(inputFile, singleLine)) {
        std::vector<char> thisLine;

        for (auto&& c : singleLine) {
            thisLine.emplace_back(c);
        }

        schematic.emplace_back(thisLine);
    }

    return schematic;
}

void printSchematic(const Schematic& schematic) {
    for (int i = 0; i < schematic.size(); i++) {
        for (int j = 0; j < schematic[i].size(); j++) {
            std::cout << schematic[i][j];
        }
        std::cout << "\r\n";
    }
}

void prettyPrintSchematic(const Schematic& schematic, std::vector<FoundNumber> foundNumbers) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = 0; i < schematic.size(); i++) {
        for (int j = 0; j < schematic[i].size(); j++) {
            
            SetConsoleTextAttribute(hConsole, 15);
            for (auto&& number : foundNumbers) {
                if (number.ContainsCoordinate(Coordinate{i, j})) {
                    
                    if (number.isSchematicPart()) {
                        SetConsoleTextAttribute(hConsole, 10);
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, 12);
                    }

                }
            }

            if (schematic[i][j] == '.') {
                SetConsoleTextAttribute(hConsole, 0);
            }

            std::cout << schematic[i][j];
        }
        std::cout << "\r\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
}

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    if (!inputFile) {
        std::cerr << "Unable to open input file";
        exit(1);
    }

    const Schematic schematic = getSchematic(inputFile);
    const std::vector<FoundNumber> numbers = getAllNumbers(schematic);

    prettyPrintSchematic(schematic, numbers);

    std::cout << "Final sum: " << getFinalSum(numbers) << " vs " << getFinalSum(numbers, false) << "\r\n";

    std::cout << "Gear sum: " << getGearSumTotal(schematic, numbers) << "\r\n";
}
