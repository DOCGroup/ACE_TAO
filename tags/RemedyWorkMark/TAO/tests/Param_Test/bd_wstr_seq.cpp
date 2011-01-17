// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_wstr_seq.cpp
//
// = DESCRIPTION
//    tests bounded wide string sequences
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "bd_wstr_seq.h"
#include "ace/OS_NS_wchar.h"

// ************************************************************************
//               Test_Bounded_WString_Sequence
// ************************************************************************

Test_Bounded_WString_Sequence::Test_Bounded_WString_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_wstrseq")),
    in_ (new Param_Test::Bounded_WStrSeq),
    inout_ (new Param_Test::Bounded_WStrSeq),
    out_ (new Param_Test::Bounded_WStrSeq),
    ret_ (new Param_Test::Bounded_WStrSeq)
{
}

Test_Bounded_WString_Sequence::~Test_Bounded_WString_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Bounded_WString_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_WString_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_Bounded_WStrSeq);

  req->invoke ();

  const Param_Test::Bounded_WStrSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Bounded_WStrSeq (*tmp);

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= tmp;
  this->inout_ = new Param_Test::Bounded_WStrSeq (*tmp);

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= tmp;
  this->out_ = new Param_Test::Bounded_WStrSeq (*tmp);
}

int
Test_Bounded_WString_Sequence::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  CORBA::ULong len = this->in_->maximum ();

  // set the length of the sequence
  this->in_->length (len);
  this->inout_->length (len);

  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_->length (); i++)
    {
      this->in_[i] = gen->gen_wstring ();

          // different from in_
      this->inout_[i] = gen->gen_wstring ();
    }

  return 0;
}

int
Test_Bounded_WString_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Bounded_WStrSeq; // delete the previous ones
  this->out_ = new Param_Test::Bounded_WStrSeq;
  this->ret_ = new Param_Test::Bounded_WStrSeq;
  return 0;
}

int
Test_Bounded_WString_Sequence::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::Bounded_WStrSeq_out out (this->out_.out ());

      this->ret_ = objref->test_bounded_wstrseq (this->in_.in (),
                                                 this->inout_.inout (),
                                                 out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Bounded_WString_Sequence::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Bounded_WString_Sequence::check_validity (void)
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
Test_Bounded_WString_Sequence::check_validity (CORBA::Request_ptr)
{
  return this->check_validity ();
}

void
Test_Bounded_WString_Sequence::print_values (void)
{
}
