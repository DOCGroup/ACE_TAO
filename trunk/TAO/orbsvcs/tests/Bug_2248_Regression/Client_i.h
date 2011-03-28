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
 */
//=============================================================================


#ifndef BUG_2248_REGRESSION_CLIENT_I_H
#define BUG_2248_REGRESSION_CLIENT_I_H

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
  int init (void);

private:
  CORBA::ORB_var orb_;

};

#include /**/ "ace/post.h"
#endif /* BUG_2248_REGRESSION_CLIENT_I_H*/
