#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <msg4r.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
	printf("%s\n", argv[0]);

#ifdef _MSC_VER
  printf("_MSC_VER = %zd\n", _MSC_VER);
  printf("sizeof(UINT) = %zd\n", sizeof(UINT));
#endif
  printf("sizeof(uint8_t) = %zd\n", sizeof(uint8_t));
	return 0;
}
