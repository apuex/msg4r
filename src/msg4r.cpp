#include <msg4r.h>

namespace msg4r {

decode_state read(std::istream& is, std::string& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    uint8_t c;
    read(is, c);
    v.push_back(c);
  }
  return DECODE_SUCCESS;
}

encode_state write(std::ostream& os, const std::string& v) {
  uint32_t length = static_cast<uint32_t>(v.length());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return ENCODE_SUCCESS;
}

}
