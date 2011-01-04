// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    big_union.cpp
//
// = DESCRIPTION
//    tests Big_Unions
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "big_union.h"

// ************************************************************************
//               Test_Big_Union
// ************************************************************************

size_t Test_Big_Union::counter = 0;

Test_Big_Union::Test_Big_Union (void)
  : opname_ (CORBA::string_dup ("test_big_union"))
{
}

Test_Big_Union::~Test_Big_Union (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Big_Union::opname (void) const
{
  return this->opname_;
}

void
Test_Big_Union::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_Big_Union);

  req->invoke ();

  Param_Test::Big_Union *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Big_Union (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = *tmp;

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::Big_Union (*tmp);
}

int
Test_Big_Union::init_parameters (Param_Test_ptr objref)
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "An exception caught in make_coffee");
    }
  return -1;
}

int
Test_Big_Union::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (counter++ % Test_Big_Union::BIG_UNION_N_BRANCHES);

  switch (index)
    {
    case 0:
      {
        Param_Test::Fixed_Array x;
        for (CORBA::ULong i = 0; i < Param_Test::DIM1; ++i)
          {
            x[i] = gen->gen_long ();
          }
        this->in_.the_array (x);
        this->inout_.the_array (x);
      }
      break;
    case 1:
      {
        this->in_.the_interface (this->cobj_.in ());
        this->inout_.the_interface (this->cobj_.in ());
      }
      break;
    case 2:
      {
        CORBA::Long x = gen->gen_long ();
        this->in_.the_long (x);
        this->inout_.the_long (x);
      }
      break;
    case 3:
      {
        Param_Test::short_array x;

        for (int i = 0; i < 32; ++i)
          {
            x[i] = gen->gen_short ();
          }
        this->in_.another_array (x);
        this->inout_.another_array (x);
      }
      break;
    case 4:
      {
        CORBA::String_var str = gen->gen_string ();
        this->in_.the_string (str);
        this->inout_.the_string (str);
      }
      break;
    case 5:
      {
        CORBA::ShortSeq seq;
        seq.length (gen->gen_short () % 50);
        for (size_t i = 0; i < seq.length (); i++)
          seq[i] = gen->gen_short ();
        this->in_.the_sequence (seq);
        this->inout_.the_sequence (seq);
      }
      break;
    case 6:
      {
        CORBA::Any any;
        any <<= CORBA::Short (25);
        this->in_.the_any (any);
        this->inout_.the_any (any);
      }
      break;
    case 7:
      {
        CORBA::Octet octet = gen->gen_short () % 255;
        this->in_.the_octet (octet);
        this->inout_.the_octet (octet);
      }
      break;
    case 8:
      {
        CORBA::Char x = '@';
        this->in_.the_char (x);
        this->inout_.the_char (x);
      }
      break;
    case 9:
      {
        CORBA::Boolean x = gen->gen_short () % 2;
        this->in_.the_boolean (x);
        this->inout_.the_boolean (x);
      }
      break;
    case 10:
      {
        Param_Test::Var_Struct var_struct;
        var_struct.dbl = 3.14159;
        var_struct.boole = gen->gen_short () % 2;
        var_struct.shrt = gen->gen_short ();
        // set the length of the sequence
        var_struct.dummy1 = gen->gen_string ();
        var_struct.dummy2 = gen->gen_string ();
        CORBA::ULong len = gen->gen_long () % 10 + 1;
        var_struct.seq.length (len);
        for (CORBA::ULong i = 0; i != len; ++i)
          {
            var_struct.seq[i] = gen->gen_string ();
          }
        this->in_.the_var_struct (var_struct);
        this->inout_.the_var_struct (var_struct);
      }
      break;
    case 11:
      {
        Param_Test::Fixed_Struct fixed_struct;
        fixed_struct.l = gen->gen_long ();
        fixed_struct.c = gen->gen_long () % 255;
        fixed_struct.s = gen->gen_long () % 32768;
        fixed_struct.o = gen->gen_long () % 255;
        fixed_struct.f = gen->gen_short () / 255.0f;
        fixed_struct.b = gen->gen_long () % 2;
        fixed_struct.d = gen->gen_short () / 255.0;
        this->in_.the_fixed_struct (fixed_struct);
        this->inout_.the_fixed_struct (fixed_struct);
      }
      break;
    }
  this->out_ = new Param_Test::Big_Union (this->in_);
  this->ret_ = new Param_Test::Big_Union (this->in_);
  return 0;
}

