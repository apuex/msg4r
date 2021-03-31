#pragma once

#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>

namespace msg4r {

#ifdef MSG4R_BIG_ENDIAN
#define from_native boost::endian::native_to_big
#define to_native boost::endian::big_to_native
#define float32_t boost::endian::big_float32_t
#define float64_t boost::endian::big_float64_t
#else
#define from_native boost::endian::native_to_little
#define to_native boost::endian::little_to_native
#define float32_t boost::endian::little_float32_t
#define float64_t boost::endian::little_float64_t
#endif

#ifdef _MSC_VER
#define MSG4R_PACKED(n) _declspec(align((n)))
#else
#define MSG4R_PACKED(n) __attribute__((packed))
#endif

enum encode_state {
  ENCODE_SUCCESS   = 0,
  ENCODE_WAITING = 1,
  ENCODE_FAILURE   = 2
};

enum decode_state {
  DECODE_SUCCESS   = 0,
  DECODE_EXPECTING = 1,
  DECODE_FAILURE   = 2
};

decode_state read(std::istream& is, std::string& v);
encode_state write(std::ostream& os, const std::string& v);

/**
 * read integers and floating-point numbers. T = { int8_t, ... }
 */
template<typename T>
decode_state read(std::istream& is, T& v) {
  T t;
  is.read((char*)&t, sizeof(t));
  v = to_native(t);
  return DECODE_SUCCESS;
}

/**
 * write integers and floating-point numbers. T = { int8_t, ... }
 */
template<typename T>
encode_state write(std::ostream& os, const T& v) {
  T t = from_native(v);
  os.write((char*)&t, sizeof(t));
  return ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::vector<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::vector<T>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::list<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::list<T>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::set<T>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.insert(c);
  }
  return DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::set<T>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return ENCODE_SUCCESS;
}

template<typename K, typename V>
decode_state read(std::istream& is, std::map<K, V>& v) {
  uint32_t length;
  read(is, length);
  for(uint32_t i = 0; i != length; ++i) {
    K key;
    V value;
    read(is, key);
    read(is, value);
    v.insert(std::make_pair(key, value));
  }
  return DECODE_SUCCESS;
}

template<typename K, typename V>
encode_state write(std::ostream& os, const std::map<K, V>& v) {
  uint32_t length = static_cast<uint32_t>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e.first);
      write(os, e.second);
    });
  return ENCODE_SUCCESS;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::map<K, V>& t) {
  os << "{ ";
  for(auto e = t.begin(); e != t.end();) {
    os << e->first << ": " << e->second;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::vector<T>& t) {
  os << "{ ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::list<T>& t) {
  os << "{ ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::set<T>& t) {
  os << "{ ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
void print_bytes(std::ostream& os, T& str) {
  os << "[ ";
  std::for_each(str.begin(), str.end(), [&](auto& e) {
      os << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
          << (static_cast<uint32_t>(e) & 0xff) << " ";
    });
    os << "]" << std::endl;
}

}
