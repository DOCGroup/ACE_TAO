#ifndef CALLEE_I_H
#define CALLEE_I_H

#include "calleeS.h"

class Callee_i : public POA_Callee
{
public:
  Callee_i () = default;
  Callee_i (Callee_i const &) = delete;
  Callee_i &operator= (Callee_i const &) = delete;
  Callee_i (Callee_i &&) = delete;
  Callee_i &operator= (Callee_i &&) = delete;

  void callback (char const *message) override;
};

#endif
