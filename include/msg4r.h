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

#ifndef MSG4R_SIZE_T
#define MSG4R_SIZE_T uint32_t
#endif

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

enum class encode_state {
  ENCODE_SUCCESS   = 0,
  ENCODE_WAITING   = 1,
  ENCODE_FAILURE   = 2
};

enum class decode_state {
  DECODE_SUCCESS   = 0,
  DECODE_EXPECTING = 1,
  DECODE_FAILURE   = 2
};

std::ostream& operator<<(std::ostream& os, const encode_state& t);
std::ostream& operator<<(std::ostream& os, const decode_state& t);

/**
 * check input stream to see if there are enough bytes available to read.
 */
bool expecting(std::istream& is, MSG4R_SIZE_T size);

decode_state read(std::istream& is, float32_t& v);
encode_state write(std::ostream& os, const float32_t& v);

decode_state read(std::istream& is, float64_t& v);
encode_state write(std::ostream& os, const float64_t& v);

decode_state read(std::istream& is, std::string& v);
encode_state write(std::ostream& os, const std::string& v);

/**
 * read integers and numbers. T = { int8_t, ... }
 */
template<typename T>
decode_state read(std::istream& is, T& v) {
  T t;
  if (expecting(is, sizeof(t))) {
    return decode_state::DECODE_EXPECTING;
  } else {
    is.read((char*)&t, sizeof(t));
    v = to_native(t);
    return decode_state::DECODE_SUCCESS;
  }
}

/**
 * write integers and numbers. T = { int8_t, ... }
 */
template<typename T>
encode_state write(std::ostream& os, const T& v) {
  T t = from_native(v);
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::vector<T>& v) {
  MSG4R_SIZE_T length;
  read(is, length);
  for (MSG4R_SIZE_T i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return decode_state::DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::vector<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::list<T>& v) {
  MSG4R_SIZE_T length;
  read(is, length);
  for (MSG4R_SIZE_T i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.push_back(c);
  }
  return decode_state::DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::list<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename T>
decode_state read(std::istream& is, std::set<T>& v) {
  MSG4R_SIZE_T length;
  read(is, length);
  for (MSG4R_SIZE_T i = 0; i != length; ++i) {
    T c;
    read(is, c);
    v.insert(c);
  }
  return decode_state::DECODE_SUCCESS;
}

template<typename T>
encode_state write(std::ostream& os, const std::set<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename K, typename V>
decode_state read(std::istream& is, std::map<K, V>& v) {
  MSG4R_SIZE_T length;
  read(is, length);
  for (MSG4R_SIZE_T i = 0; i != length; ++i) {
    K key;
    V value;
    read(is, key);
    read(is, value);
    v.insert(std::make_pair(key, value));
  }
  return decode_state::DECODE_SUCCESS;
}

template<typename K, typename V>
encode_state write(std::ostream& os, const std::map<K, V>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e.first);
      write(os, e.second);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::map<K, V>& t) {
  os << "std::map { ";
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
  os << "std::vector { ";
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
  os << "std::list { ";
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
  os << "std::set { ";
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
