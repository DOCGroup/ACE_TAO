// -*- C++ -*-

// =========================================================================
/**
 *  @file  AMH_Servant.h
 *
 *  $Id$
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================


#ifndef AMH_SERVANT_H
#define AMH_SERVANT_H

#include "ace/pre.h"
#include "tao/corba.h"
#include "ace/Reactor.h"
#include "tao/ORB_Core.h"

#include "TestS.h"

/// Implement the Rountrip interface
class AMH_Servant
  : public virtual POA_Test::AMH_Roundtrip
  , public virtual PortableServer::RefCountServantBase
{
 public:
    AMH_Servant (CORBA::ORB_ptr orb);

    /// parse arguments from command line for sleep time
    virtual int parse_args (int* argc, char **argv);

    // = The skeleton methods
    virtual void start_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                             TAO_ENV_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void end_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                           TAO_ENV_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                              Test::Timestamp send_time
                              TAO_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

 protected:
    CORBA::ORB_ptr orb_;
    int sleep_time_;
};

#include "ace/post.h"

#endif /* AMH_SERVANT_H */
