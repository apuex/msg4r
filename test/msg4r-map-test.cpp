#define BOOST_TEST_MODULE Map Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

BOOST_AUTO_TEST_CASE(string_test) {
  std::map<std::string, std::string> s1 = { {"hello", "world"}, {"bob", "alice"} };
  std::map<std::string, std::string> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  std::for_each(str.begin(), str.end(), [&](auto& e) {
      std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
          << static_cast<uint32_t>(e) << " ";
    });
    std::cout << std::endl;
  msg4r::read(ssm, s2);
  std::for_each(s2.begin(), s2.end(), [&](auto& e) {
      std::cout << "(" << e.first << ", " << e.second << ")" << std::endl;
    });
  BOOST_TEST(s1 == s2);
  s1 = { { "hi", "there" }, { "good", "night" } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  std::for_each(str.begin(), str.end(), [&](auto& e) {
      std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
          << static_cast<uint32_t>(e) << " ";
    });
    std::cout << std::endl;
  msg4r::read(ssm, s2);
  std::for_each(s2.begin(), s2.end(), [&](auto& e) {
      std::cout << "(" << e.first << ", " << e.second << ")" << std::endl;
    });
  BOOST_TEST(s1 == s2);
}
