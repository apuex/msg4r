#include <msg4r.h>

namespace msg4r {

bool expecting(std::istream& is, MSG4R_SIZE_T size) {
  std::istream::pos_type cur_pos = is.tellg();
  is.seekg(0, is.end);
  std::istream::pos_type end_pos = is.tellg();
  is.seekg(cur_pos);
  return ((end_pos - cur_pos) < size);
}

void rollback(std::istream& is, MSG4R_SIZE_T size) {
  std::istream::pos_type cur_pos = is.tellg();
  is.seekg(cur_pos - std::istream::pos_type(size));
}

template<> decode_state number_parser<float32_t>::operator()(std::istream& is, float32_t& v) {
  auto initial = is.gcount();
  is.read((char*)&t_ + index_, sizeof(float32_t));
  auto current = is.gcount();
  index_ += (current - initial);
  if(is.eof()) {
    return decode_state::DECODE_EXPECTING;
  } else {
    *reinterpret_cast<uint32_t*>(&v) = to_native(*reinterpret_cast<uint32_t*>(&t_));
    index_  = 0; // reset to initial state
    t_      = 0; // reset to initial state
    return decode_state::DECODE_SUCCESS;
  }
}

template<> decode_state number_parser<float64_t>::operator()(std::istream& is, float64_t& v) {
  auto initial = is.gcount();
  is.read((char*)&t_ + index_, sizeof(float64_t));
  auto current = is.gcount();
  index_ += (current - initial);
  if(is.eof()) {
    return decode_state::DECODE_EXPECTING;
  } else {
    *reinterpret_cast<uint64_t*>(&v) = to_native(*reinterpret_cast<uint64_t*>(&t_));
    index_  = 0; // reset to initial state
    t_      = 0; // reset to initial state
    return decode_state::DECODE_SUCCESS;
  }
}

encode_state write(std::ostream& os, const float32_t& v) {
  uint32_t t;
  t = from_native(*reinterpret_cast<const uint32_t*>(&v));
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

encode_state write(std::ostream& os, const float64_t& v) {
  uint64_t t;
  t = from_native(*reinterpret_cast<const uint64_t*>(&v));
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

decode_state read(std::istream& is, std::string& v) {
  MSG4R_SIZE_T length;
  if (decode_state::DECODE_EXPECTING == read(is, length)) {
    return decode_state::DECODE_EXPECTING;
  }

  if (expecting(is, length)) {
    rollback(is, sizeof(length));
    return decode_state::DECODE_EXPECTING;
  }

  for (MSG4R_SIZE_T i = 0; i != length; ++i) {
    uint8_t c;
    read(is, c);
    v.push_back(c);
  }
  return decode_state::DECODE_SUCCESS;
}

encode_state write(std::ostream& os, const std::string& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.length());
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

std::ostream& operator<<(std::ostream& os, const std::string& t) {
  std::operator<<(os, "\"");
  std::operator<<(os, t);
  std::operator<<(os, "\"");
  return os;
}

void print_bytes(std::ostream& os, const char* buff, const size_t length) {
  os << "[ ";
  for (size_t i = 0; i != length; ++i) {
    os << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
       << (static_cast<uint32_t>(buff[i]) & 0xff) << " ";
  }
  os << "]" << std::endl;
}

}
