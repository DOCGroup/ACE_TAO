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

#include "IORTable.h"
#include "Table_Adapter.h"

ACE_RCSID (IORTable, IORTable, "$Id$")

int
TAO_IORTable_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Table_Adapter_Factory);
}
