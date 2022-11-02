# msg4r

#### Description
message serialize/deserialize for '4 remote'

#### Software Architecture
Software architecture description

#### Installation


#### Instructions


#### Examples

Writing directly from an streambuf to a socket:
```
boost::asio::streambuf b;
std::ostream os(&b);
os << "Hello, World!\n";

// try sending some data in input sequence
size_t n = sock.send(b.data());

b.consume(n); // sent data is removed from input sequence
```
Reading from a socket directly into a streambuf:
```
boost::asio::streambuf b;

// reserve 512 bytes in output sequence
boost::asio::streambuf::mutable_buffers_type bufs = b.prepare(512);

size_t n = sock.receive(bufs);

// received data is "committed" from output sequence to input sequence
b.commit(n);

std::istream is(&b);
std::string s;
is >> s;
```
#### Requirements

Header: boost/asio/streambuf.hpp

Convenience header: boost/asio.hpp

#### Contribution

1.  Fork the repository
2.  Create Feat\_xxx branch
3.  Commit your code
4.  Create Pull Request


#### Gitee Feature

