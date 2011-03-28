
//=============================================================================
/**
 *  @file    var_struct.cpp
 *
 *  $Id$
 *
 *  test variable sized structures
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"
#include "var_struct.h"

// ************************************************************************
//               Test_Var_Struct
// ************************************************************************

Test_Var_Struct::Test_Var_Struct (void)
  : opname_ (CORBA::string_dup ("test_var_struct")),
    inout_ (new Param_Test::Var_Struct),
    out_ (new Param_Test::Var_Struct),
    ret_ (new Param_Test::Var_Struct)
{
}

Test_Var_Struct::~Test_Var_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Var_Struct::opname (void) const
{
  return this->opname_;
}

void
Test_Var_Struct::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_Var_Struct);

  req->invoke ();

  Param_Test::Var_Struct *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Var_Struct (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::Var_Struct (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::Var_Struct (*tmp);
}

int
Test_Var_Struct::init_parameters (Param_Test_ptr)
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
  this->in_.seq.length (len);

  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.seq.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.seq[i] = str;
    }

  this->inout_->dbl = 0.0;
  this->inout_->dummy1 = CORBA::string_dup ("");
  this->inout_->boole = 0;
  this->inout_->dummy2 = CORBA::string_dup ("");
  this->inout_->shrt = 0;
  // set the length of the sequence
  this->inout_->seq.length (0);

  return 0;
}

int
Test_Var_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Var_Struct; // delete the previous ones
  this->out_ = new Param_Test::Var_Struct;
  this->ret_ = new Param_Test::Var_Struct;

  this->inout_->dbl = 0.0;
  this->inout_->dummy1 = CORBA::string_dup ("");
  this->inout_->boole = 0;
  this->inout_->dummy2 = CORBA::string_dup ("");
  this->inout_->shrt = 0;
  // set the length of the sequence
  this->inout_->seq.length (0);

  return 0;
}

int
Test_Var_Struct::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      Param_Test::Var_Struct_out out (this->out_.out ());
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
  CORBA::Boolean flag = 0;
  if (ACE::is_equal (this->in_.dbl, this->inout_->dbl) &&
      ACE::is_equal (this->in_.dbl, this->out_->dbl) &&
      ACE::is_equal (this->in_.dbl, this->ret_->dbl) &&
      (!ACE_OS::strcmp (this->in_.dummy1, this->inout_->dummy1)) &&
      (!ACE_OS::strcmp (this->in_.dummy1, this->out_->dummy1)) &&
      (!ACE_OS::strcmp (this->in_.dummy1, this->ret_->dummy1)) &&
      this->in_.boole == this->inout_->boole &&
      this->in_.boole == this->out_->boole &&
      this->in_.boole == this->ret_->boole &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->inout_->dummy2)) &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->out_->dummy2)) &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->ret_->dummy2)) &&
      this->in_.shrt == this->inout_->shrt &&
      this->in_.shrt == this->out_->shrt &&
      this->in_.shrt == this->ret_->shrt &&
      (this->in_.seq.length () == this->inout_->seq.length ()) &&
      (this->in_.seq.length () == this->out_->seq.length ()) &&
      (this->in_.seq.length () == this->ret_->seq.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < this->in_.seq.length () && flag; i++)
        {
          if (ACE_OS::strcmp (this->in_.seq[i], this->inout_->seq[i]) ||
              ACE_OS::strcmp (this->in_.seq[i], this->out_->seq[i]) ||
              ACE_OS::strcmp (this->in_.seq[i], this->ret_->seq[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Var_Struct::check_validity (CORBA::Request_ptr )
{
  return this->check_validity ();
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
  for (i=0; i < this->in_.seq.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in.seq : %s\n",
                  i,
                  (this->in_.seq[i]? (const char *)this->in_.seq[i]:"<nul>")));
    }
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->inout_.ptr () && (i < this->inout_->seq.length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "inout : %s\n",
                  i,
                  (this->inout_->seq[i]? (const char *)this->inout_->seq[i]:"<nul>")));
    }
  if (!this->inout_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\ninout struct does not exist\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->out_.ptr () && (i < this->out_->seq.length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_->seq[i]? (const char *)this->out_->seq[i]:"<nul>")));
    }
  if (!this->out_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nout struct is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->ret_.ptr () && (i < this->ret_->seq.length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_->seq[i]? (const char *)this->ret_->seq[i]:"<nul>")));
    }
  if (!this->ret_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nret struct is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}
