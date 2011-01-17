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
Test_Fixed_Struct::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_;
  req->set_return_type (Param_Test::_tc_Fixed_Struct);

  req->invoke ();

  Param_Test::Fixed_Struct *tmp;
  req->return_value () >>= tmp;
  this->ret_ = *tmp;

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= tmp;
  this->inout_ = *tmp;

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= tmp;
  this->out_ = *tmp;
}

int
Test_Fixed_Struct::init_parameters (Param_Test_ptr /*objref*/
/*env*/)
{
  Generator *gen = GENERATOR::instance (); // value generator

  //ACE_UNUSED_ARG (objref);

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
Test_Fixed_Struct::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_fixed_struct (this->in_,
                                              this->inout_,
                                              this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Fixed_Struct::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Fixed_Struct::check_validity (void)
{
  if (this->in_.l == this->inout_.l &&
      this->in_.c == this->inout_.c &&
      this->in_.s == this->inout_.s &&
      this->in_.o == this->inout_.o &&
      ACE::is_equal (this->in_.f, this->inout_.f) &&
      this->in_.b == this->inout_.b &&
      ACE::is_equal (this->in_.d, this->inout_.d) &&
      this->in_.l == this->out_.l &&
      this->in_.c == this->out_.c &&
      this->in_.s == this->out_.s &&
      this->in_.o == this->out_.o &&
      ACE::is_equal (this->in_.f, this->out_.f) &&
      this->in_.b == this->out_.b &&
      ACE::is_equal (this->in_.d, this->out_.d) &&
      this->in_.l == this->ret_.l &&
      this->in_.c == this->ret_.c &&
      this->in_.s == this->ret_.s &&
      this->in_.o == this->ret_.o &&
      ACE::is_equal (this->in_.f, this->ret_.f) &&
      this->in_.b == this->ret_.b &&
      ACE::is_equal (this->in_.d, this->ret_.d))
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
