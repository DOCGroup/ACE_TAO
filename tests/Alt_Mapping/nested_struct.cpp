// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Alt_Mapping
//
// = FILENAME
//    nested_struct.cpp
//
// = DESCRIPTION
//    tests nested structs
//
// = AUTHORS
//      Aniruddha Gokhale
//      Jeff Parsons
//
// ============================================================================

#include "helper.h"
#include "nested_struct.h"

// ************************************************************************
//               Test_Nested_Struct
// ************************************************************************

Test_Nested_Struct::Test_Nested_Struct (void)
  : opname_ (CORBA::string_dup ("test_nested_struct")),
    inout_ (new Alt_Mapping::Nested_Struct),
    out_ (new Alt_Mapping::Nested_Struct),
    ret_ (new Alt_Mapping::Nested_Struct)
{
}

Test_Nested_Struct::~Test_Nested_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Nested_Struct::opname (void) const
{
  return this->opname_;
}

int
Test_Nested_Struct::init_parameters (Alt_Mapping_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  this->in_.vs.dbl = 0.0;
  this->in_.vs.dummy1 = "";
  this->in_.vs.boole = 0;
  this->in_.vs.dummy2 = "";
  this->in_.vs.shrt = 0;
  // set the length of the sequence
  this->in_.vs.seq.resize (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.vs.seq[i] = str;
    }

  this->inout_->vs.dbl = 0.0;
  this->inout_->vs.dummy1 = "";
  this->inout_->vs.boole = 0;
  this->inout_->vs.dummy2 = "";
  this->inout_->vs.shrt = 0;
  // set the length of the sequence
  this->inout_->vs.seq.resize (0);

  return 0;
}

int
Test_Nested_Struct::reset_parameters (void)
{
  this->inout_ = new Alt_Mapping::Nested_Struct;
  this->out_ = new Alt_Mapping::Nested_Struct;
  this->ret_ = new Alt_Mapping::Nested_Struct;

  this->inout_->vs.dbl = 0.0;
  this->inout_->vs.dummy1 = "";
  this->inout_->vs.boole = 0;
  this->inout_->vs.dummy2 = "";
  this->inout_->vs.shrt = 0;
  // set the length of the sequence
  this->inout_->vs.seq.resize (0);

  return 0;
}

int
Test_Nested_Struct::run_sii_test (Alt_Mapping_ptr objref)
{
  try
    {
      Alt_Mapping::Nested_Struct_out out (this->out_.out ());
      this->ret_ = objref->test_nested_struct (this->in_,
                                               this->inout_.inout (),
                                               this->out_);

      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Nested_Struct::run_sii_test\n");

    }

  return -1;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (void)
{
  CORBA::Boolean flag = false;

  if ((this->in_.vs.seq.size () == this->inout_->vs.seq.size ()) &&
      (this->in_.vs.seq.size () == this->out_->vs.seq.size ()) &&
      (this->in_.vs.seq.size () == this->ret_->vs.seq.size ()))
    {
      flag = true; // assume all are equal

      // lengths are same. Now compare the contents
      for (CORBA::ULong i = 0; i < this->in_.vs.seq.size () && flag; ++i)
        {
          if (this->in_.vs.seq[i] != this->inout_->vs.seq[i] ||
              this->in_.vs.seq[i] != this->out_->vs.seq[i] ||
              this->in_.vs.seq[i] != this->ret_->vs.seq[i])
            // not equal
            flag = false;
        }
    }

  return flag;
}

void
Test_Nested_Struct::print_values (void)
{
  for (CORBA::ULong i = 0; i < this->in_.vs.seq.size (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n*=*=*=*=*=*=*=*=*=*=\n"
                  "Element # %d\n"
                  "in (len = %d): %s\n"
                  "inout (len = %d): %s\n"
                  "out (len = %d): %s\n"
                  "ret (len = %d): %s\n",
                  i,
                  this->in_.vs.seq.size (),
                  (this->in_.vs.seq.empty () ? this->in_.vs.seq[i].c_str () : "<nul>"),
                  this->inout_->vs.seq.size (),
                  (this->inout_->vs.seq.empty () ? this->inout_->vs.seq[i].c_str () : "<nul>"),
                  this->out_->vs.seq.size (),
                  (this->out_->vs.seq.empty () ? this->out_->vs.seq[i].c_str () : "<nul>"),
                  this->ret_->vs.seq.size (),
                  (this->ret_->vs.seq.empty () ? this->ret_->vs.seq[i].c_str () : "<nul>")));
    }
}
