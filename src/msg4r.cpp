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
  if (decode_state::DECODE_EXPECTING == read(is, length)) {
    return decode_state::DECODE_EXPECTING;
  }

  if (expecting(is, length)) {
    return decode_state::DECODE_EXPECTING;
  }

  for(uint32_t i = 0; i != length; ++i) {
    uint8_t c;
    read(is, c);
    v.push_back(c);
  }
  return decode_state::DECODE_SUCCESS;
}

encode_state write(std::ostream& os, const std::string& v) {
  uint32_t length = static_cast<uint32_t>(v.length());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}


std::ostream& operator<<(std::ostream& os, const encode_state& t) {
  switch (t) {
    case encode_state::ENCODE_SUCCESS:
      os << "ENCODE_SUCCESS";
      break;
    case encode_state::ENCODE_WAITING:
      os << "ENCODE_WAITING";
      break;
    case encode_state::ENCODE_FAILURE:
      os << "ENCODE_FAILURE";
      break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const decode_state& t) {
  switch (t) {
    case decode_state::DECODE_SUCCESS:
      os << "DECODE_SUCCESS";
      break;
    case decode_state::DECODE_EXPECTING:
      os << "DECODE_EXPECTING";
      break;
    case decode_state::DECODE_FAILURE:
      os << "DECODE_FAILURE";
      break;
  }
  return os;
}

}
