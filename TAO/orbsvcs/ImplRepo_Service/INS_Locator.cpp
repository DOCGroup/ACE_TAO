//=============================================================================
/**
 *  @file   Locator.cpp
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

INS_Locator::INS_Locator (ImR_Locator_i* loc)
  :  imr_locator_ (loc)
{
}

char *
INS_Locator::locate (const char* object_key ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound))
{
  return this->imr_locator_->find_ior (object_key ACE_ENV_ARG_PARAMETER);
}
