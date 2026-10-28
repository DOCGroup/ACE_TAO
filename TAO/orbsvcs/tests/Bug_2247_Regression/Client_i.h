// -*- C++ -*-
#ifndef BUG_2247_REGRESSION_CLIENT_I_H
#define BUG_2247_REGRESSION_CLIENT_I_H

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

  Client_i (Client_i const &) = delete;
  Client_i (Client_i &&) = delete;
  Client_i &operator= (Client_i const &) = delete;
  Client_i &operator= (Client_i &&) = delete;

  int init ();
  // Initialize the ORB etc.

private:
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BUG_2247_REGRESSION_CLIENT_I_H */
