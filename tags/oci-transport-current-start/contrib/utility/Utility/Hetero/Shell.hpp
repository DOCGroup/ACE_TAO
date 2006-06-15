// file      : Utility/Hetero/Shell.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_SHELL_HPP
#define UTILITY_HETERO_SHELL_HPP

namespace Utility
{
  namespace Hetero
  {

    //
    //
    //
    class ShellCore
    {
    protected:
      template <typename F, typename T0>
      static typename F::RetType
      apply (F& f, TypedContainer<TypeList<T0> >& c)
      {
        if (type_check <T0> (c)) return type_apply<T0> (f, c);

        throw Utility::Hetero::Typing ();
      }

      template <typename F, typename T0, typename T1>
      static typename F::RetType
      apply (F& f, TypedContainer<TypeList<T0, T1> >& c)
      {
        if (type_check <T0> (c)) return type_apply<T0> (f, c);
        if (type_check <T1> (c)) return type_apply<T1> (f, c);

        throw Utility::Hetero::Typing ();
      }

      template <typename F, typename T0, typename T1, typename T2>
      static typename F::RetType
      apply (F& f, TypedContainer<TypeList<T0, T1, T2> >& c)
      {
        if (type_check <T0> (c)) return type_apply<T0> (f, c);
        if (type_check <T1> (c)) return type_apply<T1> (f, c);
        if (type_check <T2> (c)) return type_apply<T2> (f, c);

        throw Utility::Hetero::Typing ();
      }

    private:
      template <typename T>
      static bool
      type_check (Container& c)
      {
        return c.type () == typeid (T);
      }

      template <typename T, typename F>
      static typename F::RetType
      type_apply (F& f, Container& c)
      {
        return f (c. template value<T> ());
      }
    };


    //
    //
    //
    template <typename F>
    struct Shell : F, ShellCore
    {
      using F::operator ();

      template <typename T>
      typename F::RetType
      operator () (TypedContainer<T>& p)
      {
        return apply (*this, p);
      }
    };
  }
}

#endif  // UTILITY_HETERO_SHELL_HPP
//$Id$
