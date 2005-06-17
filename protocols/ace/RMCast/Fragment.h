// file      : ace/RMCast/Fragment.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_FRAGMENT_H
#define ACE_RMCAST_FRAGMENT_H

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

namespace ACE_RMCast
{
  class Fragment : public Element
  {
  public:
    Fragment ();

  public:
    virtual void
    send (Message_ptr m);
  };
}

#endif  // ACE_RMCAST_FRAGMENT_H
