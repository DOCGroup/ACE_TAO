// $Id$

#ifndef w32_test_impl_h
#define w32_test_impl_h

#include "w32_testS.h"

class W32_Test_Impl : public POA_W32_Test_Interface
{
public:
  ~W32_Test_Impl (void);

  W32_Test_Impl (void);

  char *getresponse (CORBA::Long respnr);

private:
  int blabla;
};

#endif
