/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Concrete_Proxy_Set
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_EC_CONCRETE_PROXY_SET_H
#define TAO_EC_CONCRETE_PROXY_SET_H
#include "ace/pre.h"

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"
#include "ace/RB_Tree.h"

template<class PROXY>
class TAO_EC_List_Based_Proxy_Set
{
public:
  typedef ACE_Unbounded_Set<PROXY*> Implementation;
  typedef ACE_Unbounded_Set_Iterator<PROXY*> Iterator;

  TAO_EC_List_Based_Proxy_Set (void);

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

// ****************************************************************

template<class PROXY>
class TAO_EC_RB_Tree_Based_Proxy_Set_Iterator
{
public:
  typedef ACE_RB_Tree_Iterator<PROXY*,int,ACE_Less_Than<PROXY*>,ACE_Null_Mutex> Implementation;

  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator (const Implementation &i);

  int operator == (const TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> &rhs) const;
  int operator != (const TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> &rhs) const;
  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>& operator++ (void);
  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> operator++ (int);
  PROXY *operator *(void);

private:
  Implementation impl_;
};

// ****************************************************************

template<class PROXY>
class TAO_EC_RB_Tree_Based_Proxy_Set
{
public:
  typedef ACE_RB_Tree<PROXY*,int,ACE_Less_Than<PROXY*>,ACE_Null_Mutex> Implementation;
  typedef TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> Iterator;

  TAO_EC_RB_Tree_Based_Proxy_Set (void);

  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> begin (void);
  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> end (void);
  size_t size (void) const;
  void connected (PROXY *,
                CORBA::Environment &);
  void reconnected (PROXY *,
                    CORBA::Environment &);
  void disconnected (PROXY *,
                     CORBA::Environment &);
  void shutdown (CORBA::Environment &);

private:
  Implementation impl_;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Concrete_Proxy_Set.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_Concrete_Proxy_Set.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Concrete_Proxy_Set.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_EC_CONCRETE_PROXY_SET_H */
