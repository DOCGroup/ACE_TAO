// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_objref_seq.cpp
//
// = DESCRIPTION
//    tests unbounded objref sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

#include "ub_objref_seq.h"

// ************************************************************************
//               Test_ObjRef_Sequence
// ************************************************************************

static const char *Coffee_Flavor [] = {
  "Italian Roast",
  "Irish Creme",
  "Costa Rican",
  "Colombian Supremo",
  "Macademia Nut",
  "Swiss Chocolate Mocha"
};

Test_ObjRef_Sequence::Test_ObjRef_Sequence (void)
  : opname_ (CORBA::string_dup ("test_objref_sequence")),
    inout_ (new Param_Test::Coffee_Mix),
    out_ (0),
    ret_ (0)
{
}

Test_ObjRef_Sequence::~Test_ObjRef_Sequence (void)
{
  CORBA::string_free (this->opname_);
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_ObjRef_Sequence::opname (void) const
{
  return this->opname_;
}

int
Test_ObjRef_Sequence::init_parameters (Param_Test_ptr objref,
                                     CORBA::Environment &env)
{
  Coffee::Desc desc;
  Generator *gen = GENERATOR::instance (); // value generator

  // get some sequence length (not more than 10)
  CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 10) + 1;

  // set the length of the sequence
  this->in_.length (len);
  // now set each individual element
  for (CORBA::ULong i = 0; i < this->in_.length (); i++)
    {
      // generate some arbitrary string to be filled into the ith location in
      // the sequence
      this->in_[i] = objref->make_coffee (env);
      if (env.exception ())
          {
        env.print_exception ("make_coffee");
        return -1;
          }
      // get some sequence length (not more than 10)
          CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);
          desc.name = Coffee_Flavor [index];
          // set the attribute of the object
//        this->in_[i]->description (desc, env); // set the attribute for the in object
          Coffee_ptr tmp = this->in_[i];
          tmp->description (desc, env);

          if (env.exception ())
            {
              env.print_exception ("set coffee attribute");
              return -1;
            }
    }
  return 0;
}

int
Test_ObjRef_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Coffee_Mix; // delete the previous one
  this->out_ = 0;
  this->ret_ = 0;
  return 0;
}

int
Test_ObjRef_Sequence::run_sii_test (Param_Test_ptr objref,
                                    CORBA::Environment &env)
{
  Param_Test::Coffee_Mix_out out (this->out_.out ());
  this->ret_ = objref->test_coffe_mix (this->in_,
                                       this->inout_.inout (),
                                       out,
                                       env);
  return (env.exception () ? -1:0);
}

int
Test_ObjRef_Sequence::add_args (CORBA::NVList_ptr param_list,
                                CORBA::NVList_ptr retval,
                                CORBA::Environment &env)
{
  CORBA::Any in_arg (Param_Test::_tc_Coffee_Mix, (void *) &this->in_, 0);
  CORBA::Any inout_arg (Param_Test::_tc_Coffee_Mix, &this->inout_.inout (), 0);
  CORBA::Any out_arg (Param_Test::_tc_Coffee_Mix, this->out_.out (), 0);

  // add parameters
  (void)param_list->add_value ("s1", in_arg, CORBA::ARG_IN, env);
  (void)param_list->add_value ("s2", inout_arg, CORBA::ARG_INOUT, env);
  (void)param_list->add_value ("s3", out_arg, CORBA::ARG_OUT, env);

  // add return value
  (void)retval->item (0, env)->value ()->replace (Param_Test::_tc_Coffee_Mix,
                                                  &this->ret_,
                                                  0, // does not own
                                                  env);
  return 0;
}

CORBA::Boolean
Test_ObjRef_Sequence::check_validity (void)
{
  TAO_TRY
  {
    if (this->compare (this->in_, this->inout_.in (), TAO_TRY_ENV))
    {
      TAO_CHECK_ENV;
      if (this->compare (this->in_, this->out_.in (), TAO_TRY_ENV))
      {
        TAO_CHECK_ENV;
        if (this->compare (this->in_, this->ret_.in (), TAO_TRY_ENV))
        {
          TAO_CHECK_ENV;
          return 1;
        }
      }
    }
    return 0;
  }
  TAO_CATCHANY;
  {
    return 0;
  }
  TAO_ENDTRY;
}

CORBA::Boolean
Test_ObjRef_Sequence::check_validity (CORBA::Request_ptr req)
{
#if 0
  CORBA::Environment env;
  this->inout_ = new Param_Test::Coffee_Mix (*(Param_Test::Coffee_Mix *)
                                                req->arguments ()->item
                                                (1, env)->value ()->value ());
  this->out_ = new Param_Test::Coffee_Mix (*(Param_Test::Coffee_Mix *) req->arguments
                                              ()->item (2, env)->value ()->value ());
  this->ret_ = new Param_Test::Coffee_Mix (*(Param_Test::Coffee_Mix *)req->result
                                              ()->value ()->value ());
#endif
  return this->check_validity ();
}

void
Test_ObjRef_Sequence::print_values (void)
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
Test_ObjRef_Sequence::compare (const Param_Test::Coffee_Mix &s1,
                               const Param_Test::Coffee_Mix &s2,
                                                           CORBA::Environment& env)
{
  if (s1.maximum () != s2.maximum ())
      return 0;
  if (s1.length () != s2.length ())
    return 0;

  for (CORBA::ULong i=0; i < s1.length (); i++)
    {
      const Coffee_ptr vs1 = s1[i];
      const Coffee_ptr vs2 = s2[i];

      char* n1 = vs1->description (env)->name;
      if (env.exception ())
                {
                  env.print_exception ("retrieving description for vs1");
                  return 0;
                }
          char* n2 = vs2->description (env)->name;
          if (env.exception ())
            {
              env.print_exception ("retrieving description for vs2");
              return 0;
            }
          if (!ACE_OS::strcmp(n1, n2))
            return 0;
    }

  return 1; // success
}

void
Test_ObjRef_Sequence::print_sequence (const Param_Test::Coffee_Mix &s)
{
  ACE_DEBUG ((LM_DEBUG,
              "maximum = %d\n"
              "length = %d\n",
              s.maximum (),
              s.length ()));
  ACE_DEBUG ((LM_DEBUG, "Elements -\n"));
  for (CORBA::ULong i=0; i < s.length (); i++)
    {
    /* const Coffee_ptr vs = s[i];

      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\tdesc = %d\n"
                  i,
                  vs.description(env).name)); */
    }
}
