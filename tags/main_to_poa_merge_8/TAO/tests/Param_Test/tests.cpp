// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    tests.cpp
//
// = DESCRIPTION
//    All the test objects defined here
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"
#include "tests.h"


// ************************************************************************
//               Test_Short
// ************************************************************************

Test_Short::Test_Short (void)
  : opname_ (CORBA::string_dup ("test_short"))
{
}

Test_Short::~Test_Short (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Short::opname (void) const
{
  return this->opname_;
}

int
Test_Short::init_parameters (Param_Test_ptr objref,
                             CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  this->in_ = gen->gen_short ();
  this->inout_ =  0;
  return 0;
}

int
Test_Short::reset_parameters (void)
{
  this->inout_ =  0;
  this->out_ =  0;
  this->ret_ =  0;
  return 0;
}

int
Test_Short::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  this->ret_ = objref->test_short (this->in_, this->inout_, this->out_, env);
  return (env.exception () ? -1:0);
}

int
Test_Short::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_short, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_short, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_short, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_short,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Short::check_validity (void)
{
  if (this->inout_ == this->in_*2 &&
      this->out_ == this->in_*3 &&
      this->ret_ == this->in_*4)
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_Short::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = *(CORBA::Short *) req->arguments ()->item (1, env)->value ()->value ();
  this->out_ = *(CORBA::Short *) req->arguments ()->item (2, env)->value ()->value ();
  this->ret_ = *(CORBA::Short *)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Short::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*"
              "in = %d, "
              "inout = %d, "
              "out = %d, "
              "ret = %d*=*=*=*=*=\n",
              this->in_,
              this->inout_,
              this->out_,
              this->ret_));
}


// ************************************************************************
//               Test_Unbounded_String
// ************************************************************************

Test_Unbounded_String::Test_Unbounded_String (void)
  : opname_ (CORBA::string_dup ("test_unbounded_string")),
    in_ (0),
    inout_ (0),
    out_ (0),
    ret_ (0)
{
}

Test_Unbounded_String::~Test_Unbounded_String (void)
{
  CORBA::string_free (this->opname_);
  CORBA::string_free (this->in_);
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->opname_ = 0;
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;
}

const char *
Test_Unbounded_String::opname (void) const
{
  return this->opname_;
}

int
Test_Unbounded_String::init_parameters (Param_Test_ptr objref,
                                        CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // release any previously occupied values
  CORBA::string_free (this->in_);
  CORBA::string_free (this->inout_);
  CORBA::string_free (this->out_);
  CORBA::string_free (this->ret_);
  this->in_ = 0;
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->in_ = gen->gen_string ();
  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Unbounded_String::reset_parameters (void)
{
  // release any previously occupied values
  this->inout_ = 0;
  this->out_ = 0;
  this->ret_ = 0;

  this->inout_ = CORBA::string_dup (this->in_);
  return 0;
}

int
Test_Unbounded_String::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::String_out str_out (this->out_);
  this->ret_ = objref->test_unbounded_string (this->in_, this->inout_, str_out,
                                              env);
  return (env.exception () ? -1:0);
}

int
Test_Unbounded_String::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  CORBA::Any in_arg (CORBA::_tc_string, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_string, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_string, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_string,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Unbounded_String::check_validity (void)
{
  CORBA::ULong len = ACE_OS::strlen (this->in_);

  if (!ACE_OS::strcmp (this->in_, this->out_) &&
      !ACE_OS::strcmp (this->in_, this->ret_) &&
      ACE_OS::strlen (this->inout_) == 2*len &&
      !ACE_OS::strncmp (this->in_, this->inout_, len) &&
      !ACE_OS::strncmp (this->in_, &this->inout_[len], len))
    return 1;

  return 0; // otherwise
}

CORBA::Boolean
Test_Unbounded_String::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = *(char **) req->arguments ()->item (1, env)->value ()->value ();
  this->out_ = *(char **) req->arguments ()->item (2, env)->value ()->value ();
  this->ret_ = *(char **)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Unbounded_String::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "in with len (%d) = %s\n"
              "inout with len (%d) = %s\n"
              "out with len (%d) = %s\n"
              "ret with len (%d) = %s\n",
              (this->in_ ? ACE_OS::strlen (this->in_):0),
              (this->in_ ? this->in_:"<nul string>"),
              (this->inout_ ? ACE_OS::strlen (this->inout_):0),
              (this->inout_ ? this->inout_:"<nul string>"),
              (this->out_ ? ACE_OS::strlen (this->out_):0),
              (this->out_ ? this->out_:"<nul string>"),
              (this->ret_ ? ACE_OS::strlen (this->ret_):0),
              (this->ret_ ? this->ret_:"<nul string>")));
}

