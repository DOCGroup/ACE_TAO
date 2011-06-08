// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Alt_Mapping
//
// = FILENAME
//    ub_octet_seq.cpp
//
// = DESCRIPTION
//    tests unbounded octet sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "ub_octet_seq.h"

// ************************************************************************
//               Test_Octet_Sequence
// ************************************************************************

Test_Octet_Sequence::Test_Octet_Sequence (void)
  : opname_ ("test_octet_sequence")
{
}

Test_Octet_Sequence::~Test_Octet_Sequence (void)
{
}

const char *
Test_Octet_Sequence::opname (void) const
{
  return this->opname_.c_str ();
}

int
Test_Octet_Sequence::init_parameters (Alt_Mapping_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some sequence length (not more than 10).
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // Set the length of the sequence.
  this->in_.resize (len);
  this->inout_.resize (len);
  
  // Now set each individual elements.
  for (CORBA::ULong i = 0; i < this->in_.size (); i++)
    {
      this->in_[i] = (CORBA::Octet) i;
      this->inout_[i] = (CORBA::Octet) (i + 1);
    }
    
  return 0;
}

int
Test_Octet_Sequence::reset_parameters (void)
{
  this->inout_.clear ();
  this->out_.clear ();
  this->ret_.clear ();
  
  return 0;
}

int
Test_Octet_Sequence::run_sii_test (Alt_Mapping_ptr objref)
{
  try
    {
      this->ret_ = objref->test_octet_sequence (this->in_,
                                                this->inout_,
                                                this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Long_Sequence::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Octet_Sequence::check_validity (void)
{
  CORBA::Boolean flag = false;
  
  if (this->in_.size () == this->inout_.size () &&
      this->in_.size () == this->out_.size () &&
      this->in_.size () == this->ret_.size ())
    {
      flag = true; // assume all are equal
      
      // lengths are same. Now compare the contents
      for (CORBA::ULong i = 0; i < this->in_.size () && flag; i++)
        {
          if (this->in_[i] != this->inout_[i] ||
              this->in_[i] != this->out_[i] ||
              this->in_[i] != this->ret_[i])
            // not equal
            flag = false;
        }
    }
    
  return flag;
}

void
Test_Octet_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->in_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->in_[i]));
    }
    
  if (this->in_.empty ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nin sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->inout_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->inout_[i]));
    }
    
  if (this->inout_.empty ())
    {
      ACE_DEBUG ((LM_DEBUG, "\ninout sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->out_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->out_[i]));
    }
    
  if (this->out_.empty ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nout sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->ret_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %d\n",
                  i,
                  this->ret_[i]));
    }
    
  if (this->ret_.empty ())
    {
      ACE_DEBUG ((LM_DEBUG, "\nin sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
