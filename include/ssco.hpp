// Stupid Simple CPP Opts - Mohamed Gaber - Unlicense
#ifndef _ssco_h
#define _ssco_h

#include <functional>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace SSCO {
    struct Result {
            std::map< std::string, std::string > options;
            std::vector< std::string > arguments;
    };

    struct Option {
            // Name of the option. Will be used in the final map.
            std::string name;
            // Shorthand of the option. Optional, pass nullopt if none.
            std::optional< char > shorthand;
            // Description of the option.
            std::string description;
            // Does this option have an argument? If no, the final dictionary
            // will have the string "SET" in lieu of an argument value.
            bool argument;
            // If there's no argument and this executor isn't nullopt,
            // then this executor is executed instead.
            std::optional< std::function< void(Result &) > > executor =
                std::nullopt;
    };

    class Options {
            std::map< char, Option > byShorthand;
            std::map< std::string, Option > byName;
            std::vector< Option > byOrder;

            std::string executablePath;
        public:
            Options(std::vector< Option > options);

            void printHelp(
                std::ostream &stream,
                std::optional< std::string > usage = std::nullopt
            );

            std::optional< Result > process(int argc, char **argv);
    };
}
#define SSCO_Fn(x) void x(SSCO::Result &opts)

#endif // _ssco_h