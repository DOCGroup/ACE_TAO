/* -*- C++ -*- */
// @(#)auto_builtin_ptr.h	1.1	10/18/96

template <class X>
class auto_builtin_ptr
  // = TITLE
  //     Implements an simple-minded auto_ptr abstraction for builtin types.
{
public:
  // = Initialization and termination methods
  auto_builtin_ptr (X *p = 0): p_ (p) {}
  ~auto_builtin_ptr (void) { delete [] this->p_; }

  // = Accessor methods.
  operator X * (void) const { return this->p_; }

private:
  X *p_;
};

