// file      : Test/ReferenceCounting/Interface/interface.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Utility/ReferenceCounting/Interface.hpp"

using namespace Utility::ReferenceCounting;

struct Obj : public virtual Interface
{
  Obj ()
      : ref_count_ (1)
  {
  }

  virtual
  ~Obj () throw ()
  {
  }

public:
  virtual void
  add_ref () const throw ()
  {
    add_ref_i ();
  }


  virtual void
  remove_ref () const throw ()
  {
    if (remove_ref_i ()) delete this;
  }

  virtual count_t
  refcount_value () const throw ()
  {
    return refcount_value_i ();
  }

protected:
  virtual void
  add_ref_i () const throw ()
  {
    ++ref_count_;
  }


  virtual bool
  remove_ref_i () const throw ()
  {
    return --ref_count_ == 0;
  }

  virtual count_t
  refcount_value_i () const throw ()
  {
    return ref_count_;
  }

private:
  mutable count_t ref_count_;
};


struct E {};

void postcondition (bool p)
{
  if (!p) throw E ();
}

int main ()
{
  try
  {
    // add_ref
    //
    {
      Obj* a (new Obj);

      Obj* b (add_ref (a));

      postcondition (a == b && a->refcount_value () == 2);

      a->remove_ref ();
      b->remove_ref ();
    }

    {
      Obj* a (0);

      Obj* b (add_ref (a));

      postcondition (b == 0);
    }
  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
