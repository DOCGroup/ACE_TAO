/* -*- C++ -*- */
/**
 *  @file   Dispatcher_Impl.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef DISPATCHER_IMPL_H
#define DISPATCHER_IMPL_H
#include /**/ "ace/pre.h"

#include "Kokyu_defs.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"

namespace Kokyu
{
  /**
   * @class Dispatcher
   *
   * @brief Base class for EC dispatcher implementations
   *
   * The responsibility of this class is to act as a common base class
   * for different EC dispatcher implementations. This is an
   * abstract base class and cannot be instantiated.
   */
  class Kokyu_Export Dispatcher_Impl
  {
  public:
    /// Configure the dispatcher.
    int init (const Dispatcher_Attributes&);

    int activate ();

    /// dispatch a command (eg. event) based on the QoS supplied.
    int dispatch (const Dispatch_Command*,
                  const QoSDescriptor&);

    /// shutdown the dispatcher.
    int shutdown ();

    virtual ~Dispatcher_Impl();

  private:
    //following an idiom to avoid public virtual functions.
    //instead make them private and use the template method
    //pattern - "Virtually Yours" article in CUJ Experts Forum

    virtual int init_i (const Dispatcher_Attributes&) =0;
    virtual int dispatch_i (const Dispatch_Command*,
                            const QoSDescriptor&) =0;
    virtual int shutdown_i () =0;
    virtual int activate_i () =0;

  protected:
    int thr_creation_flags_;
  };
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Dispatcher_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DISPATCHER_IMPL_H */
