/* -*- C++ -*- */
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

#include "ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker
                         TAO_ENV_ARG_DECL);
  virtual void connected (PROXY *proxy
                          TAO_ENV_ARG_DECL);
  virtual void reconnected (PROXY *proxy
                            TAO_ENV_ARG_DECL);
  virtual void disconnected (PROXY *proxy
                             TAO_ENV_ARG_DECL);
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL);

private:
  COLLECTION collection_;

  ACE_LOCK lock_;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Immediate_Changes.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Immediate_Changes.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Immediate_Changes.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_IMMEDIATE_CHANGES_H */
