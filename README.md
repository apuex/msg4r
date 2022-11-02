# msg4r

## Introduction
message serialize/deserialize for '4 remote control', both for wiring protocol and file storage.

- There're already so many serialize/deserialize libraries and frameworks out there, i.e. Google protobuf.
However, these libraries and frameworks is not intended for connect with simple embedded devices, 
which has resource constraints. We need tools for Rapid Construction of simple protocols.

- Most libraries for deserialization/parsing is implemented in stateless, synchronized style IO mode, 
which means bad scalability for large number of concurrent connections when no coroutine support;
besides, there're no coroutine support in the legacy C++ environments.
We need tools for simplifing of creating stateful, asynchronized style deserializers/parsers.
 
## Architecture

The Parser is implemented in stateful and stackless coroutine style.

## Install

This project requires C++11, Boost and CMake to build.
1.  Boost 1.75.0 or upper, 
2.  CMake 3.10.3 or upper,

### Windows
Minimium supported MSVC version is Visual Studio 2015.

#### Windows 2003/XP

1. Build boost.
Download and decompress boost package. i.e. to `C:\boost_1_80_0`
Build 32-bit:
```
C:\> cd boost_1_80_0
C:\boost_1_80_0> b2 install --prefix=C:\usr32 --build-dir=C:\usr32\build-boost --build-type=complete toolset=msvc stage link=static define=BOOST_USE_WINAPI_VERSION=0x0501 define=_WIN32_WINNT=0x0501 define=_CRT_SECURE_NO_WARNINGS=1 address-model=32 architecture=x86
```

Build 64-bit:
```
C:\> cd boost_1_80_0
C:\boost_1_80_0> b2 install --prefix=C:\usr64 --build-dir=C:\usr64\build-boost --build-type=complete toolset=msvc stage link=static define=BOOST_USE_WINAPI_VERSION=0x0502 define=_WIN32_WINNT=0x0502 define=_CRT_SECURE_NO_WARNINGS=1 address-model=64 architecture=x86
```

2. Build this project.

clone this project to `msg4r`;
```
C:\github\apuex> git clone https://gitee.com/apuex/msg4r
C:\github\apuex> cd msg4r 
C:\github\apuex\msg4r> mkdir dist-win64
C:\github\apuex\msg4r> cd dist-win64
C:\github\apuex\msg4r\dist-win64> cmake -G "NMake Makefiles" ^
-DCMAKE_C_FLAGS="/DWINVER=0x0502 /D_WIN32_WINNT=0x0502 -DBOOST_USE_WINAPI_VERSION=0x0502" ^
-DCMAKE_CXX_FLAGS="/DWINVER=0x0502 /EHsc /D_WIN32_WINNT=0x0502 -DBOOST_USE_WINAPI_VERSION=0x0502" ^
-DCMAKE_C_FLAGS_RELEASE="/O2 /DNDEBUG" ^
-DCMAKE_CXX_FLAGS_RELEASE="/O2 /DNDEBUG" ^
-DCMAKE_EXE_LINKER_FLAGS="/machine:amd64 /subsystem:console,5.02 /version:5.02 /RELEASE" ^
-DCMAKE_SHARED_LINKER_FLAGS="/machine:amd64 /subsystem:console,5.02 /version:5.02 /RELEASE" ^
-DCMAKE_MODULE_LINKER_FLAGS="/machine:amd64 /subsystem:console,5.02" ^
-DCMAKE_STATIC_LINKER_FLAGS="/machine:amd64 /subsystem:console,5.02" ^
-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL ^
-DCMAKE_INSTALL_PREFIX=C:/usr64 ^
-DCMAKE_BUILD_TYPE=Release ^
-DENABLE_TESTS=ON ^
-DENABLE_VALGRIND=OFF ^
..
C:\github\apuex\msg4r\dist-win64> nmake
```

3. Test this project.

```
C:\github\apuex\msg4r\dist-win64> nmake test
```


## Usage

## Contribution


