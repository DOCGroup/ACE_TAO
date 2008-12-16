// file      : Test/ReferenceCounting/SmartPtr/smart_ptr.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Utility/ReferenceCounting/SmartPtr.hpp"
#include "Utility/ReferenceCounting/DefaultImpl.hpp"

using namespace Utility::ReferenceCounting;

struct Base : public virtual Interface
{
  virtual
  ~Base () throw ()
  {
  }
};

typedef
SmartPtr<Base>
BasePtr;

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

private:
  bool dummy_;
  bool& destroyed_;
};

typedef
SmartPtr<Impl>
ImplPtr;

struct E {};

void postcondition (bool p)
{
  if (!p) throw E ();
}

int main ()
{
  try
  {
    // SmartPtr ()
    //
    {
      BasePtr a;

      postcondition (a.in () == 0);
    }

    // SmartPtr (Type*)
    //
    {
      Impl* a (new Impl);
      ImplPtr b (a);

      postcondition (b.in () == a && a->refcount_value () == 1);
    }

    // SmartPtr (SmartPtr<Type> const&)
    //
    {
      ImplPtr a (new Impl);
      ImplPtr b (a);

      postcondition (a.in () == b.in () && a->refcount_value () == 2);
    }

    // SmartPtr (SmartPtr<Other> const&)
    //
    {
      ImplPtr a (new Impl);
      BasePtr b (a);

      postcondition (b.in () == static_cast<Base*>(a.in ()) &&
                     b->refcount_value () == 2);
    }

    // ~SmartPtr
    //
    {
      bool destroyed (false);
      {
        ImplPtr a (new Impl (destroyed));
      }

      postcondition (destroyed == true);
    }

    // operator= (Type* ptr)
    //
    {
      Impl* a (new Impl);
      ImplPtr b;
      b = a;

      postcondition (b.in () == a && a->refcount_value () == 1);
    }

    // operator= (SmartPtr<Type> const&)
    //
    {
      ImplPtr a (new Impl);
      ImplPtr b;
      b = a;

      postcondition (b.in () == a.in () && a->refcount_value () == 2);
    }

    // operator= (SmartPtr<Other> const&)
    //
    {
      ImplPtr a (new Impl);
      BasePtr b;
      b = a;

      postcondition (b.in () == static_cast<Base*>(a.in ()) &&
                     b->refcount_value () == 2);
    }

    // operator Type*
    //
    {
      Impl* a (new Impl);
      ImplPtr b (a);
      Impl* c (b);

      postcondition (a == c);
    }

    // operator->
    //
    {
      Impl* a (new Impl);
      ImplPtr b (a);
      Impl* c (b.operator-> ());

      postcondition (a == c);
    }

    // in
    //
    {
      Impl* a (new Impl);
      ImplPtr b (a);
      Impl* c (b.in ());

      postcondition (a == c);
    }

    // retn
    //
    {
      Impl* a (new Impl);
      ImplPtr b (a);
      Impl* c (b.retn ());

      postcondition (a == c);

      b = a; // give ownership back
    }

    // add_ref
    //
    {
      ImplPtr a (new Impl);
      ImplPtr b (add_ref (a));

      postcondition (a.in () == b.in () && b->refcount_value () == 2);
    }

    // smart_cast
    //
    {
      BasePtr a (new Impl);
      ImplPtr b (smart_cast<Impl>(a));

      postcondition (b != 0 && b->refcount_value () == 2);
    }

    // acquire
    //
    {
      bool destroyed (false);
      Base::count_t c (0);
      {
        c = acquire (new Impl (destroyed))->refcount_value ();
      }

      postcondition (c == 1 && destroyed == true);
    }
  }
  catch (...)
  {
    return -1;
  }
}
//$Id$
