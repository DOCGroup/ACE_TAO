// file      : ace/RMCast/Bits.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_BITS_H
#define ACE_RMCAST_BITS_H

#include <ace/Synch.h>
#include <ace/Auto_Ptr.h>

//#include <iostream>

namespace ACE_RMCast
{
  typedef ACE_Thread_Mutex Mutex;
  typedef ACE_Guard<Mutex> Lock;
  typedef ACE_Condition<Mutex> Condition;

  using ::auto_ptr; // ACE auto_ptr.

  // tmp
  //
  //using std::cerr;
  //using std::endl;
}


#endif  // ACE_RMCAST_BITS_H
