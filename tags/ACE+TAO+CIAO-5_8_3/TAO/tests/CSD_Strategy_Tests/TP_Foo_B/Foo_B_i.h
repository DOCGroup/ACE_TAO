// $Id$
#ifndef FOO_B_I_H
#define FOO_B_I_H

#include "CSD_TP_Foo_B_Export.h"
#include "Foo_BS.h"
#include "Foo_B_Statistics.h"


class CSD_TP_Foo_B_Export Foo_B_i : public virtual POA_Foo_B
{
  public:

    Foo_B_i();
    virtual ~Foo_B_i();

    virtual void op1(void);

    virtual void op2(CORBA::Long value);

    virtual CORBA::Long op3(CORBA::Long value);

    virtual void op4(CORBA::Long value);

    virtual void op5(void);

    virtual CORBA::Boolean op6(const TimeOfDay& t,
                               char*& message);

    virtual void op7(Callback_ptr cb);

    virtual void test_unbounded_string_arg(const char* message);

    virtual void test_bounded_string_arg(const char* message);

    virtual void test_fixed_array_arg(const Fixed_Array message);

    virtual void test_var_array_arg(const Var_Array messages);

    virtual void test_bounded_var_size_arg(const Bounded_Var_Size& message);

    virtual void test_unbounded_var_size_arg(const Unbounded_Var_Size& message);

    virtual void test_fixed_size_arg(const TimeOfDay& t);

    virtual void test_special_basic_arg(CORBA::Boolean value,
                                        CORBA::Long client_id);

    virtual void test_objref_arg(Callback_ptr cb);

    virtual void done(void);

    void gather_stats (Foo_B_Statistics& stats);

  private:

    LongVector in_long_ [16];
    StringVector in_string_ [16];
    unsigned op_count_ [16];
    unsigned error_count_;
};

#endif
