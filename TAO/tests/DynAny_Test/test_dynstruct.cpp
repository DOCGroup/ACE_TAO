// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_dynstruct.cpp
//
// = DESCRIPTION
//    Implementation of the DynStruct basic test
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "test_dynstruct.h"
#include "da_testsC.h"
#include "data.h"

Test_DynStruct::Test_DynStruct (CORBA::ORB_var orb)
  : test_name_ (CORBA::string_dup ("test_dynstruct")),
    orb_ (orb),
    error_count_ (0)
{
}

Test_DynStruct::~Test_DynStruct (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynStruct::test_name (void) const
{ 
  return this->test_name_;
}

int
Test_DynStruct::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_struct ts;
  ts.c = data.m_char2;
  ts.ll = data.m_longlong2;
  ts.es.f = data.m_float2;
  ts.es.s = data.m_short2;

  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/next/seek/rewind/current_component\n"));

      CORBA_Any in_any1;
      in_any1 <<= ts;
      CORBA_DynAny_ptr dp1 = 
        this->orb_->create_dyn_any (in_any1,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_DynStruct_ptr fa1 = CORBA_DynStruct::_narrow (dp1,
                                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->insert_char (data.m_char1,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->insert_longlong (data.m_longlong1,
        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->current_component (TAO_TRY_ENV)->insert_float (data.m_float1,
                                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->current_component (TAO_TRY_ENV)->insert_short (data.m_short1,
                                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->current_component (TAO_TRY_ENV)->rewind (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      fa1->rewind (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Char c = fa1->get_char (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (c != data.m_char1)
        ++this->error_count_;
      CORBA::LongLong ll = fa1->get_longlong (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (ll != data.m_longlong1)
        ++this->error_count_;
      fa1->current_component(TAO_TRY_ENV)->seek (1,
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::Short s = fa1->current_component (TAO_TRY_ENV)->get_short (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (s != data.m_short1)
        ++this->error_count_;
      if (this->error_count_ == 0)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
 
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynStruct_ptr ftc1 = 
        this->orb_->create_dyn_struct (DynAnyTests::_tc_test_struct,
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ts.c = data.m_char1;
      ts.ll = data.m_longlong1;
      ts.es.f = data.m_float1;
      ts.es.s = data.m_short1;
      CORBA_Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA_Any* out_any1 = ftc1->to_any (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      DynAnyTests::test_struct* ts_out;
      *out_any1 >>= ts_out;
      if (ts_out->es.s == data.m_short1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      // Created with NEW
      delete out_any1;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: current_member_name/current_member_kind\n"));

      ftc1->seek (2,
                 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      FieldName fn = ftc1->current_member_name (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (ACE_OS::strcmp (fn, "es"))
        ++this->error_count_;
      CORBA::TCKind tk = ftc1->current_member_kind (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (tk != CORBA::tk_struct)
        ++this->error_count_;
      if (this->error_count_ == 0)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n")); 

      ACE_DEBUG ((LM_DEBUG,
                 "testing: get_members/set_members\n"));

      NameValuePairSeq* nvps = fa1->get_members (TAO_TRY_ENV);
      CORBA_DynStruct_ptr sm =
        this->orb_->create_dyn_struct (DynAnyTests::_tc_test_struct,
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
      sm->set_members (*nvps,
                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
      NameValuePairSeq* gm = sm->get_members (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      if (ACE_OS::strcmp ((*gm)[2].id, "es"))
        ++this->error_count_;

      // Created with NEW
      delete nvps;
      delete gm;

      fa1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (fa1);
      CORBA::release (dp1);
      ftc1->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (ftc1);
      sm->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      CORBA::release (sm);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("test_dynstruct::run_test");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

