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

const CORBA::ULong TEST_SEQ_LENGTH = 1;

ACE_RCSID(Param_Test, ub_any_seq, "$Id$")

// ************************************************************************
//               Test_AnySeq
// ************************************************************************

Test_AnySeq::Test_AnySeq (void)
  : opname_ (CORBA::string_dup ("test_anyseq")),
    in_ (new Param_Test::AnySeq (TEST_SEQ_LENGTH)),
    inout_ (new Param_Test::AnySeq (TEST_SEQ_LENGTH)),
    out_ (new Param_Test::AnySeq),
    ret_ (new Param_Test::AnySeq)
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

int
Test_AnySeq::init_parameters (Param_Test_ptr objref,
                              CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // Must be set explicitly (CORBA spec)
  this->in_->length (TEST_SEQ_LENGTH);

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 3);

      switch (index)
        {
          case 0:
            {
              CORBA::Short s;
              s = gen->gen_short ();
	      if (TAO_debug_level > 0)
		ACE_DEBUG ((LM_DEBUG, "setting short = %d\n", s));
              this->in_[i] <<= s;
              this->inout_[i] <<= 0; // different from in_
            }
            break;
          case 1:
            {
              char *str = gen->gen_string ();
	      if (TAO_debug_level > 0)
		ACE_DEBUG ((LM_DEBUG, "setting string = %s\n", str));
              this->in_[i] <<= str;
              this->inout_[i] <<= 0; // different from in_
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
                  this->in_[i] <<= cobj.in ();
                  this->inout_[i] <<= 0; // different from in_
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
    }

  return 0;
}

int
Test_AnySeq::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 3);

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
              char *str = gen->gen_string ();
              this->in_[i] <<= str;
              this->inout_[i] <<= str;
            }
            break;
          case 2:
            {
              this->inout_[i] = this->in_[i];
            }
            break;
          case 3:
            break;
          case 4:
            break;
          case 5:
            break;
        }
    }

  return 0;
}

int
Test_AnySeq::run_sii_test (Param_Test_ptr objref,
                           CORBA::Environment &env)
{
  Param_Test::AnySeq_out out (this->out_.out ());
  this->ret_ = objref->test_anyseq (this->in_.in (),
                                    this->inout_.inout (),
                                    out,
                                    env);
  return (env.exception () ? -1:0);
}

int
Test_AnySeq::add_args (CORBA::NVList_ptr param_list,
                       CORBA::NVList_ptr retval,
                       CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_AnySeq, 
                     (void *) &this->in_.in (), 
                     CORBA::B_FALSE);

  CORBA::Any inout_arg (Param_Test::_tc_AnySeq, 
                        &this->inout_.inout (), 
                        CORBA::B_FALSE);
  
  CORBA::Any out_arg (Param_Test::_tc_AnySeq, 
                      &this->out_.inout (), // .out () causes crash
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

  // add return value type
  retval->item (0, env)->value ()->replace (Param_Test::_tc_AnySeq,
                                            &this->ret_.inout (), // see above
                                            CORBA::B_FALSE, // does not own
                                            env);

  return 0;
}

CORBA::Boolean
Test_AnySeq::check_validity (void)
{
  CORBA::Short short_in, short_inout, short_out, short_ret;
  char *str_in, *str_inout, *str_out, *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;

  for (CORBA::ULong i=0; i < this->in_->length (); i++)
  {
    if ((this->in_[i] >>= short_in) &&
        (this->inout_[i] >>= short_inout) &&
        (this->out_[i] >>= short_out) &&
        (this->ret_[i] >>= short_ret))
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
    else if ((this->in_[i] >>= str_in) &&
             (this->inout_[i] >>= str_inout) &&
             (this->out_[i] >>= str_out) &&
             (this->ret_[i] >>= str_ret))
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
    else if ((this->in_[i] >>= obj_in) &&
             (this->inout_[i] >>= obj_inout) &&
             (this->out_[i] >>= obj_out) &&
             (this->ret_[i] >>= obj_ret))
    {
      // all the >>= operators returned true so we are OK.
      return 1;
    }
    else
      return 0;
  }

  // Should never reach this.
  return 0;
}

CORBA::Boolean
Test_AnySeq::check_validity (CORBA::Request_ptr req)
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
Test_AnySeq::print_sequence (const Param_Test::AnySeq &s)
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
*/    }
}
