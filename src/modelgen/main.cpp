#include <iostream>

#include <stack>

#include <ranges>

#include <kc/core/FileSystem.h>
#include <kc/core/Log.h>

using namespace kc;

enum class OutputType {
    singleFile,
    multiFile
};

auto pop(auto stack) {
    auto tmp = stack.top();
    stack.pop();

    return tmp;
}

enum class TokenType : unsigned char {
    header,
    string,
    colon,
    openingBrace,
    closingBrace,
    coma,
    constraint
};

struct Token {
    TokenType type;
    std::string value;
};

int main(int argc, char** argv) {
    ASSERT(argc == 2, "Wrong number of arguments");

    // for now, 1st argument is path to the file
    std::string filePath(argv[1]);

    core::FileSystem fs;

    auto fileContent = fs.readLines(filePath);

    static auto isLineNotEmpty = [](const std::string& line) -> bool {
        return not line.empty();
    };

    std::vector<std::string> tokens;

    for (auto& line : fileContent | std::views::filter(isLineNotEmpty)) {
        for (auto& token : core::split(line, ' ')) {
            tokens.push_back(token);

            auto& lastToken = tokens.back();
            const auto n = lastToken.size();

            if (lastToken[n - 1] == ':' || lastToken[n - 1] == ',') {
                tokens.push_back(std::string { lastToken[n - 1] });
                lastToken.pop_back();
            }
        }
    }

    for (const auto& token : tokens)
        std::cout << token << '\n';

    return 0;
}
