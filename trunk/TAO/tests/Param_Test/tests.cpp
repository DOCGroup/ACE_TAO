// $Id$


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
  // we provide top level memory to the ORB to retrieve the data
  CORBA::Any in_arg (CORBA::_tc_short, &this->in_, 0);
  CORBA::Any inout_arg (CORBA::_tc_short, &this->inout_, 0);
  CORBA::Any out_arg (CORBA::_tc_short, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value. Let the ORB allocate storage. We simply tell the ORB
  // what type we are expecting.
  (void)retval->item (0, env)->value ()->replace (CORBA::_tc_short,
                                                  0, // no value
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
#if 0
  // commented out since we really don't need to this as we have provided the
  // ORB with the memory
  *req->arguments ()->item (1, env)->value () >>= this->inout_;
  *req->arguments ()->item (2, env)->value () >>= this->out_;
#endif
  // we must retrieve the return value since we aske dthe ORB to allocate the
  // memory.
  *req->result ()->value () >>= this->ret_;
  return this->check_validity ();
}

void
Test_Short::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in = %d, "
              "inout = %d, "
              "out = %d, "
              "ret = %d\n"
              "\n=*=*=*=*=*=*\n",
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
                                                  0, // ORB will allocate
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
  // only retrieve the return value. No need to retrieve the out and inout
  // because we had provided the memory and we own it.
  *req->result ()->value () >>= this->ret_;
  return this->check_validity ();
}

void
Test_Unbounded_String::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n=*=*=*=*=*=*\n"
              "in with len (%d) = %s\n"
              "inout with len (%d) = %s\n"
              "out with len (%d) = %s\n"
              "ret with len (%d) = %s\n"
              "\n=*=*=*=*=*=*\n",
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
  // We provide the top level memory
  // the Any does not own any of these
  CORBA::Any in_arg (Param_Test::_tc_Fixed_Struct, &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_Fixed_Struct, &this->inout_, 0);
  CORBA::Any out_arg (Param_Test::_tc_Fixed_Struct, &this->out_, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value type
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Fixed_Struct,
                                                  0, // let the ORB allocate
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

  // we have forced the ORB to allocate memory for the return value so that we
  // can test the >>= operator
  Param_Test::Fixed_Struct *ret;
  *req->result ()->value () >>= ret;
  this->ret_ = *ret;
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
  // ORB will allocate
  CORBA::Any out_arg (Param_Test::_tc_StrSeq, 0, 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value type
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_StrSeq,
                                                  0,
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

  Param_Test::StrSeq *out, *ret;

  *req->arguments ()->item (2, env)->value () >>= out;
  *req->result ()->value () >>= ret;

  this->out_ = out;
  this->ret_ = ret;

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
                  this->in_[i]? (const char *)this->in_[i]:"<nul>"));
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
                  (this->inout_[i]? (const char *)this->inout_[i]:"<nul>")));
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
                  (this->out_[i]? (const char *)this->out_[i]:"<nul>")));
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
                  (this->ret_[i]? (const char *)this->ret_[i]:"<nul>")));
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
  CORBA::ULong len = (CORBA::ULong) gen->gen_long ();

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
Test_Var_Struct::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Var_Struct, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_Var_Struct, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_Var_Struct, this->out_.out (), 0);

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
  CORBA::Any in_arg (Param_Test::_tc_Nested_Struct, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_Nested_Struct, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_Nested_Struct, this->out_.out (), 0);

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
                  (this->in_.vs.seq.length ()? (const char *)this->in_.vs.seq[i]:"<nul>"),
                  this->inout_->vs.seq.length (),
                  (this->inout_->vs.seq.length ()? (const char *)this->inout_->vs.seq[i]:"<nul>"),
                  this->out_->vs.seq.length (),
                  (this->out_->vs.seq.length ()? (const char *)this->out_->vs.seq[i]:"<nul>"),
                  this->ret_->vs.seq.length (),
                  (this->ret_->vs.seq.length ()? (const char *)this->ret_->vs.seq[i]:"<nul>")));
    }
}

// ************************************************************************
//               Test_Struct_Sequence
// ************************************************************************

Test_Struct_Sequence::Test_Struct_Sequence (void)
  : opname_ (CORBA::string_dup ("test_struct_sequence")),
    inout_ (new Param_Test::StructSeq),
    out_ (0),
    ret_ (0)
{
}

Test_Struct_Sequence::~Test_Struct_Sequence (void)
{
  CORBA::string_free (this->opname_);
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_Struct_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_Struct_Sequence::init_parameters (Param_Test_ptr objref,
                                     CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator

  ACE_UNUSED_ARG (objref);
  ACE_UNUSED_ARG (env);

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.length (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = gen->gen_fixed_struct ();
    }
  return 0;
}

int
Test_Struct_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::StructSeq; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_Struct_Sequence::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::StructSeq_out out (this->out_.out ());
  this->ret_ = objref->test_struct_sequence (this->in_,
                                           this->inout_.inout (),
                                           out,
                                           env);
  return (env.exception () ? -1:0);
}

