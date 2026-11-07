#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"
#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"
#include <atomic>

class FooServantList;

class Foo_i : public virtual POA_Foo
{
  public:
    Foo_i (ACE_TCHAR const *servant_name, FooServantList *mgr);
    ~Foo_i () override = default;
    Foo_i (Foo_i const &) = delete;
    Foo_i (Foo_i &&) = delete;
    Foo_i & operator= (Foo_i const &) = delete;
    Foo_i & operator= (Foo_i &&) = delete;

    void op1 () override;

    void op2 (CORBA::Long value) override;

    CORBA::Long op3 () override;

    void op4 (CORBA::Long value) override;

    void op5 () override;

    void done () override;


  private:
    std::atomic<CORBA::Long> value_ {};

    std::atomic<unsigned> count_op1_ {};
    std::atomic<unsigned> count_op2_ {};
    std::atomic<unsigned> count_op3_ {};
    std::atomic<unsigned> count_op4_ {};
    std::atomic<unsigned> count_op5_ {};

    ACE_TString const servant_name_;
    // Non-owning back-pointer; FooServantList owns this servant and outlives it.
    FooServantList *const mgr_;
};

#endif
