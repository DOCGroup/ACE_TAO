// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynsequence.cpp
//
// = DESCRIPTION
//    Implementation of the simple DynSequence test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynsequence.h"
#include "da_testsC.h"
#include "data.h"

Test_DynSequence::Test_DynSequence (CORBA::ORB_var orb)
  : test_name_ (CORBA::string_dup ("test_dynsequence")),
    orb_ (orb),
    error_count_ (0)
{
}

Test_DynSequence::~Test_DynSequence (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynSequence::test_name (void) const
{
  return this->test_name_;
}

int
Test_DynSequence::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_seq ts (2);
  ts.length (2);

    ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[11]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/rewind/current_component\n"));

      ts[0] = data.m_string2;
      ts[1] = data.m_string2;
      CORBA_Any in_any1;
      in_any1 <<= ts;
      CORBA_DynAny_ptr dp1 =
        this->orb_->create_dyn_any (in_any1,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_DynSequence_ptr fa1 = CORBA_DynSequence::_narrow (dp1,
                                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->seek (1,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->insert_string (data.m_string1,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->rewind (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->seek (1,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::String out_str1 = fa1->get_string (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (!ACE_OS::strcmp (out_str1, data.m_string1))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynSequence_ptr ftc1 =
        this->orb_->create_dyn_sequence (DynAnyTests::_tc_test_seq,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ts[0] = CORBA::string_dup (data.m_string1);
      CORBA_Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2,
                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Any* out_any1 = ftc1->to_any (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      DynAnyTests::test_seq* ts_out;
      *out_any1 >>= ts_out;
      if (!ACE_OS::strcmp ((*ts_out)[0], data.m_string1))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else
        ++this->error_count_;

      // Created with NEW
      delete out_any1;


      ACE_DEBUG ((LM_DEBUG,
                 "testing: length/set_elements/get_elements\n"));

      if (ftc1->length (ACE_TRY_ENV) != 2)
        ++this->error_count_;
      ACE_TRY_CHECK;

      ftc1->length (3,
                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AnySeq as_in (3);
      as_in.length (3);
      CORBA_Any in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string2, 0);
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string1, 0);
      as_in[2] = in_any3;
      ftc1->set_elements (as_in,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      AnySeq* as_out = ftc1->get_elements (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_Any out_any2 = (*as_out)[2];
      CORBA::String out_str2;
      out_any2 >>= CORBA::Any::to_string (out_str2, 0);
      if (ACE_OS::strcmp (out_str2, data.m_string1))
        ++this->error_count_;
      if (this->error_count_ == 0)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));

      // Created with NEW
      delete as_out;
      delete out_str2;

      fa1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::release (fa1);
      CORBA::release (dp1);
      ftc1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::release (ftc1);
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("test_dynsequence::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));


  return 0;
}
