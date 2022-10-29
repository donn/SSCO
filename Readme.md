# Stupid Simple CPP Opts
A disgusting yet effective single header for commandline option processing in C++.

## Why?
Getopt doesn't allow you to put options after arguments. I think that requirement is kinda archaic. Also, getopt-long isn't portable, meaning that you're expected to just forget about long arguments on the BSDs and macOS. Oh, and on those platforms, you have to use the traditional UNIX flags-first paradigm- `program input -o output` is invalid for example.

## How?
Requires a C++17-compliant compiler. It makes frequent use of the language's features, including optionals, vectors, maps, lambdas, all that fun stuff. It is very much intended for modern C++ commandline tools and not for embedded systems that might require an older compiler version.

## What does this support?
Boolean option flags and string arguments, as well as an "executor" that can be executed immediately after function processing. Also automatically generates help. That's it.

```c++
using O = SSCO::Option;

SSCO::Options options({
    O{
        "version", // Long Name
        'V', // Short Name
        "Show the current version of this app.", // Description
        false, // Has Argument
        [&](SSCO::Result &_){std::cout << "0.1.1" << std::endl; exit(0);} // Lambda executor
    },
    O{
        "count",
        'c',
        "Count",
        true
    }
});

auto opts = options.process(argc, argv);
if (opts.has_value()) {
    // There is a map with options here
} else {
    opts.printHelp(std::cout);
    exit(-1);
}
```

## Wasn't this just a gist a while back?
Yes.

## Wasn't this also header-only a while back?
Also yes.

# License
The Unlicense public domain dedication/license. Check 'Unlicense'. A link back would be nice though.