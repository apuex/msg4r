#include <msg4r.h>

namespace msg4r {

std::istream& read(std::istream& is,  int8_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, int8_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  int16_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, int16_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  int32_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, int32_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  int64_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, int64_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}


std::istream& read(std::istream& is,  uint8_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, uint8_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  uint16_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, uint16_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  uint32_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, uint32_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}

std::istream& read(std::istream& is,  uint64_t& v) {
  is.read((char*)&v, sizeof(v));
  return is;
}

std::ostream& write(std::ostream& os, uint64_t& v) {
  os.write((char*)&v, sizeof(v));
  return os;
}


std::istream& read(std::istream& is, std::string& v) {
  return is;
}

std::ostream& write(std::ostream& os, const std::string& v) {
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::vector<T>& v) {
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const std::vector<T>& v) {
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::list<T>& v) {
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const std::list<T>& v) {
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::set<T>& v) {
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const std::set<T>& v) {
  return os;
}

template<typename K, typename V>
std::istream& read(std::istream& is, std::map<K, V>& v) {
  return is;
}

template<typename K, typename V>
std::ostream& write(std::ostream& os, const std::map<K, V>& v) {
  return os;
}

}
