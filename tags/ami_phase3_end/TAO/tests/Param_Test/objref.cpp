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
                              CORBA::Environment &ACE_TRY_ENV)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // New environment variable
  char msg_str[256] = "";

  ACE_TRY
    {
      ACE_OS::strcpy (msg_str, "make_cofee");

      // first get a Coffee object
      this->in_ = objref->make_coffee (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get some index into Coffee_Flavor [].
      CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
      desc.name = Coffee_Flavor [index];

      // set the attribute of the object
      ACE_OS::strcpy (msg_str, "set coffee attribute");
      this->in_->description (desc, ACE_TRY_ENV); // set the attribute for the in object
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, msg_str);
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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
  // Environemnt variable
  ACE_DECLARE_NEW_CORBA_ENV;
  //CORBA::Environment env;
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some index into Coffee_Flavor [].
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];

  ACE_TRY
    {
      // set the attribute of the object
      this->in_->description (desc, ACE_TRY_ENV); // set the attribute for the in object
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "set coffee attribute");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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
                           CORBA::Environment &ACE_TRY_ENV)
{
  Coffee_out out (this->out_.out ());
  this->ret_ = objref->test_objref (this->in_.in (),
                                    this->inout_.inout (),
                                    out,
                                    ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_ObjRef::add_args (CORBA::NVList_ptr param_list,
		       CORBA::NVList_ptr retval,
		       CORBA::Environment &ACE_TRY_ENV)
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
                         ACE_TRY_ENV);

  param_list->add_value ("o2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         ACE_TRY_ENV);

  param_list->add_value ("o3",
                         out_arg,
                         CORBA::ARG_OUT,
                         ACE_TRY_ENV);

  // Add return value.
  retval->item (0, ACE_TRY_ENV)->value ()->replace (_tc_Coffee,
                                            this->ret_courier,
                                            0, // does not own
                                            ACE_TRY_ENV);

  return 0;
}

CORBA::Boolean
Test_ObjRef::check_validity (void)
{
  // Environemnt variable
  ACE_DECLARE_NEW_CORBA_ENV;

  //CORBA::Environment env;

  Coffee::Desc_var ret_desc;
  const char *in = 0;
  const char *out = 0;
  const char *inout = 0;

  ACE_TRY
    {
      Coffee::Desc_var in_desc =
        this->in_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      in = in_desc->name.in ();

      Coffee::Desc_var inout_desc =
        this->inout_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      inout = inout_desc->name.in ();

      Coffee::Desc_var out_desc =
        this->out_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      out = out_desc->name.in ();

      ret_desc = this->out_->description (ACE_TRY_ENV);
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
Test_ObjRef::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  //CORBA::Environment env;
  ACE_DECLARE_NEW_CORBA_ENV;

  // Narrow each checked variable into its _var before
  // calling check_validity().

  ACE_TRY
    {
      this->inout_ = Coffee::_narrow (*this->inout_courier, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->out_ = Coffee::_narrow (*this->out_courier, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->ret_ = Coffee::_narrow (*this->ret_courier, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "_narrow from DII result");
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return this->check_validity ();
}

void
Test_ObjRef::print_values (void)
{
  //  CORBA::Environment env;

  // Env. variable
  ACE_DECLARE_NEW_CORBA_ENV;

  Coffee::Desc_var ret_desc;
  const char *in = 0;
  const char *out = 0;
  const char *inout = 0;
  ACE_TRY
    {
      Coffee::Desc_var in_desc =
        this->in_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      in = in_desc->name.in ();

      Coffee::Desc_var inout_desc =
        this->inout_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      inout = inout_desc->name.in ();

      Coffee::Desc_var out_desc =
        this->out_->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      out = out_desc->name.in ();

      ret_desc = this->out_->description (ACE_TRY_ENV);
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
