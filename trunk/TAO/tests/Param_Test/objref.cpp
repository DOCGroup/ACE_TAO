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
  : opname_ (CORBA::string_dup ("test_objref")),
    in_courier (new CORBA::Object_ptr),
    inout_courier (new CORBA::Object_ptr),
    out_courier (new CORBA::Object_ptr),
    ret_courier (new CORBA::Object_ptr)
{
}

Test_ObjRef::~Test_ObjRef (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;

  delete this->in_courier;
  delete this->inout_courier;
  delete this->out_courier;
  delete this->ret_courier;
}

const char *
Test_ObjRef::opname (void) const
{
  return this->opname_;
}

void
Test_ObjRef::dii_req_invoke (CORBA::Request *req,
                             CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
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
Test_ObjRef::init_parameters (Param_Test_ptr objref,
                              CORBA::Environment &env)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // first get a Coffee object
  this->in_ = objref->make_coffee (env);
  if (env.exception ())
    {
      env.print_exception ("make_coffee");
      return -1;
    }

  // Get some index into Coffee_Flavor [].
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];
  // set the attribute of the object
  this->in_->description (desc, env); // set the attribute for the in object

  if (env.exception ())
    {
      env.print_exception ("set coffee attribute");
      return -1;
    }

  this->inout_ = Coffee::_nil ();
  this->out_ = Coffee::_nil ();
  this->ret_ = Coffee::_nil ();

  // DII
  *this->in_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                        this->in_.in ());
  *this->inout_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                           this->inout_.in ());
  *this->out_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                         this->out_.in ());
  *this->ret_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                         this->ret_.in ());

  return 0;
}

int
Test_ObjRef::reset_parameters (void)
{
  CORBA::Environment env;
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some index into Coffee_Flavor [].
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];
  // set the attribute of the object
  this->in_->description (desc, env); // set the attribute for the in object

  if (env.exception ())
    {
      env.print_exception ("set coffee attribute");
      return -1;
    }

  this->inout_ = Coffee::_nil ();
  this->out_ = Coffee::_nil ();
  this->ret_ = Coffee::_nil ();

  // DII
  *this->in_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                        this->in_.in ());
  *this->inout_courier = ACE_dynamic_cast (CORBA::Object_ptr,
                                           this->inout_.in ());

  return 0;
}

int
Test_ObjRef::run_sii_test (Param_Test_ptr objref,
                           CORBA::Environment &env)
{
  Coffee_out out (this->out_.out ());
  this->ret_ = objref->test_objref (this->in_.in (),
                                    this->inout_.inout (),
                                    out,
                                    env);
  return (env.exception () ? -1:0);
}

int
Test_ObjRef::add_args (CORBA::NVList_ptr param_list,
		       CORBA::NVList_ptr retval,
		       CORBA::Environment &env)
{
  CORBA::Any in_arg (_tc_Coffee,
                     this->in_courier,
                     0);

  CORBA::Any inout_arg (_tc_Coffee,
                        this->inout_courier,
                        0);

  CORBA::Any out_arg (_tc_Coffee,
                      this->out_courier,
                      0);

  // Add parameters.
  param_list->add_value ("o1",
                         in_arg,
                         CORBA::ARG_IN,
                         env);

  param_list->add_value ("o2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         env);

  param_list->add_value ("o3",
                         out_arg,
                         CORBA::ARG_OUT,
                         env);

  // Add return value.
  retval->item (0, env)->value ()->replace (_tc_Coffee,
                                            this->ret_courier,
                                            0, // does not own
                                            env);

  return 0;
}

CORBA::Boolean
Test_ObjRef::check_validity (void)
{
  CORBA::Environment env;

  Coffee::Desc_var in_desc =
    this->in_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }
  const char* in = in_desc->name.in ();

  Coffee::Desc_var inout_desc =
    this->inout_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }
  const char* inout = inout_desc->name.in ();

  Coffee::Desc_var out_desc =
    this->out_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }
  const char* out = out_desc->name.in ();

  Coffee::Desc_var ret_desc =
    this->out_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }
  const char* ret = ret_desc->name.in ();

  // now compare them
  if (!ACE_OS::strcmp (in, inout) &&
      !ACE_OS::strcmp (in, out) &&
      !ACE_OS::strcmp (in, ret))
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_ObjRef::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
  CORBA::Environment env;

  // Narrow each checked variable into its _var before
  // calling check_validity().

  this->inout_ = Coffee::_narrow (*this->inout_courier, env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  this->out_ = Coffee::_narrow (*this->out_courier, env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  this->ret_ = Coffee::_narrow (*this->ret_courier, env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  return this->check_validity ();
}

void
Test_ObjRef::print_values (void)
{
  CORBA::Environment env;

  Coffee::Desc_var in_desc =
    this->in_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }
  const char* in = in_desc->name.in ();

  Coffee::Desc_var inout_desc =
    this->inout_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }
  const char* inout = inout_desc->name.in ();

  Coffee::Desc_var out_desc =
    this->out_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }
  const char* out = out_desc->name.in ();

  Coffee::Desc_var ret_desc =
    this->out_->description (env);
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }
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
