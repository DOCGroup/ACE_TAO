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
  req->invoke ();
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
  this->inout_ = new Param_Test::Var_Struct; // delete the previous ones
  this->out_ = new Param_Test::Var_Struct;
  this->ret_ = new Param_Test::Var_Struct;
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
                    0);

  CORBA::Any inout_arg (Param_Test::_tc_Var_Struct,
                        &this->inout_.inout (), // .out () causes crash
                        0);

  CORBA::Any out_arg (Param_Test::_tc_Var_Struct,
                      &this->out_.inout (),
                      0);

  // add parameters
  param_list->add_value ("s1",
                         in_arg,
                         CORBA::ARG_IN,
                         env);

  param_list->add_value ("s2",
                         inout_arg,
                         CORBA::ARG_INOUT,
                         env);

  param_list->add_value ("s3",
                         out_arg,
                         CORBA::ARG_OUT,
                         env);

  // add return value
  retval->item (0, env)->value ()->replace (Param_Test::_tc_Var_Struct,
                                            &this->ret_.inout (), // see above
                                            0, // does not own
                                            env);
  return 0;
}


CORBA::Boolean
Test_Var_Struct::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((!ACE_OS::strcmp (this->in_.dummy1, this->inout_->dummy1)) &&
      (!ACE_OS::strcmp (this->in_.dummy1, this->out_->dummy1)) &&
      (!ACE_OS::strcmp (this->in_.dummy1, this->ret_->dummy1)) &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->inout_->dummy2)) &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->out_->dummy2)) &&
      (!ACE_OS::strcmp (this->in_.dummy2, this->ret_->dummy2)) &&
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
Test_Var_Struct::check_validity (CORBA::Request_ptr req)
{
  ACE_UNUSED_ARG (req);
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
