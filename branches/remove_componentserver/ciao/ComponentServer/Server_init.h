// $Id$

/**
 * @file Server_init.h
 *
 * Initializing CIAO Server side ORB, if they need to.
 * I'm also puting some utilities functions here.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_SERVER_INIT_H
#define CIAO_SERVER_INIT_H

#include /**/ "ace/pre.h"

#include "ciao/ComponentServer/CIAO_ComponentServer_stub_export.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  /**
   * The initialize routine for any server that need to access
   * component.  This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_COMPONENTSERVER_STUB_Export int Server_init (CORBA::ORB_ptr o);

  namespace Utility
  {
    /// Write a string (usually a stringified IOR) to a file
    /// designated by the @c pathname.  The file named will always get
    /// overwritten.
    CIAO_COMPONENTSERVER_STUB_Export int write_IOR (const ACE_TCHAR *pathname,
                                              const char *IOR);
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_SERVER_INIT_H */