// ************************************************************************
//               Test_Fixed_Struct
// ************************************************************************

Test_Fixed_Struct::Test_Fixed_Struct (void)
  : opname_ (CORBA::string_dup ("test_fixed_struct"))
{
}

Test_Fixed_Struct::~Test_Fixed_Struct (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Fixed_Struct::opname (void) const
{
  return this->opname_;
}

int
Test_Fixed_Struct::init_parameters (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  this->in_ = gen->gen_fixed_struct ();
  ACE_OS::memset (&this->inout_, 0, sizeof (Param_Test::Fixed_Struct));
  return 0;
}

int
Test_Fixed_Struct::reset_parameters (void)
{
  ACE_OS::memset (&this->inout_, 0, sizeof (Param_Test::Fixed_Struct));
  ACE_OS::memset (&this->out_, 0, sizeof (Param_Test::Fixed_Struct));
  ACE_OS::memset (&this->ret_, 0, sizeof (Param_Test::Fixed_Struct));
  return 0;
}

int
Test_Fixed_Struct::run_sii_test (Param_Test_ptr objref,
                                 CORBA::Environment &env)
{
  this->ret_ = objref->test_fixed_struct (this->in_, this->inout_, this->out_,
                                          env);
  return (env.exception () ? -1:0);
}

int
Test_Fixed_Struct::add_args (CORBA::NVList_ptr &param_list,
                             CORBA::NVList_ptr &retval,
                             CORBA::Environment &env)
{
  // the Any does not own any of these
  CORBA::Any in_arg (Param_Test::_tc_Fixed_Struct, &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_Fixed_Struct, &this->inout_, 0);
  CORBA::Any out_arg (Param_Test::_tc_Fixed_Struct, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Fixed_Struct,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Fixed_Struct::check_validity (void)
{
  if (this->in_.l == this->inout_.l &&
      this->in_.c == this->inout_.c &&
      this->in_.s == this->inout_.s &&
      this->in_.o == this->inout_.o &&
      this->in_.f == this->inout_.f &&
      this->in_.b == this->inout_.b &&
      this->in_.d == this->inout_.d &&
      this->in_.l == this->out_.l &&
      this->in_.c == this->out_.c &&
      this->in_.s == this->out_.s &&
      this->in_.o == this->out_.o &&
      this->in_.f == this->out_.f &&
      this->in_.b == this->out_.b &&
      this->in_.d == this->out_.d &&
      this->in_.l == this->ret_.l &&
      this->in_.c == this->ret_.c &&
      this->in_.s == this->ret_.s &&
      this->in_.o == this->ret_.o &&
      this->in_.f == this->ret_.f &&
      this->in_.b == this->ret_.b &&
      this->in_.d == this->ret_.d)
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Fixed_Struct::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = *(Param_Test::Fixed_Struct *) req->arguments ()->item (1, env)
    ->value ()->value ();
  this->out_ = *(Param_Test::Fixed_Struct *) req->arguments ()->item (2, env)
    ->value ()->value ();
  this->ret_ = *(Param_Test::Fixed_Struct *)req->result ()->value ()->value ();
  return this->check_validity ();
}

void
Test_Fixed_Struct::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "inout = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "out = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "ret = {\n"
              "\tl = %d\n"
              "\tc = %c\n"
              "\ts = %d\n"
              "\to = %x\n"
              "\tf = %f\n"
              "\tb = %d\n"
              "\td = %f\n"
              "}\n"
              "=*=*=*=*=*=*\n",
              this->in_.l,
              this->in_.c,
              this->in_.s,
              this->in_.o,
              this->in_.f,
              this->in_.b,
              this->in_.d,
              this->inout_.l,
              this->inout_.c,
              this->inout_.s,
              this->inout_.o,
              this->inout_.f,
              this->inout_.b,
              this->inout_.d,
              this->out_.l,
              this->out_.c,
              this->out_.s,
              this->out_.o,
              this->out_.f,
              this->out_.b,
              this->out_.d,
              this->ret_.l,
              this->ret_.c,
              this->ret_.s,
              this->ret_.o,
              this->ret_.f,
              this->ret_.b,
              this->ret_.d));
}

