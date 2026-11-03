#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "CallbackS.h"


class Callback_i : public virtual POA_Callback
{
  public:
    Callback_i();

    ~Callback_i () override;

    void test_method () override;
};


#endif
