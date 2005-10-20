// $Id$
#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"


class Foo_i : public virtual POA_Foo
{
  public:

    Foo_i(unsigned num_clients);
    virtual ~Foo_i();

    virtual void op1(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) 
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op2(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)  
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual CORBA::Long op3(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op4(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)  
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op5(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)   
      ACE_THROW_SPEC((CORBA::SystemException, FooException));

    virtual void done(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) 
      ACE_THROW_SPEC((CORBA::SystemException));

  private:

    CORBA::Long value_;
    unsigned num_clients_;
};

#endif
