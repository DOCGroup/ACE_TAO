// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynany.cpp
//
// = DESCRIPTION
//    Implementation of the basic test for simple DynAnys
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "test_dynany.h"
#include "data.h"


Test_DynAny::Test_DynAny (CORBA::ORB_var orb)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynany")),
    error_count_ (0)
{
}

Test_DynAny::~Test_DynAny (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynAny::test_name (void) const
{ 
  return this->test_name_;
}

int
Test_DynAny::run_test (void)
{
  Data data (this->orb_);

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[8]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA_Any in1 (CORBA::_tc_double);
      CORBA_DynAny_var fa1 = this->orb_->create_dyn_any (in1,
                                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->insert_double (data.m_double1, 
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Double d_out = fa1->get_double (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (d_out == data.m_double1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      d_out = data.m_double2;
      CORBA_DynAny_var ftc1 = 
        this->orb_->create_basic_dyn_any (CORBA::_tc_double,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Any in_any1;
      in_any1 <<= data.m_double1;
      ftc1->from_any (in_any1, 
                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Any_var out_any1 = ftc1->to_any (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      out_any1 >>= d_out;

      if (d_out == data.m_double1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      fa1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ftc1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[12]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA_Any in (CORBA::_tc_TypeCode);
      CORBA_DynAny_var fa2 = 
        this->orb_->create_dyn_any (in,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa2->insert_typecode (data.m_typecode1, 
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::TypeCode_var tc_out = fa2->get_typecode (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (tc_out->equal (data.m_typecode1,
                         ACE_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynAny_var ftc2 = 
        this->orb_->create_basic_dyn_any (CORBA::_tc_TypeCode,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Any in_any2;
      in_any2 <<= data.m_typecode1;
      ftc2->from_any (in_any2, 
                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Any_var out_any2 = ftc2->to_any (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::TypeCode_ptr out_tc;
      out_any2 >>= out_tc;

      if (out_tc->equal(data.m_typecode1,
                        ACE_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_TRY_CHECK;

      fa2->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ftc2->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "test_dynany::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

