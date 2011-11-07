
//=============================================================================
/**
 *  @file    basic_func.h
 *
 *  $Id$
 *
 *  For use with basic_perf.cpp.
 *
 *
 *  @author David Levine
 */
//=============================================================================


#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

extern int A,BB,C,D,E,F;

// If your compiler optimizes away Empty_Iteration_Test::run (), then
// #defining ACE_HAS_OPTIMIZED_NULL_FUNCTIONS may help produce more
// reasonable numbers.
#if defined (_MSC_VER)
  // MSVC 5.0 needs it . . .
# define ACE_HAS_OPTIMIZED_NULL_FUNCTIONS
#endif /* _MSC_VER */

#if defined (ACE_HAS_OPTIMIZED_NULL_FUNCTIONS)
# define EXPR(R,A,BB,C,D) (R=(A*BB + C*D))
# define DO_SOMETHING EXPR(A,BB,EXPR(F,A,E,C,BB),EXPR(BB,F,A,D,E),EXPR(E,BB,F,A,C));
#else  /* ACE_HAS_OPTIMIZED_NULL_FUNCTIONS */
# define DO_SOMETHING
#endif /* ACE_HAS_OPTIMIZED_NULL_FUNCTIONS */


// An external (global) function.
void func ();


// A class with no virtual functions.
class Foo
{
  public:
    void inline_func () { DO_SOMETHING }
    void func ();
};


// A class with a virtual function.
class Foo_v
{
  public:
    virtual ~Foo_v ();
    void inline_func () { DO_SOMETHING }
    void func ();
    virtual void v_func ();
};


// A derived class.
class Foo_d_v : public Foo_v
{
  public:
    virtual void v_func ();
};

#endif /* BASIC_FUNC_H */
