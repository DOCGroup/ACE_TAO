/* -*- C++ -*- */

// =================================================================
/**
 * @file IORTable.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */
// =================================================================

#include "tao/IORTable/IORTable.h"
#include "tao/IORTable/Table_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_IORTable_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Table_Adapter_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
