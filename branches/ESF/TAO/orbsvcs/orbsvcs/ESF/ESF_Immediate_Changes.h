/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Proxy_Collection
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_IMMEDIATE_CHANGES_H
#define TAO_ESF_IMMEDIATE_CHANGES_H

#include "ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Immediate_Changes : public TAO_ESF_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_ESF_Immediate_Changes
  //
  // = DESCRIPTION
  //   Implement the Immediate_
public:
  TAO_ESF_Immediate_Changes (void);
  TAO_ESF_Immediate_Changes (const COLLECTION &collection);

  // = The TAO_ESF_Proxy methods
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV);
  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV);
  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);

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
