
//=============================================================================
/**
 *  @file    ub_wstr_seq.cpp
 *
 *  $Id$
 *
 *  tests unbounded wide string sequences
 *
 *
 *  @author   Jeff Parsons
 */
//=============================================================================


#include "helper.h"
#include "ub_wstr_seq.h"
#include "ace/OS_NS_wchar.h"

// ************************************************************************
//               Test_WString_Sequence
// ************************************************************************

Test_WString_Sequence::Test_WString_Sequence (void)
  : opname_ (CORBA::string_dup ("test_wstrseq")),
    in_ (new CORBA::WStringSeq),
    inout_ (new CORBA::WStringSeq),
    out_ (new CORBA::WStringSeq),
    ret_ (new CORBA::WStringSeq)
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
Test_WString_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_WStringSeq);

  req->invoke ();

  CORBA::WStringSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new CORBA::WStringSeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = new CORBA::WStringSeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new CORBA::WStringSeq (*tmp);
}

int
Test_WString_Sequence::init_parameters (Param_Test_ptr)
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
  this->inout_ = new CORBA::WStringSeq; // delete the previous ones
  this->out_ = new CORBA::WStringSeq;
  this->ret_ = new CORBA::WStringSeq;
  return 0;
}

int
Test_WString_Sequence::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::WStringSeq_out out (this->out_.out ());

      this->ret_ = objref->test_wstrseq (this->in_.in (),
                                         this->inout_.inout (),
                                         out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_WString_Sequence::run_sii_test\n");

    }
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
