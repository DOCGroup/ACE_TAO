// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_string.cpp
//
// = DESCRIPTION
//    tests unbounded strings
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ub_string.h"

// ************************************************************************
//               Test_Unbounded_String
// ************************************************************************

Test_Unbounded_String::Test_Unbounded_String (void)
  : opname_ (CORBA::string_dup ("test_unbounded_string"))
{
}

Test_Unbounded_String::~Test_Unbounded_String (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Unbounded_String::opname (void) const
{
  return this->opname_;
}

int
Test_Unbounded_String::init_parameters (Alt_Mapping_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  this->in_ = gen->gen_string ();
  this->inout_ = this->in_.c_str ();

  return 0;
}

int
Test_Unbounded_String::reset_parameters (void)
{
  this->inout_ = this->in_.c_str ();
  this->out_.clear ();
  this->ret_.clear ();

  return 0;
}

int
Test_Unbounded_String::run_sii_test (Alt_Mapping_ptr objref)
{
  try
    {
      this->ret_ = objref->test_unbounded_string (this->in_,
                                                  this->inout_,
                                                  this->out_);//str_out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Unbounded_String::run_sii_test\n");
    }

  return -1;
}

CORBA::Boolean
Test_Unbounded_String::check_validity (void)
{
  CORBA::ULong len = this->in_.length ();

  std::string first_half = this->inout_.substr (0, len);
  std::string second_half = this->inout_.substr (len, len);

  return (this->in_ == this->out_
          && this->in_ == this->ret_
          && this->inout_.length () == 2 * len
          && this->in_ == first_half
          && this->in_ == second_half);
}

CORBA::Boolean
Test_Unbounded_String::check_validity (CORBA::Request_ptr )
{
  // No need to retrieve anything because, for all the args and
  // the return, we provided the memory and we own it.
  return this->check_validity ();
}

void
Test_Unbounded_String::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in with len (%d) = %s\n"
              "inout with len (%d) = %s\n"
              "out with len (%d) = %s\n"
              "ret with len (%d) = %s\n"
              "\n=*=*=*=*=*=*\n",
              this->in_.length (),
              this->in_.c_str (),
              this->inout_.length (),
              this->inout_.c_str (),
              this->out_.length (),
              this->out_.c_str (),
              this->ret_.length (),
              this->ret_.c_str ()));
}
