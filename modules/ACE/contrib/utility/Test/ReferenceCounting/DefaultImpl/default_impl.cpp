// file      : Test/ReferenceCounting/DefaultImpl/default_impl.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Utility/ReferenceCounting/DefaultImpl.hpp"

using namespace Utility::ReferenceCounting;

struct Base : public virtual Interface
{
  virtual
  ~Base () throw ()
  {
  }
};


class Impl : public virtual Base,
             public virtual DefaultImpl <>
{
public:
  Impl (bool& destroyed)
      : dummy_ (false),
        destroyed_ (destroyed)
  {
  }

  Impl ()
      : dummy_ (false),
        destroyed_ (dummy_)
  {
  }

  virtual
  ~Impl () throw ()
  {
    destroyed_ = true;
  }

public:
  void
  lock ()
  {
    lock_i ();
  }

private:
  bool dummy_;
  bool& destroyed_;
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
    // DefaultImpl
    //
    {
      Impl* a (new Impl);

      postcondition (a->refcount_value () == 1);

      a->remove_ref ();
    }

    // ~DefaultImpl
    //
    {
      Impl* a (new Impl);
      a->remove_ref ();
    }

    // add_ref
    //
    {
      Impl* a (new Impl);

      a->add_ref ();

      postcondition (a->refcount_value () == 2);

      a->remove_ref ();
      a->remove_ref ();
    }


    // remove_ref
    //
    {
      bool destroyed (false);
      Impl* a (new Impl (destroyed));

      a->add_ref ();
      a->remove_ref ();

      postcondition (destroyed == false && a->refcount_value () == 1);

      a->remove_ref ();

      postcondition (destroyed == true);
    }


    // refcount_value
    //
    {
      Impl* a (new Impl);

      postcondition (a->refcount_value () == 1);
    }

    // lock_i
    //
    {
      Impl* a (new Impl);
      a->lock ();
    }
  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
