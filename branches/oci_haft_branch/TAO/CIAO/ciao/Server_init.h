// $Id$

/**
 * @file Server_init.h
 *
 * Initializing CIAO Server side ORB, if they need to.
 * I'm also puting some utilities functions here.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_SERVER_INIT_H
#define CIAO_SERVER_INIT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "CIAO_Server_Export.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * The initialize routine for any server that need to access
   * component.  This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_SERVER_Export int Server_init (CORBA::ORB_ptr o);

  namespace Utility
  {
    /// Write a string (usually a stringified IOR) to a file
    /// designated by the @c pathname.  The file named will always get
    /// overwritten.
    CIAO_SERVER_Export int write_IOR (const char *pathname,
                                      const char *IOR);
  }
}

#include "ace/post.h"
#endif /* CIAO_SERVER_INIT_H */