int
Test_Struct_Sequence::add_args (CORBA::NVList_ptr &param_list,
                                CORBA::NVList_ptr &retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_StructSeq, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_StructSeq, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_StructSeq, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_StructSeq,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_Struct_Sequence::check_validity (void)
{
  if (this->compare (this->in_, this->inout_.in ()) &&
      this->compare (this->in_, this->out_.in ()) &&
      this->compare (this->in_, this->ret_.in ()))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_Struct_Sequence::check_validity (CORBA::Request_ptr req)
{
  CORBA::Environment env;
  this->inout_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *)
                                                req->arguments ()->item
                                                (1, env)->value ()->value ());
  this->out_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *) req->arguments
                                              ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::StructSeq (*(Param_Test::StructSeq *)req->result
                                              ()->value ()->value ());
  return this->check_validity ();
}

void
Test_Struct_Sequence::print_values (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "IN sequence\n"));
  this->print_sequence (this->in_);
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "INOUT sequence\n"));
  this->print_sequence (this->inout_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "OUT sequence\n"));
  this->print_sequence (this->out_.in ());
  ACE_DEBUG ((LM_DEBUG,
              "*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n"
              "RET sequence\n"));
  this->print_sequence (this->ret_.in ());
}

CORBA::Boolean
Test_Struct_Sequence::compare (const Param_Test::StructSeq &s1,
                               const Param_Test::StructSeq &s2)
{
  if (s1.maximum () != s2.maximum ())
      return 0;
  if (s1.length () != s2.length ())
    return 0;

  for (CORBA::ULong i=0; i < s1.length (); i++)
    {
      const Param_Test::Fixed_Struct& vs1 = s1[i];
      const Param_Test::Fixed_Struct& vs2 = s2[i];

      if (vs1.l != vs2.l
          || vs1.c != vs2.c
          || vs1.s != vs2.s
          || vs1.o != vs2.o
          || vs1.f != vs2.f
          || vs1.b != vs2.b
          || vs1.d != vs2.d )
        return 0;
    }

  return 1; // success
}

void
Test_Struct_Sequence::print_sequence (const Param_Test::StructSeq &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i=0; i < s.length (); i++)
    {
      const Param_Test::Fixed_Struct& vs = s[i];

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

static const char *Coffee_Flavor [] = {
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

// ************************************************************************
//               Test_TypeCode
// ************************************************************************

Test_TypeCode::Test_TypeCode (void)
  : opname_ (CORBA::string_dup ("test_typecode"))
{
}

Test_TypeCode::~Test_TypeCode (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_TypeCode::opname (void) const
{
  return this->opname_;
}

static const CORBA::TypeCode_ptr tc_table [] = {
  // primitive parameterless typecodes
  CORBA::_tc_short,
  // typecode with a simple parameter
  CORBA::_tc_string,
  // complex typecodes
  CORBA::_tc_Object,
  _tc_Param_Test,
  Param_Test::_tc_StructSeq,
  Param_Test::_tc_Nested_Struct
};

int
Test_TypeCode::init_parameters (Param_Test_ptr objref,
                                CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);

  this->in_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  return 0;
}

int
Test_TypeCode::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
  this->in_ = CORBA::TypeCode::_duplicate (tc_table [index]);
  this->inout_ = CORBA::TypeCode::_duplicate (CORBA::_tc_null);
  return 0;
}

int
Test_TypeCode::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::TypeCode_out out (this->out_.out ());
  this->ret_ = objref->test_typecode (this->in_.in (),
                                      this->inout_.inout (),
                                      out,
                                      env);
  return (env.exception () ? -1:0);
}

