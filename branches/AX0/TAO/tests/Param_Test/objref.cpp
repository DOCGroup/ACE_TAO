// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    objref.cpp
//
// = DESCRIPTION
//    tests object references
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "objref.h"

ACE_RCSID(Param_Test, objref, "$Id$")

// ************************************************************************
//               Test_ObjRef
// ************************************************************************

Test_ObjRef::Test_ObjRef (void)
  : opname_ (CORBA::string_dup ("test_objref"))
{
}

Test_ObjRef::~Test_ObjRef (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_ObjRef::opname (void) const
{
  return this->opname_;
}

void
Test_ObjRef::dii_req_invoke (CORBA::Request *req
                             ACE_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (_tc_Coffee);

  req->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Coffee_ptr tmp;
  req->return_value () >>= tmp;
  this->ret_ = Coffee::_duplicate (tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = Coffee::_duplicate (tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = Coffee::_duplicate (tmp);
}

static const char *Coffee_Flavor [] = {
  "Italian Roast",
  "Irish Creme",
  "Costa Rican",
  "Colombian Supremo",
  "Macademia Nut",
  "Swiss Chocolate Mocha"
};

int
Test_ObjRef::init_parameters (Param_Test_ptr objref
                              ACE_ENV_ARG_DECL)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  char msg_str[256] = "";

  ACE_TRY
    {
      ACE_OS::strcpy (msg_str, "make_cofee");

      // first get a Coffee object
      this->in_ = objref->make_coffee (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get some index into Coffee_Flavor [].
      CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
      desc.name = Coffee_Flavor [index];

      // set the attribute of the object
      ACE_OS::strcpy (msg_str, "set coffee attribute");
      this->in_->description (desc ACE_ENV_ARG_PARAMETER); // set the attribute for the in object
      ACE_TRY_CHECK;

      this->inout_ = Coffee::_nil ();
      this->out_ = Coffee::_nil ();
      this->ret_ = Coffee::_nil ();

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, msg_str);
    }
  ACE_ENDTRY;
  return -1;
}

int
Test_ObjRef::reset_parameters (void)
{
  // Environemnt variable
  ACE_DECLARE_NEW_CORBA_ENV;
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some index into Coffee_Flavor [].
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];

  ACE_TRY
    {
      // set the attribute of the object
      this->in_->description (desc ACE_ENV_ARG_PARAMETER); // set the attribute for the in object
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "set coffee attribute");
      return -1;
    }
  ACE_ENDTRY;

  this->inout_ = Coffee::_nil ();
  this->out_ = Coffee::_nil ();
  this->ret_ = Coffee::_nil ();

  return 0;
}

int
Test_ObjRef::run_sii_test (Param_Test_ptr objref
                           ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      Coffee_out out (this->out_.out ());

      this->ret_ = objref->test_objref (this->in_.in (),
                                        this->inout_.inout (),
                                        out
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ObjRef::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_ObjRef::check_validity (void)
{
  // Environemnt variable
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (CORBA::is_nil (this->in_.in ())
          || CORBA::is_nil (this->inout_.in ())
          || CORBA::is_nil (this->out_.in ())
          || CORBA::is_nil (this->ret_.in ()))
        return 0;
      Coffee::Desc_var in_desc =
        this->in_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *in = in_desc->name.in ();

      Coffee::Desc_var inout_desc =
        this->inout_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *inout = inout_desc->name.in ();

      Coffee::Desc_var out_desc =
        this->out_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *out = out_desc->name.in ();

      Coffee::Desc_var ret_desc = this->out_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char* ret = ret_desc->name.in ();

      // now compare them
      if (!ACE_OS::strcmp (in, inout) &&
          !ACE_OS::strcmp (in, out) &&
          !ACE_OS::strcmp (in, ret))
        return 1; // success
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Retriving description");
      return 0;
    }
  ACE_ENDTRY;

  return 1;
}

CORBA::Boolean
Test_ObjRef::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_ObjRef::print_values (void)
{
  // Env. variable
  ACE_DECLARE_NEW_CORBA_ENV;

  Coffee::Desc_var ret_desc;
  const char *in = 0;
  const char *out = 0;
  const char *inout = 0;
  ACE_TRY
    {
      Coffee::Desc_var in_desc =
        this->in_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      in = in_desc->name.in ();

      Coffee::Desc_var inout_desc =
        this->inout_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      inout = inout_desc->name.in ();

      Coffee::Desc_var out_desc =
        this->out_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      out = out_desc->name.in ();

      ret_desc = this->out_->description (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Retrieving Description");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  const char* ret = ret_desc->name.in ();


  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*"
              "in = %s, "
              "inout = %s, "
              "out = %s, "
              "ret = %s*=*=*=*=*=\n",
              in,
              inout,
              out,
              ret));
}
