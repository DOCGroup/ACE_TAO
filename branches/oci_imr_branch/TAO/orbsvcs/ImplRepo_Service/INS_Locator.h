// -*- C++ -*-
//=============================================================================
/**
 *  @file   INS_Locator.h
 *
 *  $Id$
 *
 *  @brief  This class implements the ImR's INS Locator class
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_INS_LOCATOR_H
#define IMR_INS_LOCATOR_H
#include /**/ "ace/pre.h"

#include "tao/IORTable/IORTable.h"



class ImR_Locator_i;

/**
 * @class INS_Locator
 *
 * @brief Implementation Repository INS Locator class
 *
 * This class provides a callback for the IORTable to call when it needs
 * to dynamically receive a IOR to forward in response to an INS request.
 */
class INS_Locator : public virtual IORTable::Locator
{
public:
  INS_Locator (ImR_Locator_i& loc);

  /// Locate the appropriate IOR.
  char* locate (const char *object_key ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound));

private:
  ImR_Locator_i& imr_locator_;
};

#include /**/ "ace/post.h"
#endif 
