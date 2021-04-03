#define BOOST_TEST_MODULE Number Parser Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

BOOST_AUTO_TEST_CASE(uint8_t_test) {
  uint8_t s1 = 0xca;
  uint8_t s2;
  msg4r::number_parser<uint8_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xfe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint16_t_test) {
  uint16_t s1 = 0xdead;
  uint16_t s2;
  msg4r::number_parser<uint16_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint32_t_test) {
  uint32_t s1 = 0xdead;
  uint32_t s2;
  msg4r::number_parser<uint32_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint64_t_test) {
  uint64_t s1 = 0xdead;
  uint64_t s2;
  msg4r::number_parser<uint64_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(int8_t_test) {
  int8_t s1 = 0xca;
  int8_t s2;
  msg4r::number_parser<int8_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xfe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(int16_t_test) {
  int16_t s1 = 0xdead;
  int16_t s2;
  msg4r::number_parser<int16_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(int32_t_test) {
  int32_t s1 = 0xdead;
  int32_t s2;
  msg4r::number_parser<int32_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(int64_t_test) {
  int64_t s1 = 0xdead;
  int64_t s2;
  msg4r::number_parser<int64_t> parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = 0xcafe;
  ssm.str("");
  msg4r::write(ssm, s1);
  parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}


