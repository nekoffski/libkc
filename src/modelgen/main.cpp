#include <iostream>

#include <stack>

#include <ranges>
#include <unordered_map>

#include <kc/core/ErrorBase.hpp>
#include <kc/core/FileSystem.h>
#include <kc/core/Log.h>

#include "Core.h"
#include "Generator.h"
#include "Parser.h"
#include "Tokenizer.h"

using namespace kc;

int main(int argc, char** argv) {
    ASSERT(argc == 2, "Wrong number of arguments");

    // for now, 1st argument is a path to the file
    std::string filePath(argv[1]);
    core::FileSystem fs;

    auto fileContent = fs.readLines(filePath);

    try {
        auto tokens = Tokenizer {}.tokenize(fileContent);

        auto structures = Parser { tokens }.parseTokens();
        auto files = Generator { JsonLib::libkc }.generateCode(structures);

        std::cout << "\n\n$ Generated files:\n\n";

        for (auto& [fileContent, fileName] : files) {
            std::cout << "[" << fileName << "]\n"
                      << "\n"
                      << fileContent << "";
        }

    } catch (ModelGeneratorError& error) {
        std::cout << error.asString() << '\n';
    }

    return 0;
}