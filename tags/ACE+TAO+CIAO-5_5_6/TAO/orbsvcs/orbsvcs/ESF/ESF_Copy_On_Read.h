// -*- C++ -*-

/**
 *  @file   ESF_Copy_On_Read.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_COPY_ON_READ_H
#define TAO_ESF_COPY_ON_READ_H

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class Target> class TAO_ESF_Worker;

// ****************************************************************

/**
 * @class TAO_ESF_Copy_On_Read
 *
 * @brief Implement the Copy_On_Read protocol
 *
 *
 * The class is parametric on the kind of collection and locking
 * mechanism used.
 */
template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Copy_On_Read : public TAO_ESF_Proxy_Collection<PROXY>
{
public:
  /// Constructors
  TAO_ESF_Copy_On_Read (void);
  TAO_ESF_Copy_On_Read (const COLLECTION &collection);

  // = The TAO_ESF_Proxy_Collection methods
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker);
  virtual void connected (PROXY *proxy);
  virtual void reconnected (PROXY *proxy);
  virtual void disconnected (PROXY *proxy);
  virtual void shutdown (void);

private:
  COLLECTION collection_;

  ACE_LOCK lock_;
};

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Copy_On_Read.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Copy_On_Read.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_COPY_ON_READ_H */
