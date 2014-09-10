#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <random>

struct word_entry
{
    std::string id;
    std::string word;

    word_entry() = default;
    
    template<typename T1, typename T2>
    word_entry(T1&& id_, T2&& word_)
        : id(id_), word(word_)
    {}
};

bool operator<(word_entry const& lhs, word_entry const& rhs)
{
    return lhs.id < rhs.id;
}

word_entry const& find(std::vector<word_entry> const& container, std::string const& wordId)
{
    return *std::lower_bound(container.begin(), container.end(), word_entry(wordId, std::string()));
}

static std::regex const wordRegex("(\\d{5})\\t([^\\r\\n]+)");

std::vector<word_entry> build_word_list()
{
    std::vector<word_entry> result;
    std::string line;
    std::ifstream file("diceware_wordlist.txt");
    while (std::getline(file, line))
    {
        std::smatch matchResults;
        if (std::regex_match(line, matchResults, wordRegex))
        {
            result.emplace_back(matchResults[1].str(), matchResults[2].str());
        }
    }

    std::sort(result.begin(), result.end());

    return result;
}

static std::random_device random;
static std::uniform_int_distribution<> const diceRoller(1, 6);

std::string roll_dice()
{
    std::string result;
    result.reserve(5);
    for (int idx = 0; idx < 5; ++idx)
    {
        result.push_back('0' + static_cast<char>(diceRoller(random)));
    }

    return result;
}

int main()
{
    auto const numberOfWords = 3;
    auto const wordList = build_word_list();
    std::string password("?<1");
    for (int idx = 0; idx < numberOfWords; ++idx)
    {
        auto const& word = find(wordList, roll_dice());
        std::cout << word.id << " " << word.word << std::endl;
        password.append(word.word);
    }

    std::cout << std::endl;
    std::cout << password << std::endl;
    std::cin.get();
}
