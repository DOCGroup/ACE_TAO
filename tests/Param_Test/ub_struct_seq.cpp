// -*- C++ -*-

//=============================================================================
/**
 *  @file    ub_struct_seq.cpp
 *
 *  $Id$
 *
 *  tests unbounded struct sequences
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "ub_struct_seq.h"

// ************************************************************************
//               Test_Struct_Sequence
// ************************************************************************

Test_Struct_Sequence::Test_Struct_Sequence (void)
  : opname_ (CORBA::string_dup ("test_struct_sequence")),
    inout_ (new Param_Test::StructSeq),
    out_ (new Param_Test::StructSeq),
    ret_ (new Param_Test::StructSeq)
{
}

Test_Struct_Sequence::~Test_Struct_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Struct_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Struct_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_StructSeq);

  req->invoke ();

  Param_Test::StructSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::StructSeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::StructSeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::StructSeq (*tmp);
}

int
Test_Struct_Sequence::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.length (len);
  this->inout_->length (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // generate some arbitrary struct to be filled into the ith location in
      // the sequence
      this->in_[i] = gen->gen_fixed_struct ();
      this->inout_[i] = gen->gen_fixed_struct ();
    }
  return 0;
}

int
Test_Struct_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::StructSeq; // delete the previous one
  this->out_ = new Param_Test::StructSeq;
  this->ret_ = new Param_Test::StructSeq;
  return 0;
}

int
Test_Struct_Sequence::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::StructSeq_out out (this->out_.out ());

      this->ret_ = objref->test_struct_sequence (this->in_,
                                                 this->inout_.inout (),
                                                 out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Struct_Sequence::run_sii_test\n");

    }
  return -1;
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
Test_Struct_Sequence::check_validity (CORBA::Request_ptr )
{
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
          || !ACE::is_equal (vs1.f, vs2.f)
          || vs1.b != vs2.b
          || !ACE::is_equal (vs1.d, vs2.d))
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


// ************************************************************************
//               Test_Unbounded_Struct_Sequence
// ************************************************************************

Test_Unbounded_Struct_Sequence::Test_Unbounded_Struct_Sequence (void)
  : opname_ (CORBA::string_dup ("test_unbounded_struct_sequence")),
    inout_ (new Param_Test::PathSpec),
    out_ (new Param_Test::PathSpec),
    ret_ (new Param_Test::PathSpec)
{
}

Test_Unbounded_Struct_Sequence::~Test_Unbounded_Struct_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Unbounded_Struct_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Unbounded_Struct_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_PathSpec);

  req->invoke ();

  Param_Test::PathSpec *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::PathSpec (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::PathSpec (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::PathSpec (*tmp);
}

int
Test_Unbounded_Struct_Sequence::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.length (len);
  this->inout_->length (len);
  // now set each individual element

  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = gen->gen_step ();
      this->inout_[i] = gen->gen_step ();
    }
  return 0;
}

int
Test_Unbounded_Struct_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::PathSpec; // delete the previous one
  this->out_ = new Param_Test::PathSpec;
  this->ret_ = new Param_Test::PathSpec;
  return 0;
}

int
Test_Unbounded_Struct_Sequence::run_sii_test (Param_Test_ptr objref)
{
  Param_Test::PathSpec_out out (this->out_.out ());
  this->ret_ = objref->test_unbounded_struct_sequence (this->in_,
                                                       this->inout_.inout (),
                                                       out);

  return 0;
}

CORBA::Boolean
Test_Unbounded_Struct_Sequence::check_validity (void)
{
  if (this->compare (this->in_, this->inout_.in ()) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Unbounded_Struct_Sequence::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_Unbounded_Struct_Sequence::print_values (void)
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
Test_Unbounded_Struct_Sequence::compare (const Param_Test::PathSpec &s1,
                                         const Param_Test::PathSpec &s2)
{
  if (s1.maximum () != s2.maximum ())
      return 0;
  if (s1.length () != s2.length ())
    return 0;

  for (CORBA::ULong i=0; i < s1.length (); i++)
    {
      const Param_Test::Step& vs1 = s1[i];
      const Param_Test::Step& vs2 = s2[i];

      if (ACE_OS::strcmp (vs1.name.id.in (), vs2.name.id.in ())
          || ACE_OS::strcmp (vs1.name.kind.in (), vs2.name.kind.in ())
          || vs1.process != vs2.process)
        return 0;
    }
  return 1; // success
}

void
Test_Unbounded_Struct_Sequence::print_sequence (const Param_Test::PathSpec &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i=0; i < s.length (); i++)
    {
      const Param_Test::Step& vs = s[i];

      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\tname.id = %s\n"
                  "\tname.kind = %s\n"
                  "\tprocess = %d\n",
                  i,
                  vs.name.id.in (), vs.name.kind.in (), vs.process));
    }
}
