//=============================================================================
/**
 *  @file    test_dynarray.cpp
 *
 *  $Id$
 *
 *  Implementation of the DynArray test
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "test_dynarray.h"
#include "da_testsC.h"
#include "data.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "analyzer.h"

Test_DynArray::Test_DynArray (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynarray")),
    error_count_ (0),
    debug_ (debug)
{
}

Test_DynArray::~Test_DynArray (void)
{
  CORBA::string_free (this->test_name_);
  this->test_name_ = 0;
}

const char*
Test_DynArray::test_name (void) const
{
  return this->test_name_;
}

int
Test_DynArray::run_test (void)
{
  Data data (this->orb_);

  DynAnyTests::test_array ta = {0};

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %C =*=*=*=*\n",
                 data.labels[4]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/rewind/current_component\n"));

      CORBA::Object_var factory_obj =
        this->orb_->resolve_initial_references ("DynAnyFactory");

      DynamicAny::DynAnyFactory_var dynany_factory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ());

      if (CORBA::is_nil (dynany_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil dynamic any factory after narrow\n"),
                            -1);
        }

      DynAnyAnalyzer analyzer (this->orb_.in (),
                               dynany_factory.in (),
                               debug_);

      CORBA::Any in_any1;
      in_any1 <<= DynAnyTests::test_array_forany (ta);
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1);

      DynamicAny::DynArray_var fa1 =
        DynamicAny::DynArray::_narrow (dp1.in ());

      fa1->seek (1);

      fa1->insert_long (data.m_long1);

      fa1->rewind ();

      CORBA::Long l_out1 = data.m_long2;
      fa1->seek (1);

      l_out1 = fa1->get_long ();

      if (l_out1 == data.m_long1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      DynamicAny::DynAny_var ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (DynAnyTests::_tc_test_array);

      DynamicAny::DynArray_var ftc1 =
        DynamicAny::DynArray::_narrow (ftc1_base.in ());

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynArray::_narrow() returned nil\n"),
                            -1);
        }

      ta[1U] = data.m_long1;
      CORBA::Any in_any2;
      in_any2 <<= DynAnyTests::test_array_forany (ta);

      ftc1->from_any (in_any2);


      analyzer.analyze (ftc1.in ());

      CORBA::Any_var out_any1 =
        ftc1->to_any ();

      DynAnyTests::test_array_forany ta_out;
      out_any1.in () >>= ta_out;

      if (ta_out[1U] == data.m_long1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: set_elements/get_elements\n"));

      DynamicAny::AnySeq as_in (2);
      as_in.length (2);
      CORBA::Any in_any3;
      in_any3 <<= data.m_long1;
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      ftc1->set_elements (as_in);

      DynamicAny::AnySeq_var as_out =
        ftc1->get_elements ();
      CORBA::Any out_any2 = as_out[1U];
      CORBA::Long l_out2;
      out_any2 >>= l_out2;

      if (l_out2 == data.m_long1)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      fa1->destroy ();

      ftc1->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynarray::run_test");
      ++this->error_count_;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return !!error_count_;
}
