// $Id$
#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"
#include "CallbackC.h"
#include "ace/SString.h"

class FooServantList;


class Foo_i : public virtual POA_Foo
{
  public:

    Foo_i(const char* servant_name, FooServantList* mgr);
    virtual ~Foo_i();

    virtual void op1(void);

    virtual void op2(CORBA::Long value);

    virtual CORBA::Long op3(void);

    virtual void op4(CORBA::Long value);

    virtual void op5(void);

    virtual CORBA::Boolean op6 (
        const char * user_name,
        char *& message
      );

    virtual void test_unbounded_string_arg (
        const char * message
      );

    virtual void test_bounded_string_arg (
        const char * message
      );

    virtual void test_fixed_array_arg (
        const ::Fixed_Array message
      );

    virtual void test_var_array_arg (
        const ::Var_Array messages
      );

    virtual void test_bounded_var_size_arg (
        const ::Bounded_Var_Size & message
      );

    virtual void test_unbounded_var_size_arg (
        const ::Unbounded_Var_Size & message
      );

    virtual void test_fixed_size_arg (
        const ::TimeOfDay & t
      );

    virtual void test_fixed_size_arg_two_way (
        const ::TimeOfDay & t
      );

    virtual void test_special_basic_arg (
        ::CORBA::Boolean value
      );

    virtual void test_objref_arg (
        ::Callback_ptr cb
      );

    virtual void callback_object (
        ::Callback_ptr cb
      );

    virtual void test_callback (
      );

    virtual void done(void);

    void active (bool flag);

    bool active ();

  private:

    typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long> AtomicLong;
    typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned> AtomicUnsigned;
    typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> AtomicBool;

    AtomicLong value_;
    AtomicUnsigned count_op1_;
    AtomicUnsigned count_op2_;
    AtomicUnsigned count_op3_;
    AtomicUnsigned count_op4_;
    AtomicUnsigned count_op5_;

    ACE_CString     servant_name_;
    FooServantList* mgr_;
    AtomicBool      active_;
    Callback_var    callback_;
};


#endif
