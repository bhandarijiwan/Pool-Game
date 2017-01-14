#include <iostream>
#include "engine.h"
int main(int argc, char *argv[])
{

  // Start an engine and run it then cleanup after
  auto engine = std::make_unique<Engine>("Pool", 1200, 800);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    return 1;
  }
  engine->Run();
  return 0;
}
