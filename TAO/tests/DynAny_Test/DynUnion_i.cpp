// $Id$

// ====================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynUnion_i.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ====================================================================

#include "DynAny_i.h"
#include "DynUnion_i.h"

//////////////////////////////////////////////////////////////////////
// Constructors and destructor

DynUnion_i::DynUnion_i (const CORBA_Any& any)
{
  CORBA::Environment env;

  CORBA::TypeCode_ptr arg_tc = any.type ();

  if (DynAny_i::unalias (arg_tc, env) == CORBA::tk_union)
    {
      CORBA::TypeCode_ptr disc_tc = arg_tc->discriminator_type (env);

      // Get the CDR stream of the argument.
      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (any._tao_get_cdr ());

      TAO_InputCDR cdr (mb);

      CORBA_Any disc_any (disc_tc,
                          cdr.start ());

      cdr.skip (disc_tc);

      this->discriminator_ = DynAny_i::create_dyn_any (disc_any,
                                                       env);

      CORBA::ULong count = arg_tc->member_count (env);
      CORBA::ULong i, index;

      for (i = 0; i < count; i++)
        {
          CORBA_Any label_any = *arg_tc->member_label (i,
                                                       env);

          if (label_any.type ()->kind (env) == CORBA::tk_octet)
            {
              index = arg_tc->default_index (env);

              break;
            }
          else if (0 /* TODO: Code for comparison of any values goes here */)
            {
              index = i;

              break;
            }
        }

      if (i == count)
        env.exception (new DynAny_i::InconsistentTypeCode);
      else
        {
          CORBA_Any member_any (arg_tc->member_type (index,
                                                     env),
                                cdr.start ());

          this->member_ = DynAny_i::create_dyn_any (member_any,
                                                    env);
        }
    }
  else
    env.exception (new DynAny_i::InconsistentTypeCode);
}

DynUnion_i::DynUnion_i (CORBA_TypeCode_ptr tc)
{
  // TODO
}

DynUnion_i::~DynUnion_i (void)
{
}

/////////////////////////////////////////////////////////////////////
// Functions specific to DynUnion

CORBA::Boolean 
DynUnion_i::set_as_default (CORBA::Environment& env)
{
  // TODO
  return 0;
}

DynAny_ptr 
DynUnion_i::discriminator (CORBA::Environment& env)
{
  // TODO
  return 0;
}

TCKind 
DynUnion_i::discriminator_kind (CORBA::Environment& env)
{
  // TODO
  return CORBA::tk_null;
}

DynAny_ptr 
DynUnion_i::member (CORBA::Environment& env)
{
  // TODO
  return 0;
}

char* 
DynUnion_i::member_name (CORBA::Environment& env)
{
  // TODO
  return 0;
}

void 
DynUnion_i::member_name (const char* member_name,
                         CORBA::Environment& env)
{
  // TODO
}


TCKind 
DynUnion_i::member_kind (CORBA::Environment& env)
{
  // TODO
  return CORBA::tk_null;
}