// ************************************************************************
//               Test_String_Sequence
// ************************************************************************

Test_String_Sequence::Test_String_Sequence (void)
  : opname_ (CORBA::string_dup ("test_strseq")),
    in_ (new Param_Test::StrSeq),
    inout_ (new Param_Test::StrSeq),
    out_ (0),
    ret_ (0)
{
}

Test_String_Sequence::~Test_String_Sequence (void)
{
  CORBA::string_free (this->opname_);
  // the other data members will be freed as they are "_var"s
}

const char *
Test_String_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_String_Sequence::init_parameters (Param_Test_ptr objref,
                                       CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  const char *choiceList[] =
  {
    "one",
    "two",
    "three"
  };

  CORBA::ULong len = sizeof(choiceList)/sizeof(char *);

  // get some sequence length (not more than 10)
  //  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_->length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_->length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      //      char *str = gen->gen_string ();
      //this->in_[i] = str;
      this->in_[i] = choiceList[i];

    }
  return 0;
}

int
Test_String_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::StrSeq; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_String_Sequence::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::StrSeq_out out (this->out_.out ());
  this->ret_ = objref->test_strseq (this->in_.in (),
                                    this->inout_.inout (),
                                    out,
                                    env);
  return (env.exception () ? -1:0);
}

int
Test_String_Sequence::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_StrSeq, (void *) &this->in_.in (), 0);
  CORBA::Any inout_arg (Param_Test::_tc_StrSeq, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_StrSeq, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_StrSeq,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_String_Sequence::check_validity (void)
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
          if (ACE_OS::strcmp (this->in_[i], this->inout_[i]) ||
              ACE_OS::strcmp (this->in_[i], this->out_[i]) ||
              ACE_OS::strcmp (this->in_[i], this->ret_[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_String_Sequence::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = new Param_Test::StrSeq (*(Param_Test::StrSeq *) req->arguments
                                         ()->item (1, env)->value ()->value ());
  this->out_ = new Param_Test::StrSeq (*(Param_Test::StrSeq *) req->arguments
                                       ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::StrSeq (*(Param_Test::StrSeq *)req->result
                                       ()->value ()->value ());
  return this->check_validity ();
}

void
Test_String_Sequence::print_values (void)
{
  CORBA::ULong i;
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->in_.ptr () && (i < this->in_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->in_[i].in ()? (char *)this->in_[i].in ():"<nul>")));
    }
  if (!this->in_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->inout_.ptr () && (i < this->inout_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->inout_[i].in ()? (char *)this->inout_[i].in ():"<nul>")));
    }
  if (!this->inout_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\ninout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->out_.ptr () && (i < this->out_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->out_[i].in ()? (char *)this->out_[i].in ():"<nul>")));
    }
  if (!this->out_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nout sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
  for (i=0; this->ret_.ptr () && (i < this->ret_->length ()); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "in : %s\n",
                  i,
                  (this->ret_[i].in ()? (char *)this->ret_[i].in ():"<nul>")));
    }
  if (!this->ret_.ptr ())
    ACE_DEBUG ((LM_DEBUG, "\nin sequence is NUL\n"));
  ACE_DEBUG ((LM_DEBUG, "\n*=*=*=*=*=*=*=*=*=*=\n"));
}

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
  this->in_.dummy = gen->gen_string ();
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
Test_Var_Struct::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_StrSeq, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_StrSeq, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_StrSeq, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Var_Struct,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Var_Struct::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((!ACE_OS::strcmp (this->in_.dummy, this->inout_->dummy)) &&
      (!ACE_OS::strcmp (this->in_.dummy, this->out_->dummy)) &&
      (!ACE_OS::strcmp (this->in_.dummy, this->ret_->dummy)) &&
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
  CORBA::Environment env;
  this->inout_ = new Param_Test::Var_Struct (*(Param_Test::Var_Struct *) req->arguments
                                         ()->item (1, env)->value ()->value ());
  this->out_ = new Param_Test::Var_Struct (*(Param_Test::Var_Struct *) req->arguments
                                       ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::Var_Struct (*(Param_Test::Var_Struct *)req->result
                                       ()->value ()->value ());
  return this->check_validity ();
}

void
Test_Var_Struct::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n*=*=*=*=*=*=*=*=*=*=\n"
              "in_.dummy = %s\n"
              "inout_.dummy = %s\n"
              "out_.dummy = %s\n"
              "ret_.dummy = %s\n",
              this->in_.dummy.in (),
              this->inout_->dummy.in (),
              this->out_->dummy.in (),
              this->ret_->dummy.in ()));

  for (CORBA::ULong i=0; i < this->in_.seq.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n*=*=*=*=*=*=*=*=*=*=\n"
                  "Element # %d\n"
                  "in (len = %d): %s\n"
                  "inout (len = %d): %s\n"
                  "out (len = %d): %s\n"
                  "ret (len = %d): %s\n",
                  this->in_.seq.length (),
                  (this->in_.seq.length ()? (char *)this->in_.seq[i]:"<nul>"),
                  this->inout_->seq.length (),
                  (this->inout_->seq.length ()? (char *)this->inout_->seq[i]:"<nul>"),
                  this->out_->seq.length (),
                  (this->out_->seq.length ()? (char *)this->out_->seq[i]:"<nul>"),
                  this->ret_->seq.length (),
                  (this->ret_->seq.length ()? (char *)this->ret_->seq[i]:"<nul>")));
    }
}

