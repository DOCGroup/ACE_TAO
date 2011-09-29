// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_STACK_H
#define ACE_RMCAST_STACK_H

#include "Protocol.h"

namespace ACE_RMCast
{
  struct Out_Element
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


  struct In_Element
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


  struct Element : In_Element, Out_Element
  {
  };
}

#endif  // ACE_RMCAST_STACK_H
