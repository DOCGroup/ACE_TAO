// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_short_seq.cpp
//
// = DESCRIPTION
//    tests unbounded short sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "ub_short_seq.h"

ACE_RCSID(Param_Test, ub_short_seq, "$Id$")

// ************************************************************************
//               Test_Short_Sequence
// ************************************************************************

Test_Short_Sequence::Test_Short_Sequence (void)
  : opname_ (CORBA::string_dup ("test_short_sequence")),
    in_ (new Param_Test::Short_Seq),
    inout_ (new Param_Test::Short_Seq),
    out_ (0),
    ret_ (0)
{
}

Test_Short_Sequence::~Test_Short_Sequence (void)
{
  CORBA::string_free (this->opname_);
}

const char *
Test_Short_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_Short_Sequence::init_parameters (Param_Test_ptr objref,
                                      CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_->length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = i;

    }
  return 0;
}

int
Test_Short_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Short_Seq; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Short_Sequence::run_sii_test (Param_Test_ptr objref,
                                   CORBA::Environment &env)
{
  Param_Test::Short_Seq_out out (this->out_.out ());
  this->ret_ = objref->test_short_sequence (this->in_.in (),
                                            this->inout_.inout (),
                                            out,
                                            env);
  return (env.exception () ? -1:0);
}

int
Test_Short_Sequence::add_args (CORBA::NVList_ptr param_list,
                               CORBA::NVList_ptr retval,
                               CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Short_Seq,
                     (void *) &this->in_.in (),
                     CORBA::B_FALSE);

  CORBA::Any inout_arg (Param_Test::_tc_Short_Seq,
                        &this->inout_.inout (),
                        CORBA::B_FALSE);
 
  CORBA::Any out_arg (Param_Test::_tc_Short_Seq,
                      &this->dii_out_,
                      CORBA::B_FALSE);

  // add parameters
  param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value type
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Short_Seq,
                                            &this->dii_ret_,
                                            CORBA::B_FALSE, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Short_Sequence::check_validity_engine (const Param_Test::Short_Seq &the_in,
                                            const Param_Test::Short_Seq &the_inout,
                                            const Param_Test::Short_Seq &the_out,
                                            const Param_Test::Short_Seq &the_ret)
{
  CORBA::Boolean flag = 0;
  if ((the_in.length () == the_inout.length ()) &&
      (the_in.length () == the_out.length ()) &&
      (the_in.length () == the_ret.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < the_in.length () && flag; i++)
        {
          if ((the_in[i] != the_inout[i]) ||
              (the_in[i] != the_out[i]) ||
              (the_in[i] != the_ret[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Short_Sequence::check_validity (void)
{
  return this->check_validity_engine (this->in_.in (),
                                      this->inout_.in (),
                                      this->out_.in (),
                                      this->ret_.in ());
}

CORBA::Boolean
Test_Short_Sequence::check_validity (CORBA::Request_ptr req)
{
  return this->check_validity_engine (this->in_.in (),
                                      this->inout_.in (),
                                      this->dii_out_,
                                      this->dii_ret_);
}

void
Test_Short_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->in_.ptr () && (i < this->in_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  this->in_[i]? (const char *)this->in_[i]:"<nul>"));
    }
  if (!this->in_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->inout_.ptr () && (i < this->inout_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->inout_[i]? (const char *)this->inout_[i]:"<nul>")));
    }
  if (!this->inout_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->out_.ptr () && (i < this->out_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_[i]? (const char *)this->out_[i]:"<nul>")));
    }
  if (!this->out_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->ret_.ptr () && (i < this->ret_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_[i]? (const char *)this->ret_[i]:"<nul>")));
    }
  if (!this->ret_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}

