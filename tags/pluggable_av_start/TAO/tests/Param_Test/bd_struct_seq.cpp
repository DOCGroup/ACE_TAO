// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_struct_seq.cpp
//
// = DESCRIPTION
//    tests bounded struct sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "bd_struct_seq.h"

ACE_RCSID(Param_Test, bd_struct_seq, "$Id$")

const CORBA::ULong MAX_STRUCTSEQ_LEN = 1;

// ************************************************************************
//               Test_Bounded_Struct_Sequence
// ************************************************************************

Test_Bounded_Struct_Sequence::Test_Bounded_Struct_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_struct_sequence")),
    inout_ (new Param_Test::Bounded_StructSeq),
    out_ (new Param_Test::Bounded_StructSeq),
    ret_ (new Param_Test::Bounded_StructSeq)
{
}

Test_Bounded_Struct_Sequence::~Test_Bounded_Struct_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Bounded_Struct_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_Struct_Sequence::dii_req_invoke (CORBA::Request *req,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Bounded_Struct_Sequence::init_parameters (Param_Test_ptr /*objref*/,
                                               CORBA::Environment &/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  //ACE_UNUSED_ARG (objref);
  //ACE_UNUSED_ARG (env);

  // set the length of the sequence
  this->in_.length (MAX_STRUCTSEQ_LEN);
  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = gen->gen_fixed_struct ();
    }
  return 0;
}

int
Test_Bounded_Struct_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Bounded_StructSeq; // delete the previous ones
  this->out_ = new Param_Test::Bounded_StructSeq;
  this->ret_ = new Param_Test::Bounded_StructSeq;
  return 0;
}

int
Test_Bounded_Struct_Sequence::run_sii_test (Param_Test_ptr objref,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  Param_Test::Bounded_StructSeq_out out (this->out_.out ());
  this->ret_ = objref->test_bounded_struct_sequence (this->in_,
                                                     this->inout_.inout (),
                                                     out,
                                                     ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_Bounded_Struct_Sequence::add_args (CORBA::NVList_ptr param_list,
                                        CORBA::NVList_ptr retval,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Any in_arg (Param_Test::_tc_Bounded_StructSeq,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Bounded_StructSeq,
                        &this->inout_.inout (),
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Bounded_StructSeq,
                      &this->out_.inout (), // .out () causes crash
                      0);

  // add parameters
  param_list->add_value ("s1",
                         in_arg,
                         CORBA::ARG_IN,
                         ACE_TRY_ENV);

  param_list->add_value ("s2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         ACE_TRY_ENV);

  param_list->add_value ("s3",
                         out_arg,
                         CORBA::ARG_OUT,
                         ACE_TRY_ENV);

  // add return value type
  retval->item (0, ACE_TRY_ENV)->value ()->replace (Param_Test::_tc_Bounded_StructSeq,
                                            &this->ret_.inout (), // see above
                                            0, // does not own
                                            ACE_TRY_ENV);
  return 0;
}

CORBA::Boolean
Test_Bounded_Struct_Sequence::check_validity (void)
{
  if (this->compare (this->in_, this->inout_.in ()) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Bounded_Struct_Sequence::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Bounded_Struct_Sequence::print_values (void)
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
Test_Bounded_Struct_Sequence::compare (const Param_Test::Bounded_StructSeq &s1,
                                       const Param_Test::Bounded_StructSeq &s2)
{
  if (s1.maximum () != s2.maximum ())
      return 0;
  if (s1.length () != s2.length ())
    return 0;

  for (CORBA::ULong i=0; i < s1.length (); i++)
    {
      const Param_Test::Fixed_Struct& vs1 = s1[i];
      const Param_Test::Fixed_Struct& vs2 = s2[i];

      if (vs1.l != vs2.l
          || vs1.c != vs2.c
          || vs1.s != vs2.s
          || vs1.o != vs2.o
          || vs1.f != vs2.f
          || vs1.b != vs2.b
          || vs1.d != vs2.d )
        return 0;
    }

  return 1; // success
}

void
Test_Bounded_Struct_Sequence::print_sequence (const Param_Test::Bounded_StructSeq &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i=0; i < s.length (); i++)
    {
      const Param_Test::Fixed_Struct& vs = s[i];

      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\tl = %d\n"
                  "\tc = %c\n"
                  "\ts = %d\n"
                  "\to = %x\n"
                  "\tf = %f\n"
                  "\tb = %d\n"
                  "\td = %f\n",
                  i,
                  vs.l, vs.c, vs.s, vs.o, vs.f, vs.b, vs.d));
    }
}
