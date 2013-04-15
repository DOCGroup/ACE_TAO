/* -*- C++ -*- */

// =================================================================
/**
 * @file Async_IORTable.cpp
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 *
 */
// =================================================================

#include "tao/Async_IORTable/Async_IORTable.h"
#include "tao/Async_IORTable/Async_Table_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Async_IORTable_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Table_Adapter_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
