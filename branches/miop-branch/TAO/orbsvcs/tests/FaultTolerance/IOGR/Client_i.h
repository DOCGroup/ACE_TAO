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


#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class Client_i
{
public:
  Client_i (CORBA::ORB_ptr orb);
  // Our constructor

  void init (TAO_ENV_SINGLE_ARG_DECL);
  // Initialize the ORB etc.

private:
  CORBA::ORB_var orb_;

};

#include "ace/post.h"
#endif /*TEST_FT_IOGR_CLIENT_I_H*/
