#define BOOST_TEST_MODULE Integer Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

BOOST_AUTO_TEST_CASE(string_test) {
  std::list<uint16_t> s1 = { 0xcafe, 0xbeef };
  std::list<uint16_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  msg4r::read(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = { 0xdead, 0xface};
  s2 = { };
  msg4r::write(ssm, s1);
  msg4r::read(ssm, s2);
  BOOST_TEST(s1 == s2);
}
