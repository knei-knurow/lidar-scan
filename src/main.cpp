#include <memory>
#include <string>
#include <vector>
#include "../include/app.hpp"

std::unique_ptr<App> app;

#ifdef _WIN32
#include <Windows.h>
BOOL WINAPI ctrl_c_handler(DWORD signal) {
  if (signal == CTRL_C_EVENT) {
    if (app) {
      app->close();
      return TRUE;
    } else {
      return FALSE;
    }
  }
  return TRUE;
}
#elif __linux__
#include <csignal>
#include <unistd.h>
void ctrl_c_handler(int signal){
  app->close();
}
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
  if (!SetConsoleCtrlHandler(ctrl_c_handler, TRUE)) {
    return -1;
  }
#elif __linux__
   struct sigaction handler;
   handler.sa_handler = ctrl_c_handler;
   sigemptyset(&handler.sa_mask);
   handler.sa_flags = 0;
   sigaction(SIGINT, &handler, NULL);
#endif

  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.push_back(std::string(argv[i]));
  }
  app = std::make_unique<App>(args);
  return app->run();
}
