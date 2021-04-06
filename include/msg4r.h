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
#include <utility>
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
#else
#define from_native boost::endian::native_to_little
#define to_native boost::endian::little_to_native
#endif

#define float32_t float
#define float64_t double

#ifdef _MSC_VER
#define MSG4R_PACKED(n) _declspec(align((n)))
#else
#define MSG4R_PACKED(n) __attribute__((packed))
#endif

#define BEGIN_STATE(state) switch(state) { case 0:

#define PARSE_STATE(state, op, s, v)  \
  case __LINE__: /* fall through*/    \
    {state = __LINE__;                \
    auto status = op(s, v);           \
    if (decode_state::DECODE_SUCCESS != status) return status;}

#define PARSE_LIST_STATE(state, op, s, T, t_, add, length, index) \
  case __LINE__: /* fall through*/                 \
    state = __LINE__;                              \
    for (index = 0; index != length;) {            \
      T c;                                         \
      auto status = op(s, c);                      \
      if (decode_state::DECODE_SUCCESS != status) return status; \
      index += 1;                                  \
      t_.add(c);                                   \
    }

#define END_STATE(state, t_, v)          \
  case __LINE__: /* fall through*/       \
    state = __LINE__;                    \
    v = t_;                              \
    reset();                             \
  }                                      \
  return decode_state::DECODE_SUCCESS;   


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
void rollback(std::istream& is, MSG4R_SIZE_T size);

template <typename T>
struct number_parser {
  number_parser();
  virtual ~number_parser();
  decode_state operator()(std::istream& is, T& v);
  void reset();

  MSG4R_SIZE_T index_; 
  T t_;
};

template<typename T> number_parser<T>::number_parser(): index_(0), t_() { }
template<typename T> number_parser<T>::~number_parser() { }

template<> decode_state number_parser<float32_t>::operator()(std::istream& is, float32_t& v);
template<> decode_state number_parser<float64_t>::operator()(std::istream& is, float64_t& v);

template<typename T>
decode_state number_parser<T>::operator()(std::istream& is, T& v) {
  auto initial = is.gcount();
  is.read((char*)&t_ + index_, sizeof(T));
  auto current = is.gcount();
  index_ += (current - initial);
  if(is.eof()) {
    return decode_state::DECODE_EXPECTING;
  } else {
    v = to_native(t_);
    reset();
    return decode_state::DECODE_SUCCESS;
  }
}

template <typename T>
void number_parser<T>::reset() {
  index_ = 0;   // reset to initial state
  t_ = 0;       // reset to initial state
}

encode_state write(std::ostream& os, const float32_t& v);

encode_state write(std::ostream& os, const float64_t& v);

