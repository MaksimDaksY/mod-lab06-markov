// Copyright 2026 Maksim Drobyak

#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <deque>
#include <map>
#include <string>
#include <vector>

class TextGenerator {
public:
    using Prefix = std::deque<std::string>;
    using Statetab = std::map<Prefix, std::vector<std::string>>;

    explicit TextGenerator(int npref = 2);
    void parseFile(const std::string& filename);
    void generate(int maxWords, const std::string& outputFile);
    std::string generateString(int maxWords);

    const Statetab& getStatetab() const { return statetab; }
    const Prefix& getFirstPrefix() const { return firstPrefix; }
    void setTable(const Statetab& newTable, const Prefix& first) {
        statetab = newTable;
        firstPrefix = first;
    }

private:
    int NPREF;
    Statetab statetab;
    Prefix firstPrefix;
};

#endif
