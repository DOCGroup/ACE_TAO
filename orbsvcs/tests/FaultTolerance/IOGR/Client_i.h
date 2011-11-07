// -*- C++ -*-

//=============================================================================
/**
 *  @file    Client_i.h
 *
 *  $Id$
 *
 *  A helper class for the client
 *
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
  Client_i (CORBA::ORB_ptr orb);

  /// Initialize the ORB etc.
  void init (void);

private:
  CORBA::ORB_var orb_;

};

#include /**/ "ace/post.h"
#endif /*TEST_FT_IOGR_CLIENT_I_H*/
