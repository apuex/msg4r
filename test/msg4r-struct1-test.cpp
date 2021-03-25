#define BOOST_TEST_MODULE Struct1 Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.h>
#include <sstream>

namespace msg4r {
typedef struct struct1 {
  uint16_t version;
  uint16_t class_type;
  uint32_t id;
  uint16_t value_type;
  std::string name;
  uint16_t state;
  float64_t analog;
  std::string str;
  bool operator==(const struct1& v) const {
    if(this->version != v.version) return false;
    if(this->class_type != v.class_type) return false;
    if(this->id != v.id) return false;
    if(this->value_type != v.value_type) return false;
    if(this->name != v.name) return false;
    if(this->state != v.state) return false;
    if(this->analog != v.analog) return false;
    if(this->str != v.str) return false;
    return true;
  }
} struct1_t;

std::istream& read(std::istream& is, struct1_t& v) {
  msg4r::read(is, v.version);
  msg4r::read(is, v.class_type);
  msg4r::read(is, v.id);
  msg4r::read(is, v.value_type);
  msg4r::read(is, v.name);
  msg4r::read(is, v.state);
  msg4r::read(is, v.analog);
  msg4r::read(is, v.str);
  return is;
}

std::ostream& write(std::ostream& os, const struct1_t& v) {
  msg4r::write(os, v.version);
  msg4r::write(os, v.class_type);
  msg4r::write(os, v.id);
  msg4r::write(os, v.value_type);
  msg4r::write(os, v.name);
  msg4r::write(os, v.state);
  msg4r::write(os, v.analog);
  msg4r::write(os, v.str);
  return os;
}

std::ostream& operator<<(std::ostream& os, const msg4r::struct1& v) {
  os << "msg4r::struct1 {"
     << " version: 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v.version << ","
     << " class_type: 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v.class_type << ","
     << " id: " << std::dec << v.id << ","
     << " value_type: " << v.value_type << ","
     << " name: \"" << v.name << "\","
     << " state: " << v.state << ","
     << " analog: " << v.analog << ","
     << " str: \"" << v.str << "\""
     << " }";
  return os;
}

} // namespace msg4r

BOOST_AUTO_TEST_CASE(struct1_test) {
  msg4r::struct1_t s1 = {
    0xcafe,
    0xbeef,
    0x00000001,
    0x0001,
    "cpu.temp",
    0x01,
    35.6,
    "Fuck Trump. If you like Trump, Fuck you too."
  };
  msg4r::struct1_t s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  std::cout << "bytes: [ ";
  std::for_each(str.begin(), str.end(), [&](auto& e) {
      std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
          << (static_cast<uint32_t>(e) & 0xff) << " ";
    });
  std::cout << "]" << std::endl;
  msg4r::read(ssm, s2);
  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

