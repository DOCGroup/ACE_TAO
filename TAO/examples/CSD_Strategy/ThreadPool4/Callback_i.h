#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "CallbackS.h"


class Callback_i : public virtual POA_Callback
{
  public:
    Callback_i();

    ~Callback_i () override = default;

    Callback_i (Callback_i const &) = delete;
    Callback_i (Callback_i &&) = delete;
    Callback_i & operator= (Callback_i const &) = delete;
    Callback_i & operator= (Callback_i &&) = delete;

    void test_method () override;
};


#endif
