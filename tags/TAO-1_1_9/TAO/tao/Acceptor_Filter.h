/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Acceptor_Filter.h
//
// = AUTHOR
//    Carlos O'Ryan (coryan@uci.edu)
//
// ============================================================================

#ifndef TAO_ACCEPTOR_FILTER_H
#define TAO_ACCEPTOR_FILTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Acceptor;

class TAO_Export TAO_Acceptor_Filter
{
public:
  virtual ~TAO_Acceptor_Filter (void);

  virtual int evaluate (TAO_Acceptor *acceptor) = 0;
  // Return 0 if <acceptor> should not be included in the profile.
};

#if defined (__ACE_INLINE__)
# include "Acceptor_Filter.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_OBJECT_ADAPTER_H */
