#include "rebel_test.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/utsname.h>
#endif

void print_test_info()
{
#ifdef _WIN32
  SYSTEM_INFO info;
  GetNativeSystemInfo(&info);

  std::cout << "OS      : Windows" << std::endl;
  std::cout << "Machine : " << info.wProcessorArchitecture << std::endl;
#else
  struct utsname buf;
  if (uname(&buf) != 0) {
    std::cout << "OS      : unknown (uname failed)" << std::endl;
    return;
  }

  std::cout << "OS      : " << buf.sysname << std::endl;
  std::cout << "Release : " << buf.release << std::endl;
  std::cout << "Version : " << buf.version << std::endl;
  std::cout << "Machine : " << buf.machine << std::endl;
  std::cout << "Node    : " << buf.nodename << std::endl;
#endif
}
