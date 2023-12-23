// day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

struct Card {
    Card(int gameId, std::vector<int> winningNumbers, std::vector<int> playerNumbers)
        : mGameId(gameId)
        , mWinningNumbers(winningNumbers)
        , mPlayerNumbers(playerNumbers) {}
    
    const int mGameId;
    const std::vector<int> mWinningNumbers;
    const std::vector<int> mPlayerNumbers;

    int mCopies = 1;
    
    void addCopies(int copiesToAdd) {
        mCopies += copiesToAdd;
    }

    int getNumberOfMatches() const {
        int numberOfMatches = 0;

        for (int i = 0; i < mWinningNumbers.size(); i++) {
            for (int j = 0; j < mPlayerNumbers.size(); j++) {
                if (mWinningNumbers[i] == mPlayerNumbers[j]) {
                    numberOfMatches++;
                }
            }
        }

        return numberOfMatches;
    }

    int getScore() const {
        int score = 0;

        for (int i = 0; i < mWinningNumbers.size(); i++) {
            for (int j = 0; j < mPlayerNumbers.size(); j++) {
                if (mWinningNumbers[i] == mPlayerNumbers[j]) {
                    if (score == 0) {
                        score = 1;
                    }
                    else {
                        score *= 2;
                    }
                }
            }
        }

        return score;
    }

    void print() const {
        std::cout << "Card " << mGameId << ": ";
        
        for (auto&& num : mWinningNumbers) {
            std::cout << std::setw(2) << num << " ";
        }

        std::cout << "| ";

        for (auto&& num : mPlayerNumbers) {
            std::cout << std::setw(2) << num << " ";
        }

        std::cout << "Score: " << getScore() << " | ";

        std::cout << "Copies: " << mCopies;

        std::cout << "\r\n";
    }
};

int GetNumberOfDigits(int numberToCheck) {
    int value = abs(numberToCheck);
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

int getIntFromDigits(const std::vector<int>& digits) {
    int result = 0;
    for (auto digit : digits)
    {
        result = result * 10 + digit;
    }

    return result;
}

int extractNumberAtPosition(const std::string& line, int position) {
    std::vector<int> foundDigits;
    while (true) {
        if (!std::isdigit(line[position])) {
            break;
        }

        foundDigits.emplace_back(line[position] - '0');
        position++;
    }

    return getIntFromDigits(foundDigits);
}

std::vector<int> getPlayerNumbers(const std::string& line) {
    int position = 0;

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '|') {
            position = i + 1;
            break;
        }
    }

    std::vector<int> numbersFound;

    while (true) {

        if (line[position] == '\0') {
            break;
        }

        if (std::isdigit(line[position])) {
            numbersFound.emplace_back(extractNumberAtPosition(line, position));
            position += GetNumberOfDigits(numbersFound.back());
        }
        else {
            position++;
        }
    }

    return numbersFound;
}

std::vector<int> getWinningNumbers(const std::string& line) {
    int position = 0;

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ':') {
            position = i + 1;
            break;
        }
    }

    std::vector<int> numbersFound;

    while (true) {

        if (line[position] == '|') {
            break;
        }

        if (std::isdigit(line[position])) {
            numbersFound.emplace_back(extractNumberAtPosition(line, position));
            position += GetNumberOfDigits(numbersFound.back());
        }
        else {
            position++;
        }
    }

    return numbersFound;
    
}

int getGameId(const std::string& line) {
    for (int i = 0; i < line.length(); i++) {
        if (std::isdigit(line[i])) {
            return extractNumberAtPosition(line, i);
        }
    }

    throw("Couldn't find a valid game id");
}

Card getCardFromString(const std::string& line) {
    
    const int gameId = getGameId(line);
    const std::vector<int> winningNumbers = getWinningNumbers(line);
    const std::vector<int> playerNumbers = getPlayerNumbers(line);

    return Card(gameId, winningNumbers, playerNumbers);
}

std::vector<Card> getAllCards(std::ifstream& input) {
    std::vector<Card> cards;
    std::string singleLine;

    while (std::getline(input, singleLine)) {
        cards.emplace_back(getCardFromString(singleLine));
    }

    return cards;
}

int getNewTotalScore(std::vector<Card>& cards) {
    int tally = 0;
    for (int i = 0; i < cards.size(); i++) {
        tally += cards[i].mCopies;

        for (int j = i + 1; j < i + cards[i].getNumberOfMatches() + 1; j++) {
            cards[j].addCopies(cards[i].mCopies);
        }
    }

    return tally;
}

int getTotalScore(const std::vector<Card>& cards) {
    int tally = 0;
    
    for (auto&& card : cards) {
        tally += card.getScore();
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

    std::vector<Card> allCards = getAllCards(inputFile);
    
    std::cout << "Total score: " << getTotalScore(allCards) << "\r\n";

    std::cout << "New rules total score: " << getNewTotalScore(allCards) << "\r\n";
}