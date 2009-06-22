// $Id$

#ifndef CALLEE_I_H
#define CALLEE_I_H

#include "calleeS.h"

class Callee_i : public POA_Callee {
  void callback (const char * message);
};

#endif
