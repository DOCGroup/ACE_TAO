// file      : XSCRT/Traversal.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XSCRT_TRAVERSAL_HPP
#define XSCRT_TRAVERSAL_HPP

#include <cstddef>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "ace/XML_Utils/XSCRT/ExtendedTypeInfo.hpp"

namespace XSCRT
{
  namespace Traversal
  {
    template <typename B>
    class TraverserBase
    {
    protected:
      virtual ~TraverserBase () = default;

    public:
      virtual void trampoline (B &n) = 0;

      virtual void trampoline (B const &n) = 0;
    };

    template <typename B>
    class DispatcherBase
    {
    public:
      virtual ~DispatcherBase () = default;

      virtual void dispatch (B &n);

      virtual void dispatch (B const &n);

      void map (TypeId id, TraverserBase<B> &t)
      {
        Traversers &traversers = traversal_map_[id];
        traversers.push_back (std::addressof (t));
      }

    public:
      using Traversers = std::vector<TraverserBase<B> *>;
      using TraversalMap = std::map<TypeId, Traversers>;
      using Iterator = typename TraversalMap::const_iterator;

      Iterator begin () const
      {
        return traversal_map_.begin ();
      }

      Iterator end () const
      {
        return traversal_map_.end ();
      }

    private:
      struct TypeInfoComparator
      {
        bool operator() (ExtendedTypeInfo const &x, ExtendedTypeInfo const &y) const
        {
          return x.type_id () < y.type_id ();
        }
      };

      using LevelMap = std::map<ExtendedTypeInfo, std::size_t, TypeInfoComparator>;
      using TypeInfoSet = std::set<ExtendedTypeInfo, TypeInfoComparator>;

      static std::size_t compute_levels (ExtendedTypeInfo const &ti, std::size_t cur, LevelMap &map);

      static void flatten_tree (ExtendedTypeInfo const &ti, TypeInfoSet &set);

    private:
      TraversalMap traversal_map_{};
    };

    template <typename B>
    class Dispatcher : public virtual DispatcherBase<B>
    {
    public:
      Dispatcher () = default;

      void traverser (DispatcherBase<B> &d)
      {
        for (auto const &entry : d)
        {
          for (TraverserBase<B> *traverser : entry.second)
          {
            dispatcher_.map (entry.first, *traverser);
          }
        }
      }

    public:
      void dispatch (B &n) override
      {
        merge ();
        dispatcher_.dispatch (n);
      }

      void dispatch (B const &n) override
      {
        merge ();
        dispatcher_.dispatch (n);
      }

      using DispatcherBase<B>::begin;
      using DispatcherBase<B>::end;

    private:
      void merge ()
      {
        if (merge_)
        {
          for (auto const &entry : *this)
          {
            for (TraverserBase<B> *traverser : entry.second)
            {
              dispatcher_.map (entry.first, *traverser);
            }
          }

          merge_ = false;
        }
      }

    protected:
      template <typename X, typename A, typename I>
      void iterate_and_dispatch (I begin, I end, X &x, void (X::*next) (A &), A &a)
      {
        for (; begin != end;)
        {
          dispatch (*begin);

          if (++begin != end)
            (x.*next) (a);
        }
      }

    private:
      bool merge_{true};
      DispatcherBase<B> dispatcher_{};
    };

    template <typename T, typename B>
    struct Traverser
      : TraverserBase<B>
      , virtual Dispatcher<B>
    {
      using Type = T;

      Traverser ()
      {
        DispatcherBase<B>::map (TypeId (typeid (Type)), *this);
      }

      virtual void traverse (Type &)
      {
        abort ();
      }

      virtual void traverse (Type const &)
      {
        abort ();
      }

    protected:
      void trampoline (B &n) override
      {
        traverse (dynamic_cast<Type &> (n));
      }

      void trampoline (B const &n) override
      {
        traverse (dynamic_cast<Type const &> (n));
      }
    };
  }
}

#include <ace/XML_Utils/XSCRT/Traversal.tpp>

#endif // XSCRT_TRAVERSAL_HPP
