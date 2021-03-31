#define BOOST_TEST_MODULE Map Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

using namespace msg4r;

BOOST_AUTO_TEST_CASE(string_test) {
  std::map<std::string, std::string> s1 = { {"hello", "world"}, {"bob", "alice"} };
  std::map<std::string, std::string> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { "hi", "there" }, { "good", "night" } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint16_test) {
  std::map<uint16_t, uint16_t> s1 = { {0xcafe, 0xbeef} };
  std::map<uint16_t, uint16_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { 0xdead, 0xface } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint32_test) {
  std::map<uint32_t, uint32_t> s1 = { {0xcafe, 0xbeef} };
  std::map<uint32_t, uint32_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { 0xdead, 0xface } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

