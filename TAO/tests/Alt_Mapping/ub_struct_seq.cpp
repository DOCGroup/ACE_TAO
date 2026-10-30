
//=============================================================================
/**
 *  @file    ub_struct_seq.cpp
 *
 *  tests unbounded struct sequences
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "ub_struct_seq.h"

// ************************************************************************
//               Test_Struct_Sequence
// ************************************************************************

char const *
Test_Unbounded_Struct_Sequence::opname () const
{
  return this->opname_.c_str ();
}

int
Test_Unbounded_Struct_Sequence::init_parameters (Alt_Mapping_ptr)
{
  Generator *gen = GENERATOR::instance (); // value generator

  // Get some sequence length (not more than 10).
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.resize (len);
  this->inout_.resize (len);

  // Now set each individual element.
  for (CORBA::ULong i = 0; i < this->in_.size (); i++)
    {
      // Generate some arbitrary struct to be filled into the ith location in
      // the sequence.
      this->in_[i] = gen->gen_fixed_struct ();
      this->inout_[i] = gen->gen_fixed_struct ();
    }

  return 0;
}

int
Test_Unbounded_Struct_Sequence::reset_parameters ()
{
  this->inout_.clear ();
  this->out_.clear ();
  this->ret_.clear ();

  return 0;
}

int
Test_Unbounded_Struct_Sequence::run_sii_test (
  Alt_Mapping_ptr objref)
{
  try
    {
      this->ret_ = objref->test_struct_sequence (this->in_,
                                                 this->inout_,
                                                 this->out_);

      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Struct_Sequence::run_sii_test\n");
    }
  return -1;
}

CORBA::Boolean
Test_Unbounded_Struct_Sequence::check_validity () const
{
  return (this->compare (this->in_, this->inout_)
          && this->compare (this->in_, this->out_)
          && this->compare (this->in_, this->ret_));
}

void
Test_Unbounded_Struct_Sequence::print_values () const
{
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "IN sequence\n"));
  this->print_sequence (this->in_);
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "INOUT sequence\n"));
  this->print_sequence (this->inout_);
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "OUT sequence\n"));
  this->print_sequence (this->out_);
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "RET sequence\n"));
  this->print_sequence (this->ret_);
}

CORBA::Boolean
Test_Unbounded_Struct_Sequence::compare (
  Alt_Mapping::StructSeq const &s1,
  Alt_Mapping::StructSeq const &s2) const
{
  ACE_CDR::ULong s1v = s1.capacity ();
  ACE_CDR::ULong s2v = s2.capacity ();

  if (s1v != s2v)
    {
      return false;
    }

  s1v = s1.size ();
  s2v = s2.size ();

  if (s1v != s2v)
    {
      return false;
    }

  for (CORBA::ULong i=0; i < s1.size (); i++)
    {
      Alt_Mapping::Fixed_Struct const &vs1 = s1[i];
      Alt_Mapping::Fixed_Struct const &vs2 = s2[i];

      if (vs1.l != vs2.l
          || vs1.c != vs2.c
          || vs1.s != vs2.s
          || vs1.o != vs2.o
          || !ACE::is_equal (vs1.f, vs2.f)
          || vs1.b != vs2.b
          || !ACE::is_equal (vs1.d, vs2.d))
        return false;
    }

  return true; // success
}

void
Test_Unbounded_Struct_Sequence::print_sequence (
  Alt_Mapping::StructSeq const &s) const
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.capacity (),
              s.size ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));

  for (CORBA::ULong i = 0; i < s.size (); i++)
    {
      Alt_Mapping::Fixed_Struct const &vs = s[i];

      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\tl = %d\n"
                  "\tc = %c\n"
                  "\ts = %d\n"
                  "\to = %x\n"
                  "\tf = %f\n"
                  "\tb = %d\n"
                  "\td = %f\n",
                  i,
                  vs.l, vs.c, vs.s, vs.o, vs.f, vs.b, vs.d));
    }
}
