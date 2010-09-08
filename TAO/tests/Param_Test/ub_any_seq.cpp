// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_any_seq.cpp
//
// = DESCRIPTION
//    tests unbounded Any sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ub_any_seq.h"
#include "tao/debug.h"

const CORBA::ULong TEST_SEQ_LENGTH = 5;
const CORBA::ULong NUM_TEST_TYPES = 4;

// ************************************************************************
//               Test_AnySeq
// ************************************************************************

Test_AnySeq::Test_AnySeq (void)
  : opname_ (CORBA::string_dup ("test_anyseq")),
    in_ (new CORBA::AnySeq (TEST_SEQ_LENGTH)),
    inout_ (new CORBA::AnySeq (TEST_SEQ_LENGTH)),
    out_ (new CORBA::AnySeq),
    ret_ (new CORBA::AnySeq)
{
}

Test_AnySeq::~Test_AnySeq (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_AnySeq::opname (void) const
{
  return this->opname_;
}

void
Test_AnySeq::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_AnySeq);

  req->invoke ();

  CORBA::AnySeq* tmp;
  req->return_value () >>= tmp;
  this->ret_ = new CORBA::AnySeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = new CORBA::AnySeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new CORBA::AnySeq (*tmp);
}

int
Test_AnySeq::init_parameters (Param_Test_ptr objref)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // Must be set explicitly (CORBA spec)
  this->in_->length (TEST_SEQ_LENGTH);
  this->inout_->length (TEST_SEQ_LENGTH);

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      CORBA::ULong index =
        (CORBA::ULong) (gen->gen_short () % NUM_TEST_TYPES);
      switch (index)
        {
        case 0:
          {
            CORBA::Short s;
            s = gen->gen_short ();
                  if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "setting short = %d\n", s));
            this->in_[i] <<= s;
            this->inout_[i] <<= 0; // different from in_
          }
          break;
        case 1:
          {
            CORBA::String_var str = gen->gen_string ();
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "setting string = %s\n", str.in ()));
            this->in_[i] <<= str.in ();
            this->inout_[i] <<= 0; // different from in_
          }
          break;
        case 2:
          {
                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                          "setting coffee object\n" ));
            try
              {
                // get access to a Coffee Object
                Coffee_var cobj = objref->make_coffee ();

                // insert the coffee object into the Any
                this->in_[i] <<= cobj.in ();
                this->inout_[i] <<= 0; // different from in_
              }
            catch (const CORBA::SystemException& sysex)
              {
                sysex._tao_print_exception (
                  "System Exception doing make_coffee");
                return -1;
              }
          }
          break;
        case 3:
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "setting constant string\n" ));
          this->in_[i] <<= "Const string";
          this->inout_[i] <<= 0; // different from in_
          break;
        }
    }

  return 0;
}

int
Test_AnySeq::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      CORBA::ULong index =
        (CORBA::ULong) (gen->gen_long () % NUM_TEST_TYPES);

      switch (index)
        {
        case 0:
          {
            CORBA::Short s;
            s = gen->gen_short ();
            this->in_[i] <<= s;
            this->inout_[i] <<= s;
          }
          break;
        case 1:
          {
            CORBA::String_var str = gen->gen_string ();
            this->in_[i] <<= str.in ();
            this->inout_[i] <<= str.in ();
          }
          break;
        case 2:
          {
            this->inout_[i] = this->in_[i];
          }
          break;
        case 3:
                this->in_[i] <<= "Const string";
                this->inout_[i] <<= "Const string";
          break;
        }
    }

  return 0;
}


int
Test_AnySeq::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::AnySeq_out out (this->out_.out ());

      this->ret_ = objref->test_anyseq (this->in_.in (),
                                        this->inout_.inout (),
                                        out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_AnySeq::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_AnySeq::check_validity (void)
{
  CORBA::Short short_in, short_inout, short_out, short_ret;
  const char *str_in;
  const char *str_inout;
  const char *str_out;
  const char *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      if ((this->in_[i] >>= short_in) &&
          (this->inout_[i] >>= short_inout) &&
          (this->out_[i] >>= short_out) &&
          (this->ret_[i] >>= short_ret))
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Received shorts: in = %d, "
                          "inout = %d, out = %d, ret = %d\n",
                          short_in,
                          short_inout,
                          short_out,
                          short_ret));
            }

          if ((short_in != short_inout) ||
              (short_in != short_out) ||
              (short_in != short_ret))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of short values\n"));
              return 0;
            }
          else
            continue;
        }
      else if ((this->in_[i] >>= str_in) &&
               (this->inout_[i] >>= str_inout) &&
               (this->out_[i] >>= str_out) &&
               (this->ret_[i] >>= str_ret))
        {
          if (ACE_OS::strcmp (str_in, str_inout) ||
              ACE_OS::strcmp (str_in, str_out) ||
              ACE_OS::strcmp (str_in, str_ret))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of string values\n"));
              return 0;
            }
          else
            continue;
        }
      else if ((this->in_[i] >>= obj_in) &&
               (this->inout_[i] >>= obj_inout) &&
               (this->out_[i] >>= obj_out) &&
               (this->ret_[i] >>= obj_ret))
        {
          if (!(obj_in->_is_equivalent (obj_inout)) ||
              !(obj_in->_is_equivalent (obj_out)) ||
              !(obj_in->_is_equivalent (obj_ret)))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of Coffee values\n"));
              return 0;
            }
        }
      else
        continue;
    }

  // Everything checks out.
  return 1;
}

CORBA::Boolean
Test_AnySeq::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_AnySeq::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "IN sequence\n"));
  this->print_sequence (this->in_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "INOUT sequence\n"));
  this->print_sequence (this->inout_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "OUT sequence\n"));
  this->print_sequence (this->out_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "RET sequence\n"));
  this->print_sequence (this->ret_.in ());
}

void
Test_AnySeq::print_sequence (const CORBA::AnySeq &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i=0; i < s.length (); i++)
    {
      /*     const CORBA::Any& vs = s[i];

             ACE_DEBUG ((LM_DEBUG,
             "Element #%d\n"
             "\tl = %d\n"
             "\tc = %c\n"
             "\ts = %d\n"
             "\to = %x\n"
             "\tf = %f\n"
             "\tb = %d\n"
             "\td = %f\n",
             i,
             vs.l, vs.c, vs.s, vs.o, vs.f, vs.b, vs.d));
      */
    }
}
