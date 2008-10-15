// -*- C++ -*-
// $Id$

// ============================================================================
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    A helper class for the client
//
// ============================================================================

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
  Client_i (CORBA::ORB_ptr orb);
  // Our constructor

  int init (void);
  // Initialize the ORB etc.

private:
  CORBA::ORB_var orb_;

};

#include /**/ "ace/post.h"
#endif /* BUG_2248_REGRESSION_CLIENT_I_H*/
