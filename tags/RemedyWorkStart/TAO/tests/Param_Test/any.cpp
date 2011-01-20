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
#include "tao/debug.h"

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

void
Test_Any::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("o1") <<= this->in_;
  req->add_inout_arg ("o2") <<= this->inout_;
  req->add_out_arg ("o3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_any);

  req->invoke ();

  const CORBA::Any *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new CORBA::Any (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = CORBA::Any (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new CORBA::Any (*tmp);
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
Test_Any::init_parameters (Param_Test_ptr objref)
{
  try
    {
      // get access to a Coffee Object
      this->cobj_ = objref->make_coffee ();

      this->reset_parameters ();
      return 0;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception doing make_coffee");
    }
  return -1;
}

int
Test_Any::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (counter++ % Test_Any::ANY_LAST_TEST_ITEM);

  switch (index)
    {
    default:
    case Test_Any::ANY_SHORT:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_SHORT subtest\n"));
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
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_STRING subtest\n"));
        char *str = gen->gen_string ();

        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG, "setting string = %s\n", str));
        this->in_ <<= str;
        this->inout_ <<= str;
        CORBA::string_free (str);
      }
      break;

    case Test_Any::ANY_OBJREF:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_OBJREF subtest\n"));

        // insert the coffee object into the Any
        this->in_ <<= this->cobj_.in ();
        this->inout_ <<= this->cobj_.in ();
      }
      break;

    case Test_Any::ANY_ARRAY:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_ARRAY subtest\n"));

        Param_Test::Fixed_Array array;
        for (size_t i = 0; i < Param_Test::DIM1; i++)
          array[i] = i;
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG, "Setting Fixed_Array\n"));
        this->in_ <<= Param_Test::Fixed_Array_forany (array);
        this->inout_ <<= Param_Test::Fixed_Array_forany (array);
      }
      break;

    case Test_Any::ANY_SHORT_SEQ:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_SHORT_SEQ subtest\n"));
        CORBA::ShortSeq seq;
        seq.length (gen->gen_short () % 16);
        for (size_t i = 0; i < seq.length (); i++)
          seq[i] = gen->gen_short ();
        this->in_    <<= seq;
        this->inout_ <<= seq;
      }
      break;

    case Test_Any::ANY_BD_SHORT_SEQ:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_BD_SHORT_SEQ subtest\n"));
        Param_Test::Bounded_Short_Seq seq;
        seq.length (gen->gen_short () % seq.maximum ());
        for (size_t i = 0; i < seq.length (); i++)
          seq[i] = gen->gen_short ();
        this->in_    <<= seq;
        this->inout_ <<= seq;
      }
      break;

    case Test_Any::ANY_STRUCT:
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "Param_Test: ANY_STRUCT subtest\n"));
        Param_Test::Fixed_Struct structure;
        structure = gen->gen_fixed_struct ();
        this->in_ <<= structure;
        this->inout_ <<= structure;
      }
      break;

    case Test_Any::ANY_BIG_UNION:
      {
        CORBA::Long x = gen->gen_long ();
        Param_Test::Big_Union the_union;
        the_union.the_long (x);
        this->in_    <<= the_union;
        this->inout_ <<= the_union;

        if (TAO_debug_level > 0)
          {
            Param_Test::Big_Union *bu_in, *bu_inout;
            this->in_ >>= bu_in;
            this->inout_ >>= bu_inout;
            ACE_DEBUG ((LM_DEBUG,
                        "Param_Test: ANY_BIG_UNION subtest\n"
                        "  in %d\n"
                        "  inout %d\n",
                        bu_in->the_long (),
                        bu_inout->the_long ()));
          }
      }
      break;

    case Test_Any::ANY_SMALL_UNION:
      {
        CORBA::Long x = gen->gen_long ();
        Param_Test::Small_Union the_union;
        the_union.the_long (x);
        this->in_    <<= the_union;
        this->inout_ <<= the_union;

        if (TAO_debug_level > 0)
          {
            Param_Test::Small_Union *bu_in, *bu_inout;
            this->in_ >>= bu_in;
            this->inout_ >>= bu_inout;
            ACE_DEBUG ((LM_DEBUG,
                        "Param_Test: ANY_SMALL_UNION subtest\n"
                        "  in %d\n"
                        "  inout %d\n",
                        bu_in->the_long (),
                        bu_inout->the_long ()));
          }
      }
      break;
    }
  return 0;
}

