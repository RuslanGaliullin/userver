#include <gtest/gtest.h>

#include <userver/utils/regex.hpp>

USERVER_NAMESPACE_BEGIN

TEST(Regex, Ctors) {
  utils::regex r1;
  utils::regex r2("regex*test");
  utils::regex r3(std::move(r2));
  utils::regex r4(r3);
  utils::regex r5;
  r5 = std::move(r4);
  utils::regex r6;
  r6 = r5;
}

TEST(Regex, Match) {
  utils::regex r("^[a-z][0-9]+");
  EXPECT_FALSE(utils::regex_match({}, r));
  EXPECT_FALSE(utils::regex_match("a", r));
  EXPECT_FALSE(utils::regex_match("123", r));
  EXPECT_TRUE(utils::regex_match("a123", r));
  EXPECT_TRUE(utils::regex_match("a1234", r));
  EXPECT_FALSE(utils::regex_match("a123a", r));
}

TEST(Regex, Search) {
  utils::regex r("^[a-z][0-9]+");
  EXPECT_FALSE(utils::regex_search({}, r));
  EXPECT_FALSE(utils::regex_search("a", r));
  EXPECT_FALSE(utils::regex_search("123", r));
  EXPECT_TRUE(utils::regex_search("a123", r));
  EXPECT_TRUE(utils::regex_search("a1234", r));
  EXPECT_TRUE(utils::regex_search("a123a", r));
}

TEST(Regex, SearchWithResult) {
  utils::regex r("^[a-z][0-9]+");
  utils::smatch fail;
  const std::string str_empty{};
  EXPECT_FALSE(utils::regex_search(str_empty, fail, r));
  ASSERT_EQ(fail.size(), 1);
  const std::string empty = fail[0];
  EXPECT_EQ(empty, str_empty);
  utils::smatch success;
  const std::string str{"a1234"};
  EXPECT_TRUE(utils::regex_search(str, success, r));
  ASSERT_EQ(success.size(), 1);
  const std::string res = success[0];
  EXPECT_EQ(res, str);
}

TEST(Regex, Replace) {
  utils::regex r("[a-z]{2}");
  std::string repl{"R"};
  EXPECT_EQ(utils::regex_replace({}, r, repl), "");
  EXPECT_EQ(utils::regex_replace({"a0AB1c2"}, r, repl), "a0AB1c2");
  EXPECT_EQ(utils::regex_replace("ab0ef1", r, repl), "R0R1");
  EXPECT_EQ(utils::regex_replace("abcd", r, repl), "RR");
}

TEST(Regex, MatchWithResult) {
  utils::regex r("^[a-z][0-9]+");
  utils::smatch fail;
  const std::string str_empty{};
  EXPECT_FALSE(utils::regex_search(str_empty, fail, r));
  ASSERT_EQ(fail.size(), 1);
  const std::string empty = fail[0];
  EXPECT_EQ(empty, str_empty);
  utils::smatch success;
  const std::string str{"a1234"};
  EXPECT_TRUE(utils::regex_match(str, success, r));
  ASSERT_EQ(success.size(), 1);
  const std::string res = success[0];
  EXPECT_EQ(res, str);
}

USERVER_NAMESPACE_END