// ************************************************************************
//               Test_Nested_Struct
// ************************************************************************

Test_Nested_Struct::Test_Nested_Struct (void)
  : opname_ (CORBA::string_dup ("test_nested_struct")),
    inout_ (new Param_Test::Nested_Struct),
    out_ (0),
    ret_ (0)
{
}

Test_Nested_Struct::~Test_Nested_Struct (void)
{
  CORBA::string_free (this->opname_);
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Nested_Struct::opname (void) const
{
  return this->opname_;
}

int
Test_Nested_Struct::init_parameters (Param_Test_ptr objref,
                                     CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.vs.seq.length (len);
  // now set each individual element
  for (CORBA::ULong i=0; i < this->in_.vs.seq.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      char *str = gen->gen_string ();
      this->in_.vs.seq[i] = str;
    }
  return 0;
}

int
Test_Nested_Struct::reset_parameters (void)
{
  this->inout_ = new Param_Test::Nested_Struct; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Nested_Struct::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::Nested_Struct_out out (this->out_.out ());
  this->ret_ = objref->test_nested_struct (this->in_,
                                           this->inout_.inout (),
                                           out,
                                           env);
  return (env.exception () ? -1:0);
}

int
Test_Nested_Struct::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_StrSeq, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_StrSeq, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_StrSeq, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Nested_Struct,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (void)
{
  CORBA::Boolean flag = 0;
  if ((this->in_.vs.seq.length () == this->inout_->vs.seq.length ()) &&
      (this->in_.vs.seq.length () == this->out_->vs.seq.length ()) &&
      (this->in_.vs.seq.length () == this->ret_->vs.seq.length ()))
    {
      flag = 1; // assume all are equal
      // lengths are same. Now compare the contents
      for (CORBA::ULong i=0; i < this->in_.vs.seq.length () && flag; i++)
        {
          if (ACE_OS::strcmp (this->in_.vs.seq[i], this->inout_->vs.seq[i]) ||
              ACE_OS::strcmp (this->in_.vs.seq[i], this->out_->vs.seq[i]) ||
              ACE_OS::strcmp (this->in_.vs.seq[i], this->ret_->vs.seq[i]))
            // not equal
            flag = 0;
        }
    }
  return flag;
}

CORBA::Boolean
Test_Nested_Struct::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = new Param_Test::Nested_Struct (*(Param_Test::Nested_Struct *)
                                                req->arguments ()->item
                                                (1, env)->value ()->value ());
  this->out_ = new Param_Test::Nested_Struct (*(Param_Test::Nested_Struct *) req->arguments
                                              ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::Nested_Struct (*(Param_Test::Nested_Struct *)req->result
                                              ()->value ()->value ());
  return this->check_validity ();
}

void
Test_Nested_Struct::print_values (void)
{
  for (CORBA::ULong i=0; i < this->in_.vs.seq.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n*=*=*=*=*=*=*=*=*=*=\n"
                  "Element # %d\n"
                  "in (len = %d): %s\n"
                  "inout (len = %d): %s\n"
                  "out (len = %d): %s\n"
                  "ret (len = %d): %s\n",
                  this->in_.vs.seq.length (),
                  (this->in_.vs.seq.length ()? (char *)this->in_.vs.seq[i]:"<nul>"),
                  this->inout_->vs.seq.length (),
                  (this->inout_->vs.seq.length ()? (char *)this->inout_->vs.seq[i]:"<nul>"),
                  this->out_->vs.seq.length (),
                  (this->out_->vs.seq.length ()? (char *)this->out_->vs.seq[i]:"<nul>"),
                  this->ret_->vs.seq.length (),
                  (this->ret_->vs.seq.length ()? (char *)this->ret_->vs.seq[i]:"<nul>")));
    }
}

