
//=============================================================================
/**
 *  @file    bd_short_seq.cpp
 *
 *  $Id$
 *
 *  tests bounded short sequences
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "bd_short_seq.h"

// ************************************************************************
//               Test_Bounded_Short_Sequence
// ************************************************************************

Test_Bounded_Short_Sequence::Test_Bounded_Short_Sequence (void)
  : opname_ (CORBA::string_dup ("test_bounded_short_sequence")),
    in_ (new Param_Test::Bounded_Short_Seq),
    inout_ (new Param_Test::Bounded_Short_Seq),
    out_ (new Param_Test::Bounded_Short_Seq),
    ret_ (new Param_Test::Bounded_Short_Seq)
{
}

Test_Bounded_Short_Sequence::~Test_Bounded_Short_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Bounded_Short_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Bounded_Short_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();
  req->set_return_type (Param_Test::_tc_Bounded_Short_Seq);

  req->invoke ();

  Param_Test::Bounded_Short_Seq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Bounded_Short_Seq (*tmp);

  CORBA::NamedValue_ptr arg2 =
    req->arguments ()->item (1);
  *arg2->value () >>= tmp;
  this->inout_ = new Param_Test::Bounded_Short_Seq (*tmp);

  CORBA::NamedValue_ptr arg3 =
    req->arguments ()->item (2);
  *arg3->value () >>= tmp;
  this->out_ = new Param_Test::Bounded_Short_Seq (*tmp);
}

int
Test_Bounded_Short_Sequence::init_parameters (Param_Test_ptr /*objref*/
 /*env*/)
{
  // ACE_UNUSED_ARG (objref);

  // get some sequence length (32 in this case)
  CORBA::ULong len = this->in_->maximum ();

  // set the length of the sequence
  this->in_->length (len);
  this->inout_->length (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = i;
      this->inout_[i] = i + 1; // different from in_
    }

  this->inout_->length (0);
  this->out_->length (0);
  this->ret_->length (0);

  return 0;
}

int
Test_Bounded_Short_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Bounded_Short_Seq; // delete the previous ones
  this->out_ = new Param_Test::Bounded_Short_Seq;
  this->ret_ = new Param_Test::Bounded_Short_Seq;

  this->inout_->length (0);
  this->out_->length (0);
  this->ret_->length (0);

  return 0;
}

int
Test_Bounded_Short_Sequence::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::Bounded_Short_Seq_out out (this->out_.out ());

      this->ret_ = objref->test_bounded_short_sequence (this->in_.in (),
                                                        this->inout_.inout (),
                                                        out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Bounded_Short_Sequence::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Bounded_Short_Sequence::check_validity (void)
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
          if (this->in_[i] != this->inout_[i] ||
              this->in_[i] != this->out_[i] ||
              this->in_[i] != this->ret_[i])
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Bounded_Short_Sequence::check_validity (CORBA::Request_ptr /*req*/)
{
  return this->check_validity ();
}

void
Test_Bounded_Short_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->in_.ptr () && (i < this->in_->length ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->in_[i]));
    }

  if (!this->in_.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->inout_.ptr () && (i < this->inout_->length ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "inout : %d\n",
                  i,
                  this->inout_[i]));
    }

  if (!this->inout_.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->out_.ptr () && (i < this->out_->length ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "out : %d\n",
                  i,
                  this->out_[i]));
    }

  if (!this->out_.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nout sequence is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->ret_.ptr () && (i < this->ret_->length ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->ret_[i]));
    }

  if (!this->ret_.ptr ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
