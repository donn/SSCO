test: example.cpp include/SSCO/StupidSimpleCPPOpts.h
	c++ -std=c++17 -I include -o $@ example.cpp