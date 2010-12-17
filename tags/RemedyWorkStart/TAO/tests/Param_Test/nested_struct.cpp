// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    nested_struct.cpp
//
// = DESCRIPTION
//    tests nested structs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "nested_struct.h"

// ************************************************************************
//               Test_Nested_Struct
// ************************************************************************

Test_Nested_Struct::Test_Nested_Struct (void)
  : opname_ (CORBA::string_dup ("test_nested_struct")),
    inout_ (new Param_Test::Nested_Struct),
    out_ (new Param_Test::Nested_Struct),
    ret_ (new Param_Test::Nested_Struct)
{
}

Test_Nested_Struct::~Test_Nested_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Nested_Struct::opname (void) const
{
  return this->opname_;
}

void
Test_Nested_Struct::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();
  req->set_return_type (Param_Test::_tc_Nested_Struct);

  req->invoke ();

  Param_Test::Nested_Struct *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Nested_Struct (*tmp);

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= tmp;
  this->inout_ = new Param_Test::Nested_Struct (*tmp);

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= tmp;
  this->out_ = new Param_Test::Nested_Struct (*tmp);
}

int
Test_Nested_Struct::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  this->in_.vs.dbl = 0.0;
  this->in_.vs.dummy1 = CORBA::string_dup ("");
  this->in_.vs.boole = 0;
  this->in_.vs.dummy2 = CORBA::string_dup ("");
  this->in_.vs.shrt = 0;
  // set the length of the sequence
  this->in_.vs.seq.length (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.vs.seq[i] = str;
    }

  this->inout_->vs.dbl = 0.0;
  this->inout_->vs.dummy1 = CORBA::string_dup ("");
  this->inout_->vs.boole = 0;
  this->inout_->vs.dummy2 = CORBA::string_dup ("");
  this->inout_->vs.shrt = 0;
  // set the length of the sequence
  this->inout_->vs.seq.length (0);

  return 0;
}

int
Test_Nested_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Nested_Struct; // delete the previous ones
  this->out_ = new Param_Test::Nested_Struct;
  this->ret_ = new Param_Test::Nested_Struct;

  this->inout_->vs.dbl = 0.0;
  this->inout_->vs.dummy1 = CORBA::string_dup ("");
  this->inout_->vs.boole = 0;
  this->inout_->vs.dummy2 = CORBA::string_dup ("");
  this->inout_->vs.shrt = 0;
  // set the length of the sequence
  this->inout_->vs.seq.length (0);

  return 0;
}

int
Test_Nested_Struct::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::Nested_Struct_out out (this->out_.out ());
      this->ret_ = objref->test_nested_struct (this->in_,
                                               this->inout_.inout (),
                                               out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Nested_Struct::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((this->in_.vs.seq.length () == this->inout_->vs.seq.length ()) &&
      (this->in_.vs.seq.length () == this->out_->vs.seq.length ()) &&
      (this->in_.vs.seq.length () == this->ret_->vs.seq.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < this->in_.vs.seq.length () && flag; i++)
        {
          if (ACE_OS::strcmp (this->in_.vs.seq[i], this->inout_->vs.seq[i]) ||
              ACE_OS::strcmp (this->in_.vs.seq[i], this->out_->vs.seq[i]) ||
              ACE_OS::strcmp (this->in_.vs.seq[i], this->ret_->vs.seq[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Nested_Struct::print_values (void)
{
  for (CORBA::ULong i=0; i < this->in_.vs.seq.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n*=*=*=*=*=*=*=*=*=*=\n"
                  "Element # %d\n"
                  "in (len = %d): %s\n"
                  "inout (len = %d): %s\n"
                  "out (len = %d): %s\n"
                  "ret (len = %d): %s\n",
                  i,
                  this->in_.vs.seq.length (),
                  (this->in_.vs.seq.length ()? (const char *)this->in_.vs.seq[i]:"<nul>"),
                  this->inout_->vs.seq.length (),
                  (this->inout_->vs.seq.length ()? (const char *)this->inout_->vs.seq[i]:"<nul>"),
                  this->out_->vs.seq.length (),
                  (this->out_->vs.seq.length ()? (const char *)this->out_->vs.seq[i]:"<nul>"),
                  this->ret_->vs.seq.length (),
                  (this->ret_->vs.seq.length ()? (const char *)this->ret_->vs.seq[i]:"<nul>")));
    }
}
