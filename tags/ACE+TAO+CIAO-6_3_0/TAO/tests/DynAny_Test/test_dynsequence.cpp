//=============================================================================
/**
 *  @file    test_dynsequence.cpp
 *
 *  $Id$
 *
 *  Implementation of the simple DynSequence test
 *
 *
 *  @author Jeff Parsons <jp4@cs.wustl.edu>
 */
//=============================================================================


#include "test_dynsequence.h"
#include "da_testsC.h"
#include "data.h"
#include "analyzer.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "ace/OS_NS_string.h"

namespace
{

bool
compare_string_sequence_elements(
    CORBA::ULong i,
    DynamicAny::DynAnySeq const & lhs,
    DynamicAny::DynAnySeq const & rhs)
{
    CORBA::String_var lhs_string = lhs[i]->get_string ();

    ACE_DEBUG ((LM_DEBUG, "elem[%d] = %C\n", i, lhs_string.in()));

    CORBA::Boolean match =
        lhs[i]->equal (rhs[i]);

    if (!match)
    {
        CORBA::String_var rhs_string =
            rhs[i]->get_string();

        ACE_DEBUG ((LM_DEBUG,
                    "  mismatch with elements[%d] = %C\n", i,
                    rhs_string.in() ));
    }

    return match;
}
}

