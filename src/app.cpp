using namespace std;
#include "goldenRatio.h"
#include<iostream>

int main(int argc, char* argv[])
{
  if(argc == 1)
  {
    cout << "Please provide image file in fixture directory." << endl;
    return  EXIT_FAILURE;
  }

  char* cstr = argv[1];
  calculate(cstr);
  return EXIT_SUCCESS;
}
