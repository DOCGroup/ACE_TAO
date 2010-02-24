// $Id$
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

    virtual ~Callback_i();

    virtual void test_method(void);

    void gather_stats(Foo_B_Statistics& stats);


  private:

     unsigned num_callbacks_;
};


#endif
