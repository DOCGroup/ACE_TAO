// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_long_seq.cpp
//
// = DESCRIPTION
//    tests unbounded long sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "ub_long_seq.h"

ACE_RCSID (Param_Test,
           ub_long_seq,
           "$Id$")

// ************************************************************************
//               Test_Long_Sequence
// ************************************************************************

Test_Long_Sequence::Test_Long_Sequence (void)
  : opname_ (CORBA::string_dup ("test_long_sequence")),
    in_ (new CORBA::LongSeq),
    inout_ (new CORBA::LongSeq),
    out_ (new CORBA::LongSeq),
    ret_ (new CORBA::LongSeq)
{
}

Test_Long_Sequence::~Test_Long_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Long_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_Long_Sequence::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_.in ();
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_LongSeq);

  req->invoke ();

  CORBA::LongSeq *tmp;
  req->return_value () >>= tmp;
  this->ret_ = CORBA::LongSeq (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = CORBA::LongSeq (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = CORBA::LongSeq (*tmp);
}

int
Test_Long_Sequence::init_parameters (Param_Test_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_->length (len);
  this->inout_->length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      this->in_[i] = i;
      this->inout_[i] = i+1;
    }
  return 0;
}

int
Test_Long_Sequence::reset_parameters (void)
{
  this->inout_ = new CORBA::LongSeq; // delete the previous ones
  this->out_ = new CORBA::LongSeq;
  this->ret_ = new CORBA::LongSeq;
  return 0;
}

int
Test_Long_Sequence::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      CORBA::LongSeq_out out (this->out_.out ());

      this->ret_ = objref->test_long_sequence (this->in_.in (),
                                               this->inout_.inout (),
                                               out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Long_Sequence::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Long_Sequence::check_validity (void)
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
Test_Long_Sequence::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
}

void
Test_Long_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->in_.ptr () && (i < this->in_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->in_[i]));
    }
  if (!this->in_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->inout_.ptr () && (i < this->inout_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->inout_[i]));
    }
  if (!this->inout_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->out_.ptr () && (i < this->out_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->out_[i]));
    }
  if (!this->out_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->ret_.ptr () && (i < this->ret_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->ret_[i]));
    }
  if (!this->ret_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
