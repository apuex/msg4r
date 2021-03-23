#include <msg4r.h>
#include <algorithm>
#include <boost/endian/conversion.hpp>

namespace msg4r {

std::istream& read(std::istream& is, std::string& v) {
  return is;
}

std::ostream& write(std::ostream& os, const std::string& v) {
  uint32_t length = static_cast<uint32_t>(v.length());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](auto& e) {
      write(os, e);
    });
  return os;
}

}
