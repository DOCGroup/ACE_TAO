// -*- C++ -*-

//=============================================================================
/**
 *  @file FTRT_ClientORB_Loader.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FTRT_CLIENTORB_LOADER_H
#define FTRT_CLIENTORB_LOADER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/FtRtEvent/ClientORB/ftrt_clientorb_export.h"

#include "tao/orbconf.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace TAO_FTRT {

  class TAO_FTRT_Export FTRT_ClientORB_Loader : public ACE_Service_Object
  {
  public:
    /// Constructor.
    FTRT_ClientORB_Loader (void);

    /// Destructor.
    virtual ~FTRT_ClientORB_Loader (void);

    /// Initialize the TransactionDepth loader hooks.
    virtual int init (int argc,
      ACE_TCHAR* []);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FTRT, FTRT_ClientORB_Loader)
ACE_STATIC_SVC_REQUIRE(FTRT_ClientORB_Loader)
ACE_FACTORY_DECLARE (TAO_FTRT, FTRT_ClientORB_Loader)

#include /**/ "ace/post.h"
#endif //FTRT_CLIENTORB_LOADER_H
