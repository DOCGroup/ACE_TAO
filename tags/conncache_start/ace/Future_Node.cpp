// Future.cpp
// $Id$

#define ACE_BUILD_DLL

#ifndef ACE_FUTURE_NODE_CPP
#define ACE_FUTURE_NODE_CPP

#include "ace/OS.h"
#include "ace/Future_Node.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(ace, Future_Node, "$Id$")

#if defined (ACE_HAS_THREADS)

template <class T>
ACE_DLList_Future_Node<T>::ACE_DLList_Future_Node (void)
  : next_ (0),
    prev_ (0)
{
}

template <class T>
ACE_DLList_Future_Node<T>::ACE_DLList_Future_Node (const ACE_Future<T> &item,
                                                   ACE_DLList_Future_Node<T> *n,
                                                   ACE_DLList_Future_Node<T> *p)
: item_ (item),
  next_ (n),
  prev_ (p)
{
}

template <class T>
ACE_DLList_Future_Node<T>::~ACE_DLList_Future_Node (void)
{
}

#endif /* ACE_HAS_THREADS */
#endif /* ACE_FUTURE_NODE_CPP */
