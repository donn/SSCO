// Example usage

#include <SSCO/StupidSimpleCPPOpts.h>

#include <iostream>
#include <sysexits.h>

int main(int argc, char** argv) {
    SSCO::Options options({
        {"version", 'V',            "Show the current version of this app.",    false,  [&](){std::cout << "0.1.0" << std::endl; exit(0);}},
        {"help",    'h',            "Show this message and exit.",              false,  [&](){options.printHelp(std::cout); exit(0);}},
        {"eggs",    'e',            "How many eggs to fry.",                    true,   std::nullopt},
        {"evil",    std::nullopt,   "Evil name for the omelette.",              true,   std::nullopt},
        {"butter",  'b',            "Use butter?",                              false,  std::nullopt}
    });

    auto opts = options.process(argc, argv);

    if (opts.has_value()) {
        std::cout << "Successful." << std::endl;
        std::cout << "Options: " << std::endl;
        for (auto option: opts.value().options) {
            std::cout << option.first << ": " << option.second << std::endl;
        }
        std::cout << std::endl << "Arguments: " << std::endl;
        for (auto argument: opts.value().arguments) {
            std::cout << argument << ", ";
        }
        std::cout << std::endl;
    } else {
        options.printHelp(std::cout);
        return EX_USAGE;
    }
}