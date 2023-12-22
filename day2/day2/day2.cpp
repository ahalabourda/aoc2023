// day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Colour { Red, Green, Blue };
using CubeCount = std::pair<Colour, int>;

struct Game {
    Game(int gameId, int redCount, int greenCount, int blueCount)
        : mId(gameId)
        , mRedCount(redCount)
        , mGreenCount(greenCount)
        , mBlueCount(blueCount) {}
    
    const int mId;
    const int mRedCount;
    const int mGreenCount;
    const int mBlueCount;

    int GetPower() const {
        return mRedCount * mGreenCount * mBlueCount;
    }
};

struct CubeMaxList {
    CubeMaxList(int redMax, int greenMax, int blueMax)
        : mRedMax(redMax)
        , mGreenMax(greenMax) 
        , mBlueMax(blueMax) {}

    const int mRedMax;
    const int mGreenMax;
    const int mBlueMax;
};

bool _isTerminatorCharacter(char characterToCheck) {
    return
        characterToCheck == ':' ||
        characterToCheck == ' ' ||
        characterToCheck == ',' ||
        characterToCheck == ';' ||
        characterToCheck == '\0';
}

Colour _getColourAtPosition(const std::string& input, size_t position) {
    for (size_t i = position; i < input.length(); i++) {
        if (input[i] == 'r') {
            return Red;
        }
        if (input[i] == 'g') {
            return Green;
        }
        if (input[i] == 'b') {
            return Blue;
        }
    }

    throw("No valid colour found at position %i", position);
}

int _getNumberAtPosition(const std::string& input, size_t position) {
    std::vector<int> digitsFound;

    while (std::isdigit(input[position]) && !_isTerminatorCharacter(input[position])) {
        digitsFound.emplace_back(input[position] - '0');
        ++position;
    }

    int result = 0;
    for (auto digit : digitsFound)
    {
        result = result * 10 + digit;
    }

    return result;
}

int _getGameId(const std::string& input) {
    // hardcoding start position because we know every line starts with "Game "
    for (int i = 5; i < input.length(); i++) {
        if (std::isdigit(input[i])) {
            return _getNumberAtPosition(input, i);
        }
    }

    throw("No game ID found");
}

CubeMaxList _getAllMaxCubeCounts(const std::string& input) {

    size_t startPos = input.find(':');
    int highestRed = 0;
    int highestGreen = 0;
    int highestBlue = 0;

    for (size_t i = startPos; i < input.length(); i++) {
        if (std::isdigit(input[i])) {
            int foundCount = _getNumberAtPosition(input, i);
            Colour foundColour = _getColourAtPosition(input, i); // we could technically bump this position forward, but because digit count is variable, the act of counting digits would be slower than just starting at the same position

            switch (foundColour) {
            case Red:
                highestRed = std::max(highestRed, foundCount);
                break;
            case Green:
                highestGreen = std::max(highestGreen, foundCount);
                break;
            case Blue:
                highestBlue = std::max(highestBlue, foundCount);
                break;
            }

            i += 3; // lol
        }

    }

    return { highestRed, highestGreen, highestBlue };
}

Game getGameFromInput(const std::string& input) {
    int id = _getGameId(input);

    const CubeMaxList cubeCounts = _getAllMaxCubeCounts(input);

    return Game(id, cubeCounts.mRedMax, cubeCounts.mGreenMax, cubeCounts.mBlueMax);
}

std::vector<std::string> getLines(std::ifstream& inputFile) {
    std::vector<std::string> lines;
    std::string singleLine;

    while (std::getline(inputFile, singleLine)) {
        lines.emplace_back(singleLine);
    }

    return lines;
}

bool _isGamePossible(const Game& game, const CubeMaxList& limits) {
    return game.mRedCount <= limits.mRedMax
        && game.mGreenCount <= limits.mGreenMax
        && game.mBlueCount <= limits.mBlueMax;
}

int sumValidGameIds(const std::vector<Game> games, const CubeMaxList& limits) {
    int tally = 0;

    for (auto&& game : games) {
        if (_isGamePossible(game, limits)) {
            tally += game.mId;
        }
    }

    return tally;
}

int sumGamePowers(const std::vector<Game> games) {
    int tally = 0;

    for (auto&& game : games) {
        tally += game.GetPower();
    }

    return tally;
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
    
    std::vector<Game> games;

    for (auto&& line : lines) {
        games.emplace_back(getGameFromInput(line));
    }

    const CubeMaxList limits(12, 13, 14);
    const int sumOfValidGameIds = sumValidGameIds(games, limits);
    const int sumOfAllGamePowers = sumGamePowers(games);

    std::cout << "Final sum of all legal game IDs: " << sumOfValidGameIds << "\r\n";
    std::cout << "Sum of the power of all sets: " << sumOfAllGamePowers << "\r\n";

}
