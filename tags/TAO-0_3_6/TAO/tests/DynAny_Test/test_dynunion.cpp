// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynunion.cpp
//
// = DESCRIPTION
//    Implementation of the simple DynUnion test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynunion.h"
#include "da_testsC.h"
#include "data.h"

Test_DynUnion::Test_DynUnion (CORBA::ORB_var orb)
  : test_name_ (CORBA::string_dup ("test_dynunion")),
    orb_ (orb),
    error_count_ (0)
{
}

Test_DynUnion::~Test_DynUnion (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynUnion::test_name (void) const
{ 
  return this->test_name_;
}

int
Test_DynUnion::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_union tu;

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[3]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      tu._d(DynAnyTests::FIRST);
      tu.s (data.m_short2);
      CORBA_Any in_any1;
      in_any1 <<= tu;
      CORBA_DynAny_ptr dp1 =
        this->orb_->create_dyn_any (in_any1,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_DynUnion_ptr fa1 = CORBA_DynUnion::_narrow (dp1,
                                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->insert_short (data.m_short1,
                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Short s_out1 = fa1->get_short (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (s_out1 == data.m_short1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_Any* out_any1 = fa1->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_DynUnion_ptr ftc1 = 
        this->orb_->create_dyn_union (DynAnyTests::_tc_test_union,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ftc1->from_any (*out_any1,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Short s_out2 = ftc1->get_short (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (s_out2 == data.m_short1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_as_default\n"));

      if (!ftc1->set_as_default (TAO_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
                 "testing:discriminator/discriminator_kind\n"));

      CORBA_DynAny_ptr dp2 = ftc1->discriminator (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (dp2->type (TAO_TRY_ENV)->kind () 
            == ftc1->discriminator_kind (TAO_TRY_ENV))
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_; 
      
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
                 "testing:member/member_kind/member_name\n"));

      CORBA_DynAny_ptr dp3 = ftc1->member (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::String m_nm = ftc1->member_name (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::TCKind tk = ftc1->member_kind (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (!ACE_OS::strcmp (m_nm, "s")         && 
          tk == CORBA::tk_short               &&
          dp3->get_short (TAO_TRY_ENV) == data.m_short1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_; 

      TAO_CHECK_ENV;

      // Created with string_dup
      CORBA::string_free (m_nm);
      
      fa1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (fa1);
      CORBA::release (dp1);
      ftc1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (ftc1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_dynunion::run_test");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

