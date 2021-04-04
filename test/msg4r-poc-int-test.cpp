#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <sstream>

template <typename T>
struct integer_parser {
  integer_parser(): value(0), index(0) { }
  enum state { COMPLETE=0, INPROG=1, ERROR=2 };  
  
  /**
   * POC, one byte a time.
   */
  state accept(char c) {
    if(sizeof(T) == index) {
      printf("POC: %d - already complete.\n", index);
      return COMPLETE;
    }
    *(reinterpret_cast<char*>(&value) + index) = c;
    index += 1;
    if(index == sizeof(T)) {
      printf("POC: %d complete.\n", index);
      return COMPLETE;
    } else {
      printf("POC: %d in-progress.\n", index);
      return INPROG;
    }
  }

  /**
   * High performance version.
   */
  state operator()(std::istream& is, T& v) {
    if(sizeof(T) == index) {
      printf("HPV: %d - already complete.\n", index);
      return COMPLETE;
    }
    auto initial = is.gcount();
    is.read((char*)&value + index, sizeof(T) - index);
    auto current = is.gcount();
    index += (current - initial);
    if(index == sizeof(T)) {
      v = value;
      printf("HPV: %d complete.\n", index);
      return COMPLETE;
    } else {
      printf("HPV: %d in-progress.\n", index);
      return INPROG;
    }
  }

  T value;
  int8_t index;
};

int main(int argc, char* argv[]) {
  uint32_t s1 = 0xcafebeef;
  uint32_t s2 = 0x0000;
  integer_parser<uint32_t> parse_uint32;
  integer_parser<uint32_t> uint32_parser;
  std::stringstream ssm1;
  std::stringstream ssm2;
  ssm1.write((char*)&s1, sizeof(s1) / 2);
  ssm2.write((char*)&s1 + sizeof(s1) / 2, sizeof(s1) / 2);
  parse_uint32(ssm1, s2);
  parse_uint32(ssm2, s2);
  printf("HPV: 0x%08X => 0x%08X\n", s1, s2);
  auto str1 = ssm1.str();
  auto str2 = ssm2.str();
  using namespace std::placeholders;
  std::for_each(str1.begin(), str1.end(), std::bind(&integer_parser<uint32_t>::accept, &uint32_parser, _1));
  std::for_each(str2.begin(), str2.end(), std::bind(&integer_parser<uint32_t>::accept, &uint32_parser, _1));
  printf("POC: 0x%08X => 0x%08X.\n", s1, uint32_parser.value);

  return 0;
}