int
Test_TypeCode::add_args (CORBA::NVList_ptr &param_list,
                      CORBA::NVList_ptr &retval,
                      CORBA::Environment &env)
{
  return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (void)
{
  CORBA::Environment env;
  if (this->in_->equal (this->inout_.in (), env) &&
      this->in_->equal (this->out_.in (), env) &&
      this->in_->equal (this->ret_.in (), env))
    return 1;
  else
    return 0;
}

CORBA::Boolean
Test_TypeCode::check_validity (CORBA::Request_ptr req)
{
  return this->check_validity ();
}

void
Test_TypeCode::print_values (void)
{
}

// ************************************************************************
//               Test_Any
// ************************************************************************

Test_Any::Test_Any (void)
  : opname_ (CORBA::string_dup ("test_any"))
{
}

Test_Any::~Test_Any (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Any::opname (void) const
{
  return this->opname_;
}

static const CORBA::TypeCode_ptr any_table [] =
{
  // primitive parameterless typecodes
  CORBA::_tc_short,
  // typecode with a simple parameter
  CORBA::_tc_string,
  // complex typecodes
  _tc_Coffee,
  Param_Test::_tc_StrSeq,
  Param_Test::_tc_StructSeq,
  Param_Test::_tc_Nested_Struct
};

int
Test_Any::init_parameters (Param_Test_ptr objref,
                           CORBA::Environment &env)
{
  Generator *gen = GENERATOR::instance (); // value generator
  //  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 3);
  CORBA::ULong index = 2;

  switch (index)
    {
    case 0:
      {
        CORBA::Short s;
        s = gen->gen_short ();
        ACE_DEBUG ((LM_DEBUG, "setting short = %d\n", s));
        this->in_ <<= s;
        this->inout_ <<= s;
      }
      break;
    case 1:
      {
        char *str = gen->gen_string ();
        ACE_DEBUG ((LM_DEBUG, "setting string = %s\n", str));
        this->in_ <<= str;
        this->inout_ <<= str;
      }
      break;
    case 2:
      {
        TAO_TRY
          {
            // get access to a Coffee Object
            Coffee_var cobj = objref->make_coffee (TAO_TRY_ENV);
            TAO_CHECK_ENV;

            // insert the coffee object into the Any
            this->in_ <<= cobj.in ();
            this->inout_ <<= cobj.in ();
          }
        TAO_CATCH (CORBA::SystemException, sysex)
          {
            ACE_UNUSED_ARG (sysex);
            TAO_TRY_ENV.print_exception ("System Exception doing make_coffee");
            return -1;
          }
        TAO_ENDTRY;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    }
  return 0;
}

int
Test_Any::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator
  //  CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 2);
  CORBA::ULong index = 2;

  switch (index)
    {
    case 0:
      {
        CORBA::Short s;
        s = gen->gen_short ();
        this->in_ <<= s;
        this->inout_ <<= s;
      }
      break;
    case 1:
      {
        char *str = gen->gen_string ();
        this->in_ <<= str;
        this->inout_ <<= str;
      }
      break;
    case 2:
      {
        this->inout_ = this->in_;
      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    }
  return 0;
}

int
Test_Any::run_sii_test (Param_Test_ptr objref,
                          CORBA::Environment &env)
{
  CORBA::Any_out out (this->out_.out ());
  this->ret_ = objref->test_any (this->in_,
                                 this->inout_,
                                 out,
                                 env);
  return (env.exception () ? -1:0);
}

int
Test_Any::add_args (CORBA::NVList_ptr &param_list,
                    CORBA::NVList_ptr &retval,
                    CORBA::Environment &env)
{
  return 0;
}

CORBA::Boolean
Test_Any::check_validity (void)
{
  CORBA::Environment env;
  CORBA::Short short_in, short_inout, short_out, short_ret;
  char *str_in, *str_inout, *str_out, *str_ret;
  Coffee_ptr obj_in, obj_inout, obj_out, obj_ret;

  if ((this->in_ >>= short_in) &&
      (this->inout_ >>= short_inout) &&
      (this->out_.in () >>= short_out) &&
      (this->ret_.in () >>= short_ret))
    {
      ACE_DEBUG ((LM_DEBUG, "Received shorts: in = %d, "
                  "inout = %d, out = %d, ret = %d\n",
                  short_in, short_inout, short_out, short_ret));

      if ((short_in == short_inout) &&
          (short_in == short_out) &&
          (short_in == short_ret))
        return 1;
      else
        {
          ACE_DEBUG ((LM_DEBUG, "mismatch of short values\n"));
          return 0;
        }
    }
  else if ((this->in_ >>= str_in) &&
           (this->inout_ >>= str_inout) &&
           (this->out_.in () >>= str_out) &&
           (this->ret_.in () >>= str_ret))
    {
      if (!ACE_OS::strcmp (str_in, str_inout) &&
          !ACE_OS::strcmp (str_in, str_out) &&
          !ACE_OS::strcmp (str_in, str_ret))
        return 1;
      else
        {
          ACE_DEBUG ((LM_DEBUG, "mismatch of string values\n"));
          return 0;
        }
    }
  else if ((this->in_ >>= obj_in) &&
           (this->inout_ >>= obj_inout) &&
           (this->out_.in () >>= obj_out) &&
           (this->ret_.in () >>= obj_ret))
    {
      // all the >>= operators returned true so we are OK.
      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
Test_Any::check_validity (CORBA::Request_ptr req)
{
  return this->check_validity ();
}

void
Test_Any::print_values (void)
{
}
