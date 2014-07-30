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

  try
    {
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
      {
        ACE_DEBUG ((LM_DEBUG,
                  "\t*=*=*=*= %C =*=*=*=*\n",
                  data.labels[8]));

        ACE_DEBUG ((LM_DEBUG,
                  "testing: constructor(Any)/insert/get\n"));

        CORBA::Any in1;
        in1 <<= data.m_double2;
        DynamicAny::DynAny_var fa1 =
          dynany_factory->create_dyn_any (in1);

        if (fa1->equal(fa1))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "++ OK ++\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Double Not equal\n"));
            ++this->error_count_;
          }
        fa1->insert_double (data.m_double1);

        CORBA::Double d_out =
          fa1->get_double ();

        if (ACE::is_equal (d_out, data.m_double1))
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
          dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_double);
        CORBA::Any in_any1;
        in_any1 <<= data.m_double1;
        ftc1->from_any (in_any1);
        analyzer.analyze (ftc1.in());
        CORBA::Any_var out_any1 = ftc1->to_any ();
        out_any1 >>= d_out;

        if (ACE::is_equal (d_out, data.m_double1))
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

      {
        ACE_DEBUG ((LM_DEBUG,
          "\t*=*=*=*= %C =*=*=*=*\n",
          data.labels[17]));

        ACE_DEBUG ((LM_DEBUG,
          "testing: constructor(Any)/insert/get\n"));

        CORBA::Any in1;
        in1 <<= data.m_longdouble2;
        DynamicAny::DynAny_var fa1 =
          dynany_factory->create_dyn_any (in1);

        if (fa1->equal(fa1))
          {
            ACE_DEBUG ((LM_DEBUG,
              "++ OK ++\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Long Double Not equal\n"));
            ++this->error_count_;
          }

        fa1->insert_longdouble (data.m_longdouble1);

        CORBA::LongDouble d_out = fa1->get_longdouble ();

        if (ACE::is_equal (d_out, data.m_longdouble1))
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

        d_out = data.m_longdouble2;
        DynamicAny::DynAny_var ftc1 =
          dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_longdouble);

        CORBA::Any in_any1;
        in_any1 <<= data.m_longdouble1;
        ftc1->from_any (in_any1);

        analyzer.analyze(ftc1.in());

        CORBA::Any_var out_any1 = ftc1->to_any ();

        out_any1.in() >>= d_out;

        if (ACE::is_equal (d_out, data.m_longdouble1))
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

      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %C =*=*=*=*\n",
                 data.labels[12]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA::Any in;
      in <<= data.m_typecode2;
      DynamicAny::DynAny_var fa2 =
        dynany_factory->create_dyn_any (in);
      fa2->insert_typecode (data.m_typecode1);
      CORBA::TypeCode_var tc_out =
        fa2->get_typecode ();

      if (tc_out->equal (data.m_typecode1))
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
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_TypeCode);
      CORBA::Any in_any2;
      in_any2 <<= data.m_typecode1;
      ftc2->from_any (in_any2);
      analyzer.analyze (ftc2.in());
      CORBA::Any_var out_any2 = ftc2->to_any ();
      CORBA::TypeCode_ptr out_tc;
      out_any2 >>= out_tc;

      CORBA::Boolean equal = out_tc->equal (data.m_typecode1);

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
                 "\t*=*=*=*= %C =*=*=*=*\n",
                 data.labels[16]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get\n"));

      CORBA::Any in3;
      CORBA::ShortSeq ss;
      ss.length (0UL);
      in3 <<= ss;
      DynamicAny::DynAny_var fa3 =
        dynany_factory->create_dyn_any (in3);
      fa3->insert_short_seq (data.m_shortseq1);
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
        dynany_factory->create_dyn_any_from_type_code (CORBA::_tc_ShortSeq);
      CORBA::Any in_any3;
      in_any3 <<= data.m_shortseq1;
      ftc3->from_any (in_any3);
      analyzer.analyze (ftc3.in ());
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

      ACE_DEBUG ((LM_DEBUG,
                 "testing: equal\n"));

      // Simple equal to self test for basic sequence
      // Check equal to self, not equal for different length
      // not equal for different values
      CORBA::Any in4;
      in4 <<= data.m_shortseq1;
      DynamicAny::DynAny_var fa4 =
        dynany_factory->create_dyn_any (in4);

      // is equal to self
      good = fa4->equal(fa4);

      CORBA::ShortSeq ss5;
      ss5.length(1);
      ss5[0] = 5;
      CORBA::Any in5;
      in5 <<= ss5;
      DynamicAny::DynAny_var fa5 =
        dynany_factory->create_dyn_any (in5);

      // is not equal for different lengths
      good = good && !fa4->equal(fa5);

      CORBA::ShortSeq ss6(data.m_shortseq1);
      ss6[0]++;
      CORBA::Any in6;
      in6 <<= ss6;

      DynamicAny::DynAny_var fa6 =
        dynany_factory->create_dyn_any (in6);

      // is not equal for different values 
      good = good && !fa4->equal(fa6);


      CORBA::Any in7;
      in7 <<= data.m_shortseq1;
      DynamicAny::DynAny_var fa7 =
        dynany_factory->create_dyn_any (in7);


      // is equal to identical sequence, not self
      good = good && fa4->equal(fa7);


      if (good)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynany::run_test");
      ++this->error_count_;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));

  return error_count_;
}
