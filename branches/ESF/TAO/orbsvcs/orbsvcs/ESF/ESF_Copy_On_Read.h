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

#ifndef TAO_ESF_COPY_ON_READ_H
#define TAO_ESF_COPY_ON_READ_H

#include "ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Target> class TAO_ESF_Worker;

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Copy_On_Read : public TAO_ESF_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_ESF_Copy_On_Read
  //
  // = DESCRIPTION
  //   Implement the Copy_On_Read protocol
  //   The class is parametric on the kind of collection and locking
  //   mechanism used.
  //
  // = TODO
  //
public:
  TAO_ESF_Copy_On_Read (void);
  TAO_ESF_Copy_On_Read (const COLLECTION &collection);
  // Constructors

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
#include "ESF_Copy_On_Read.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Copy_On_Read.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Copy_On_Read.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_COPY_ON_READ_H */
