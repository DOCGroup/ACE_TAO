// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    fixed_struct.cpp
//
// = DESCRIPTION
//    tests fixed sized structs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "fixed_struct.h"

ACE_RCSID(Param_Test, fixed_struct, "$Id$")

// ************************************************************************
//               Test_Fixed_Struct
// ************************************************************************

Test_Fixed_Struct::Test_Fixed_Struct (void)
  : opname_ (CORBA::string_dup ("test_fixed_struct"))
{
}

Test_Fixed_Struct::~Test_Fixed_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Fixed_Struct::opname (void) const
{
  return this->opname_;
}

void
Test_Fixed_Struct::dii_req_invoke (CORBA::Request *req,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Fixed_Struct::init_parameters (Param_Test_ptr /*objref*/,
                                    CORBA::Environment &/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  //ACE_UNUSED_ARG (objref);
  //ACE_UNUSED_ARG (env);

  this->in_ = gen->gen_fixed_struct ();
  ACE_OS::memset (&this->inout_,
                  0,
                  sizeof (Param_Test::Fixed_Struct));
  return 0;
}

int
Test_Fixed_Struct::reset_parameters (void)
{
  ACE_OS::memset (&this->inout_,
                  0,
                  sizeof (Param_Test::Fixed_Struct));

  ACE_OS::memset (&this->out_,
                  0,
                  sizeof (Param_Test::Fixed_Struct));

  ACE_OS::memset (&this->ret_,
                  0,
                  sizeof (Param_Test::Fixed_Struct));

  return 0;
}

int
Test_Fixed_Struct::run_sii_test (Param_Test_ptr objref,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->ret_ = objref->test_fixed_struct (this->in_,
                                          this->inout_,
                                          this->out_,
                                          ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_Fixed_Struct::add_args (CORBA::NVList_ptr param_list,
                             CORBA::NVList_ptr retval,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // We provide the top level memory
  // the Any does not own any of these
  CORBA::Any in_arg (Param_Test::_tc_Fixed_Struct,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Fixed_Struct,
                        &this->inout_,
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Fixed_Struct,
                      &this->out_,
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
  retval->item (0, ACE_TRY_ENV)->value ()->replace (Param_Test::_tc_Fixed_Struct,
                                                    &this->ret_,
                                                    0, // does not own
                                                    ACE_TRY_ENV);
  return 0;
}

CORBA::Boolean
Test_Fixed_Struct::check_validity (void)
{
  if (this->in_.l == this->inout_.l &&
      this->in_.c == this->inout_.c &&
      this->in_.s == this->inout_.s &&
      this->in_.o == this->inout_.o &&
      this->in_.f == this->inout_.f &&
      this->in_.b == this->inout_.b &&
      this->in_.d == this->inout_.d &&
      this->in_.l == this->out_.l &&
      this->in_.c == this->out_.c &&
      this->in_.s == this->out_.s &&
      this->in_.o == this->out_.o &&
      this->in_.f == this->out_.f &&
      this->in_.b == this->out_.b &&
      this->in_.d == this->out_.d &&
      this->in_.l == this->ret_.l &&
      this->in_.c == this->ret_.c &&
      this->in_.s == this->ret_.s &&
      this->in_.o == this->ret_.o &&
      this->in_.f == this->ret_.f &&
      this->in_.b == this->ret_.b &&
      this->in_.d == this->ret_.d)
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Fixed_Struct::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Fixed_Struct::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "inout = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "out = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "ret = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "=*=*=*=*=*=*\n",
              this->in_.l,
              this->in_.c,
              this->in_.s,
              this->in_.o,
              this->in_.f,
              this->in_.b,
              this->in_.d,
              this->inout_.l,
              this->inout_.c,
              this->inout_.s,
              this->inout_.o,
              this->inout_.f,
              this->inout_.b,
              this->inout_.d,
              this->out_.l,
              this->out_.c,
              this->out_.s,
              this->out_.o,
              this->out_.f,
              this->out_.b,
              this->out_.d,
              this->ret_.l,
              this->ret_.c,
              this->ret_.s,
              this->ret_.o,
              this->ret_.f,
              this->ret_.b,
              this->ret_.d));
}
