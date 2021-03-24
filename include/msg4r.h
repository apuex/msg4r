#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <boost/endian/conversion.hpp>

namespace msg4r {

#ifdef BIG_ENDIAN
#define from_native boost::endian::native_to_big
#define to_native boost::endian::big_to_native
#else
#define from_native boost::endian::native_to_little
#define to_native boost::endian::little_to_native
#endif

std::istream& read(std::istream& is, std::string& v);
std::ostream& write(std::ostream& os, const std::string& v);

template<typename T>
std::istream& read(std::istream& is, T& v) {
  T t;
  is.read((char*)&t, sizeof(t));
  v = to_native(t);
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const T& v) {
  T t = from_native(v);
  os.write((char*)&t, sizeof(t));
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::vector<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const std::vector<T>& v) {
  uint32_t length = static_cast<uint32_t>(v.length());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::list<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return is;
}

template<typename T>
std::ostream& write(std::ostream& os, const std::list<T>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return os;
}

template<typename T>
std::istream& read(std::istream& is, std::set<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.insert(c);
  }
  return is;
}

template<typename K, typename V>
std::istream& read(std::istream& is, std::map<K, V>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    K k;
    V v;
    read(is, k);
    read(is, v);
    v.insert(std::make_pair(k, v));
  }
  return is;
}

template<typename K, typename V>
std::ostream& write(std::ostream& os, const std::map<K, V>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e.first);
      write(os, e.second);
    });
  return os;
}

}
