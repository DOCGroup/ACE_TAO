/* -*- C++ -*- */
/**
 *  @file   ESF_Worker.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_WORKER_H
#define TAO_ESF_WORKER_H

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Define the interface for the Worker objects
/**
 * The Event Service Framework uses Worker classes to iterate over
 * collections of proxies.
 * The following class defines the worker interface, basically users of
 * the framework implement a worker object and pass it to one
 * collection.  The collection invokes the worker for each element the
 * colection contains.
 */
template<class Object>
class TAO_ESF_Worker
{
public:
  virtual ~TAO_ESF_Worker (void);

  /// Callback interface.
  virtual void work (Object *object
                     ACE_ENV_ARG_DECL) = 0;
};

#if defined (__ACE_INLINE__)
#include "ESF_Worker.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Worker.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Worker.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_WORKER_H */
