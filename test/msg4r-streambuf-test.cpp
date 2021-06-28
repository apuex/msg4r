#define BOOST_TEST_MODULE Streambuf Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/streambuf.hpp>

BOOST_AUTO_TEST_CASE(string_test) {
  std::string str_out("hello, world!");
  std::string str_in;
  boost::asio::streambuf sb_out;
  boost::asio::streambuf sb_in;
  std::ostream os(&sb_out);
  os << str_out;
  msg4r::print_bytes(std::cout,
    boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  std::copy(boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size(),
    boost::asio::buffers_begin(sb_in.prepare(sb_out.size())));
  sb_in.commit(sb_out.size());
  BOOST_TEST(sb_in.size() == sb_out.size());

  std::istream is(&sb_in);
  std::getline(is, str_in);
  std::cout << str_in << std::endl;
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
}

BOOST_AUTO_TEST_CASE(uint16_streambuf_test) {
  std::vector<uint16_t> s1 = { 0xcafe, 0xbeef };
  std::vector<uint16_t> s2;
  msg4r::vector_parser<msg4r::number_parser<uint16_t> > uint16_vector_parser;

  boost::asio::streambuf sb_out;
  boost::asio::streambuf sb_in;
  std::ostream os(&sb_out);
  msg4r::write(os, s1);
  msg4r::print_bytes(std::cout,
    boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  std::copy(boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size(),
    boost::asio::buffers_begin(sb_in.prepare(sb_out.size())));
  sb_in.commit(sb_out.size());
  BOOST_TEST(sb_in.size() == sb_out.size());

  std::istream is(&sb_out);
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
  uint16_vector_parser(is, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint16_stringstream_test) {
  std::vector<uint16_t> s1 = { 0xcafe, 0xbeef };
  std::vector<uint16_t> s2;
  msg4r::vector_parser<msg4r::number_parser<uint16_t> > uint16_vector_parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  uint16_vector_parser(ssm, s2);
  BOOST_TEST(s1 == s2);

  s1 = { 0xdead, 0xface};
  s2.clear();
  msg4r::write(ssm, s1);
  uint16_vector_parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}
