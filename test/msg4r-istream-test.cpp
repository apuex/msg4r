#define BOOST_TEST_MODULE IStream Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

BOOST_AUTO_TEST_CASE(istream1_test) {
  uint8_t b1 = 0x7e;
  uint8_t b2;
  std::string s1 = "01234567";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  msg4r::write(ssm, b1);
  msg4r::read(ssm, s2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(s1 == s2);
  msg4r::read(ssm, b2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(b1 == b2);
}

BOOST_AUTO_TEST_CASE(istream2_test) {
  uint8_t b1 = 0x7e;
  uint8_t b2;
  std::string s1 = "01234567";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  msg4r::read(ssm, s2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(s1 == s2);
  msg4r::read(ssm, b2);
  BOOST_TEST(ssm.eof());
  BOOST_TEST(b1 != b2);
}

