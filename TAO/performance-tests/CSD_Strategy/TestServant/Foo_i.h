#ifndef FOO_I_H
#define FOO_I_H

#include "CSD_PT_TestServant_Export.h"
#include "TestServant/FooS.h"
#include "Foo_Statistics.h"


class CSD_PT_TestServant_Export Foo_i : public virtual POA_Foo
{
  public:
    Foo_i();
    virtual ~Foo_i();

    virtual void op1();

    virtual void op2(CORBA::Long value);

    virtual CORBA::Long op3(CORBA::Long value);

    virtual void op4(CORBA::Long value);

    virtual void op5();

    virtual void done();


    void gather_stats(Foo_Statistics& stats);


  private:
    unsigned op_count_[5];
};

#endif
