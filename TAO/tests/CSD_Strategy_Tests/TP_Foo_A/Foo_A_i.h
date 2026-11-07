#ifndef FOO_A_I_H
#define FOO_A_I_H

#include "CSD_TP_Foo_A_Export.h"
#include "Foo_AS.h"
#include "Foo_A_Statistics.h"


class CSD_TP_Foo_A_Export Foo_A_i : public virtual POA_Foo_A
{
  public:
    Foo_A_i();
    ~Foo_A_i() override = default;

    void op1() override;

    void op2(CORBA::Long value) override;

    CORBA::Long op3(CORBA::Long value) override;

    void op4(CORBA::Long value) override;

    void op5() override;

    void done() override;

    void gather_stats (Foo_A_Statistics& stats);

    Foo_A_i (Foo_A_i const &) = delete;
    Foo_A_i (Foo_A_i &&) = delete;
    Foo_A_i & operator= (Foo_A_i const &) = delete;
    Foo_A_i & operator= (Foo_A_i &&) = delete;

  private:
    LongVector in_values_ [5];
    unsigned op_count_ [5] {};
};

#endif
