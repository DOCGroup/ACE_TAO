// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_array_seq.cpp
//
// = DESCRIPTION
//    tests bounded array sequences
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "helper.h"
#include "bd_array_seq.h"

const CORBA::ULong MAX_ARRAYSEQ_LEN = 1;

// ************************************************************************
//               Test_Bounded_Array_Sequence
// ************************************************************************

Test_Bounded_Array_Sequence::Test_Bounded_Array_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_array_sequence")),
    inout_ (new Param_Test::Bounded_ArraySeq),
    out_ (new Param_Test::Bounded_ArraySeq),
    ret_ (new Param_Test::Bounded_ArraySeq)
{
}

Test_Bounded_Array_Sequence::~Test_Bounded_Array_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Bounded_Array_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_Array_Sequence::dii_req_invoke (CORBA::Request *req,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Bounded_Array_Sequence::init_parameters (Param_Test_ptr,
                                               CORBA::Environment &)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // set the length of the sequence
  this->in_.length (MAX_ARRAYSEQ_LEN);

  // now set each individual element
  Param_Test::Fixed_Array tmp;

  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // Generate some arbitrary array to be filled into the ith
      // location in the sequence.
      for (CORBA::ULong j = 0; j < Param_Test::DIM1; j++)
        {
          tmp[j] = gen->gen_long ();
        }

      Param_Test::Fixed_Array_copy (this->in_[i], tmp);
    }

  return 0;
}

int
Test_Bounded_Array_Sequence::reset_parameters (void)
{
  // Delete the previous ones.
  this->inout_ = new Param_Test::Bounded_ArraySeq;
  this->out_ = new Param_Test::Bounded_ArraySeq;
  this->ret_ = new Param_Test::Bounded_ArraySeq;
  return 0;
}

int
Test_Bounded_Array_Sequence::run_sii_test (Param_Test_ptr objref,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Param_Test::Bounded_ArraySeq_out out (this->out_.out ());

      this->ret_ = 
        objref->test_bounded_array_sequence (this->in_,
                                             this->inout_.inout (),
                                             out,
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Bounded_Array_Sequence::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

int
Test_Bounded_Array_Sequence::add_args (CORBA::NVList_ptr param_list,
                                        CORBA::NVList_ptr retval,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any in_arg (Param_Test::_tc_Bounded_ArraySeq,
                         &this->in_,
                         0);

      CORBA::Any inout_arg (Param_Test::_tc_Bounded_ArraySeq,
                            &this->inout_.inout (),
                            0);

      CORBA::Any out_arg (Param_Test::_tc_Bounded_ArraySeq,
                          &this->out_.inout (),
                          0);

      // add parameters
      param_list->add_value ("s1",
                             in_arg,
                             CORBA::ARG_IN,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s2",
                             inout_arg,
                             CORBA::ARG_INOUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s3",
                             out_arg,
                             CORBA::ARG_OUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // add return value type
      CORBA::NamedValue *item = retval->item (0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      item->value ()->replace (Param_Test::_tc_Bounded_ArraySeq,
                               &this->ret_.inout (),
                               0, // does not own
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Bounded_Array_Sequence::add_args\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_Bounded_Array_Sequence::check_validity (void)
{
  if (this->compare (this->in_, this->inout_.in ()) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Bounded_Array_Sequence::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_Bounded_Array_Sequence::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "IN sequence\n"));
  this->print_sequence (this->in_);
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "INOUT sequence\n"));
  this->print_sequence (this->inout_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "OUT sequence\n"));
  this->print_sequence (this->out_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "RET sequence\n"));
  this->print_sequence (this->ret_.in ());
}

CORBA::Boolean
Test_Bounded_Array_Sequence::compare (const Param_Test::Bounded_ArraySeq &s1,
                                      const Param_Test::Bounded_ArraySeq &s2)
{
  if (s1.maximum () != s2.maximum ())
    {
      return 0;
    }

  if (s1.length () != s2.length ())
    {
      return 0;
    }

  for (CORBA::ULong i = 0; i < s1.length (); i++)
    {
      const Param_Test::Fixed_Array& vs1 = s1[i];
      const Param_Test::Fixed_Array& vs2 = s2[i];

      for (CORBA::ULong j = 0; j < Param_Test::DIM1; j++)
        {
          if (vs1[j] != vs2[j])
            {
              return 0;
            }
        }
    }

  return 1; // success
}

void
Test_Bounded_Array_Sequence::print_sequence (const Param_Test::Bounded_ArraySeq &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));

  for (CORBA::ULong i=0; i < s.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d --\n",
                  i));

      const Param_Test::Fixed_Array& vs = s[i];

      for (CORBA::ULong j = 0; j < Param_Test::DIM1; j++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Element%d[%d] = %d\n",
                      i,
                      j,
                      vs[j]));
        }
    }
}
