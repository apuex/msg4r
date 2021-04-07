#define BOOST_TEST_MODULE Struct1 Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

namespace msg4r {

typedef struct struct1 {
  std::string name;
  float32_t salary;
  float64_t capital;
  std::string title;
  std::map<std::string, std::string> props;
} struct1_t;

// for equals tests in testcases.
bool operator==(const struct1& lhs, const struct1& v) {
  return std::tie( lhs.name
                 , lhs.salary
                 , lhs.capital
                 , lhs.title
                 , lhs.props
                 )
                 == 
         std::tie( v.name
                 , v.salary
                 , v.capital
                 , v.title
                 , v.props
                 );
}

typedef struct struct1_parser {
  typedef struct1 value_type;
  struct1_parser();
  virtual ~struct1_parser();

  decode_state operator()(std::istream& is, value_type& v);
  void reset();

  // internal states 
  // to remember parsing progress
  int state_;
  msg4r::string_parser parse_string_;
  msg4r::number_parser<float32_t> parse_float32_;
  msg4r::number_parser<float64_t> parse_float64_;
  msg4r::map_parser<msg4r::string_parser, msg4r::string_parser> parse_string_string_map_;
} struct1_parser_t;

decode_state struct1_parser::operator()(std::istream& is, value_type& v) {
  BEGIN_PARSER()
  PARSE_FIELD(parse_string_, is, v.name)
  PARSE_FIELD(parse_float32_, is, v.salary)
  PARSE_FIELD(parse_float64_, is, v.capital)
  PARSE_FIELD(parse_string_, is, v.title)
  PARSE_FIELD(parse_string_string_map_, is, v.props)
  END_PARSER()
}

/*
decode_state struct1_parser::operator()(std::istream& is, value_type& v) {
  decode_state field_state;
  // duplicated case pattern can be replaced by 
  // list iteration or macro
  // to be declarative instead of instruction procedures.
  switch (state_) { 
  case 0:
    field_state = parse_string_(is, v.name);
    if (decode_state::DECODE_SUCCESS != field_state) return field_state;
    state_ += 1;
  case 1:
    field_state = parse_float32_(is, v.salary);
    if (decode_state::DECODE_SUCCESS != field_state) return field_state;
    state_ += 1;
  case 2:
    field_state = parse_float64_(is, v.capital);
    if (decode_state::DECODE_SUCCESS != field_state) return field_state;
    state_ += 1;
  case 3:
    field_state = parse_string_(is, v.title);
    if (decode_state::DECODE_SUCCESS != field_state) return field_state;
    state_ += 1;
  case 4:
    field_state = parse_string_string_map_(is, v.props);
    if (decode_state::DECODE_SUCCESS != field_state) return field_state;
    state_ += 1;
  case 5:
    reset();
    return decode_state::DECODE_SUCCESS;
  default:
    return decode_state::DECODE_FAILURE;
  }
}
*/

struct1_parser::struct1_parser()
    : state_(),
      parse_string_(),
      parse_float32_(),
      parse_float64_(),
      parse_string_string_map_() {}

struct1_parser::~struct1_parser() {}

void struct1_parser::reset() {
  state_ = 0;
  parse_string_.reset();
  parse_float32_.reset();
  parse_float64_.reset();
  parse_string_string_map_.reset();
  parse_string_.reset();
}

std::ostream& write(std::ostream& os, const struct1_t& v) {
  msg4r::write(os, v.name);
  msg4r::write(os, v.salary);
  msg4r::write(os, v.capital);
  msg4r::write(os, v.title);
  msg4r::write(os, v.props);
  return os;
}

std::ostream& operator<<(std::ostream& os, const msg4r::struct1& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::struct1 {"
     << " name: " << v.name << ","
     << " salary: " << v.salary << ","
     << " capital: " << v.capital << ","
     << " title: " << v.title << ","
     << " props: " << v.props << ""
     << " }";
  return os;
}

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(struct1_test) {
  msg4r::struct1_parser_t parse_struct1;
  msg4r::struct1_t s1 = {
    "Donald J. Trump",
    35.6f,
    12345678901.234,
    "President",
    { {"mother", "fucker"}, { "dead", "face" } }
  };
  msg4r::struct1_t s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse_struct1(ssm, s2);
  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  std::cout << "(s1 == s2) => " << (s1 == s2) << std::endl;
  BOOST_TEST(s1 == s2);
}

