/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Proxy_RB_Tree
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_PROXY_RB_TREE_H
#define TAO_ESF_PROXY_RB_TREE_H

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/RB_Tree.h"

template<class PROXY>
class TAO_ESF_Proxy_RB_Tree_Iterator
{
public:
  typedef ACE_RB_Tree_Iterator<PROXY*,int,ACE_Less_Than<PROXY*>,ACE_Null_Mutex> Implementation;

  TAO_ESF_Proxy_RB_Tree_Iterator (const Implementation &i);

  int operator == (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const;
  int operator != (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const;
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>& operator++ (void);
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> operator++ (int);
  PROXY *operator *(void);

private:
  Implementation impl_;
};

// ****************************************************************

template<class PROXY>
class TAO_ESF_Proxy_RB_Tree
{
public:
  typedef ACE_RB_Tree<PROXY*,int,ACE_Less_Than<PROXY*>,ACE_Null_Mutex> Implementation;
  typedef TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> Iterator;

  TAO_ESF_Proxy_RB_Tree (void);

  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> begin (void);
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> end (void);
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

#if defined (__ACE_INLINE__)
#include "ESF_Proxy_RB_Tree.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Proxy_RB_Tree.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_RB_Tree.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_RB_TREE_H */
