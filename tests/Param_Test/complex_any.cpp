
//=============================================================================
/**
 *  @file    complex_any.cpp
 *
 *  $Id$
 *
 *  tests complex Anys
 *
 *
 *  @author Aniruddha Gokhale
 *  @author Jeff Parsons
 *  @author Frank Buschmann
 */
//=============================================================================


#include "helper.h"
#include "complex_any.h"
#include "tao/debug.h"

const int LEVEL_2_LENGTH = 5;
const int LEVEL_3_LENGTH = 3;

// ************************************************************************
//               Test_Complex_Any
// ************************************************************************

size_t Test_Complex_Any::counter = 0;

Test_Complex_Any::Test_Complex_Any (void)
  : opname_ (CORBA::string_dup ("test_complex_any")),
    out_ (new CORBA::Any),
    ret_ (new CORBA::Any)
{
}

Test_Complex_Any::~Test_Complex_Any (void)
{
  CORBA::string_free (this->opname_);
  this->opname_ = 0;
}

const char *
Test_Complex_Any::opname (void) const
{
  return this->opname_;
}

void
Test_Complex_Any::dii_req_invoke (CORBA::Request *req)
{
  req->add_in_arg ("s1") <<= this->in_;
  req->add_inout_arg ("s2") <<= this->inout_;
  req->add_out_arg ("s3") <<= this->out_.in ();

  req->set_return_type (CORBA::_tc_any);

  req->invoke ();

  const CORBA::Any *tmp;
  req->return_value () >>= tmp;
  this->ret_ = new CORBA::Any (*tmp);

  CORBA::NamedValue_ptr o2 =
    req->arguments ()->item (1);
  *o2->value () >>= tmp;
  this->inout_ = CORBA::Any (*tmp);

  CORBA::NamedValue_ptr o3 =
    req->arguments ()->item (2);
  *o3->value () >>= tmp;
  this->out_ = new CORBA::Any (*tmp);
}

int
Test_Complex_Any::init_parameters (Param_Test_ptr)
{
  return this->reset_parameters ();
}

// This Any is a real-world use cases
// submitted by Frank Buschmann of Siemens. Here's the structure:
//
// Any <- AnySeq <- AnySeq <- struct <- string
//                                   <- Any <- Any <- Any <- struct <- string
//                                                                  <- boolean
//                                                                  <- short
int
Test_Complex_Any::reset_parameters (void)
{
  Generator *gen = GENERATOR::instance (); // value generator

  CORBA::AnySeq level2_seq, level3_seq;
  level2_seq.length (LEVEL_2_LENGTH);
  level3_seq.length (LEVEL_3_LENGTH);

  for (int j = 0; j < LEVEL_2_LENGTH; j++)
    {
      for (int i = 0; i < LEVEL_3_LENGTH; i++)
        {
          Param_Test::level4 level4_struct;

          char *l5_str = gen->gen_string ();

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "setting level 5 string = %s\n",
                        l5_str));

          level4_struct.level5_string = l5_str;

          CORBA::Any level6_any, level7_any;

          Param_Test::level8 level8_struct;

          char *l9_str = gen->gen_string ();

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "setting level 9 string = %s\n",
                        l9_str));

          level8_struct.level9_string = l9_str;

          CORBA::Short l9_bool = ACE_OS::rand () % 2;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "setting level 9 boolean = %d\n",
                        l9_bool));

          level8_struct.level9_boolean = (CORBA::Boolean) l9_bool;

          CORBA::Short l9_short = gen->gen_short ();

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "setting level 9 short = %d\n",
                        l9_short));

          level8_struct.level9_short = l9_short;

          level7_any <<= level8_struct;

          level6_any <<= level7_any;

          level4_struct.level5_any <<= level6_any;

          level3_seq[i] <<= level4_struct;
        }

      level2_seq[j] <<= level3_seq;
    }

  this->in_ <<= level2_seq;

  return 0;
}

