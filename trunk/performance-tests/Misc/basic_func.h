// $Id$

// ============================================================================
//
// = LIBRARY
//    performance-tests/Misc
// 
// = FILENAME
//    basic_func.h
//
// = DESCRIPTION
//    For use with basic_perf.cpp.
//
// = AUTHOR
//    David Levine
// 
// ============================================================================

// An external (global) function.
void func ();


// A class with no virtual functions.
class Foo
{
  public:
    void inline_func () {}
    void func ();
};


// A class with a virtual function.
class Foo_v
{
  public:
    void inline_func () {}
    void func ();
    virtual void v_func ();
};


// EOF
