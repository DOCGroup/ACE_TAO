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

ACE_RCSID(Param_Test, ub_objref_seq, "$Id$")

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
  : opname_ (CORBA::string_dup ("test_coffe_mix")),
    inout_ (new Param_Test::Coffee_Mix),
    out_ (new Param_Test::Coffee_Mix),
    ret_ (new Param_Test::Coffee_Mix)
{
}

Test_ObjRef_Sequence::~Test_ObjRef_Sequence (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
  // the other data members will be freed as they are "_var"s and objects
  // (rather than pointers to objects)
}

const char *
Test_ObjRef_Sequence::opname (void) const
{
  return this->opname_;
}

void
Test_ObjRef_Sequence::dii_req_invoke (CORBA::Request *req
                                      TAO_ENV_ARG_DECL)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_.in ();
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (Param_Test::_tc_Coffee_Mix);

  req->invoke (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  Param_Test::Coffee_Mix *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new Param_Test::Coffee_Mix (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o2->value () >>= tmp;
  this->inout_ = new Param_Test::Coffee_Mix (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  *o3->value () >>= tmp;
  this->out_ = new Param_Test::Coffee_Mix (*tmp);
}

int
Test_ObjRef_Sequence::init_parameters (Param_Test_ptr objref
                                       TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      Coffee::Desc desc;
      Generator *gen = GENERATOR::instance (); // value generator

      // get some sequence length (not more than 10)
      CORBA::ULong len = (CORBA::ULong) (gen->gen_long () % 5) + 5;

      // set the length of the sequence
      this->in_.length (len);
      // now set each individual element

      for (CORBA::ULong i = 0; i < this->in_.length (); i++)
        {
          // generate some arbitrary string to be filled into the ith location in
          // the sequence
          this->in_[i] = objref->make_coffee (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // select a Coffee flavor at random
          CORBA::ULong index = (CORBA::ULong) (gen->gen_long () % 6);

          desc.name = Coffee_Flavor [index];
          // set the attribute for the in object
          Coffee_ptr tmp = this->in_[i];

          tmp->description (desc
                            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

        return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ObjRef_Sequence::init_parameters\n");

    }
  ACE_ENDTRY;
  return -1;
}

int
Test_ObjRef_Sequence::reset_parameters (void)
{
  this->inout_ = new Param_Test::Coffee_Mix; // delete the previous ones
  this->out_ = new Param_Test::Coffee_Mix;
  this->ret_ = new Param_Test::Coffee_Mix;
  return 0;
}

int
Test_ObjRef_Sequence::run_sii_test (Param_Test_ptr objref
                                    TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      Param_Test::Coffee_Mix_out out (this->out_.out ());

      this->ret_ = objref->test_coffe_mix (this->in_,
                                           this->inout_.inout (),
                                           out
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ObjRef_Sequence::run_sii_test\n");

    }
  ACE_ENDTRY;
  return -1;
}

CORBA::Boolean
Test_ObjRef_Sequence::check_validity (void)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      this->compare (this->in_,
                     this->inout_.in ()
                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->compare (this->in_,
                     this->out_.in ()
                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->compare (this->in_,
                     this->ret_.in ()
                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      return 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ObjRef_Sequence::check_validity");

    }
  ACE_ENDTRY;
  return 0;
}

CORBA::Boolean
Test_ObjRef_Sequence::check_validity (CORBA::Request_ptr )
{
  //ACE_UNUSED_ARG (req);
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
                               const Param_Test::Coffee_Mix &s2
                               TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      if (s1.maximum () != s2.maximum ())
        {
          return 0;
        }
      if (s1.length () != s2.length ())
        {
          return 0;
        }

      for (CORBA::ULong i=0; i < s1.length (); i++)
        {
          Coffee_ptr vs1 = s1[i];
          Coffee_ptr vs2 = s2[i];

          if (CORBA::is_nil (vs1) && CORBA::is_nil (vs2))
            {
              continue;
            }

          if (CORBA::is_nil (vs1) || CORBA::is_nil (vs2))
            {
              return 0;
            }

          Coffee::Desc_var desc1 = vs1->description (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::String_var n1 = desc1->name.in ();

          Coffee::Desc_var desc2 = vs2->description (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::String_var n2 = desc2->name.in ();

          if (ACE_OS::strcmp(n1.in (), n2.in ()) != 0)
            {
              return 0;
            }
        }

      return 1; // success
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_ObjRef_Sequence::compare");

    }
  ACE_ENDTRY;
  return 0;
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
      Coffee_ptr c = s[i];
      if (CORBA::is_nil (c))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Element #%d is nil\n", i));
          continue;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Element #%d\n"
                  "\ttype = <%s>\n",
                  i,
                  c->_interface_repository_id ()));
    }
}
