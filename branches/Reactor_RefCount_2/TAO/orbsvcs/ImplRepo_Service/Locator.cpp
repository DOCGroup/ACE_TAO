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

#include "Locator.h"
#include "ImR_Activator_i.h"

ImR_Locator::ImR_Locator (ImR_Activator_i *repo)
  :  repo_ (repo)
{
}

char *
ImR_Locator::locate (const char *object_key
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound))
{
  ACE_CString key (object_key);
  return this->repo_->find_ior (key ACE_ENV_ARG_PARAMETER);
}
