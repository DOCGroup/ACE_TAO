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

ACE_RCSID(Param_Test, any, "$Id$")

// ************************************************************************
//               Test_Any
// ************************************************************************

size_t Test_Any::counter = 0;

Test_Any::Test_Any (void)
  : opname_ (CORBA::string_dup ("test_any")),
    out_ (new CORBA::Any),
    ret_ (new CORBA::Any)
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
  TAO_TRY
    {
      // get access to a Coffee Object
      this->cobj_ = objref->make_coffee (TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception doing make_coffee");
      return -1;
    }
  TAO_ENDTRY;

  this->reset_parameters ();
  return 0;
}

int
Test_Any::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (counter++ % Test_Any::ANY_LAST_TEST_ITEM);

  switch (index)
    {
    case Test_Any::ANY_SHORT:
      {
        CORBA::Short s;
        s = gen->gen_short ();
	if (TAO_debug_level > 0)
	  ACE_DEBUG ((LM_DEBUG, "setting short = %d\n", s));
        this->in_ <<= s;
        this->inout_ <<= s;
      }
      break;
    case Test_Any::ANY_STRING:
      {
        char *str = gen->gen_string ();
	if (TAO_debug_level > 0)
	  ACE_DEBUG ((LM_DEBUG, "setting string = %s\n", str));
        this->in_ <<= str;
        this->inout_ <<= str;
      }
      break;
    case Test_Any::ANY_OBJREF:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG, "Testing any with Coffee object.\n"));

        // insert the coffee object into the Any
        this->in_ <<= this->cobj_.in ();
        this->inout_ <<= this->cobj_.in ();
      }
      break;
    case Test_Any::ANY_ARRAY:
      // test array
      {
        Param_Test::Fixed_Array array;
        for (size_t i = 0; i < Param_Test::DIM1; i++)
          array[i] = i;
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG, "Setting Fixed_Array\n"));
        this->in_ <<= Param_Test::Fixed_Array_forany (array);
        this->inout_ <<= Param_Test::Fixed_Array_forany (array);
      }
      break;
    case Test_Any::ANY_BD_SHORT_SEQ:
      {
        Param_Test::Bounded_Short_Seq seq;
        seq.length (gen->gen_short () % seq.maximum ());
        for (size_t i = 0; i < seq.length (); i++)
          seq[i] = gen->gen_short ();
        this->in_ <<= seq;
        this->inout_ <<= seq;
      }
      break;
    case Test_Any::ANY_STRUCT:
      {
        Param_Test::Fixed_Struct structure;
        structure = gen->gen_fixed_struct ();
        this->in_ <<= structure;
        this->inout_ <<= structure;
      }
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
  CORBA::Any in_arg (CORBA::_tc_any,
                     &this->in_,
                     0);

  CORBA::Any inout_arg (CORBA::_tc_any,
                        &this->inout_,
                        0);

  CORBA::Any out_arg (CORBA::_tc_any,
                      &this->out_.inout (), // .out () causes crash
                      0);

  // add parameters
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

  // add return value
  retval->item (0, env)->value ()->replace (CORBA::_tc_any,
                                            &this->ret_.inout (), // see above
                                            0, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Any::check_validity (void)
{
  CORBA::Short short_in, short_inout, short_out, short_ret;
  char *str_in, *str_inout, *str_out, *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;
  Param_Test::Fixed_Array_forany array_in, array_inout, array_out, array_ret;
  Param_Test::Bounded_Short_Seq *bdss_in, *bdss_inout, *bdss_out, *bdss_ret;
  Param_Test::Fixed_Struct *fs_in, *fs_inout, *fs_out, *fs_ret;

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
  else if ((this->in_ >>= array_in) &&
           (this->inout_ >>= array_inout) &&
           (this->out_.in () >>= array_out) &&
           (this->ret_.in () >>= array_ret))
    {
      for (size_t i = 0; i < Param_Test::DIM1; i ++)
        {
          size_t square = i * i;
          if (array_in[i] != i ||
              array_out[i] != i ||
              array_inout[i] != square ||
              array_ret[i] != square)
            return 0;
        }
      return 1;
    }
  else if ((this->in_ >>= bdss_in) &&
           (this->inout_ >>= bdss_inout) &&
           (this->out_.in () >>= bdss_out) &&
           (this->ret_.in () >>= bdss_ret))
    {
      for (size_t i = 0; i < bdss_in->length (); i ++)
        {
          size_t square = i * i;
          if ((*bdss_in)[i] != (*bdss_out)[i] ||
              (*bdss_inout)[i] != square ||
              (*bdss_inout)[i] != (*bdss_ret)[i])
            return 0;
        }
      return 1;
    }
  else if ((this->in_ >>= fs_in) &&
           (this->inout_ >>= fs_inout) &&
           (this->out_.in () >>= fs_out) &&
           (this->ret_.in () >>= fs_ret))
    {
      // @@ Added test for data validity here.
      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
Test_Any::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Any::print_values (void)
{
}
