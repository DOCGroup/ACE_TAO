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

int
Test_Objref_Struct::init_parameters (Param_Test_ptr objref,
				     CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // set the length of the sequence
  this->in_.x = gen->gen_long ();
  this->in_.y = objref->make_coffee (env);
  if (env.exception () != 0) return -1;
  Coffee::Desc d;
  d.name = gen->gen_string ();
  this->in_.y->description (d, env);
  if (env.exception () != 0) return -1;

  return 0;
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
                               CORBA::Environment &env)
{
  Param_Test::Objref_Struct_out out (this->out_.out ());
  this->ret_ = objref->test_objref_struct (this->in_,
                                        this->inout_.inout (),
                                        out,
                                        env);
  return (env.exception () ? -1:0);
}

int
Test_Objref_Struct::add_args (CORBA::NVList_ptr param_list,
			   CORBA::NVList_ptr retval,
			   CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Objref_Struct, 
                    &this->in_, 
                    CORBA::B_FALSE);

  CORBA::Any inout_arg (Param_Test::_tc_Objref_Struct, 
                        &this->inout_.inout (), // .out () causes crash 
                        CORBA::B_FALSE);

  CORBA::Any out_arg (Param_Test::_tc_Objref_Struct, 
                      &this->out_.inout (),
                      CORBA::B_FALSE);

  // add parameters
  param_list->add_value ("s1",
                         in_arg,
                         CORBA::ARG_IN,
                         env);

  param_list->add_value ("s2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         env);

  param_list->add_value ("s3",
                         out_arg,
                         CORBA::ARG_OUT,
                         env);

  // add return value
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Objref_Struct,
                                            &this->ret_.inout (), // see above
                                            CORBA::B_FALSE, // does not own
                                            env);
  return 0;
}


CORBA::Boolean
Test_Objref_Struct::check_validity (void)
{
  if (this->in_.x != this->inout_->x
      || this->in_.x != this->out_->x
      || this->in_.x != this->ret_->x)
    return CORBA::B_FALSE;

  CORBA::Environment env;
  if (CORBA::is_nil (this->in_.y.in ())
      || CORBA::is_nil (this->out_->y.in ())
      || CORBA::is_nil (this->ret_->y.in ())
      || CORBA::is_nil (this->inout_->y.in ()) )
    return CORBA::B_FALSE;

  Coffee::Desc_var s_in = this->in_.y->description (env);
  if (env.exception () != 0) return CORBA::B_FALSE;
  Coffee::Desc_var s_out = this->out_->y->description (env);
  if (env.exception () != 0) return CORBA::B_FALSE;
  Coffee::Desc_var s_inout = this->inout_->y->description (env);
  if (env.exception () != 0) return CORBA::B_FALSE;
  Coffee::Desc_var s_ret = this->ret_->y->description (env);
  if (env.exception () != 0) return CORBA::B_FALSE;

  if (ACE_OS::strcmp (s_in->name, s_out->name) != 0
      || ACE_OS::strcmp (s_in->name, s_inout->name) != 0
      || ACE_OS::strcmp (s_in->name, s_ret->name) != 0 )
    return CORBA::B_FALSE;

  return CORBA::B_TRUE;
}

CORBA::Boolean
Test_Objref_Struct::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
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

  CORBA::Environment env;

  Coffee::Desc_var s_in = this->in_.y->description (env);
  if (env.exception () != 0) return;
  Coffee::Desc_var s_out = this->out_->y->description (env);
  if (env.exception () != 0) return;
  Coffee::Desc_var s_inout = this->inout_->y->description (env);
  if (env.exception () != 0) return;
  Coffee::Desc_var s_ret = this->ret_->y->description (env);
  if (env.exception () != 0) return;

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

