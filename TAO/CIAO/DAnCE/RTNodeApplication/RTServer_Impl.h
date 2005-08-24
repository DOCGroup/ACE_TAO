// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTServer_Impl.h
 *
 *  $Id$
 *
 *  This file contains real-time extended implementations for the
 *  servants of
 *         Deployment::NodeApplication
 *
 *  It will also contain the real-time extended version of
 *  CIAO::Session_Container.
 *
 *  Currently, they are only used in the RTComponentServer
 *  implementation.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_RTSERVER_IMPL_H
#define CIAO_RTSERVER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/NodeApplication_Impl.h"
#include "ciao/Container_Base.h"
#include "tao/RTCORBA/RTCORBA.h"

namespace CIAO
{
  namespace RTServer
  {
    /**
     * @class RTNodeApplication
     *
     * @brief Real-time extention servant implementation of
     *        Deployment::NodeApplication
     *
     * This class implements the Deployment::NodeApplication
     * interface as defined by the DnC spcification.  As the interface
     * implies, this is actually part of the deployment interface and is
     * used to manage the lifecycle of containers running on the server.
     */
    class RTNodeApplication_Impl
      : public virtual CIAO::NodeApplication_Impl
    {
    public:
      /// Constructor
      RTNodeApplication_Impl (CORBA::ORB_ptr o,
                              RTCORBA::RTORB_ptr rto,
                              PortableServer::POA_ptr p);

      /// Destructor
      virtual ~RTNodeApplication_Impl (void);

    protected:
      RTCORBA::RTORB_var rtorb_;
    };

  }
}

#if defined (__ACE_INLINE__)
# include "RTServer_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_RTSERVER_IMPL_H */
