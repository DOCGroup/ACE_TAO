// $Id$

/**
 * @file Client_init.h
 *
 * Initializing CIAO client side ORB, if they need to.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_CLIENT_INIT_H
#define CIAO_CLIENT_INIT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "CIAO_Client_Export.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
#pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * The initialize routine for any clients that need to access
   * component.  This routine now simply registers various valuetype
   * factories defined in CIAO_Component.pidl.  Currently, this method
   * should be call right after ORB initialization but we should try
   * to register these stuff automatically.
   */
  CIAO_CLIENT_Export int Client_init (CORBA::ORB_ptr o);
}

#include "ace/post.h"
#endif /* CIAO_CLIENT_INIT_H */
