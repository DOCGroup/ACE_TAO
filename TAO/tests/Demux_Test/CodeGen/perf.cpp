#include "tao.h"

void GenGPERFCode()
{
  char command[500];

  // first generate code for object hash
  sprintf(command, 
    "gperf -c -C -D -E -T -f 0 -a -o -t -p -L C++ -Z Method_Hash %s >\
gperf_method.i", "gperf_methods.dat");
  system(command);
  
  // omit the -C switch for gperf because we will not know the address of the
  // objects until they are instantiated. But we do know the keys in advance.
  sprintf(command, 
    "gperf -c -D -E -T -f 0 -a -o -t -p -L C++ -Z Object_Hash %s >\
gperf_object.i", "gperf_objects.dat");
  system(command);

}

