// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_str_seq.cpp
//
// = DESCRIPTION
//    tests unbounded string sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ub_str_seq.h"

ACE_RCSID(Param_Test, ub_str_seq, "$Id$")

// ************************************************************************
//               Test_String_Sequence
// ************************************************************************

Test_String_Sequence::Test_String_Sequence (void)
  : opname_ (CORBA::string_dup ("test_strseq")),
    in_ (new Param_Test::StrSeq),
    inout_ (new Param_Test::StrSeq),
    out_ (new Param_Test::StrSeq),
    ret_ (new Param_Test::StrSeq)
{
}

Test_String_Sequence::~Test_String_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_String_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_String_Sequence::dii_req_invoke (CORBA::Request *req
                                      TAO_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_StrSeq);

  req->invoke (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Param_Test::StrSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::StrSeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::StrSeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::StrSeq (*tmp);
}

int
Test_String_Sequence::init_parameters (Param_Test_ptr
                                       TAO_ENV_ARG_DECL_NOT_USED)
{
  const char *choiceList[] =
  {
    "one",
    "two",
    "three",
    "four"
  };

  CORBA::ULong len = sizeof(choiceList)/sizeof(char *) - 1;

  // set the length of the sequences
  this->in_->length (len);
  this->inout_->length (len);

  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      this->in_[i] = choiceList[i];
      this->inout_[i] = choiceList[i+1];
    }
  return 0;
}

int
Test_String_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::StrSeq; // delete the previous ones
  this->out_ = new Param_Test::StrSeq;
  this->ret_ = new Param_Test::StrSeq;
  return 0;
}

int
Test_String_Sequence::run_sii_test (Param_Test_ptr objref
                                    TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      Param_Test::StrSeq_out out (this->out_.out ());

      this->ret_ = objref->test_strseq (this->in_.in (),
                                        this->inout_.inout (),
                                        out
                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_String_Sequence::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_String_Sequence::check_validity (void)
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
Test_String_Sequence::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_String_Sequence::print_values (void)
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
