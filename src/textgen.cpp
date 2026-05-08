// Copyright 2026 Maksim Drobyak

#include "textgen.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

TextGenerator::TextGenerator(int npref) : NPREF(npref), rng(std::random_device{}()) {}

void TextGenerator::parseFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return;
  }

  std::vector<std::string> words;
  std::string word;
  while (file >> word) {
    words.push_back(word);
  }
  file.close();

  if (words.size() < static_cast<size_t>(NPREF) + 1) {
    std::cerr << "File too short for given prefix size." << std::endl;
    return;
  }

  firstPrefix.clear();
  for (int i = 0; i < NPREF; ++i) {
    firstPrefix.push_back(words[i]);
  }

  for (size_t i = 0; i <= words.size() - NPREF - 1; ++i) {
    Prefix prefix;
    for (int j = 0; j < NPREF; ++j) {
      prefix.push_back(words[i + j]);
    }
    std::string suffix = words[i + NPREF];
    statetab[prefix].push_back(suffix);
  }
}

void TextGenerator::generate(int maxWords, const std::string& outputFile) {
  std::ofstream out(outputFile);
  if (!out.is_open()) {
    std::cerr << "Cannot create output file: " << outputFile << std::endl;
    return;
  }
  out << generateString(maxWords);
  out.close();
}

std::string TextGenerator::generateString(int maxWords) {
  if (statetab.empty()) {
    return "";
  }
  std::string result;
  Prefix current = firstPrefix;
  for (const auto& w : current) {
    result += w + " ";
  }
  int generated = NPREF;
  while (generated < maxWords) {
    auto it = statetab.find(current);
    if (it == statetab.end() || it->second.empty()) {
      break;
    }
    const auto& suffixes = it->second;
    std::uniform_int_distribution<> dist(0, suffixes.size() - 1);
    int index = dist(rng);
    std::string nextWord = suffixes[index];
    result += nextWord + " ";
    current.pop_front();
    current.push_back(nextWord);
    generated++;
  }
  return result;
}
