// -*- C++ -*-

//=============================================================================
/**
 *  @file    Client_i.h
 *
 *  A helper class for the client
 *
 *  @author  Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TEST_FT_IOGR_CLIENT_I_H
#define TEST_FT_IOGR_CLIENT_I_H

#include /**/ "ace/pre.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Client_i
{
public:
  /// Our constructor
  explicit Client_i (CORBA::ORB_ptr orb);
  Client_i (Client_i const&) = delete;
  Client_i (Client_i&&) = delete;
  Client_i& operator= (Client_i const&) = delete;
  Client_i& operator= (Client_i&&) = delete;

  /// Initialize the ORB etc.
  int init (ACE_TCHAR const* ior_file);

private:
  CORBA::ORB_var orb_ {};
};

#include /**/ "ace/post.h"
#endif /*TEST_FT_IOGR_CLIENT_I_H*/