struct string_parser {
  string_parser();
  virtual ~string_parser();
  decode_state operator()(std::istream& is, std::string& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::string t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  number_parser<std::string::value_type> t_parser;
};

decode_state read(std::istream& is, std::string& v);
encode_state write(std::ostream& os, const std::string& v);

std::ostream& operator<<(std::ostream& os, const std::string& t);

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

template<typename T, typename P>
struct vector_parser {
  vector_parser();
  virtual ~vector_parser();
  decode_state operator()(std::istream& is, std::vector<T>& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::vector<T> t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  P t_parser_;
};

template <typename T, typename P>
vector_parser<T, P>::vector_parser()
    : state_(0),
      length_(0),
      index_(0),
      t_(), length_parser_(), t_parser_() {}

template <typename T, typename P>
vector_parser<T, P>::~vector_parser() {}

template <typename T, typename P>
void vector_parser<T, P>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename T, typename P>
decode_state vector_parser<T, P>::operator()(std::istream& is, std::vector<T>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, typename std::vector<T>::value_type, t_, push_back, length_, index_)
  END_STATE(state_, t_, v)
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

template <typename T, typename P>
struct list_parser {
  list_parser();
  virtual ~list_parser();
  decode_state operator()(std::istream& is, std::list<T>& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::list<T> t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  P t_parser_;
};

template <typename T, typename P>
list_parser<T, P>::list_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template <typename T, typename P>
list_parser<T, P>::~list_parser() {}

template <typename T, typename P>
void list_parser<T, P>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename T, typename P>
decode_state list_parser<T, P>::operator()(std::istream& is,
                                             std::list<T>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, typename std::list<T>::value_type, t_,
                   push_back, length_, index_)
  END_STATE(state_, t_, v)
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

template <typename T, typename P>
struct set_parser {
  set_parser();
  virtual ~set_parser();
  decode_state operator()(std::istream& is, std::set<T>& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::set<T> t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  P t_parser_;
};

template <typename T, typename P>
set_parser<T, P>::set_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template <typename T, typename P>
set_parser<T, P>::~set_parser() {}

template <typename T, typename P>
void set_parser<T, P>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename T, typename P>
decode_state set_parser<T, P>::operator()(std::istream& is, std::set<T>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, typename std::set<T>::value_type, t_,
                   insert, length_, index_)
  END_STATE(state_, t_, v)
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

template<typename K, typename KP, typename V, typename VP>
struct pair_parser {
  pair_parser();
  virtual ~pair_parser();
  decode_state operator()(std::istream& is, std::pair<K, V>& v);
  void reset();

  int state_;
  K k_;
  V v_;
  KP k_parser_;
  VP v_parser_;
};

template<typename K, typename KP, typename V, typename VP>
pair_parser<K, KP, V, VP>::pair_parser()
    : state_(0),
      k_(), v_(), k_parser_(), v_parser_() {}

template<typename K, typename KP, typename V, typename VP>
pair_parser<K, KP, V, VP>::~pair_parser() {}

template<typename K, typename KP, typename V, typename VP>
void pair_parser<K, KP, V, VP>::reset() {
  state_ = 0;   // reset to initial state
  k_parser_.reset();
  v_parser_.reset();
}

template<typename K, typename KP, typename V, typename VP>
decode_state pair_parser<K, KP, V, VP>::operator()(std::istream& is, std::pair<K, V>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, k_parser_, is, k_)
  PARSE_STATE(state_, v_parser_, is, v_)
  END_STATE(state_, std::make_pair(k_, v_), v)
}

template<typename K, typename V>
encode_state write(std::ostream& os, const std::pair<K, V>& v) {
  write(os, v.first);
  write(os, v.second);
  return encode_state::ENCODE_SUCCESS;
}

template<typename K, typename KP, typename V, typename VP>
struct map_parser {
  typedef std::pair<K, V> E;
  typedef pair_parser<K, KP, V, VP> P;
  map_parser();
  virtual ~map_parser();
  decode_state operator()(std::istream& is, std::map<K, V>& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::map<K, V> t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  P t_parser_;
};

template<typename K, typename KP, typename V, typename VP>
map_parser<K, KP, V, VP>::map_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template<typename K, typename KP, typename V, typename VP>
map_parser<K, KP, V, VP>::~map_parser() {}

template<typename K, typename KP, typename V, typename VP>
void map_parser<K, KP, V, VP>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template<typename K, typename KP, typename V, typename VP>
decode_state map_parser<K, KP, V, VP>::operator()(std::istream& is, std::map<K, V>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, E, t_,
                   insert, length_, index_)
  END_STATE(state_, t_, v)
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
                         const std::pair<K, V>& t) {
  os << t.first << ": " << t.second;
  return os;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::map<K, V>& t) {
  os << "std::map { ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    //os << e->first << ": " << e->second;
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

void print_bytes(std::ostream& os, const char* buff, const size_t length);

/*
template<typename K, typename V>
constexpr bool operator==(const std::pair<K, V>& lhs,
                         const std::pair<K, V>& rhs) {
  return std::tie(lhs.first, lhs.second) == std::tie(rhs.first, rhs.second);
}
*/

}
