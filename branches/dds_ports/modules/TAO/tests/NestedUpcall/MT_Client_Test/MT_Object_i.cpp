// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    MT_Object_i.cpp
//
// = DESCRIPTION
//    This class implements the Object A  of the
//    Nested Upcalls - MT Client test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "MT_Object_i.h"

#include "tao/Exception.h"

ACE_RCSID(MT_Client_Test, MT_Object_i, "$Id$")

#define MAX_HOP_COUNT 20

// CTOR
MT_Object_i::MT_Object_i (void)
{
}

// DTOR
MT_Object_i::~MT_Object_i (void)
{
}


CORBA::Long
MT_Object_i::yadda (CORBA::Long hop_count,
                       MT_Object_ptr partner)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) MT_Object_i::yadda () hop count = %d\n",
              hop_count));
#endif /*if 0*/

  if (hop_count < MAX_HOP_COUNT)
  {
    if (partner != 0)
      return partner->yadda (hop_count + 1,
                             this->_this ()) + 1;
  }

  return 0;
}
