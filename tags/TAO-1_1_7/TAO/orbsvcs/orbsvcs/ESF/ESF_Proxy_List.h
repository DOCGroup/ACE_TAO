/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Proxy_List
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_PROXY_LIST_H
#define TAO_ESF_PROXY_LIST_H

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

template<class PROXY>
class TAO_ESF_Proxy_List
{
  // = DESCRIPTION
  //   A concrete proxy collection.
  //   Based on the ACE_Unbounded_Set<> collection, used a double
  //   linked list internally.
  //
public:
  typedef ACE_Unbounded_Set<PROXY*> Implementation;
  typedef ACE_Unbounded_Set_Iterator<PROXY*> Iterator;

  TAO_ESF_Proxy_List (void);

  ACE_Unbounded_Set_Iterator<PROXY*> begin (void);
  ACE_Unbounded_Set_Iterator<PROXY*> end (void);
  size_t size (void) const;
  void connected (PROXY *,
                CORBA::Environment &);
  void reconnected (PROXY *,
                    CORBA::Environment &);
  void disconnected (PROXY *,
                     CORBA::Environment &);
  void shutdown (CORBA::Environment &);

private:
  ACE_Unbounded_Set<PROXY*> impl_;
};

#if defined (__ACE_INLINE__)
#include "ESF_Proxy_List.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Proxy_List.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_List.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_LIST_H */
