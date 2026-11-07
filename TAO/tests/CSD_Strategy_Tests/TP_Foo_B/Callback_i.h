#ifndef CALLBACK_I_H
#define CALLBACK_I_H

#include "CSD_TP_Foo_B_Export.h"
#include "CallbackS.h"
#include "Foo_B_Statistics.h"


class CSD_TP_Foo_B_Export Callback_i
                              : public virtual POA_Callback
{
  public:
    Callback_i();

    ~Callback_i() override;

    void test_method() override;

    void gather_stats(Foo_B_Statistics& stats);

    Callback_i (Callback_i const &) = delete;
    Callback_i (Callback_i &&) = delete;
    Callback_i & operator= (Callback_i const &) = delete;
    Callback_i & operator= (Callback_i &&) = delete;


  private:
     unsigned num_callbacks_ {};
};


#endif
