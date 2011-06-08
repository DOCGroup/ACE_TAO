// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Alt_Mapping
//
// = FILENAME
//    var_struct.cpp
//
// = DESCRIPTION
//    test variable sized structures
//
// = AUTHORS
//      Aniruddha Gokhale
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "var_struct.h"

// ************************************************************************
//               Test_Var_Struct
// ************************************************************************

Test_Var_Struct::Test_Var_Struct (void)
  : opname_ (CORBA::string_dup ("test_var_struct")),
    inout_ (new Alt_Mapping::Var_Struct),
    out_ (new Alt_Mapping::Var_Struct),
    ret_ (new Alt_Mapping::Var_Struct)
{
}

Test_Var_Struct::~Test_Var_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Var_Struct::opname (void) const
{
  return this->opname_;
}

int
Test_Var_Struct::init_parameters (Alt_Mapping_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  this->in_.dbl = 3.14159;
  this->in_.dummy1 = gen->gen_string ();
  this->in_.boole = gen->gen_short () % 2;
  this->in_.dummy2 = gen->gen_string ();
  this->in_.shrt = gen->gen_short ();

  // set the length of the sequence
  this->in_.seq.resize (len);

  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.seq.size (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.seq[i] = str;
    }

  this->inout_->dbl = 0.0;
  this->inout_->dummy1 = "";
  this->inout_->boole = 0;
  this->inout_->dummy2 = "";
  this->inout_->shrt = 0;

  // set the length of the sequence
  this->inout_->seq.resize (0);

  return 0;
}

int
Test_Var_Struct::reset_parameters (void)
{
  this->inout_ = new Alt_Mapping::Var_Struct;
  this->out_ = new Alt_Mapping::Var_Struct;
  this->ret_ = new Alt_Mapping::Var_Struct;

  this->inout_->dbl = 0.0;
  this->inout_->dummy1 = "";
  this->inout_->boole = 0;
  this->inout_->dummy2 = "";
  this->inout_->shrt = 0;
  // set the length of the sequence
  this->inout_->seq.resize (0);

  return 0;
}

int
Test_Var_Struct::run_sii_test (Alt_Mapping_ptr objref)
{
  try
    {
      Alt_Mapping::Var_Struct_out out (this->out_.out ());
      this->ret_ = objref->test_var_struct (this->in_,
                                            this->inout_.inout (),
                                            out);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Var_Struct::run_sii_test\n");

    }
  return -1;
}

CORBA::Boolean
Test_Var_Struct::check_validity (void)
{
  CORBA::Boolean flag = false;

  if (this->in_.dbl == this->inout_->dbl &&
      this->in_.dbl == this->out_->dbl &&
      this->in_.dbl == this->ret_->dbl &&
      this->in_.dummy1 != this->inout_->dummy1 &&
      this->in_.dummy1 != this->out_->dummy1 &&
      this->in_.dummy1 != this->ret_->dummy1 &&
      this->in_.boole == this->inout_->boole &&
      this->in_.boole == this->out_->boole &&
      this->in_.boole == this->ret_->boole &&
      this->in_.dummy2 != this->inout_->dummy2 &&
      this->in_.dummy2 != this->out_->dummy2 &&
      this->in_.dummy2 != this->ret_->dummy2 &&
      this->in_.shrt == this->inout_->shrt &&
      this->in_.shrt == this->out_->shrt &&
      this->in_.shrt == this->ret_->shrt &&
      (this->in_.seq.size () == this->inout_->seq.size ()) &&
      (this->in_.seq.size () == this->out_->seq.size ()) &&
      (this->in_.seq.size () == this->ret_->seq.size ()))
    {
      flag = true; // assume all are equal

      // lengths are same. Now compare the contents
      for (CORBA::ULong i = 0; i < this->in_.seq.size () && flag; ++i)
        {
          if (this->in_.seq[i] != this->inout_->seq[i] ||
              this->in_.seq[i] != this->out_->seq[i] ||
              this->in_.seq[i] != this->ret_->seq[i])
            // not equal
            flag = false;
        }
    }

  return flag;
}

void
Test_Var_Struct::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n*=*=*=*=*=*=*=*=*=*=\n"
              "in_.dummy1 = %s\n"
              "inout_.dummy1 = %s\n"
              "out_.dummy1 = %s\n"
              "ret_.dummy1 = %s\n",
              this->in_.dummy1.in (),
              this->inout_->dummy1.in (),
              this->out_->dummy1.in (),
              this->ret_->dummy1.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "\n*=*=*=*=*=*=*=*=*=*=\n"
              "in_.dummy2 = %s\n"
              "inout_.dummy2 = %s\n"
              "out_.dummy2 = %s\n"
              "ret_.dummy2 = %s\n",
              this->in_.dummy2.in (),
              this->inout_->dummy2.in (),
              this->out_->dummy2.in (),
              this->ret_->dummy2.in ()));

  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; i < this->in_.seq.size (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in.seq : %s\n",
                  i,
                  (this->in_.seq[i].empty () ? this->in_.seq[i].c_str () : "<nul>")));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->inout_.ptr () && (i < this->inout_->seq.size ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "inout : %s\n",
                  i,
                  (this->inout_->seq[i].empty () ? this->inout_->seq[i].c_str () : "<nul>")));
    }

  if (this->inout_.ptr () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\ninout struct does not exist\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->out_.ptr () && (i < this->out_->seq.size ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_->seq[i].empty () ? this->out_->seq[i].c_str () : "<nul>")));
    }

  if (this->out_.ptr () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\nout struct is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));

  for (i = 0; this->ret_.ptr () && (i < this->ret_->seq.size ()); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_->seq[i].empty () ? this->ret_->seq[i].c_str () : "<nul>")));
    }
  if (this->ret_.ptr () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "\nret struct is NUL\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
