// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Alt_Mapping
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

// ************************************************************************
//               Test_String_Sequence
// ************************************************************************

Test_String_Sequence::Test_String_Sequence (void)
  : opname_ (CORBA::string_dup ("test_strseq"))
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

int
Test_String_Sequence::init_parameters (Alt_Mapping_ptr)
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
  this->in_.resize (len);
  this->inout_.resize (len);

  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.size (); i++)
    {
      this->in_[i] = choiceList[i];
      this->inout_[i] = choiceList[i + 1];
    }
    
  return 0;
}

int
Test_String_Sequence::reset_parameters (void)
{
  this->inout_.clear ();
  this->out_.clear ();
  this->ret_.clear ();
  
  return 0;
}

int
Test_String_Sequence::run_sii_test (Alt_Mapping_ptr objref)
{
  try
    {
      this->ret_ = objref->test_strseq (this->in_,
                                        this->inout_,
                                        this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_String_Sequence::run_sii_test\n");

    }
    
  return -1;
}

CORBA::Boolean
Test_String_Sequence::check_validity (void)
{
  CORBA::Boolean flag = false;
  
  if ((this->in_.size () == this->inout_.size ()) &&
      (this->in_.size () == this->out_.size ()) &&
      (this->in_.size () == this->ret_.size ()))
    {
      flag = true;
      
      // Now compare the contents
      for (CORBA::ULong i = 0; i < this->in_.size () && flag; i++)
        {
          if (this->in_[i] != this->inout_[i]
              || this->in_[i] != this->out_[i]
              || this->in_[i] != this->ret_[i])
            {
              flag = false;
            }
        }
    }
    
  return flag;
}

void
Test_String_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->in_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->in_[i].size () > 0
                     ? this->in_[i].c_str ()
                     : "<empty>")));
    }
    
  if (this->in_.size () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\nin sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->inout_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->inout_[i].size () > 0
                     ? this->inout_[i].c_str ()
                     : "<empty>")));
    }
  if (!this->inout_.size () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\ninout sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for ( i = 0; i < this->out_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_[i].size () > 0
                     ? this->out_[i].c_str ()
                     : "<empty>")));
    }
  if (!this->out_.size () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\nout sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  
  for (i = 0; i < this->ret_.size (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_[i].size () > 0
                     ? this->ret_[i].c_str ()
                     : "<empty>")));
    }
  if (!this->ret_.size () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\nreturn sequence is empty\n"));
    }
    
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
