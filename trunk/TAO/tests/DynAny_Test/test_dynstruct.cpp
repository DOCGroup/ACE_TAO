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
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "test_dynstruct.h"
#include "da_testsC.h"
#include "data.h"

Test_DynStruct::Test_DynStruct (CORBA::ORB_var orb)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynstruct")),
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
  ts.l = data.m_long2;
  ts.es.f = data.m_float2;
  ts.es.s = data.m_short2;

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/next/seek/rewind/current_component\n"));

      CORBA_Any in_any1;
      in_any1 <<= ts;
      CORBA_DynAny_var dp1 = 
        this->orb_->create_dyn_any (in_any1,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_DynStruct_var fa1 = CORBA_DynStruct::_narrow (dp1,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->insert_char (data.m_char1,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
      fa1->insert_long (data.m_long1,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::DynAny_var cc = fa1->current_component (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cc->insert_float (data.m_float1,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      cc = fa1->current_component (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cc->insert_short (data.m_short1,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      cc = fa1->current_component (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cc->rewind (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      fa1->rewind (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Char c = fa1->get_char (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (c != data.m_char1)
        ++this->error_count_;
      CORBA::Long l = fa1->get_long (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (l != data.m_long1)
        ++this->error_count_;

      cc = fa1->current_component (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cc->seek (1,
                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      cc = fa1->current_component (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::Short s = cc->get_short (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (s != data.m_short1)
        ++this->error_count_;
      if (this->error_count_ == 0)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
 
      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      CORBA_DynStruct_var ftc1 = 
        this->orb_->create_dyn_struct (DynAnyTests::_tc_test_struct,
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ts.c = data.m_char1;
      ts.l = data.m_long1;
      ts.es.f = data.m_float1;
      ts.es.s = data.m_short1;
      CORBA_Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2,
                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_Any_var out_any1 = ftc1->to_any (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      DynAnyTests::test_struct* ts_out;
      out_any1.in () >>= ts_out;
      if (ts_out->es.s == data.m_short1)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n"));
      else 
        ++this->error_count_;

      ACE_DEBUG ((LM_DEBUG,
                 "testing: current_member_name/current_member_kind\n"));

      ftc1->seek (2,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::FieldName_var fn = ftc1->current_member_name (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (ACE_OS::strcmp (fn.in (), "es"))
        ++this->error_count_;
      CORBA::TCKind tk = ftc1->current_member_kind (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (tk != CORBA::tk_struct)
        ++this->error_count_;
      if (this->error_count_ == 0)
        ACE_DEBUG ((LM_DEBUG,
                   "++ OK ++\n")); 

      ACE_DEBUG ((LM_DEBUG,
                 "testing: get_members/set_members\n"));

      CORBA::NameValuePairSeq_var nvps = fa1->get_members (ACE_TRY_ENV);
      CORBA_DynStruct_var sm =
        this->orb_->create_dyn_struct (DynAnyTests::_tc_test_struct,
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sm->set_members (nvps.in (),
                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::NameValuePairSeq_var gm = sm->get_members (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::ULong index = 2;
      if (ACE_OS::strcmp (gm[index].id, "es"))
        ++this->error_count_;

      fa1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ftc1->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sm->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "test_dynstruct::run_test");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return 0;
}

