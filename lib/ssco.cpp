#include "ssco.hpp"

using namespace SSCO;

Options::Options(std::vector< Option > options) {
    for (Option option : options) {
        byName[option.name] = option;
        if (option.shorthand.has_value()) {
            byShorthand[option.shorthand.value()] = option;
        }
        byOrder.push_back(option);
    }

    executablePath = "executable";
}

void Options::printHelp(
    std::ostream &stream,
    std::optional< std::string > usage
) {
    if (usage.has_value()) {
        stream << usage.value();
    } else {
        stream << "Usage: " << executablePath << " [Options] <Arguments>"
               << std::endl;
    }
    stream << std::endl;

    for (auto &option : byOrder) {
        if (option.shorthand.has_value()) {
            stream << "-" << option.shorthand.value() << " / ";
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
        stream << std::string(emptySpace, ' ') << option.description
               << std::endl;
        stream << std::endl;
    }
}

std::optional< Result > Options::process(int argc, char **argv) {
    Result result;
    executablePath = std::string(argv[0]);
    std::optional< std::function< void(Result &) > > foundExecutor =
        std::nullopt;
    for (int i = 1; i < argc; i += 1) {
        std::string argument = std::string(argv[i]);

        if (argument.rfind("--", 0) == 0) {
            auto name = std::string(argv[i] + 2);
            auto iterator = byName.find(name);
            if (iterator == byName.end()) {
                return std::nullopt;
            }
            auto &option = iterator->second;
            if (option.argument) {
                if (i + 1 == argc) {
                    return std::nullopt;
                }
                result.options[name] = std::string(argv[i + 1]);
                i += 1;
            } else {
                result.options[option.name] = "SET";
            }
            if (option.executor.has_value() && !foundExecutor.has_value()) {
                foundExecutor = option.executor;
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
                if (option.argument && (j != argument.length() - 1)) {
                    return std::nullopt;
                } else {
                    result.options[option.name] = "SET";
                }

                if (option.executor.has_value() && !foundExecutor.has_value()) {
                    // "Now, some people will claim that having
                    // 8-character indentations makes the code
                    // move too far to the right, and makes it
                    // hard to read on a 80-character terminal
                    // screen. The answer to that is that if you
                    // need more than 3 levels of indentation,
                    // you’re screwed anyway, and should fix
                    // your program." - Linus Torvalds
                    foundExecutor = option.executor;
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
    if (foundExecutor.has_value()) {
        foundExecutor.value()(result);
    }
    return result;
}
