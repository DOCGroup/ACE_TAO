// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    small_union.cpp
//
// = DESCRIPTION
//    tests Small_Unions
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "small_union.h"

ACE_RCSID(Param_Test, small_union, "$Id$")

// ************************************************************************
//               Test_Small_Union
// ************************************************************************

size_t Test_Small_Union::counter = 0;

Test_Small_Union::Test_Small_Union (void)
  : opname_ (CORBA::string_dup ("test_small_union"))
{
}

Test_Small_Union::~Test_Small_Union (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Small_Union::opname (void) const
{
  return this->opname_;
}

void
Test_Small_Union::dii_req_invoke (CORBA::Request *req
                                ACE_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_Small_Union);

  req->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Param_Test::Small_Union *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Small_Union (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = *tmp;

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::Small_Union (*tmp);
}

int
Test_Small_Union::init_parameters (Param_Test_ptr objref
                                 ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // get access to a Coffee Object
      this->cobj_ = objref->make_coffee (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->reset_parameters ();
      return 0;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,"System Exception doing make_coffee");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "An exception caught in make_coffee");
    }
  ACE_ENDTRY;
  return -1;
}

int
Test_Small_Union::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (counter++ % 2);

  switch (index)
    {
    case 0:
      {
        CORBA::Long l = gen->gen_long ();
        this->in_.the_long (l);
        this->inout_.the_long (l);
      }
      break;
    default:
    case 1:
      {
        CORBA::Short s = gen->gen_short ();
        this->in_.the_short (s);
        this->inout_.the_short (s);
      }
      break;
    }
  this->out_ = new Param_Test::Small_Union (this->in_);
  this->ret_ = new Param_Test::Small_Union (this->in_);
  return 0;
}

int
Test_Small_Union::run_sii_test (Param_Test_ptr objref
                                ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->ret_ = objref->test_small_union (this->in_,
                                           this->inout_,
                                           this->out_
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Small_Union::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_Small_Union::check_validity (void)
{
  if (this->in_._d () != this->inout_._d ()
      || this->in_._d () != this->out_->_d ()
      || this->in_._d () != this->ret_->_d ())
    {
      ACE_DEBUG ((LM_DEBUG, "mismatch of discriminators\n"));
      return 0;
    }


  switch (this->in_._d ())
    {
    case Param_Test::A_LONG:
      {
        CORBA::Long in    = this->in_.the_long ();
        CORBA::Long inout = this->inout_.the_long ();
        CORBA::Long out   = this->out_->the_long ();
        CORBA::Long ret   = this->ret_->the_long ();
        if (in != out || in != inout || in != ret)
          return 0;
      }
      break;

    case Param_Test::A_SHORT:
      {
        CORBA::Short in    = this->in_.the_short ();
        CORBA::Short inout = this->inout_.the_short ();
        CORBA::Short out   = this->out_->the_short ();
        CORBA::Short ret   = this->ret_->the_short ();
        if (in != out || in != inout || in != ret)
          return 0;
      }
      break;

    default:
      return 0;
    }

  return 1;
}

CORBA::Boolean
Test_Small_Union::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Small_Union::print_values (void)
{
}
