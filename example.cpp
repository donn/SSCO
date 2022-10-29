// Example usage

#include <iostream>
#include <ssco.hpp>
#include <sysexits.h>

int main(int argc, char **argv) {
    using O = SSCO::Option;
    SSCO::Options options(
        {O{"version",
           'V',
           "Show the current version of this app.",
           false,
           [&](SSCO::Result &_) {
               std::cout << "2.0.0" << std::endl;
               exit(0);
           }},
         O{"help",
           'h',
           "Show this message and exit.",
           false,
           [&](SSCO::Result &_) {
               options.printHelp(std::cout);
               exit(0);
           }},
         O{"eggs", 'e', "How many eggs to fry.", true},
         O{"evil", std::nullopt, "Evil name for the omelette.", true},
         O{"butter", 'b', "Use butter?", false}}
    );

    auto opts = options.process(argc, argv);

    if (opts.has_value()) {
        std::cout << "Successful." << std::endl;
        std::cout << "Options: " << std::endl;
        for (auto option : opts.value().options) {
            std::cout << option.first << ": " << option.second << std::endl;
        }
        std::cout << std::endl << "Arguments: " << std::endl;
        for (auto argument : opts.value().arguments) {
            std::cout << argument << ", ";
        }
        std::cout << std::endl;
    } else {
        options.printHelp(std::cout);
        return EX_USAGE;
    }
}