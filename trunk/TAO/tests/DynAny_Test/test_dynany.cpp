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
//    Jeff Parsons <jp4@cs.wustl.edu>
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

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[8]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA_Any in1 (CORBA::_tc_double);
      CORBA_DynAny_ptr fa1 = this->orb_->create_dyn_any (in1,
                                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->insert_double (data.m_double1, 
                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Double d_out = fa1->get_double (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (d_out == data.m_double1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      d_out = data.m_double2;
      CORBA_DynAny_ptr ftc1 = 
        this->orb_->create_basic_dyn_any (CORBA::_tc_double,
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Any in_any1;
      in_any1 <<= data.m_double1;
      ftc1->from_any (in_any1, 
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Any* out_any1 = ftc1->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      *out_any1 >>= d_out;

      // value in DynAny_i created with NEW
      delete out_any1;

      if (d_out == data.m_double1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      fa1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (fa1);
      ftc1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (ftc1);

      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[12]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA_Any in (CORBA::_tc_TypeCode);
      CORBA_DynAny_ptr fa2 = 
        this->orb_->create_dyn_any (in,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa2->insert_typecode (data.m_typecode1, 
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::TypeCode_ptr tc_out = fa2->get_typecode (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (tc_out->equal (data.m_typecode1,
                         TAO_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      tc_out = data.m_typecode2;
      CORBA_DynAny_ptr ftc2 = 
        this->orb_->create_basic_dyn_any (CORBA::_tc_TypeCode,
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Any in_any2;
      in_any2 <<= data.m_typecode1;
      ftc2->from_any (in_any2, 
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Any* out_any2 = ftc2->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      *out_any2 >>= tc_out;

      // value in DynAny_i created with NEW
      delete out_any2;

      if (tc_out->equal(data.m_typecode1,
                        TAO_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      TAO_CHECK_ENV;

      fa2->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (fa2);
      ftc2->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (ftc2);
      CORBA::release (tc_out);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_dynany::run_test");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

