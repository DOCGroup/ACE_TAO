//=============================================================================
/**
 *  @file     nsconf.h
 *
 *  $Id$
 *
 *  Name service configuration file.
 *
 *
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_NAMESERVICE_CONF_H
#define TAO_NAMESERVICE_CONF_H
#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// TAO Naming Service.

// Memory mapping address used by TAO's Naming Service when run in
// persistent mode.
#if !defined (TAO_NAMING_BASE_ADDR)
#  define TAO_NAMING_BASE_ADDR ACE_DEFAULT_BASE_ADDR
#endif /* ! TAO_NAMING_BASE_ADDR */

// Poa id of the root Naming Context in a Naming server.
#if !defined (TAO_ROOT_NAMING_CONTEXT)
#  define TAO_ROOT_NAMING_CONTEXT "NameService"
#endif /* ! TAO_ROOT_NAMING_CONTEXT */

// The name under which the index of naming contexts is stored in
// persistent naming service.
#if !defined (TAO_NAMING_CONTEXT_INDEX)
#  define TAO_NAMING_CONTEXT_INDEX "Naming_Context_Index"
#endif /* ! TAO_NAMING_CONTEXT_INDEX */


#include /**/ "ace/post.h"
#endif /*TAO_NAMESERVICE_CONF_H*/
