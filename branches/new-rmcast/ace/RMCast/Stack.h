// file      : ace/RMCast/Stack.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_STACK_H
#define ACE_RMCAST_STACK_H

#include <ace/RMCast/Protocol.h>

namespace ACE_RMCast
{
  struct Out_Element
  {
    virtual
    ~Out_Element ()
    {
    }

    Out_Element ()
        : out_ (0)
    {
    }

    virtual void
    out_start (Out_Element* out)
    {
      out_ = out;
    }

    virtual void
    send (Message_ptr m)
    {
      if (out_) out_->send (m);
    }

    virtual void
    out_stop ()
    {
      out_ = 0;
    }

  protected:
    Out_Element* out_;
  };


  struct In_Element
  {
    virtual
    ~In_Element ()
    {
    }

    In_Element ()
        : in_ (0)
    {
    }

    virtual void
    in_start (In_Element* in)
    {
      in_ = in;
    }

    virtual void
    recv (Message_ptr m)
    {
      if (in_) in_->recv (m);
    }

    virtual void
    in_stop ()
    {
      in_ = 0;
    }

  protected:
    In_Element* in_;
  };

  struct Element : In_Element, Out_Element
  {
  };

}

#endif  // ACE_RMCAST_STACK_H
