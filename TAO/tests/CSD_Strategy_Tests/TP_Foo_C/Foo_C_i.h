// $Id$
#ifndef FOO_C_I_H
#define FOO_C_I_H

#include "CSD_TP_Foo_C_Export.h"
#include "Foo_CS.h"
#include "Foo_C_Statistics.h"


class CSD_TP_Foo_C_Export Foo_C_i : public virtual POA_Foo_C
{
  public:

    Foo_C_i();
    virtual ~Foo_C_i();

    virtual void op1(void);

    virtual void op2(CORBA::Long value);

    virtual CORBA::Long op3(CORBA::Long value);

    virtual void op4(CORBA::Long value);

    virtual void op5(void);

    virtual void done(void);

    void cust_op1(void);
    void cust_op2(long value);
    long cust_op3(long value);
    void cust_op4(long value);
    void cust_op5(void);

    void gather_stats(Foo_C_Statistics& stats);

    void dump();


  private:

    LongVector in_values_ [10];
    unsigned count_[10];
};

#endif