// ************************************************************************
//               Test_ObjRef
// ************************************************************************

Test_ObjRef::Test_ObjRef (void)
  : opname_ (CORBA::string_dup ("test_objref"))
{
}

Test_ObjRef::~Test_ObjRef (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_ObjRef::opname (void) const
{
  return this->opname_;
}

static char *Coffee_Flavor [] = {
  "Italian Roast",
  "Irish Creme",
  "Costa Rican",
  "Colombian Supremo",
  "Macademia Nut",
  "Swiss Chocolate Mocha"
};

int
Test_ObjRef::init_parameters (Param_Test_ptr objref,
                              CORBA::Environment &env)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // first get a Coffee object
  this->in_ = objref->make_coffee (env);
  if (env.exception ())
    {
      env.print_exception ("make_coffee");
      return -1;
    }

  // get some sequence length (not more than 10)
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];
  // set the attribute of the object
  this->in_->description (desc, env); // set the attribute for the in object
  if (env.exception ())
    {
      env.print_exception ("set coffee attribute");
      return -1;
    }
  return 0;
}

int
Test_ObjRef::reset_parameters (void)
{
  CORBA::Environment env;
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  desc.name = Coffee_Flavor [index];
  // set the attribute of the object
  this->in_->description (desc, env); // set the attribute for the in object
  if (env.exception ())
    {
      env.print_exception ("set coffee attribute");
      return -1;
    }

  this->inout_ = Coffee::_nil ();
  this->out_ =  Coffee::_nil ();
  this->ret_ =  Coffee::_nil ();
  return 0;
}

int
Test_ObjRef::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  Coffee_out out (this->out_.out ());
  this->ret_ = objref->test_objref (this->in_.in (),
                                    this->inout_.inout (),
                                    out,
                                    env);
  return (env.exception () ? -1:0);
}

int
Test_ObjRef::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  CORBA::Any in_arg (_tc_Coffee, &this->in_, 0);
  CORBA::Any inout_arg (_tc_Coffee, &this->inout_, 0);
  CORBA::Any out_arg (_tc_Coffee, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("o1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("o2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("o3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (_tc_Coffee,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_ObjRef::check_validity (void)
{
  CORBA::Environment env;
  char   // attribute names
    *in,
    *inout,
    *out,
    *ret;

  in = this->in_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }

  inout = this->inout_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }

  out = this->out_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }

  ret = this->ret_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return 0;
    }

  // now compare them
  if (!ACE_OS::strcmp (in, inout) &&
      !ACE_OS::strcmp (in, out) &&
      !ACE_OS::strcmp (in, ret))
    return 1; // success
  else
    return 0;
}

CORBA::Boolean
Test_ObjRef::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = Coffee::_narrow ((CORBA::Object_ptr) req->arguments ()->item
                                  (1, env)->value ()->value (), env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  this->out_ = Coffee::_narrow ((CORBA::Object_ptr) req->arguments ()->item
                                (2, env)->value ()->value (), env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  this->ret_ = Coffee::_narrow ((CORBA::Object_ptr)req->result ()->value
                                ()->value (), env);
  if (env.exception ())
    {
      env.print_exception ("_narrow from DII result");
      return 0;
    }

  return this->check_validity ();
}

void
Test_ObjRef::print_values (void)
{
  CORBA::Environment env;
  char   // attribute names
    *in,
    *inout,
    *out,
    *ret;

  in = this->in_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }

  inout = this->inout_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }

  out = this->out_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }

  ret = this->ret_->description (env)->name;
  if (env.exception ())
    {
      env.print_exception ("retrieving description");
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*"
              "in = %s, "
              "inout = %s, "
              "out = %s, "
              "ret = %s*=*=*=*=*=\n",
              in,
              inout,
              out,
              ret));
}
