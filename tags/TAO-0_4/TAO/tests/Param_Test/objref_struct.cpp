// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    objref_struct.cpp
//
// = DESCRIPTION
//    test structures containing object references.
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "objref_struct.h"

ACE_RCSID(Param_Test, objref_struct, "$Id$")

// ************************************************************************
//               Test_Objref_Struct
// ************************************************************************

Test_Objref_Struct::Test_Objref_Struct (void)
  : opname_ (CORBA::string_dup ("test_objref_struct")),
    inout_ (new Param_Test::Objref_Struct),
    out_ (new Param_Test::Objref_Struct),
    ret_ (new Param_Test::Objref_Struct)
{
}

Test_Objref_Struct::~Test_Objref_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Objref_Struct::opname (void) const
{
  return this->opname_;
}

void
Test_Objref_Struct::dii_req_invoke (CORBA::Request *req,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Objref_Struct::init_parameters (Param_Test_ptr objref,
				                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Generator *gen = GENERATOR::instance (); // value generator

      // set the length of the sequence
      this->in_.x = gen->gen_long ();

      this->in_.y = objref->make_coffee (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc d;
      d.name = gen->gen_string ();

      this->in_.y->description (d, 
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Objref_Struct::init_parameters\n");

      return -1;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return -1;)
}

int
Test_Objref_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Objref_Struct; // delete the previous ones
  this->out_ = new Param_Test::Objref_Struct;
  this->ret_ = new Param_Test::Objref_Struct;
  return 0;
}

int
Test_Objref_Struct::run_sii_test (Param_Test_ptr objref,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Param_Test::Objref_Struct_out out (this->out_.out ());

      this->ret_ = objref->test_objref_struct (this->in_,
                                               this->inout_.inout (),
                                               out,
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Objref_Struct::run_sii_test\n");

      return -1;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return -1;)
}

int
Test_Objref_Struct::add_args (CORBA::NVList_ptr param_list,
                              CORBA::NVList_ptr retval,
                              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Any in_arg (Param_Test::_tc_Objref_Struct,
                         &this->in_,
                         0);

      CORBA::Any inout_arg (Param_Test::_tc_Objref_Struct,
                            &this->inout_.inout (), // .out () causes crash
                            0);

      CORBA::Any out_arg (Param_Test::_tc_Objref_Struct,
                          &this->out_.inout (),
                          0);

      // add parameters
      param_list->add_value ("s1",
                             in_arg,
                             CORBA::ARG_IN,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s2",
                             inout_arg,
                             CORBA::ARG_INOUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      param_list->add_value ("s3",
                             out_arg,
                             CORBA::ARG_OUT,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // add return value
      CORBA::NamedValue *item = retval->item (0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      item->value ()->replace (Param_Test::_tc_Objref_Struct,
                               &this->ret_.inout (), // see above
                               0, // does not own
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Objref_Struct::add_args\n");

      return -1;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return 0;)
}


CORBA::Boolean
Test_Objref_Struct::check_validity (void)
{
  if (this->in_.x != this->inout_->x
      || this->in_.x != this->out_->x
      || this->in_.x != this->ret_->x)
    return 0;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    { 
      if (CORBA::is_nil (this->in_.y.in ())
          || CORBA::is_nil (this->out_->y.in ())
          || CORBA::is_nil (this->ret_->y.in ())
          | CORBA::is_nil (this->inout_->y.in ()) )
        {
          return 0;
        }
 
      Coffee::Desc_var s_in = this->in_.y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_out = this->out_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_inout = this->inout_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_ret = this->ret_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (s_in->name, s_out->name) != 0
          || ACE_OS::strcmp (s_in->name, s_inout->name) != 0
          || ACE_OS::strcmp (s_in->name, s_ret->name) != 0 )
        {
          return 0;
        }

      return 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Objref_Struct::check_validity\n");

      return 0;
    }
  ACE_ENDTRY;
  ACE_NOTREACHED (return 0;)
}

CORBA::Boolean
Test_Objref_Struct::check_validity (CORBA::Request_ptr /*req*/)
{
  return this->check_validity ();
}

void
Test_Objref_Struct::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n*=*=*=*=*=*=*=*=*=*=\n"
              "in_.x = %d\n"
              "inout_.x = %d\n"
              "out_.x = %d\n"
              "ret_.x = %d\n",
              this->in_.x,
              this->inout_->x,
              this->out_->x,
              this->ret_->x ));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {

      Coffee::Desc_var s_in = this->in_.y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_out = this->out_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_inout = this->inout_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Coffee::Desc_var s_ret = this->ret_->y->description (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n*=*=*=*=*=*=*=*=*=*=\n"
                  "in_ name = %s\n"
                  "inout_ name = %s\n"
                  "out_ name = %s\n"
                  "ret_ name = %s\n",
                  s_in->name.in (),
                  s_inout->name.in (),
                  s_out->name.in (),
                  s_ret->name.in () ));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Objref_Struct::print_values\n");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}
