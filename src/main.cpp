// Copyright 2026 Maksim Drobyak

#include "textgen.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    int npref = 2;
    int maxgen = 1500;
    std::string inputFile = "input.txt";
    std::string outputFile = "result/gen.txt";

    if (argc > 1) inputFile = argv[1];
    if (argc > 2) maxgen = std::stoi(argv[2]);
    if (argc > 3) npref = std::stoi(argv[3]);

    TextGenerator tg(npref);
    tg.parseFile(inputFile);
    tg.generate(maxgen, outputFile);

    std::cout << "Generated text saved to " << outputFile << std::endl;
    return 0;
}
