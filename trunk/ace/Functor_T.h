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
//     Templatized classes for implementing the GOF Command Pattern,
//     also known as functors or function objects.
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
// ============================================================================

#ifndef ACE_FUNCTOR_T_H
#define ACE_FUNCTOR_T_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class RECEIVER, class ACTION>
class ACE_Command_Callback : public ACE_Command_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a callback to an
  //    object without knowing anything about the object except its type.
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

/////////////////////////////
// Unary functor templates //
/////////////////////////////

template <class OPERAND>
class ACE_Unary_Functor_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a function object
  //    over a single non-const parameterized type without knowing anything
  //    about the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a unary operation over a
  //    single object of the non-const paramterized type.  A class can invoke
  //    such operation without knowing anything about it, or how it was
  //    implemented.
  //
public:

  virtual ~ACE_Unary_Functor_Base () {};
  // Virtual destructor.

  virtual int execute (OPERAND &operand) = 0;
  // Invokes the function object.

  virtual ACE_Unary_Functor_Base * clone () = 0;
  // Creates another object of the same type.
};

template <class OPERAND>
class ACE_Const_Unary_Functor_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a function object
  //    over a single parameterized type without knowing anything about
  //    the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a unary operation over a
  //    single object of the paramterized type.  A class can invoke such
  //    an operation without knowing anything about it, or its implementation.
  //
public:

  virtual ~ACE_Const_Unary_Functor_Base () {};
  // Virtual destructor.

  virtual int execute (const OPERAND &operand) = 0;
  // Invokes the function object.

  virtual ACE_Const_Unary_Functor_Base * clone () = 0;
  // Creates another object of the same type.
};

/////////////////////////////
// Binary functor templates //
/////////////////////////////

template <class OPERAND1, class OPERAND2>
class ACE_Binary_Functor_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a binary function
  //    object over two non-const parameterized types without knowing anything
  //    about the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a binary operation over two
  //    objects of the paramterized non-const types.  A class can invoke such
  //    an operation without knowing anything about it, or its implementation.
  //
public:

  virtual ~ACE_Binary_Functor_Base () {};
  // Virtual destructor.

  virtual int execute (OPERAND1 &operand1, OPERAND2 &operand2) = 0;
  // Invokes the function object.

  virtual ACE_Binary_Functor_Base * clone () = 0;
  // Creates another object of the same type.
};

template <class OPERAND1, class OPERAND2>
class ACE_Const_Binary_Functor_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a binary function
  //    object over two parameterized types without knowing anything about
  //    the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a binary operation over two
  //    objects of the paramterized types.  A class can invoke such
  //    an operation without knowing anything about it, or its implementation.
  //
public:

  virtual ~ACE_Const_Binary_Functor_Base () {};
  // Virtual destructor.

  virtual int execute (const OPERAND1 &operand1, const OPERAND2 &operand2) = 0;
  // Invokes the function object.

  virtual ACE_Const_Binary_Functor_Base * clone () = 0;
  // Creates another object of the same type.
};


template <class OPERAND1, class OPERAND2>
class ACE_Less_Than_Functor :
  public ACE_Const_Binary_Functor_Base<OPERAND1, OPERAND2>
{
  // = TITLE
  //    Defines a class template that allows us to invoke a binary less than
  //    function over two parameterized types without knowing anything about
  //    the function and operand objects except their types.
  //
  // = DESCRIPTION
  //    This class depends on the definition
  //    objects of the paramterized types.  A class can invoke such
  //    an operation without knowing anything about it, or its implementation.
  //
public:

  virtual int execute (const OPERAND1 &operand1, const OPERAND2 &operand2);
  // Invokes the function object.

  virtual
    ACE_Const_Binary_Functor_Base<OPERAND1, OPERAND2>
          * clone ();
  // Creates another object of the same type.
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