int
Test_Complex_Any::run_sii_test (Param_Test_ptr objref)
{
  try
    {
      this->ret_ = objref->test_complex_any (this->in_,
                                             this->inout_,
                                             this->out_.out ());
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

CORBA::Boolean
Test_Complex_Any::check_validity (void)
{
  CORBA::AnySeq *level2_in_seq;
  CORBA::AnySeq *level2_test_seq;

  if (!(this->in_ >>= level2_in_seq))
    return 0;

  if (!(this->inout_ >>= level2_test_seq))
    return 0;

  if (this->comp_engine (level2_in_seq, level2_test_seq) == 0)
    return 0;

  if (!(this->out_.in () >>= level2_test_seq))
    return 0;

  if (this->comp_engine (level2_in_seq, level2_test_seq) == 0)
    return 0;

  if (!(this->ret_.in () >>= level2_test_seq))
    return 0;

  if (this->comp_engine (level2_in_seq, level2_test_seq) == 0)
    return 0;

  return 1;
}

// Private helper function for check_validity.
CORBA::Boolean
Test_Complex_Any::comp_engine (const CORBA::AnySeq *level2_in_seq,
                               const CORBA::AnySeq *level2_comp_seq)
{
  if (level2_in_seq->length() != level2_comp_seq->length())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "mismatch of level 2 sequence lengths\n"));
      return 0;
    }

  for (CORBA::ULong i = 0; i < level2_in_seq->length (); i++)
    {
      CORBA::AnySeq *level3_in_seq;
      CORBA::AnySeq *level3_test_seq;

      if (!((*level2_in_seq)[i] >>= level3_in_seq))
        return 0;
      if (!((*level2_comp_seq)[i] >>= level3_test_seq))
        return 0;

      if (level3_in_seq->length() != level3_test_seq->length())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "mismatch of level 3 sequence lengths\n"));
          return 0;
        }

      for (CORBA::ULong j = 0; j < level3_in_seq->length (); j ++)
        {
          Param_Test::level4 *level4_in_struct;
          Param_Test::level4 *level4_test_struct;

          if (!((*level3_in_seq)[j] >>= level4_in_struct))
            return 0;
          if (!((*level3_test_seq)[j] >>= level4_test_struct))
            return 0;

          if (ACE_OS::strcmp (level4_in_struct->level5_string,
                              level4_test_struct->level5_string))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of level 5 string values\n"));
              return 0;
            }

          const CORBA::Any *level6_in_any;
          const CORBA::Any *level6_test_any;
          const CORBA::Any *level7_in_any;
          const CORBA::Any *level7_test_any;

          if (!(level4_in_struct->level5_any >>= level6_in_any))
            return 0;
          if (!(level4_test_struct->level5_any >>= level6_test_any))
            return 0;

          if (!((*level6_in_any) >>= level7_in_any))
            return 0;
          if (!((*level6_test_any) >>= level7_test_any))
            return 0;

          Param_Test::level8 *level8_in_struct;
          Param_Test::level8 *level8_test_struct;

          if (!((*level7_in_any) >>= level8_in_struct))
            return 0;
          if (!((*level7_test_any) >>= level8_test_struct))
            return 0;

          if (ACE_OS::strcmp (level8_in_struct->level9_string,
                               level8_test_struct->level9_string))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of level 9 string values\n"));
              return 0;
            }

          if (level8_in_struct->level9_boolean != level8_test_struct->level9_boolean)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of level 9 boolean values\n"));
              return 0;
            }

          if (level8_in_struct->level9_short != level8_test_struct->level9_short)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "mismatch of level 9 short values\n"));
              return 0;
            }
        }
    }

  // If we get here, all is correct.
  return 1;
}

CORBA::Boolean
Test_Complex_Any::check_validity (CORBA::Request_ptr /*req*/)
{
  return this->check_validity ();
}

void
Test_Complex_Any::print_values (void)
{
}