int
Test_Big_Union::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_big_union (this->in_,
                                           this->inout_,
                                           this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Big_Union::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Big_Union::check_validity (void)
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
    case 0:
      {
        Param_Test::Fixed_Array_slice* in_array    = this->in_.the_array ();
        Param_Test::Fixed_Array_slice* inout_array = this->inout_.the_array ();
        Param_Test::Fixed_Array_slice* out_array   = this->out_->the_array ();
        Param_Test::Fixed_Array_slice* ret_array   = this->ret_->the_array ();
        for (CORBA::ULong i = 0; i < Param_Test::DIM1; ++i)
          {
            if (in_array[i] != inout_array[i]
                || in_array[i] != out_array[i]
                || in_array[i] != ret_array[i])
              {
                ACE_DEBUG ((LM_DEBUG,
                            "mismatch of arrays\n"));
                return 0;
              }
          }
      }
      break;
    case 1:
      {
        try
          {
            Coffee_ptr in    = this->in_.the_interface ();
            Coffee_ptr inout = this->inout_.the_interface ();
            Coffee_ptr out   = this->out_->the_interface ();
            Coffee_ptr ret   = this->ret_->the_interface ();

            if (CORBA::is_nil (in)
                || CORBA::is_nil (inout)
                || CORBA::is_nil (out)
                || CORBA::is_nil (ret))
              return 0;

            Coffee::Desc_var in_desc =
              in->description ();
            Coffee::Desc_var inout_desc =
              inout->description ();
            Coffee::Desc_var out_desc =
              out->description ();
            Coffee::Desc_var ret_desc =
              ret->description ();

            if (ACE_OS::strcmp (in_desc->name.in (),
                                inout_desc->name.in ())
                || ACE_OS::strcmp (in_desc->name.in (),
                                   out_desc->name.in ())
                || ACE_OS::strcmp (in_desc->name.in (),
                                   ret_desc->name.in ()))
              return 0;
          }
        catch (const CORBA::Exception&)
          {
            return 0;
          }
      }
      break;
    case 2:
      {
        CORBA::Long in    = this->in_.the_long ();
        CORBA::Long inout = this->inout_.the_long ();
        CORBA::Long out   = this->out_->the_long ();
        CORBA::Long ret   = this->ret_->the_long ();

        if (in != out || in != inout || in != ret)
          {
            return 0;
          }
      }
      break;
    case 3:
      {
        Param_Test::short_array_slice* in_array    =
          this->in_.another_array ();
        Param_Test::short_array_slice* inout_array =
          this->inout_.another_array ();
        Param_Test::short_array_slice* out_array   =
          this->out_->another_array ();
        Param_Test::short_array_slice* ret_array   =
          this->ret_->another_array ();

        for (int i = 0; i != 32; ++i)
          {
            if (in_array[i] != inout_array[i]
                || in_array[i] != out_array[i]
                || in_array[i] != ret_array[i])
              {
                return 0;
              }
          }
      }
      break;
    case 4:
      {
        const char* in    = this->in_.the_string ();
        const char* inout = this->inout_.the_string ();
        const char* out   = this->out_->the_string ();
        const char* ret   = this->ret_->the_string ();

        if (ACE_OS::strcmp (in,out)
            || ACE_OS::strcmp (in,inout)
            || ACE_OS::strcmp (in,ret))
          {
            return 0;
          }
      }
      break;
    case 5:
      {
        const CORBA::ShortSeq& in =
          this->in_.the_sequence ();
        const CORBA::ShortSeq& inout =
          this->inout_.the_sequence ();
        const CORBA::ShortSeq& out =
          this->out_->the_sequence ();
        const CORBA::ShortSeq& ret =
          this->ret_->the_sequence ();

        if (in.length () != out.length ()
            || in.length () != inout.length ()
            || in.length () != ret.length ())
          return 0;

        for (CORBA::ULong i = 0; i != in.length (); ++i)
          {
            if (in[i] != out[i]
                || in[i] != inout[i]
                || in[i] != ret[i])
              {
                return 0;
              }
          }
      }
      break;
    case 6:
      {
        CORBA::Any in = this->in_.the_any ();
        CORBA::Any inout = this->inout_.the_any ();
        CORBA::Any out = this->out_->the_any ();
        CORBA::Any ret = this->ret_->the_any ();

        CORBA::Short in_short;
        CORBA::Short inout_short;
        CORBA::Short out_short;
        CORBA::Short ret_short;

        if (!(in >>= in_short)
            || !(inout >>= inout_short)
            || !(out >>= out_short)
            || !(ret >>= ret_short))
          {
            return 0;
          }

        if (in_short != inout_short
            || in_short != out_short
            || in_short != ret_short)
          {
            return 0;
          }
      }
      break;
    case 7:
      {
        CORBA::Octet in    = this->in_.the_octet ();
        CORBA::Octet inout = this->inout_.the_octet ();
        CORBA::Octet out   = this->out_->the_octet ();
        CORBA::Octet ret   = this->ret_->the_octet ();

        if (in != out || in != inout || in != ret)
          {
            return 0;
          }
      }
      break;
    case 8:
      {
        CORBA::Char in    = this->in_.the_char ();
        CORBA::Char inout = this->inout_.the_char ();
        CORBA::Char out   = this->out_->the_char ();
        CORBA::Char ret   = this->ret_->the_char ();

        if (in != out || in != inout || in != ret)
          {
            return 0;
          }
      }
      break;
    case 9:
      {
        CORBA::Boolean in    = this->in_.the_boolean ();
        CORBA::Boolean inout = this->inout_.the_boolean ();
        CORBA::Boolean out   = this->out_->the_boolean ();
        CORBA::Boolean ret   = this->ret_->the_boolean ();

        if (in != out || in != inout || in != ret)
          {
            return 0;
          }
      }
      break;
    case 10:
      {
        const Param_Test::Var_Struct& in =
          this->in_.the_var_struct ();
        const Param_Test::Var_Struct& inout =
          this->inout_.the_var_struct ();
        const Param_Test::Var_Struct& out =
          this->out_->the_var_struct ();
        const Param_Test::Var_Struct& ret =
          this->ret_->the_var_struct ();

        if (!(ACE_OS::strcmp (in.dummy1.in (),
                              inout.dummy1.in ()) == 0
              && ACE_OS::strcmp (in.dummy2.in (),
                                 inout.dummy2.in ()) == 0)
            || !(ACE_OS::strcmp (in.dummy1.in (),
                                 out.dummy1.in ()) == 0
               && ACE_OS::strcmp (in.dummy2.in (),
                                  out.dummy2.in ()) == 0)
            || !(ACE_OS::strcmp (in.dummy1.in (),
                                 ret.dummy1.in ()) == 0
               && ACE_OS::strcmp (in.dummy2.in (),
                                  ret.dummy2.in ()) == 0))
          {
            return 0;
          }

        if (in.seq.length () != inout.seq.length ()
            || in.seq.length () != out.seq.length ()
            || in.seq.length () != ret.seq.length ())
          {
            return 0;
          }

        CORBA::ULong len = in.seq.length ();

        for (CORBA::ULong i = 0; i != len; ++i)
          {
            if (ACE_OS::strcmp (in.seq[i],
                                inout.seq[i])
                || ACE_OS::strcmp (in.seq[i],
                                   out.seq[i])
                || ACE_OS::strcmp (in.seq[i],
                                   ret.seq[i]))
              {
                return 0;
              }
          }
      }
      break;
    case 11:
      {
        const Param_Test::Fixed_Struct& in =
          this->in_.the_fixed_struct ();
        const Param_Test::Fixed_Struct& inout =
          this->inout_.the_fixed_struct ();
        const Param_Test::Fixed_Struct& out =
          this->out_->the_fixed_struct ();
        const Param_Test::Fixed_Struct& ret =
          this->ret_->the_fixed_struct ();

        if (! (in.l == inout.l
               && in.c == inout.c
               && in.s == inout.s
               && in.o == inout.o
               && in.f == inout.f
               && in.b == inout.b
               && in.d == inout.d)
            || !(in.l == out.l
                 && in.c == out.c
                 && in.s == out.s
                 && in.o == out.o
                 && in.f == out.f
                 && in.b == out.b
                 && in.d == out.d)
            || !(in.l == ret.l
                 && in.c == ret.c
                 && in.s == ret.s
                 && in.o == ret.o
                 && in.f == ret.f
                 && in.b == ret.b
                 && in.d == ret.d))
          {
            return 0;
          }
      }
      break;
    }

  return 1;
}

CORBA::Boolean
Test_Big_Union::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Big_Union::print_values (void)
{
}
