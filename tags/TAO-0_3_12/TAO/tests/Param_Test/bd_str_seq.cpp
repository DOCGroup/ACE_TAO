// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_str_seq.cpp
//
// = DESCRIPTION
//    tests bounded string sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "bd_str_seq.h"

ACE_RCSID(Param_Test, bd_str_seq, "$Id$")

// ************************************************************************
//               Test_Bounded_String_Sequence
// ************************************************************************

Test_Bounded_String_Sequence::Test_Bounded_String_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_strseq")),
    in_ (new Param_Test::Bounded_StrSeq),
    inout_ (new Param_Test::Bounded_StrSeq),
    out_ (new Param_Test::Bounded_StrSeq),
    ret_ (new Param_Test::Bounded_StrSeq)
{
}

Test_Bounded_String_Sequence::~Test_Bounded_String_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Bounded_String_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_String_Sequence::dii_req_invoke (CORBA::Request *req,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  req->invoke (ACE_TRY_ENV);
}

int
Test_Bounded_String_Sequence::init_parameters (Param_Test_ptr objref,
                                               CORBA::Environment &env)
{
  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  const char *choiceList[] =
  {
    "one",
    "two",
    "three"
  };

  CORBA::ULong len = this->in_->maximum ();

  // set the length of the sequence
  this->in_->length (len);
  this->inout_->length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      this->in_[i] = choiceList[i%3];
      this->inout_[i] = choiceList[(i+1)%3]; // different from in_
    }
  return 0;
}

int
Test_Bounded_String_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Bounded_StrSeq; // delete the previous ones
  this->out_ = new Param_Test::Bounded_StrSeq;
  this->ret_ = new Param_Test::Bounded_StrSeq;
  return 0;
}

int
Test_Bounded_String_Sequence::run_sii_test (Param_Test_ptr objref,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  Param_Test::Bounded_StrSeq_out out (this->out_.out ());
  this->ret_ = objref->test_bounded_strseq (this->in_.in (),
                                            this->inout_.inout (),
                                            out,
                                            ACE_TRY_ENV);
  return (ACE_TRY_ENV.exception () ? -1:0);
}

int
Test_Bounded_String_Sequence::add_args (CORBA::NVList_ptr param_list,
                                        CORBA::NVList_ptr retval,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Any in_arg (Param_Test::_tc_Bounded_StrSeq,
                     (void *) &this->in_.in (),
                     0);

  CORBA::Any inout_arg (Param_Test::_tc_Bounded_StrSeq,
                        &this->inout_.inout (),
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Bounded_StrSeq,
                      &this->out_.inout (), // .out () causes crash
                      0);

  // add parameters
  param_list->add_value ("s1",
                         in_arg,
                         CORBA::ARG_IN,
                         ACE_TRY_ENV);

  param_list->add_value ("s2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         ACE_TRY_ENV);

  param_list->add_value ("s3",
                         out_arg,
                         CORBA::ARG_OUT,
                         ACE_TRY_ENV);

  // add return value type
  retval->item (0, ACE_TRY_ENV)->value ()->replace (Param_Test::_tc_Bounded_StrSeq,
                                            &this->ret_.inout (), // see above
                                            0, // does not own
                                            ACE_TRY_ENV);
  return 0;
}

CORBA::Boolean
Test_Bounded_String_Sequence::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((this->in_->length () == this->inout_->length ()) &&
      (this->in_->length () == this->out_->length ()) &&
      (this->in_->length () == this->ret_->length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < this->in_->length () && flag; i++)
        {
          if (ACE_OS::strcmp(this->in_[i], this->inout_[i]) ||
              ACE_OS::strcmp(this->in_[i], this->out_[i]) ||
              ACE_OS::strcmp(this->in_[i], this->ret_[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Bounded_String_Sequence::check_validity (CORBA::Request_ptr /*req*/)
{
  //ACE_UNUSED_ARG (req);
  return this->check_validity ();
}

void
Test_Bounded_String_Sequence::print_values (void)
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