Test_DynSequence::Test_DynSequence (CORBA::ORB_var orb, int debug)
  : orb_ (orb),
    test_name_ (CORBA::string_dup ("test_dynsequence")),
    error_count_ (0),
    debug_ (debug)
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

    try
    {
      ACE_DEBUG ((LM_DEBUG,
                 "\t*=*=*=*= %C =*=*=*=*\n",
                 data.labels[11]));

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(Any)/insert/get/seek/"
                 "rewind/current_component\n"));

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

      DynAnyTests::SeqShort shortseq1;

      shortseq1.length (2);
      shortseq1[0] = 2;
      shortseq1[1] = -2;

      CORBA::Any any;
      any <<= shortseq1;

      DynamicAny::DynAny_var shortdany =
        dynany_factory->create_dyn_any (any);

      analyzer.analyze(shortdany.in ());

      CORBA::Any_var any3;
      any3 = shortdany->to_any ();

      ts[0] = data.m_string2;
      ts[1] = data.m_string2;
      CORBA::Any in_any1;
      in_any1 <<= ts;
      DynamicAny::DynAny_var dp1 =
        dynany_factory->create_dyn_any (in_any1);

      DynamicAny::DynSequence_var fa1 =
        DynamicAny::DynSequence::_narrow (dp1.in ());

      fa1->seek (1);

      fa1->insert_string (data.m_string1);

      fa1->rewind ();

      fa1->seek (1);

      CORBA::String_var out_str1 =
        fa1->get_string ();

      if (!ACE_OS::strcmp (out_str1.in (), data.m_string1))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((
          LM_DEBUG,
          "testing: set_elements_as_dyn_any/get_elements_as_dyn_any/equal\n"
        ));

      static const char *values[] =
      {
        "zero",
        "one",
        "two"
      };

      CORBA::ULong length = 3;
      DynamicAny::DynAnySeq *elem_ptr = 0;
      ACE_NEW_RETURN (elem_ptr,
                      DynamicAny::DynAnySeq (length),
                      -1);
      elem_ptr->length (length);
      DynamicAny::DynAnySeq_var elements (elem_ptr);
      CORBA::ULong i;

      for (i = 0; i < length; ++i)
        {
          CORBA::Any elem_any;
          elem_any <<= CORBA::Any::from_string (values[i], 8);
          elements[i] = dynany_factory->create_dyn_any (elem_any);
        }

      fa1->set_elements_as_dyn_any (elements.in ());

      DynamicAny::DynAnySeq_var out_elems =
        fa1->get_elements_as_dyn_any ();

      if (out_elems->length () != 3)
        {
          ++this->error_count_;
          return -1;
        }

      analyzer.analyze (fa1.in());

      for (i = 0; i < length; ++i)
        {
          if (!compare_string_sequence_elements(i,
                                                elements.in(), out_elems.in()))
          {
              error_count_++;
          }

          // To show that calling destroy() on a component does
          // nothing, as required by the spec.
          out_elems[i]->destroy ();
        }

      // Now verify that we can replace an element using the in/out return
      // type.  This illustrates the problems described in bug # 2877:
      //   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=2877
      // notice that this code has side-effects on the out_elems sequence, so
      // do not move this code up or you will make the comparisons above fail.
      {
          char replacement[] = "replacement";
          CORBA::Any tmp;
          tmp <<= CORBA::Any::from_string (replacement,20);
          CORBA::ULong io_index = 0;
          // Since we directly change an element in a sequence we give
          // ownership for the previous element to a local temporary object.
          DynamicAny::DynAny_var io_owner = out_elems[io_index].inout();
          out_elems[io_index].inout() = dynany_factory->create_dyn_any (tmp);

          // Compare the value of the replaced string.
          CORBA::String_var new_str = out_elems[io_index]->get_string();
          if (0 != ACE_OS::strcmp(new_str.in(), replacement))
          {
              ACE_DEBUG((LM_DEBUG,
                         "  Mismatch after replacing through in/out.\n"
                         "  expected = %C, got = %C\n",
                         replacement, new_str.in()));
              this->error_count_++;
          }
      }



      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "++ OK ++\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: constructor(TypeCode)/from_any/to_any\n"));

      DynamicAny::DynAny_var ftc1_base =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_test_seq
                          );

      DynamicAny::DynSequence_var ftc1 =
        DynamicAny::DynSequence::_narrow (ftc1_base.in ());

      if (CORBA::is_nil (ftc1.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DynSequence::_narrow() returned nil\n"),
                            -1);
        }

      ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("testing: constructor(%C)/create_dyn_any_from_type_code\n"),
        "BooleanSeq"
        ));

      DynamicAny::DynAny_var da_bool =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_SeqBoolean
                          );

      DynamicAny::DynSequence_var ds_bool =
        DynamicAny::DynSequence::_narrow (da_bool.in ());

      if (CORBA::is_nil (ds_bool.in ()))
      {
        ACE_DEBUG ((
          LM_DEBUG,
          ACE_TEXT ("DynSequence::_narrow() returned nil\n")
          ));
        this->error_count_++;
      }

      ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("testing: constructor(%C)/create_dyn_any_from_type_code\n"),
        "ShortSeq"
        ));

      DynamicAny::DynAny_var da_short =
        dynany_factory->create_dyn_any_from_type_code (
                            DynAnyTests::_tc_SeqShort
                          );

      DynamicAny::DynSequence_var ds_short =
        DynamicAny::DynSequence::_narrow (da_short.in ());

      if (CORBA::is_nil (ds_short.in ()))
      {
        ACE_DEBUG ((
          LM_DEBUG,
          ACE_TEXT ("DynSequence::_narrow() returned nil\n")
          ));
        this->error_count_++;
      }

      ts[0] = CORBA::string_dup (data.m_string1);
      CORBA::Any in_any2;
      in_any2 <<= ts;
      ftc1->from_any (in_any2);

      analyzer.analyze (ftc1.in());

      CORBA::Any_var out_any1 =
        ftc1->to_any ();

      DynAnyTests::test_seq *ts_out;
      out_any1.in () >>= ts_out;

      if (!ACE_OS::strcmp ((*ts_out)[0U], data.m_string1))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }
      else
        {
          ++this->error_count_;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "testing: length/set_elements/get_elements\n"));

      length = ftc1->get_length ();

      if (length != 2)
        {
          ++this->error_count_;
        }

      ftc1->set_length (3);
      DynamicAny::AnySeq as_in (3);
      as_in.length (3);
      CORBA::Any in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string2, 8);
      as_in[0] = in_any3;
      as_in[1] = in_any3;
      in_any3 <<= CORBA::Any::from_string (data.m_string1, 8);
      as_in[2] = in_any3;
      ftc1->set_elements (as_in);

      DynamicAny::AnySeq_var as_out =
        ftc1->get_elements ();

      CORBA::ULong index = 2;
      CORBA::Any out_any2 = as_out[index];
      char* out_str2;
      out_any2 >>= CORBA::Any::to_string (out_str2, 8);

      if (ACE_OS::strcmp (out_str2, data.m_string1))
        {
          ++this->error_count_;
        }

      if (this->error_count_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "++ OK ++\n"));
        }

      fa1->destroy ();

      ftc1->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_dynsequence::run_test");
      ++this->error_count_;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n%d errors\n",
              this->error_count_));


  return !!error_count_;
}
