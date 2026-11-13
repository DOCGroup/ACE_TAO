// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_STACK_H
#define ACE_RMCAST_STACK_H

#include "Protocol.h"

namespace ACE_RMCast
{
  struct Out_Element
  {
    Out_Element (Out_Element const&) = delete;
    Out_Element& operator= (Out_Element const&) = delete;
    Out_Element (Out_Element&&) = delete;
    Out_Element& operator= (Out_Element&&) = delete;

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
    Out_Element* out_ {};
  };


  struct In_Element
  {
    In_Element (In_Element const&) = delete;
    In_Element& operator= (In_Element const&) = delete;
    In_Element (In_Element&&) = delete;
    In_Element& operator= (In_Element&&) = delete;

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
    In_Element* in_ {};
  };


  struct Element : In_Element, Out_Element
  {
  };
}

#endif  // ACE_RMCAST_STACK_H
