#include <iostream.h>
#include "orb.h"

#define SHOWSIZE(t) cout << "sizeof(" #t ") = " << sizeof(t) << endl;

int
main(int argc, char* argv[])
{
  SHOWSIZE(CORBA_Short);
  SHOWSIZE(CORBA_Long);
  SHOWSIZE(CORBA_LongLong);
  SHOWSIZE(CORBA_Float);
  SHOWSIZE(CORBA_Double);
  SHOWSIZE(CORBA_LongDouble);
}
