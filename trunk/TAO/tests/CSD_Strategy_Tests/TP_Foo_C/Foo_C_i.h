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

    virtual void op1(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) 
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op2(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                  ACE_THROW_SPEC((CORBA::SystemException));

    virtual CORBA::Long op3(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                  ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op4(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                  ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op5(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)           
                                  ACE_THROW_SPEC((CORBA::SystemException,
                                                  FooException));

    virtual void done(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)           
      ACE_THROW_SPEC((CORBA::SystemException));

    void cust_op1(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    void cust_op2(long value ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    long cust_op3(long value ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    void cust_op4(long value ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    void cust_op5(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    void gather_stats(Foo_C_Statistics& stats);

    void dump();


  private:

    LongVector in_values_ [10];
    unsigned count_[10];
};

#endif
