// file      : ace/RMCast/Stack.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_STACK_H
#define ACE_RMCAST_STACK_H

#include "Protocol.h"
#include "RMCast_Export.h"

namespace ACE_RMCast
{
  struct ACE_RMCast_Export Out_Element
  {
    virtual
    ~Out_Element ();

    Out_Element ();

    virtual void
    out_stop ();

    virtual void
    out_start (Out_Element* out);

    virtual void
    send (Message_ptr m);

  protected:
    Out_Element* out_;
  };


  struct ACE_RMCast_Export In_Element
  {
    virtual
    ~In_Element ();

    In_Element ();

    virtual void
    in_stop ();

    virtual void
    in_start (In_Element* in);

    virtual void
    recv (Message_ptr m);

  protected:
    In_Element* in_;
  };


  struct ACE_RMCast_Export Element : In_Element, Out_Element
  {
  };
}

#endif  // ACE_RMCAST_STACK_H
