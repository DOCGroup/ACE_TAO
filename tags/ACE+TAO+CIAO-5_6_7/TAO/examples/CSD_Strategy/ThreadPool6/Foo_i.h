// $Id$
#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"


class Foo_i
{
  public:

    Foo_i(unsigned num_clients);
    virtual ~Foo_i();

    virtual void op1(void);

    virtual void op2(CORBA::Long value);

    virtual CORBA::Long op3(void);

    virtual void op4(CORBA::Long value);

    virtual void op5(void);

    virtual void done(void);


  private:

    CORBA::Long value_;
    unsigned num_clients_;

    CORBA::ORB_var orb_;

    unsigned count_op1_;
    unsigned count_op2_;
    unsigned count_op3_;
    unsigned count_op4_;
    unsigned count_op5_;
};

#endif
