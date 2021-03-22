#define BOOST_TEST_MODULE My Test
#include <boost/test/included/unit_test.hpp>

#include <ostream>
#include <boost/json/value_to.hpp>
#include <boost/json/src.hpp>

typedef struct my_struct {
  my_struct() : i() {}
  my_struct(int v) : i(v) {}
  my_struct(const my_struct& t) : i(t.i) {}

  my_struct& operator=(const my_struct& t) { 
    this->i = t.i;
    return *this;
  }

  bool operator==(const my_struct& t) const { 
    return (this->i == t.i);
  }

  int i;
} my_struct_t;

std::ostream& operator<<(std::ostream& os, const my_struct& t) {
  os << "struct my_struct { i = " << t.i << " }";
  return os;
}

void tag_invoke(
  boost::json::value_from_tag,
  boost::json::value& jv,
  my_struct_t const & t) {
  jv = {
    {"i", t.i}
  };
}

my_struct_t tag_invoke(
  boost::json::value_to_tag<my_struct_t>,
  const boost::json::value& jv) {
  boost::json::object const& obj = jv.as_object();
  return my_struct_t(boost::json::value_to<int>(obj.at("i")));
}

template <typename T>
void check(T t) {
  BOOST_TEST(boost::json::value_to<T>(boost::json::value_from(t)) == t);
}

BOOST_AUTO_TEST_CASE(first_test) {
  int i = 1;
  my_struct_t s1{i};
  my_struct_t s2{i};
  BOOST_TEST(s1 == s2);
  check(i);
  check(s1);
  std::cout << boost::json::serialize(boost::json::value_from(s1));
  boost::json::value jv = boost::json::parse("{\"i\":1}");
  s2 = boost::json::value_to<my_struct_t>(jv);
  BOOST_TEST(s1 == s2);
}


