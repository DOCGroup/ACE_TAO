// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    MT_Object_Impl.cpp
//
// = DESCRIPTION
//    This class implements the Object A  of the 
//    Nested Upcalls - MT Client test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "tao/corba.h"
#include "MT_Object_Impl.h"

ACE_RCSID(MT_Client_Test, MT_Object_Impl, "$Id$")

#define MAX_HOP_COUNT 20

// CTOR
MT_Object_Impl::MT_Object_Impl (void)
{
}

// DTOR
MT_Object_Impl::~MT_Object_Impl (void)
{
}


CORBA::Long
MT_Object_Impl::yadda (CORBA::Long hop_count,
                       MT_Object_ptr partner,
                       CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) MT_Object_Impl::yadda () hop count = %d\n",
              hop_count));

  if (hop_count < MAX_HOP_COUNT)
  {
    if (partner != 0)
      return partner->yadda (hop_count + 1, this->_this (env), env) + 1;
  }

  return 0;
}

