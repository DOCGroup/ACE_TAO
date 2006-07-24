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

    virtual void op1(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)              
      ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op2(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual CORBA::Long op3(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op4(CORBA::Long value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op5(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)              
      ACE_THROW_SPEC((CORBA::SystemException, FooException));

    virtual CORBA::Boolean op6(const TimeOfDay& t, 
                               char*& message 
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void op7(Callback_ptr cb
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException,
                                                    FooException));

    virtual void test_unbounded_string_arg(const char* message 
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));
  
    virtual void test_bounded_string_arg(const char* message
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));
  
    virtual void test_fixed_array_arg(const Fixed_Array message
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));
  
    virtual void test_var_array_arg(const Var_Array messages
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void test_bounded_var_size_arg(const Bounded_Var_Size& message
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));
    
    virtual void test_unbounded_var_size_arg(const Unbounded_Var_Size& message
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void test_fixed_size_arg(const TimeOfDay& t
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void test_special_basic_arg(CORBA::Boolean value,
                                        CORBA::Long client_id
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void test_objref_arg(Callback_ptr cb
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                    ACE_THROW_SPEC((CORBA::SystemException));

    virtual void done(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)             
      ACE_THROW_SPEC((CORBA::SystemException));

    void gather_stats (Foo_B_Statistics& stats); 

  private:
  
    LongVector in_long_ [16];
    StringVector in_string_ [16];
    unsigned op_count_ [16];
    unsigned error_count_;
};

#endif
