# msg4r



序列化、反序列化工具。

## 问题

目前有很多序列化、反序列化需求，限定了数据表示格式，不能采用常见的JSON、XML、Apache Thrift、Google Protobuf来解决。

可以用来定义数据格式的通用库，例如MFC的文档序列化框架、boost::serialization，都有与目标格式冲突且无法兼容的框架特有的数据。

依据数据格式规范编制解析程序，已经有很多方法，非常繁琐、容易出错。最大的问题是，它不能支持在异步IO或非阻塞IO条件下，对chunked数据流的支持。它适合先用消息载体（例如MQ消息），而不是直接解析流不引入载体格式的情形。


## 目标

探索异步IO、非阻塞IO条件下，支持chunked流解析的声明式序列化、反序列化解决方案。

例如，有数据表示如下：

```
struct my_struct {
  uint32_t id;
  std::string name;
  std::map<std::string, std::string> props;
};
```

解析程序可以类似如下：

```
enum decode_state {
  DECODE_SUCCESS    = 0;
  DECODE_INPROGRESS = 0;
  DECODE_FAILURE    = 0;
}

...

struct my_struct_parser {
  ..
  decode_state operator()(std::istream& is, my_struct& t) {
    BEGIN_PARSER(t)
      PARSE(t.id)
      PARSE(t.name)
      PARSE(t.props)
    END_PARSER(t)
  }
  ..
};
```

直接声明字段列表即可，不用管内部结构以及解析的时候会遇到何种问题，以及该如何处理。
嵌套的数据结构，可以递归处理，只需声明当前数据的解析。

## 理论依据

任何一个非平凡（一步到位）的问题，都可以表示为一个可递归的子问题列表。

此表示是递归的。

数学归纳法：
对于平凡的问题，有解；
对于某一个非平凡问题表示成的子问题列表中，所有的子问题都已经有解，那么这个问题就可解。




