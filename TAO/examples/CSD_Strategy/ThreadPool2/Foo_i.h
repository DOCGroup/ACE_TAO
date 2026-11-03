#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"
#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"

class FooServantList;

class Foo_i : public virtual POA_Foo
{
  public:
    Foo_i(const ACE_TCHAR* servant_name, FooServantList* mgr);
    ~Foo_i () override;

    void op1 () override;

    void op2 (CORBA::Long value) override;

    CORBA::Long op3 () override;

    void op4 (CORBA::Long value) override;

    void op5 () override;

    void done () override;


  private:
    CORBA::Long value_;

    unsigned count_op1_;
    unsigned count_op2_;
    unsigned count_op3_;
    unsigned count_op4_;
    unsigned count_op5_;

    ACE_TString     servant_name_;
    FooServantList* mgr_;
};

#endif
