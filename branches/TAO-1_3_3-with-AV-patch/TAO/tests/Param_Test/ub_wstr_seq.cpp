// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_wstr_seq.cpp
//
// = DESCRIPTION
//    tests unbounded wide string sequences
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "ub_wstr_seq.h"

ACE_RCSID(Param_Test, ub_wstr_seq, "$Id$")

// ************************************************************************
//               Test_WString_Sequence
// ************************************************************************

Test_WString_Sequence::Test_WString_Sequence (void)
  : opname_ (CORBA::string_dup ("test_wstrseq")),
    in_ (new Param_Test::WStrSeq),
    inout_ (new Param_Test::WStrSeq),
    out_ (new Param_Test::WStrSeq),
    ret_ (new Param_Test::WStrSeq)
{
}

Test_WString_Sequence::~Test_WString_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_WString_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_WString_Sequence::dii_req_invoke (CORBA::Request *req
                                       ACE_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_WStrSeq);

  req->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Param_Test::WStrSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::WStrSeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::WStrSeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::WStrSeq (*tmp);
}

int
Test_WString_Sequence::init_parameters (Param_Test_ptr
                                        ACE_ENV_ARG_DECL_NOT_USED)
{
  Generator *gen = GENERATOR::instance (); // value generator

  CORBA::ULong len = 2;

  // set the length of the sequences
  this->in_->length (len);
  this->inout_->length (len);

  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_->length (); i++)
    {
      this->in_[i] = gen->gen_wstring ();
      this->inout_[i] = gen->gen_wstring ();
    }

  return 0;
}

int
Test_WString_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::WStrSeq; // delete the previous ones
  this->out_ = new Param_Test::WStrSeq;
  this->ret_ = new Param_Test::WStrSeq;
  return 0;
}

int
Test_WString_Sequence::run_sii_test (Param_Test_ptr objref
                                     ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      Param_Test::WStrSeq_out out (this->out_.out ());

      this->ret_ = objref->test_wstrseq (this->in_.in (),
                                         this->inout_.inout (),
                                         out
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_WString_Sequence::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_WString_Sequence::check_validity (void)
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
          if (ACE_OS::wscmp (this->in_[i], this->inout_[i]) ||
              ACE_OS::wscmp (this->in_[i], this->out_[i]) ||
              ACE_OS::wscmp (this->in_[i], this->ret_[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_WString_Sequence::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_WString_Sequence::print_values (void)
{
}
