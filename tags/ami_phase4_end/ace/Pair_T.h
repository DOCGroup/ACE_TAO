/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Pair_T.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef ACE_PAIR_T_H
#define ACE_PAIR_T_H

#include "ace/Pair.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

////////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
class ACE_Pair
{
  // = TITLE  
  //     Defines a pair.
  //
  // = DESCRIPTION
  //     Similar to the STL pair.
public:

  // = Traits.
  typedef T1 first_type;
  typedef T2 second_type;

  // = Initialization and termination methods.
  ACE_Pair (const T1 &t1, 
            const T2 &t2);
  // Constructor.

  ACE_Pair (void);
  // Default constructor.

  T1 &first (void);
  void first (const T1 &t1);
  // Get/Set first.

  T2 &second (void);
  void second (const T2 &t2);
  // Access second.

protected:

  T1 first_;
  T2 second_;
};

////////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
class ACE_Reference_Pair
{
  // = TITLE  
  //     Defines a pair that only hold references.
  //
  // = DESCRIPTION
  //     Similar to the STL pair (but restricted to holding references
  //     and not copies).
public:

  // = Traits.
  typedef T1 first_type;
  typedef T2 second_type;

  // = Initialization and termination methods.
  ACE_Reference_Pair (T1 &t1, 
                      T2 &t2);
  // Constructor.

  T1 &first (void);
  // Access first.

  T2 &second (void);
  // Access second.

protected:

  T1 &first_;
  T2 &second_;
};

////////////////////////////////////////////////////////////////////////////////

#if defined (__ACE_INLINE__)
#include "ace/Pair_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Pair_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Pair_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_PAIR_T_H */
