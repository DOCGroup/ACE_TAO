/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor_T.i
//
// = DESCRIPTION
//    Inlinable method definitions for templatized classes
//    implementing the GOF Command Pattern, also known as functors
//    or function objects.
//
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================


// Invokes the less than function object.

template <class OPERAND1, class OPERAND2> ACE_INLINE int
ACE_Less_Than_Functor<OPERAND1, OPERAND2>::execute (const OPERAND1 &operand1,
                                                    const OPERAND2 &operand2)
{
  return (operand1 < operand2) ? 1 : 0;
}

// Creates another object of the same type.

template <class OPERAND1, class OPERAND2> ACE_INLINE
ACE_Const_Binary_Functor_Base<OPERAND1, OPERAND2> *
ACE_Less_Than_Functor<OPERAND1, OPERAND2>::clone ()
{
  return new ACE_Less_Than_Functor<OPERAND1, OPERAND2>;
}
