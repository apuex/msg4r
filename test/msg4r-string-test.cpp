#define BOOST_TEST_MODULE Integer Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

BOOST_AUTO_TEST_CASE(string_test) {
  std::string s1 = "hello, world!";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  msg4r::read(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = "hi, honey!";
  s2 = "";
  msg4r::write(ssm, s1);
  msg4r::read(ssm, s2);
  BOOST_TEST(s1 == s2);
}
