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
Test_Nested_Struct::dii_req_invoke (CORBA::Request *req, CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Nested_Struct::init_parameters (Param_Test_ptr /*objref*/,
                                     CORBA::Environment &/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  //ACE_UNUSED_ARG (objref);
  //ACE_UNUSED_ARG (env);

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
  this->inout_ = new Param_Test::Nested_Struct; // delete the previous ones
  this->out_ = new Param_Test::Nested_Struct;
  this->ret_ = new Param_Test::Nested_Struct;
  return 0;
}

int
Test_Nested_Struct::run_sii_test (Param_Test_ptr objref,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  Param_Test::Nested_Struct_out out (this->out_.out ());
  this->ret_ = objref->test_nested_struct (this->in_,
                                           this->inout_.inout (),
                                           out,
                                           ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_Nested_Struct::add_args (CORBA::NVList_ptr param_list,
			      CORBA::NVList_ptr retval,
			      CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Any in_arg (Param_Test::_tc_Nested_Struct,
                     (void *) &this->in_,
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Nested_Struct,
                        &this->inout_.inout (),
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Nested_Struct,
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

  // add return value
  retval->item (0, ACE_TRY_ENV)->value ()->replace (Param_Test::_tc_Nested_Struct,
                                                    &this->ret_.inout (), // see above
                                                    0, // does not own
                                                    ACE_TRY_ENV);
  return 0;
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
