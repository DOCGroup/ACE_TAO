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
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"
#include "Kokyu.h"

namespace Kokyu
{
  class Dispatcher_Impl
  {
  public:
    //following an idiom to avoid public virtual functions.
    //instead make them private and use the template method
    //pattern - "Virtually Yours" article in CUJ Experts Forum
    int init (const ConfigInfoSet&);
    int dispatch (const Dispatch_Command*,
                  const QoSDescriptor&);
    int shutdown ();
    virtual ~Dispatcher_Impl();
  private:
    virtual int init_i (const ConfigInfoSet&) =0;
    virtual int dispatch_i (const Dispatch_Command*,
                            const QoSDescriptor&) =0;
    virtual int shutdown_i () =0;

  };
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DISPATCHER_IMPL_H */
