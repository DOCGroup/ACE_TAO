// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    any.cpp
//
// = DESCRIPTION
//    tests Anys
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "any.h"

// ************************************************************************
//               Test_Any
// ************************************************************************

Test_Any::Test_Any (void)
  : opname_ (CORBA::string_dup ("test_any"))
{
}

Test_Any::~Test_Any (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Any::opname (void) const
{
  return this->opname_;
}

#if 0 /* any_table isn't currently used */
static const CORBA::TypeCode_ptr any_table [] =
{
  // primitive parameterless typecodes
  CORBA::_tc_short,
  // typecode with a simple parameter
  CORBA::_tc_string,
  // complex typecodes
  _tc_Coffee,
  Param_Test::_tc_StrSeq,
  Param_Test::_tc_StructSeq,
  Param_Test::_tc_Nested_Struct
};
#endif /* any_table isn't currently used */

int
Test_Any::init_parameters (Param_Test_ptr objref,
                           CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  //  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 3);
  CORBA::ULong index = 2;

  switch (index)
    {
    case 0:
      {
        CORBA::Short s;
        s = gen->gen_short ();
        ACE_DEBUG ((LM_DEBUG, "setting short = %d\n", s));
        this->in_ <<= s;
        this->inout_ <<= s;
      }
      break;
    case 1:
      {
        char *str = gen->gen_string ();
        ACE_DEBUG ((LM_DEBUG, "setting string = %s\n", str));
        this->in_ <<= str;
        this->inout_ <<= str;
      }
      break;
    case 2:
      {
        TAO_TRY
          {
            // get access to a Coffee Object
            Coffee_var cobj = objref->make_coffee (TAO_TRY_ENV);
            TAO_CHECK_ENV;

            // insert the coffee object into the Any
            this->in_ <<= cobj.in ();
            this->inout_ <<= cobj.in ();
          }
        TAO_CATCH (CORBA::SystemException, sysex)
          {
            ACE_UNUSED_ARG (sysex);
            TAO_TRY_ENV.print_exception ("System Exception doing make_coffee");
            return -1;
          }
        TAO_ENDTRY;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    }
  return 0;
}

int
Test_Any::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  //  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 2);
  CORBA::ULong index = 2;

  switch (index)
    {
    case 0:
      {
        CORBA::Short s;
        s = gen->gen_short ();
        this->in_ <<= s;
        this->inout_ <<= s;
      }
      break;
    case 1:
      {
        char *str = gen->gen_string ();
        this->in_ <<= str;
        this->inout_ <<= str;
      }
      break;
    case 2:
      {
        this->inout_ = this->in_;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    }
  return 0;
}

int
Test_Any::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::Any_out out (this->out_.out ());
  this->ret_ = objref->test_any (this->in_,
                                 this->inout_,
                                 out,
                                 env);
  return (env.exception () ? -1:0);
}

int
Test_Any::add_args (CORBA::NVList_ptr param_list,
                    CORBA::NVList_ptr retval,
                    CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_any, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_any, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_any, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("o1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("o2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("o3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_any,
                                                  0,
                                                  CORBA::B_FALSE, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Any::check_validity (void)
{
  CORBA::Environment env;
  CORBA::Short short_in, short_inout, short_out, short_ret;
  char *str_in, *str_inout, *str_out, *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;

  if ((this->in_ >>= short_in) &&
      (this->inout_ >>= short_inout) &&
      (this->out_.in () >>= short_out) &&
      (this->ret_.in () >>= short_ret))
    {
      ACE_DEBUG ((LM_DEBUG, "Received shorts: in = %d, "
                  "inout = %d, out = %d, ret = %d\n",
                  short_in, short_inout, short_out, short_ret));

      if ((short_in == short_inout) &&
          (short_in == short_out) &&
          (short_in == short_ret))
        return 1;
      else
        {
          ACE_DEBUG ((LM_DEBUG, "mismatch of short values\n"));
          return 0;
        }
    }
  else if ((this->in_ >>= str_in) &&
           (this->inout_ >>= str_inout) &&
           (this->out_.in () >>= str_out) &&
           (this->ret_.in () >>= str_ret))
    {
      if (!ACE_OS::strcmp (str_in, str_inout) &&
          !ACE_OS::strcmp (str_in, str_out) &&
          !ACE_OS::strcmp (str_in, str_ret))
        return 1;
      else
        {
          ACE_DEBUG ((LM_DEBUG, "mismatch of string values\n"));
          return 0;
        }
    }
  else if ((this->in_ >>= obj_in) &&
           (this->inout_ >>= obj_inout) &&
           (this->out_.in () >>= obj_out) &&
           (this->ret_.in () >>= obj_ret)) 
    {
      // all the >>= operators returned true so we are OK.
      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
Test_Any::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;

  CORBA::Any_ptr out = this->out_;
  CORBA::Any_ptr ret = this->out_;
  *req->arguments ()->item (2, env)->value () >>= *out;
  *req->result ()->value () >>= *ret;

  return this->check_validity ();
}

void
Test_Any::print_values (void)
{
}

