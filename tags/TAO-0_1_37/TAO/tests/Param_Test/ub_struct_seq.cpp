// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_struct_seq.cpp
//
// = DESCRIPTION
//    tests unbounded struct sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "ub_struct_seq.h"

// ************************************************************************
//               Test_Struct_Sequence
// ************************************************************************

Test_Struct_Sequence::Test_Struct_Sequence (void)
  : opname_ (CORBA::string_dup ("test_struct_sequence")),
    inout_ (new Param_Test::StructSeq),
    out_ (0),
    ret_ (0)
{
}

Test_Struct_Sequence::~Test_Struct_Sequence (void)
{
  CORBA::string_free (this->opname_);
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Struct_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_Struct_Sequence::init_parameters (Param_Test_ptr objref,
                                     CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.length (len);
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
Test_Struct_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::StructSeq; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Struct_Sequence::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::StructSeq_out out (this->out_.out ());
  this->ret_ = objref->test_struct_sequence (this->in_,
                                           this->inout_.inout (),
                                           out,
                                           env);
  return (env.exception () ? -1:0);
}

int
Test_Struct_Sequence::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_StructSeq, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_StructSeq, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_StructSeq, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_StructSeq,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Struct_Sequence::check_validity (void)
{
  if (this->compare (this->in_, this->inout_.in ()) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Struct_Sequence::check_validity (CORBA::Request_ptr req)
{
#if 0
  CORBA::Environment env;
  this->inout_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *)
                                                req->arguments ()->item
                                                (1, env)->value ()->value ());
  this->out_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *) req->arguments
                                              ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *)req->result
                                              ()->value ()->value ());

#endif
  return this->check_validity ();
}

void
Test_Struct_Sequence::print_values (void)
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
Test_Struct_Sequence::compare (const Param_Test::StructSeq &s1,
                               const Param_Test::StructSeq &s2)
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
Test_Struct_Sequence::print_sequence (const Param_Test::StructSeq &s)
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

