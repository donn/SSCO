// Stupid Simple CPP Opts - Mohamed Gaber - Unlicense
#ifndef _stupidsimplecppopts_h
#define _stupidsimplecppopts_h

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <optional>
#include <ostream>

namespace SSCO {
    struct Result {
        std::map<std::string, std::string> options;
        std::vector<std::string> arguments;
    };

    struct Option {
        // Name of the option. Will be used in the final map.
        std::string name; 
        // Shorthand of the option. Optional, pass nullopt if none.
        std::optional<char> shorthand; 
        // Description of the option.
        std::string description;
        // Does this option have an argument? If no, the final dictionary will
        // have the string "SET" in lieu of an argument value.
        bool argument; 
        // If there's no argument and this executor isn't nullopt,
        // then this executor is executed instead.
        std::optional< std::function<void()> > executor; 
    };

    class Options {
        std::map<char, Option> byShorthand;
        std::map<std::string, Option> byName;
        std::vector<Option> byOrder;

        std::string executablePath;

    public:
        Options(std::vector<Option> options) {
            for (Option option: options) {
                byName[option.name] = option;
                if (option.shorthand.has_value()) {
                    byShorthand[option.shorthand.value()] = option;
                }
                byOrder.push_back(option);
            }

            executablePath = "executable";
        }

        void printHelp(std::ostream& stream, std::optional<std::string> usage = std::nullopt) {
            if (usage.has_value()) {
                stream << usage.value();
            } else {
                stream << "Usage: " << executablePath << " [Options] <Arguments>" << std::endl;
            }
            stream << std::endl;
            
            for (auto& option: byOrder) {
                if (option.shorthand.has_value()) {
                    stream << "-" << option.shorthand.value();
                    if (option.argument) {
                        stream << " argument";
                    }
                    stream << std::endl;
                }
                stream << "--" << option.name;
                if (option.argument) {
                    stream << " argument";
                }
                stream << std::endl;
                int emptySpace = 80 - option.description.length();
                if (emptySpace < 0) {
                    emptySpace = 0;
                }
                stream << std::string(emptySpace, ' ') << option.description << std::endl;
                stream << std::endl;
            }
        }

        std::optional<Result> process(int argc, char** argv) {
            Result result;
            executablePath = std::string(argv[0]);
            for (int i = 1; i < argc; i += 1) {
                std::string argument = std::string(argv[i]);
                if (argument.rfind("--", 0) == 0) {
                    auto name = std::string(argv[i] + 2);
                    auto iterator = byName.find(name);
                    if (iterator == byName.end()) {
                        return std::nullopt;
                    }
                    auto& option = iterator->second;
                    if (option.argument) {
                        if (i + 1 == argc) {
                            return std::nullopt;
                        }
                        result.options[name] = std::string(argv[i + 1]);
                        i += 1;
                    } else {
                        if (option.executor.has_value()) {
                            option.executor.value()();
                        } else {
                            result.options[option.name] = "SET";
                        }
                    }
                } else if (argument.rfind("-", 0) == 0) {
                    // Short arguments
                    Option option;
                    for (size_t j = 1; j < argument.length(); j += 1) {
                        auto iterator = byShorthand.find(argument[j]);
                        if (iterator == byShorthand.end()) {
                            return std::nullopt;
                        }
                        option = iterator->second;
                        if (option.argument &&  (j != argument.length() - 1)) {
                            return std::nullopt;
                        } else {
                            if (option.executor.has_value()) {
                            // "Now, some people will claim that having 8-character indentations makes the code move too far to the right, and makes it hard to read on a 80-character terminal screen. The answer to that is that if you need more than 3 levels of indentation, you’re screwed anyway, and should fix your program." - Linus Torvalds
                                option.executor.value()();
                            } else {
                                result.options[option.name] = "SET";
                            }
                        }
                    }
                    if (option.argument) {
                        if (i + 1 == argc) {
                            return std::nullopt;
                        }
                        result.options[option.name] = std::string(argv[i + 1]);
                        i += 1;
                    }
                } else {
                    result.arguments.push_back(argument);
                }
            }
            return result;
        }
    };
}

#endif // _stupidsimplecppopts_h
