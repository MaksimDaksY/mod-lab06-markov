// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "textgen.h"

using Prefix = std::deque<std::string>;
using Statetab = std::map<Prefix, std::vector<std::string>>;

TEST(MarkovTest, PrefixFormation) {
  TextGenerator tg(2);
  Statetab table;
  Prefix first = {"hello", "world"};
  tg.setTable(table, first);
  const auto& fp = tg.getFirstPrefix();
  EXPECT_EQ(fp.size(), 2);
  EXPECT_EQ(fp[0], "hello");
  EXPECT_EQ(fp[1], "world");
}

TEST(MarkovTest, PrefixSuffixEntry) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref = {"a", "b"};
  table[pref].push_back("c");
  tg.setTable(table, pref);
  const auto& st = tg.getStatetab();
  ASSERT_EQ(st.size(), 1);
  auto it = st.find(pref);
  ASSERT_NE(it, st.end());
  EXPECT_EQ(it->second.size(), 1);
  EXPECT_EQ(it->second[0], "c");
}

TEST(MarkovTest, SingleSuffixChoice) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref = {"one", "two"};
  table[pref].push_back("three");
  tg.setTable(table, pref);
  std::string result = tg.generateString(3);
  EXPECT_EQ(result, "one two three ");
}

TEST(MarkovTest, GenerateExactLength) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref1 = {"a", "b"}; table[pref1].push_back("c");
  Prefix pref2 = {"b", "c"}; table[pref2].push_back("d");
  Prefix pref3 = {"c", "d"}; table[pref3].push_back("e");
  tg.setTable(table, pref1);
  std::string result = tg.generateString(5);
  EXPECT_EQ(result, "a b c d e ");
}

TEST(MarkovTest, StopWhenNoSuffix) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref = {"x", "y"};
  table[pref].push_back("z");
  tg.setTable(table, pref);
  std::string result = tg.generateString(10);
  EXPECT_EQ(result, "x y z ");
}

TEST(MarkovTest, PrefixSizeEffect) {
  TextGenerator tg(1);
  Statetab table;
  Prefix pref1 = {"a"}; table[pref1].push_back("b");
  Prefix pref2 = {"b"}; table[pref2].push_back("a");
  tg.setTable(table, pref1);
  std::string result = tg.generateString(4);
  EXPECT_EQ(result, "a b a b ");
}

TEST(MarkovTest, FirstPrefixSaved) {
  TextGenerator tg(2);
  Prefix expected = {"start", "middle"};
  Statetab table;
  tg.setTable(table, expected);
  const auto& first = tg.getFirstPrefix();
  ASSERT_EQ(first.size(), 2);
  EXPECT_EQ(first[0], "start");
  EXPECT_EQ(first[1], "middle");
}

TEST(MarkovTest, GenerateDoesNotModifyTable) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref = {"const", "test"};
  table[pref].push_back("suffix");
  tg.setTable(table, pref);
  tg.generateString(4);
  const auto& after = tg.getStatetab();
  ASSERT_EQ(after.size(), 1);
  auto it = after.find(pref);
  ASSERT_NE(it, after.end());
  EXPECT_EQ(it->second.size(), 1);
  EXPECT_EQ(it->second[0], "suffix");
}

TEST(MarkovTest, EmptyTableReturnsEmptyString) {
  TextGenerator tg(2);
  Statetab empty;
  tg.setTable(empty, {});
  std::string result = tg.generateString(5);
  EXPECT_TRUE(result.empty());
}

TEST(MarkovTest, MultipleSuffixesSamePrefix) {
  TextGenerator tg(2);
  Statetab table;
  Prefix pref = {"same", "prefix"};
  table[pref].push_back("A");
  table[pref].push_back("B");
  table[pref].push_back("C");
  tg.setTable(table, pref);
  std::set<std::string> found;
  for (int i = 0; i < 50; ++i) {
    std::string result = tg.generateString(3);
    std::string suffix = result.substr(12, 1);
    found.insert(suffix);
    if (found.size() == 3) break;
  }
  EXPECT_EQ(found.size(), 3);
}
