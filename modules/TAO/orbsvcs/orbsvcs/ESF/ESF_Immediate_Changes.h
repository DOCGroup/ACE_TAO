// -*- C++ -*-
/**
 *  @file   ESF_Immediate_Changes.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_IMMEDIATE_CHANGES_H
#define TAO_ESF_IMMEDIATE_CHANGES_H

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ESF_Immediate_Changes
 *
 * @brief Implement the Immediate_Changes strategy
 *
 */
template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Immediate_Changes : public TAO_ESF_Proxy_Collection<PROXY>
{
public:
  TAO_ESF_Immediate_Changes (void);
  TAO_ESF_Immediate_Changes (const COLLECTION &collection);

  // = The TAO_ESF_Proxy methods
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

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Immediate_Changes.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Immediate_Changes.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Immediate_Changes.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_IMMEDIATE_CHANGES_H */
