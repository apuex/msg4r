#define BOOST_TEST_MODULE Struct1 Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

namespace msg4r {

enum class data_point_type {
  DI = 0,
  AI = 1,
  SI = 2,
};

std::ostream& operator<<(std::ostream& os, const data_point_type& v) {
  switch(v) {
    case data_point_type::DI:
      os << "data_point_type::DI";
      break;
    case data_point_type::AI:
      os << "data_point_type::AI";
      break;
    case data_point_type::SI:
      os << "data_point_type::SI";
      break;
    default:
      os << "data_point_type::UNKNOWN";
      break;
  }
  return os;
}

class data_point {
protected:
  data_point(const data_point_type t);
public:
  virtual ~data_point();
  const data_point_type data_type;
};

data_point::data_point(const data_point_type v): data_type(v) {}
data_point::~data_point() {}

class di_point: public data_point {
public:
  di_point();
  di_point(bool v);
  virtual ~di_point();
  bool value;
};

di_point::di_point(): data_point(data_point_type::DI), value() {}
di_point::di_point(bool v): data_point(data_point_type::DI), value(v) {}
di_point::~di_point() {}

// for equals tests in testcases.
bool operator==(const di_point& lhs, const di_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const di_point& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::di_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << std::boolalpha
     << " value: " << v.value
     << " }";
  return os;
}

class ai_point: public data_point {
public:
  ai_point();
  ai_point(double v);
  virtual ~ai_point();
  double value;
};

ai_point::ai_point(): data_point(data_point_type::AI), value() {}
ai_point::ai_point(double v): data_point(data_point_type::AI), value(v) {}
ai_point::~ai_point() {}

// for equals tests in testcases.
bool operator==(const ai_point& lhs, const ai_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const ai_point& v) {
  os << std::fixed
     << "msg4r::ai_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << std::fixed
     << " value: " << v.value
     << " }";
  return os;
}


class si_point: public data_point {
public:
  si_point();
  si_point(const std::string& v);
  virtual ~si_point();
  std::string value;
};

si_point::si_point(): data_point(data_point_type::SI), value() {}
si_point::si_point(const std::string& v): data_point(data_point_type::SI), value(v) {}
si_point::~si_point() {}

  // for equals tests in testcases.
bool operator==(const si_point& lhs, const si_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const si_point& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::si_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << " value: " << v.value
     << " }";
  return os;
}

/*
typedef struct data_point_parser {
  DECLARE_PARSER_FOR(data_point)
public:
  data_point_parser();
  virtual ~data_point_parser();

private:
  msg4r::number_parser<uint8_t> parse_uint8_;
  msg4r::number_parser<float64_t> parse_float64_;
  msg4r::string_parser string_parser;
} data_point_parser_t;

BEGIN_IMPLEMENT_PARSER(data_point_parser)
  PARSE_FIELD(parse_uint8_, is, v.data_type)
  PARSE_DYNAMIC_FIELD(parse_factory, v.data_type, is, v.value)
END_IMPLEMENT_PARSER()

data_point_parser::data_point_parser()
    : state_(),
      parse_uint32_(),
      parse_float64_() {}

data_point_parser::~data_point_parser() {}

void data_point_parser::reset() {
  state_ = 0;
  parse_float64_.reset();
  parse_uint8_.reset();
}

template<typename T>
std::ostream& write(std::ostream& os, const T& v) {
  msg4r::write(os, v.data_type);
  msg4r::write(os, v.value);
  return os;
}
*/

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(data_point_test) {
  msg4r::di_point di = {
    true
  };
  std::cout << "di = " << di << std::endl;
  msg4r::ai_point ai = {
    35.6f
  };
  std::cout << "ai = " << ai << std::endl;
  msg4r::si_point si = {
    "hello, world!"
  };
  std::cout << "si = " << si << std::endl;
  /*
  msg4r::data_point_parser_t parse_data_point;
  msg4r::ai_point_t s2;
  std::stringstream ssm;
  msg4r::decode_state state;
  msg4r::write(ssm, s1);
  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(63));
  ssm.seekg(pos);
  
  std::string str = ssm.str();
  std::cout << "complete message:" << std::endl; 
  msg4r::print_bytes(std::cout, str);

  state = parse_data_point(ssm, s2);
  std::cout << "parse ssm => " << state << std::endl;
  BOOST_TEST(decode_state::DECODE_SUCCESS == state);

  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  std::cout << "(s1 == s2) => " << (s1 == s2) << std::endl;
  BOOST_TEST(s1 == s2);
   */
}

