/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor_T.h
//
// = DESCRIPTION
//     Templatized classes for implementing function objects that are used in 
//     various places in ACE.  There are currently two major categories of  
//     function objects in ACE: GOF Command Pattern objects, and STL-style 
//     functors for comparison of container elements.  The command objects
//     are invoked via an execute () method, while the STL-style functors are
//     invoked via an operator() () method.
//
// = AUTHOR
//     Chris Gill          <cdgill@cs.wustl.edu>
//
//     Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
//    and on STL-style functor implementations originally done by
//
//    Irfan Pyarali  <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FUNCTOR_T_H
#define ACE_FUNCTOR_T_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

///////////////////////////////////
// GOF Command Pattern Templates //
///////////////////////////////////

template <class RECEIVER, class ACTION>
class ACE_Command_Callback : public ACE_Command_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a GOF command style
  //    callback to an object without knowing anything about the object except
  //    its type.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute operations,
  //    binding a RECEIVER object with an ACTION.  The RECEIVER knows
  //    how to implement the operation.  A class can invoke operations
  //    without knowing anything about it, or how it was implemented.
public:

  ACE_Command_Callback (RECEIVER &recvr, ACTION action);
  // Constructor: sets the <receiver_> of the Command to recvr, and the
  // <action_> of the Command to <action>.

  virtual ~ACE_Command_Callback (void) {}
  // Virtual destructor.

  virtual int execute (void *arg = 0);
  // Invokes the method <action_> from the object <receiver_>.

private:

  RECEIVER &receiver_;
  // Object where the method resides.

  ACTION action_;
  // Method that is going to be invoked.
};

/////////////////////////////////
// STL-style Functor Templates //
/////////////////////////////////

template <class TYPE>
class ACE_Hash
{
  // = TITLE
  //
  //     Function object for hashing
  //
public:
  u_long operator () (const TYPE &t) const;  
  // Simply calls t.hash ()
};

template <class TYPE>
class ACE_Equal_To
{
  // = TITLE
  //
  //     Function object for comparing two objects of 
  //     the given type for equality.
  //
public:
  int operator () (const TYPE &lhs,
                   const TYPE &rhs) const;  
  // Simply calls operator==
};

template <class TYPE>
class ACE_Less_Than
{
  // = TITLE
  //
  //     Function object for determining whether the first object of the
  //     given type is less than the second object of the same type.
  //
public:
  int operator () (const TYPE &lhs,
                   const TYPE &rhs) const;
  // Simply calls operator<
};


#if defined (__ACE_INLINE__)
#include "ace/Functor_T.i"
#endif /* __ACE_INLINE__ */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Functor_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Functor_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_FUNCTOR_T_H */
