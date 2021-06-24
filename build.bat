cmake.exe ^
  -DBoost_DIR=C:\usr\lib\cmake\Boost-1.75.0 ^
  -DBoost_INCLUDE_DIR=C:\usr\include ^
  -DBoost_LIBRARY_DIRS=C:\usr\lib ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=C:\usr ^
  -S . ^
  -B dist ^
  -G "NMake Makefiles"

cmake.exe ^
  -DBoost_DIR=C:\usr\lib\cmake\Boost-1.75.0 ^
  -DBoost_INCLUDE_DIR=C:\usr\include ^
  -DBoost_LIBRARY_DIRS=C:\usr\lib ^
  -DCMAKE_INSTALL_PREFIX=C:\usr ^
  -S . ^
  -B vs ^
  -G "Visual Studio 16 2019"

