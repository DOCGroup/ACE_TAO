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

ACE_RCSID(Param_Test, nested_struct, "$Id$")

// ************************************************************************
//               Test_Nested_Struct
// ************************************************************************

Test_Nested_Struct::Test_Nested_Struct (void)
  : opname_ (CORBA::string_dup ("test_nested_struct")),
    inout_ (new Param_Test::Nested_Struct),
    out_ (0),
    ret_ (0)
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

int
Test_Nested_Struct::init_parameters (Param_Test_ptr objref,
                                     CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.vs.seq.length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_.vs.seq.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.vs.seq[i] = str;
    }
  return 0;
}

int
Test_Nested_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Nested_Struct; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Nested_Struct::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::Nested_Struct_out out (this->out_.out ());
  this->ret_ = objref->test_nested_struct (this->in_,
                                           this->inout_.inout (),
                                           out,
                                           env);
  return (env.exception () ? -1:0);
}

int
Test_Nested_Struct::add_args (CORBA::NVList_ptr param_list,
			      CORBA::NVList_ptr retval,
			      CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Nested_Struct,
                     (void *) &this->in_,
                     CORBA::B_FALSE);

  CORBA::Any inout_arg (Param_Test::_tc_Nested_Struct,
                        &this->inout_.inout (),
                        CORBA::B_FALSE);

  CORBA::Any out_arg (Param_Test::_tc_Nested_Struct,
                      0,//this->dii_out_,
                      CORBA::B_FALSE);

  // add parameters
  param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Nested_Struct,
                                            0,//this->dii_ret_,
                                            CORBA::B_FALSE, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Nested_Struct::check_validity_engine (Param_Test::Nested_Struct the_in,
                                           Param_Test::Nested_Struct the_inout,
                                           Param_Test::Nested_Struct the_out,
                                           Param_Test::Nested_Struct the_ret)
{
  CORBA::Boolean flag = 0;
  if ((the_in.vs.seq.length () == the_inout.vs.seq.length ()) &&
      (the_in.vs.seq.length () == the_out.vs.seq.length ()) &&
      (the_in.vs.seq.length () == the_ret.vs.seq.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < the_in.vs.seq.length () && flag; i++)
        {
          if (ACE_OS::strcmp (the_in.vs.seq[i], the_inout.vs.seq[i]) ||
              ACE_OS::strcmp (the_in.vs.seq[i], the_out.vs.seq[i]) ||
              ACE_OS::strcmp (the_in.vs.seq[i], the_ret.vs.seq[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (void)
{
  return check_validity_engine (this->in_,
                                this->inout_.in (),
                                this->out_.in (),
                                this->ret_.in ()); 
}

CORBA::Boolean
Test_Nested_Struct::check_validity (CORBA::Request_ptr req)
{

  CORBA::Environment env;

  *req->arguments ()->item (2, env)->value () >>= this->dii_out_;
  *req->result ()->value () >>= this->dii_ret_;

  return this->check_validity_engine (this->in_,
                                      this->inout_.in (),
                                      *this->dii_out_,
                                      *this->dii_ret_);
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


