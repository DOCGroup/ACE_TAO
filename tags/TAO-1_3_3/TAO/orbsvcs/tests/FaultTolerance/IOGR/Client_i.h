// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    test
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    A helper class for the client
//
// = AUTHOR
//     Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TEST_FT_IOGR_CLIENT_I_H
#define TEST_FT_IOGR_CLIENT_I_H

#include "ace/pre.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Client_i
{
public:
  Client_i (CORBA::ORB_ptr orb);
  // Our constructor

  void init (ACE_ENV_SINGLE_ARG_DECL);
  // Initialize the ORB etc.

private:
  CORBA::ORB_var orb_;

};

#include "ace/post.h"
#endif /*TEST_FT_IOGR_CLIENT_I_H*/
