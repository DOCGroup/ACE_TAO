#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"
#include <atomic>

class Foo_i
{
  public:
    explicit Foo_i (unsigned num_clients);
    virtual ~Foo_i () = default;

    Foo_i (Foo_i const &) = delete;
    Foo_i (Foo_i &&) = delete;
    Foo_i &operator= (Foo_i const &) = delete;
    Foo_i &operator= (Foo_i &&) = delete;

    virtual void op1 ();

    virtual void op2 (CORBA::Long value);

    virtual CORBA::Long op3 ();

    virtual void op4 (CORBA::Long value);

    virtual void op5 ();

    virtual void done ();

  private:
    std::atomic<CORBA::Long> value_ {};
    std::atomic<unsigned> num_clients_ {};

    std::atomic<unsigned> count_op1_ {};
    std::atomic<unsigned> count_op2_ {};
    std::atomic<unsigned> count_op3_ {};
    std::atomic<unsigned> count_op4_ {};
    std::atomic<unsigned> count_op5_ {};
};

#endif