int
Test_Any::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_any (this->in_,
                                     this->inout_,
                                     this->out_.out ());
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

CORBA::Boolean
Test_Any::check_validity (void)
{
  CORBA::Short short_in, short_inout, short_out, short_ret;
  const char *str_in;
  const char *str_inout;
  const char *str_out;
  const char *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;
  Param_Test::Fixed_Array_forany array_in, array_inout, array_out, array_ret;
  Param_Test::Bounded_Short_Seq *bdss_in, *bdss_inout, *bdss_out, *bdss_ret;
  CORBA::ShortSeq *ubss_in, *ubss_inout, *ubss_out, *ubss_ret;
  Param_Test::Fixed_Struct *fs_in, *fs_inout, *fs_out, *fs_ret;
  Param_Test::Big_Union *bu_in, *bu_inout, *bu_out, *bu_ret;
  Param_Test::Small_Union *su_in, *su_inout, *su_out, *su_ret;

  if ((this->in_ >>= short_in) &&
      (this->inout_ >>= short_inout) &&
      (this->out_.in () >>= short_out) &&
      (this->ret_.in () >>= short_ret))
    {
      // ACE_DEBUG ((LM_DEBUG, "Received shorts: in = %d, "
      //            "inout = %d, out = %d, ret = %d\n",
      //            short_in, short_inout, short_out, short_ret));

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
      for (CORBA::ULong i = 0; i < Param_Test::DIM1; i ++)
        {
          CORBA::Long ii = i;
          CORBA::Long square = i * i;
          if (array_in[i] != ii ||
              array_out[i] != ii ||
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
          ssize_t square = i * i;
          if ((*bdss_in)[i] != (*bdss_out)[i] ||
              (*bdss_inout)[i] != square ||
              (*bdss_inout)[i] != (*bdss_ret)[i])
            return 0;
        }
      return 1;
    }
  else if ((this->in_ >>= ubss_in) &&
           (this->inout_ >>= ubss_inout) &&
           (this->out_.in () >>= ubss_out) &&
           (this->ret_.in () >>= ubss_ret))
    {
      for (size_t i = 0; i < ubss_in->length (); i ++)
        {
          ssize_t square = i * i;
          if ((*ubss_in)[i] != (*ubss_out)[i] ||
              (*ubss_inout)[i] != square ||
              (*ubss_inout)[i] != (*ubss_ret)[i])
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
  else if ((this->in_ >>= bu_in) &&
           (this->inout_ >>= bu_inout) &&
           (this->out_.in () >>= bu_out) &&
           (this->ret_.in () >>= bu_ret))
    {
      if (bu_in->_d () != 2
          || bu_inout->_d () != 2
          || bu_out->_d () != 2
          || bu_ret->_d () != 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Any - not all unions contain a long\n"));
          return 0;
        }

      if (bu_in->the_long () != bu_inout->the_long ()
          || bu_in->the_long () != bu_out->the_long ()
          || bu_in->the_long () != bu_ret->the_long ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Any - values mismatch\n"
                      "  in %d\n"
                      "  inout %d\n"
                      "  out %d\n"
                      "  ret %d\n",
                      bu_in->the_long (),
                      bu_inout->the_long (),
                      bu_out->the_long (),
                      bu_ret->the_long () ));
          return 0;
        }
      return 1;
    }
  else if ((this->in_ >>= su_in) &&
           (this->inout_ >>= su_inout) &&
           (this->out_.in () >>= su_out) &&
           (this->ret_.in () >>= su_ret))
    {
      if (su_in->_d () != Param_Test::A_LONG
          || su_inout->_d () != Param_Test::A_LONG
          || su_out->_d () != Param_Test::A_LONG
          || su_ret->_d () != Param_Test::A_LONG)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Any - not all unions contain a long\n"));
          return 0;
        }

      if (su_in->the_long () != su_inout->the_long ()
          || su_in->the_long () != su_out->the_long ()
          || su_in->the_long () != su_ret->the_long ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Test_Any - values mismatch\n"
                      "  in %d\n"
                      "  inout %d\n"
                      "  out %d\n"
                      "  ret %d\n",
                      su_in->the_long (),
                      su_inout->the_long (),
                      su_out->the_long (),
                      su_ret->the_long () ));
          return 0;
        }
      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
Test_Any::check_validity (CORBA::Request_ptr /*req*/)
{
  return this->check_validity ();
}

void
Test_Any::print_values (void)
{
}
