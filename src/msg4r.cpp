#include <msg4r.h>

namespace msg4r {

bool expecting(std::istream& is, size_t size) {
  std::istream::pos_type cur_pos = is.tellg();
  is.seekg(0, is.end);
  std::istream::pos_type end_pos = is.tellg();
  is.seekg(cur_pos);
  return ((end_pos - cur_pos) < size);
}

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
