#ifndef w32_test_impl_h
#define w32_test_impl_h

#include "w32_testS.h"

class W32_Test_Impl : public POA_W32_Test_Interface
{
public:
  ~W32_Test_Impl ();

  W32_Test_Impl ();

  char *getresponse (CORBA::Long respnr);

private:
  int blabla;
};

#endif
