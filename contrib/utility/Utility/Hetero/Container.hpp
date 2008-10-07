// file      : Utility/Hetero/Container.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_CONTAINER_HPP
#define UTILITY_HETERO_CONTAINER_HPP

#include <typeinfo>

namespace Utility
{
  namespace Hetero
  {
    //
    //
    //
    class Typing {};

    //
    //
    //
    class Container
    {
    public:
      ~Container ()
      {
        delete holder_;
      }

    public:
      template <typename T>
      Container (T const& t)
          : holder_ (new TypedHolder<T> (t))
      {
      }

      Container (Container const& c)
          : holder_ (c.holder_->clone ())
      {
      }

    public:
      template <typename T>
      Container&
      operator= (T const& t)
      {
        delete holder_;
        holder_ = 0;
        holder_ = new TypedHolder<T> (t);
        return *this;
      }

      Container&
      operator= (Container const& c)
      {
        delete holder_;
        holder_ = 0;
        holder_ = c.holder_->clone ();
        return *this;
      }

    public:
      template <typename T>
      operator T& ()
      {
        return value<T> ();
      }

      template <typename T>
      operator T const& () const
      {
        return value<T> ();
      }

    public:
      template <typename T>
      T&
      value ()
      {
        if (holder_->type () == typeid (T))
        {
          return dynamic_cast<TypedHolder<T>*>(holder_)->value ();
        }
        else
        {
          throw Typing ();
        }
      }

      template <typename T>
      T const&
      value () const
      {
        if (holder_->type () == typeid (T))
        {
          return dynamic_cast<TypedHolder<T>*>(holder_)->value ();
        }
        else
        {
          throw Typing ();
        }
      }

    public:
      std::type_info const&
      type () const
      {
        return holder_->type ();
      }

    public:
      template <typename T>
      friend T
      operator+ (Container const& a, T const& b)
      {
        return a.value<T> () + b;
      }

      template <typename T>
      friend T
      operator+ (T const& a, Container const& b)
      {
        return a + b.value<T> ();
      }

    private:
      struct Holder
      {
        virtual
        ~Holder () {}

        virtual Holder*
        clone () const = 0;

        virtual std::type_info const&
        type () const = 0;
      };

      template <typename T>
      struct TypedHolder : public Holder
      {
        TypedHolder (T const& value)
            : value_ (value)
        {
        }

        virtual Holder*
        clone () const
        {
          return new TypedHolder<T> (value_);
        }

        virtual std::type_info const&
        type () const
        {
          return typeid (T);
        }

        T const&
        value () const
        {
          return value_;
        }

        T&
        value ()
        {
          return value_;
        }

      private:
        T value_;
      };

      Holder* holder_;
    };
  }
}

#endif  // UTILITY_HETERO_CONTAINER_HPP
//$Id$
