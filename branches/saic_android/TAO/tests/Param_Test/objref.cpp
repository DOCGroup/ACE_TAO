
//=============================================================================
/**
 *  @file    objref.cpp
 *
 *  $Id$
 *
 *  tests object references
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "objref.h"

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
Test_ObjRef::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (_tc_Coffee);

  req->invoke ();

  Coffee_ptr tmp;
  req->return_value () >>= tmp;
  this->ret_ = Coffee::_duplicate (tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = Coffee::_duplicate (tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
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
Test_ObjRef::init_parameters (Param_Test_ptr objref)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  char msg_str[256] = "";

  try
    {
      ACE_OS::strcpy (msg_str, "make_cofee");

      // first get a Coffee object
      this->in_ = objref->make_coffee ();

      // Get some index into Coffee_Flavor [].
      CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
      desc.name = Coffee_Flavor [index];

      // set the attribute of the object
      ACE_OS::strcpy (msg_str, "set coffee attribute");
      this->in_->description (desc); // set the attribute for the in object

      this->inout_ = Coffee::_nil ();
      this->out_ = Coffee::_nil ();
      this->ret_ = Coffee::_nil ();

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (msg_str);
    }
  return -1;
}

int
Test_ObjRef::reset_parameters (void)
{
  // Environemnt variable
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some index into Coffee_Flavor [].
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];

  try
    {
      // set the attribute of the object
      this->in_->description (desc); // set the attribute for the in object
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("set coffee attribute");
      return -1;
    }

  this->inout_ = Coffee::_nil ();
  this->out_ = Coffee::_nil ();
  this->ret_ = Coffee::_nil ();

  return 0;
}

int
Test_ObjRef::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ =
        objref->test_objref (this->in_.in (),
                             this->inout_.inout (),
                             this->out_.out ());

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_ObjRef::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_ObjRef::check_validity (void)
{
  // Environemnt variable

  try
    {
      if (CORBA::is_nil (this->in_.in ())
          || CORBA::is_nil (this->inout_.in ())
          || CORBA::is_nil (this->out_.in ())
          || CORBA::is_nil (this->ret_.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil object references returned\n"));
          return 0;
        }
      Coffee::Desc_var in_desc =
        this->in_->description ();

      const char *in = in_desc->name.in ();

      Coffee::Desc_var inout_desc =
        this->inout_->description ();

      const char *inout = inout_desc->name.in ();

      Coffee::Desc_var out_desc =
        this->out_->description ();

      const char *out = out_desc->name.in ();

      Coffee::Desc_var ret_desc = this->out_->description ();

      const char* ret = ret_desc->name.in ();

      // now compare them
      if (!ACE_OS::strcmp (in, inout) &&
          !ACE_OS::strcmp (in, out) &&
          !ACE_OS::strcmp (in, ret))
        return 1; // success
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Retrieving description");
      return 0;
    }

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

  Coffee::Desc_var in_desc;
  Coffee::Desc_var inout_desc;
  Coffee::Desc_var out_desc;
  Coffee::Desc_var ret_desc;
  const char *in = 0;
  const char *out = 0;
  const char *inout = 0;
  const char *ret = 0;
  try
    {
      if (!CORBA::is_nil (this->in_.in ()))
        {
          in_desc =
            this->in_->description ();
          in = in_desc->name.in ();
        }

      if (!CORBA::is_nil (this->inout_.in ()))
        {
          inout_desc =
            this->inout_->description ();
          inout = inout_desc->name.in ();
        }

      if (!CORBA::is_nil (this->out_.in ()))
        {
          out_desc =
            this->out_->description ();
          out = out_desc->name.in ();
        }

      if (!CORBA::is_nil (this->ret_.in ()))
        {
          ret_desc =
            this->ret_->description ();
          ret = ret_desc->name.in ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Retrieving Description");
      return;
    }



  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*"
              "in = %s, "
              "inout = %s, "
              "out = %s, "
              "ret = %s*=*=*=*=*=\n",
              in?in:"ERROR(null string)",
              inout?inout:"ERROR(null string)",
              out?out:"ERROR(null string)",
              ret?ret:"ERROR(null string)"
              ));
}
