// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    var_struct.cpp
//
// = DESCRIPTION
//    test variable sized structures
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "var_struct.h"

ACE_RCSID(Param_Test, var_struct, "$Id$")

// ************************************************************************
//               Test_Var_Struct
// ************************************************************************

Test_Var_Struct::Test_Var_Struct (void)
  : opname_ (CORBA::string_dup ("test_var_struct")),
    inout_ (new Param_Test::Var_Struct),
    out_ (0),
    ret_ (0)
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

int
Test_Var_Struct::init_parameters (Param_Test_ptr objref,
                                  CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.dummy1 = gen->gen_string ();
  this->in_.dummy2 = gen->gen_string ();
  this->in_.seq.length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_.seq.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.seq[i] = str;
    }
  return 0;
}

int
Test_Var_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Var_Struct; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Var_Struct::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::Var_Struct_out out (this->out_.out ());
  this->ret_ = objref->test_var_struct (this->in_,
                                        this->inout_.inout (),
                                        out,
                                        env);
  return (env.exception () ? -1:0);
}

int
Test_Var_Struct::add_args (CORBA::NVList_ptr param_list,
			   CORBA::NVList_ptr retval,
			   CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Var_Struct, 
                    &this->in_, 
                    CORBA::B_FALSE);

  CORBA::Any inout_arg (Param_Test::_tc_Var_Struct, 
                        &this->inout_.inout (), 
                        CORBA::B_FALSE);

  CORBA::Any out_arg (Param_Test::_tc_Var_Struct, 
                      0,//this->dii_out_, 
                      CORBA::B_FALSE);

  // add parameters
  param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Var_Struct,
                                            0,//this->dii_ret_,
                                            CORBA::B_FALSE, // does not own
                                            env);
  return 0;
}

CORBA::Boolean
Test_Var_Struct::check_validity_engine (Param_Test::Var_Struct the_in,
                                        Param_Test::Var_Struct the_inout,
                                        Param_Test::Var_Struct the_out,
                                        Param_Test::Var_Struct the_ret)
{
  CORBA::Boolean flag = 0;
  if ((!ACE_OS::strcmp (the_in.dummy1, the_inout.dummy1)) &&
      (!ACE_OS::strcmp (the_in.dummy1, the_out.dummy1)) &&
      (!ACE_OS::strcmp (the_in.dummy1, the_ret.dummy1)) &&
      (!ACE_OS::strcmp (the_in.dummy2, the_inout.dummy2)) &&
      (!ACE_OS::strcmp (the_in.dummy2, the_out.dummy2)) &&
      (!ACE_OS::strcmp (the_in.dummy2, the_ret.dummy2)) &&
      (the_in.seq.length () == the_inout.seq.length ()) &&
      (the_in.seq.length () == the_out.seq.length ()) &&
      (the_in.seq.length () == the_ret.seq.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < the_in.seq.length () && flag; i++)
        {
          if (ACE_OS::strcmp (the_in.seq[i], the_inout.seq[i]) ||
              ACE_OS::strcmp (the_in.seq[i], the_out.seq[i]) ||
              ACE_OS::strcmp (the_in.seq[i], the_ret.seq[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Var_Struct::check_validity (void)
{
  return check_validity_engine (this->in_,
                                this->inout_.in (),
                                this->out_.in (),
                                this->ret_.in ());
}

CORBA::Boolean
Test_Var_Struct::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;

  *req->arguments ()->item (2, env)->value () >>= this->dii_out_;
  *req->result ()->value () >>= this->dii_ret_;

  return this->check_validity_engine (this->in_,
                                      this->inout_.in (),
                                      *this->dii_out_,
                                      *this->dii_ret_);
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

