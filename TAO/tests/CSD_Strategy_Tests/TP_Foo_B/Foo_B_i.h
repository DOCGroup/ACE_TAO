#ifndef FOO_B_I_H
#define FOO_B_I_H

#include "CSD_TP_Foo_B_Export.h"
#include "Foo_BS.h"
#include "Foo_B_Statistics.h"


class CSD_TP_Foo_B_Export Foo_B_i : public virtual POA_Foo_B
{
  public:
    Foo_B_i();
    ~Foo_B_i() override = default;

    void op1() override;

    void op2(CORBA::Long value) override;

    CORBA::Long op3(CORBA::Long value) override;

    void op4(CORBA::Long value) override;

    void op5() override;

    CORBA::Boolean op6(const TimeOfDay& t,
                       char*& message) override;

    void op7(Callback_ptr cb) override;

    void test_unbounded_string_arg(const char* message) override;

    void test_bounded_string_arg(const char* message) override;

    void test_fixed_array_arg(const Fixed_Array message) override;

    void test_var_array_arg(const Var_Array messages) override;

    void test_bounded_var_size_arg(const Bounded_Var_Size& message) override;

    void test_unbounded_var_size_arg(const Unbounded_Var_Size& message) override;

    void test_fixed_size_arg(const TimeOfDay& t) override;

    void test_special_basic_arg(CORBA::Boolean value,
                                CORBA::Long client_id) override;

    void test_objref_arg(Callback_ptr cb) override;

    void done() override;

    void gather_stats (Foo_B_Statistics& stats);

    Foo_B_i (Foo_B_i const &) = delete;
    Foo_B_i (Foo_B_i &&) = delete;
    Foo_B_i & operator= (Foo_B_i const &) = delete;
    Foo_B_i & operator= (Foo_B_i &&) = delete;

  private:
    LongVector in_long_ [16];
    StringVector in_string_ [16];
    unsigned op_count_ [16] {};
    unsigned error_count_ {};
};

#endif
