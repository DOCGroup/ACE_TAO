//=============================================================================
/**
 *  @file   INS_Locator.cpp
 *
 *  $Id$
 *
 *  @brief  Implementation of the ImR's INS Locator class
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "INS_Locator.h"
#include "ImR_Locator_i.h"

INS_Locator::INS_Locator (ImR_Locator_i& loc)
:imr_locator_ (loc)
{
}

char *
INS_Locator::locate (const char* object_key ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound))
{
  ACE_ASSERT(object_key != 0);
  ACE_TRY 
  {
    char* located =
       this->imr_locator_.find_ior_i (object_key ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    return located;
  }
  ACE_CATCH(ImplementationRepository::NotFound, e)
  {
    ACE_TRY_THROW(IORTable::NotFound());
  }
  ACE_ENDTRY;
  return 0;
}
