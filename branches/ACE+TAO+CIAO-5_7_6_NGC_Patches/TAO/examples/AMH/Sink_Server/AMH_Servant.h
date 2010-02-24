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

#include /**/ "ace/pre.h"
#include "tao/ORB_Core.h"

#include "TestS.h"

/// Implement the Rountrip interface
class AMH_Servant
  : public virtual POA_Test::AMH_Roundtrip
{
public:
  AMH_Servant (CORBA::ORB_ptr orb);

  virtual ~AMH_Servant (void);

  /// parse arguments from command line for sleep time
  virtual int parse_args (int &argc, ACE_TCHAR **argv);

  // = The skeleton methods
  virtual void start_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh);

  virtual void end_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh);

  virtual void test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                            Test::Timestamp send_time);

protected:
  ACE_Reactor *reactor_;
  int sleep_time_;
};

#include /**/ "ace/post.h"

#endif /* AMH_SERVANT_H */
