// file      : Utility/hetero/map.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_MAP_HPP
#define UTILITY_HETERO_MAP_HPP

#include <typeinfo>
#include <map>

namespace Utility
{
  namespace hetero
  {
    template <typename Key>
    class map
    {
    private:
      struct ValueBase
      {
        virtual
        ~ValueBase () {}
      };

      template <typename T>
      class Value : public ValueBase
      {
      public:
        Value (T const& t)
            : t_ (t)
        {
        }

        T t_;
      };

      typedef
      std::map<Key, ValueBase*>
      ValueMap;

      typedef
      typename std::map<Key, ValueBase*>::value_type
      ValueType;

    public:

      class Typing {};

      template <typename T>
      struct type
      {
        typedef std::pair <Key const, T> value_type;
      };

      template <typename First>
      struct Pair
      {
        First first;

        Pair ()
            : first (),
              second_ (0)
        {
        }

        Pair (First const& k, ValueBase* v)
            : first (k),
              second_ (v)
        {
        }

        template <typename T>
        T&
        second ()
        {
          Value<T>* v = dynamic_cast<Value<T>*> (second_);
          if (v == 0) throw Typing ();

          return v->t_;
        }

        template <typename T>
        T const&
        second () const
        {
          Value<T>* v = dynamic_cast<Value<T>*> (second_);
          if (v == 0) throw Typing ();

          return v->t_;
        }

      private:
        ValueBase* second_;
      };

      friend
      class iterator
      {
      public:
        iterator (typename ValueMap::iterator const& i)
            : i_ (i)
        {
        }

      public:

        Pair<Key>&
        operator* ()
        {
          current_ = Pair<Key> (i_->first, i_->second);
          return current_;
        }

        Pair<Key>*
        operator-> ()
        {
          current_ = Pair<Key> (i_->first, i_->second);
          return &current_;
        }

      public:
        bool
        operator== (iterator const& other)
        {
          return i_ == other.i_;
        }

        bool
        operator!= (iterator const& other)
        {
          return i_ != other.i_;
        }

      private:
        typename ValueMap::iterator i_;
        Pair<Key> current_;
      };


      friend
      class const_iterator
      {
      public:
        const_iterator (typename ValueMap::const_iterator const& i)
            : i_ (i)
        {
        }

      public:

        Pair<Key> const&
        operator* ()
        {
          current_ = Pair<Key> (i_->first, i_->second);
          return current_;
        }

        Pair<Key> const*
        operator-> ()
        {
          current_ = Pair<Key> (i_->first, i_->second);
          return &current_;
        }

      public:
        bool
        operator== (const_iterator const& other)
        {
          return i_ == other.i_;
        }

        bool
        operator!= (const_iterator const& other)
        {
          return i_ != other.i_;
        }

      private:
        typename ValueMap::const_iterator i_;
        Pair<Key> current_;
      };

    public:
      iterator
      begin ()
      {
        return iterator (map_.begin ());
      }

      const_iterator
      begin () const
      {
        return const_iterator (map_.begin ());
      }

      iterator
      end ()
      {
        return iterator (map_.end ());
      }

      const_iterator
      end () const
      {
        return const_iterator (map_.end ());
      }

    public:

      template <typename T>
      bool
      insert (std::pair <Key, T> const& x)
      {
        ValueType v (x.first, new Value<T> (x.second));
        return map_.insert (v).second;
      }

      template <typename T>
      bool
      insert (Key const& k, T const& t)
      {
        ValueType v (k, new Value<T> (t));
        return map_.insert (v).second;
      }

    public:
      iterator
      find (Key const& k)
      {
        return iterator (map_.find (k));
      }

      const_iterator
      find (Key const& k) const
      {
        return const_iterator (map_.find (k));
      }

    private:
      ValueMap map_;
    };
  }
}

#include "Utility/hetero/map.tpp"

#endif  // UTILITY_HETERO_MAP_HPP
//$Id$
