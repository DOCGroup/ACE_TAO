#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"
#include <atomic>


class Foo_i : public virtual POA_Foo
{
  public:
    explicit Foo_i (unsigned num_clients);
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
    std::atomic<unsigned> num_clients_ {};
};

#endif
