
//=============================================================================
/**
 *  @file    MT_Object_i.cpp
 *
 *  This class implements the Object A  of the
 *  Nested Upcalls - MT Client test.
 *
 *  @author Michael Kircher
 */
//=============================================================================

#include "MT_Object_i.h"

CORBA::Long
MT_Object_i::yadda (CORBA::Long hop_count,
                    MT_Object_ptr partner)
{
  constexpr CORBA::Long max_hop_count = 20;
  if (hop_count < max_hop_count)
    {
      if (partner != nullptr)
        return partner->yadda (hop_count + 1,
                               this->_this ()) + 1;
    }

  return 0;
}
