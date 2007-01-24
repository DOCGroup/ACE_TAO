//=============================================================================
/**
 *  @file    test_dynany.cpp
 *
 *  $Id$
 *
 *  Implementation of the basic test for simple DynAnys
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/ShortSeqA.h"
#include "tao/DynamicAny/DynamicAny.h"

#include "test_dynany.h"
#include "data.h"
#include "analyzer.h"

Test_DynAny::Test_DynAny (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynany")),
    error_count_ (0),
    debug_ (debug)
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

      CORBA::Object_var factory_obj =
        this->orb_->resolve_initial_references ("DynAnyFactory"
                                                ACE_ENV_ARG_PARAMETER);

      DynamicAny::DynAnyFactory_var dynany_factory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ()
                                            ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (dynany_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil dynamic any factory after narrow\n"),
                            -1);
        }

      DynAnyAnalyzer analyzer (this->orb_.in (),
                               dynany_factory.in (),
                               debug_);

      CORBA::Any in1;
      in1 <<= data.m_double2;
      DynamicAny::DynAny_var fa1 =
        dynany_factory->create_dyn_any (in1 ACE_ENV_ARG_PARAMETER);

      fa1->insert_double (data.m_double1
                          ACE_ENV_ARG_PARAMETER);

      CORBA::Double d_out =
        fa1->get_double ();

      if (d_out == data.m_double1)
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

      d_out = data.m_double2;
      DynamicAny::DynAny_var ftc1 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_double
                                                       ACE_ENV_ARG_PARAMETER);
      CORBA::Any in_any1;
      in_any1 <<= data.m_double1;
      ftc1->from_any (in_any1
                      ACE_ENV_ARG_PARAMETER);
      analyzer.analyze (ftc1.in() ACE_ENV_ARG_PARAMETER);
      CORBA::Any_var out_any1 = ftc1->to_any ();
      out_any1 >>= d_out;

      if (d_out == data.m_double1)
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

      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[12]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA::Any in;
      in <<= data.m_typecode2;
      DynamicAny::DynAny_var fa2 =
        dynany_factory->create_dyn_any (in
                                        ACE_ENV_ARG_PARAMETER);
      fa2->insert_typecode (data.m_typecode1
                            ACE_ENV_ARG_PARAMETER);
      CORBA::TypeCode_var tc_out =
        fa2->get_typecode ();

      if (tc_out->equal (data.m_typecode1
                         ACE_ENV_ARG_PARAMETER))
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

      DynamicAny::DynAny_var ftc2 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_TypeCode
                                                       ACE_ENV_ARG_PARAMETER);
      CORBA::Any in_any2;
      in_any2 <<= data.m_typecode1;
      ftc2->from_any (in_any2
                      ACE_ENV_ARG_PARAMETER);
      analyzer.analyze (ftc2.in() ACE_ENV_ARG_PARAMETER);
      CORBA::Any_var out_any2 = ftc2->to_any ();
      CORBA::TypeCode_ptr out_tc;
      out_any2 >>= out_tc;

      CORBA::Boolean equal = out_tc->equal (data.m_typecode1
                                            ACE_ENV_ARG_PARAMETER);

      if (equal)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }


      fa2->destroy ();
      ftc2->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %s =*=*=*=*\n",
                 data.labels[16]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA::Any in3;
      CORBA::ShortSeq ss;
      ss.length (0UL);
      in3 <<= ss;
      DynamicAny::DynAny_var fa3 =
        dynany_factory->create_dyn_any (in3
                                        ACE_ENV_ARG_PARAMETER);
      fa3->insert_short_seq (data.m_shortseq1
                             ACE_ENV_ARG_PARAMETER);
      data.m_shortseq2 =
        fa3->get_short_seq ();

      bool good =
        data.m_shortseq2->length () == data.m_shortseq1.length ();

      if (good)
        {
          for (CORBA::ULong i = 0; i < data.m_shortseq1.length (); ++i)
            {
              if (data.m_shortseq2[i] != data.m_shortseq1[i])
                {
                  good = false;
                  break;
                }
            }
        }

     if (good)
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

      DynamicAny::DynAny_var ftc3 =
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ShortSeq
                                                       ACE_ENV_ARG_PARAMETER);
      CORBA::Any in_any3;
      in_any3 <<= data.m_shortseq1;
      ftc3->from_any (in_any3
                      ACE_ENV_ARG_PARAMETER);
      analyzer.analyze (ftc3.in () ACE_ENV_ARG_PARAMETER);
      CORBA::Any_var out_any3 = ftc3->to_any ();

      CORBA::ShortSeq *outseq = 0;
      out_any3.in () >>= outseq;

      good =
        outseq->length () == data.m_shortseq1.length ();

      if (good)
        {
          for (CORBA::ULong i = 0; i < data.m_shortseq1.length (); ++i)
            {
              if ((*outseq)[i] != data.m_shortseq1[i])
                {
                  good = false;
                  break;
                }
            }
        }

      if (good)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }


      fa3->destroy ();
      ftc3->destroy ();
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
