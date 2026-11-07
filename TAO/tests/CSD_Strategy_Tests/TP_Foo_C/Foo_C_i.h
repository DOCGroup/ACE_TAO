#ifndef FOO_C_I_H
#define FOO_C_I_H

#include "CSD_TP_Foo_C_Export.h"
#include "Foo_CS.h"
#include "Foo_C_Statistics.h"


class CSD_TP_Foo_C_Export Foo_C_i : public virtual POA_Foo_C
{
  public:
    Foo_C_i();
    ~Foo_C_i() override;

    void op1() override;

    void op2(CORBA::Long value) override;

    CORBA::Long op3(CORBA::Long value) override;

    void op4(CORBA::Long value) override;

    void op5() override;

    void done() override;

    void cust_op1();
    void cust_op2(long value);
    long cust_op3(long value);
    void cust_op4(long value);
    void cust_op5();

    void gather_stats(Foo_C_Statistics& stats);

    void dump();

    Foo_C_i (Foo_C_i const &) = delete;
    Foo_C_i (Foo_C_i &&) = delete;
    Foo_C_i & operator= (Foo_C_i const &) = delete;
    Foo_C_i & operator= (Foo_C_i &&) = delete;


  private:
    LongVector in_values_ [10];
    unsigned count_[10] {};
};

#endif
