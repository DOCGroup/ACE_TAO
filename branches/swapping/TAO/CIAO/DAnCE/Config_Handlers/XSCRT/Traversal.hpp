// file      : XSCRT/Traversal.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XSCRT_TRAVERSAL_HPP
#define XSCRT_TRAVERSAL_HPP

#include <map>
#include <set>
#include <vector>

#include "XSCRT/ExtendedTypeInfo.hpp"

namespace XSCRT
{
  namespace Traversal
  {
    //
    //
    //
    template<typename B>
    class TraverserBase
    {
    protected:
      virtual
      ~TraverserBase ();

      //@@ VC6
    public:
      virtual void
      trampoline (B& n) = 0;

      /*@@ VC6
      template <typename X>
      friend class DispatcherBase;
      */
    };


    //
    //
    //
    template <typename B>
    class DispatcherBase
    {
    public:
      virtual
      ~DispatcherBase ();

      virtual void
      dispatch (B& n);

      void
      map (TypeId id, TraverserBase<B>& t)
      {
        //@@ VC6
        Traversers& traversers = traversal_map_[id];
        traversers.push_back (&t);
      }

    public:
      typedef
      std::vector<TraverserBase<B>*>
      Traversers;

      typedef
      std::map<TypeId, Traversers>
      TraversalMap;

      typedef
      typename TraversalMap::const_iterator
      Iterator;

      Iterator
      begin () const
      {
        return traversal_map_.begin ();
      }

      Iterator
      end () const
      {
        return traversal_map_.end ();
      }

    private:
      struct TypeInfoComparator
      {
        bool
        operator () (ExtendedTypeInfo const& x,
                     ExtendedTypeInfo const& y) const
        {
          return x.type_id () < y.type_id ();
        }
      };

      typedef
      std::map<ExtendedTypeInfo, unsigned long, TypeInfoComparator>
      LevelMap;

      typedef
      std::set<ExtendedTypeInfo, TypeInfoComparator>
      TypeInfoSet;

      static unsigned long
      compute_levels (ExtendedTypeInfo const& ti,
                      unsigned long cur,
                      LevelMap& map);

      static void
      flatten_tree (ExtendedTypeInfo const& ti, TypeInfoSet& set);

    private:
      TraversalMap traversal_map_;
    };


    //
    //
    //
    template <typename B>
    class Dispatcher : public virtual DispatcherBase<B>
    {
    public:
      void
      traverser (Dispatcher& d)
      {
        for (typename DispatcherBase<B>::Iterator
               i (d.begin ()), end (d.end ());
             i != end; ++i)
        {
          for (typename DispatcherBase<B>::Traversers::const_iterator
                 t (i->second.begin ()), end (i->second.end ());
               t != end; ++t)
          {
            dispatcher_.map (i->first, **t);
          }
        }
      }

    protected:
      DispatcherBase<B>&
      traverser ()
      {
        return dispatcher_;
      }

      template <typename I>
      void
      iterate_and_dispatch (I begin, I end)
      {
        for (; begin != end; ++begin)
        {
          traverser ().dispatch (*begin);
        }
      }

    private:
      DispatcherBase<B> dispatcher_;
    };



    //
    //
    //
    template <typename T, typename B>
    struct Traverser : TraverserBase<B>, virtual Dispatcher<B>
    {
      typedef
      T
      Type;

      Traverser ()
      {
        map (typeid (Type), *this);
      }

      virtual void
      traverse (Type&) = 0;

    protected:
      virtual void
      trampoline (B& n)
      {
        //cerr << "trampoline for " << &n << " to type "
        //     << typeid (Type).name () << endl;

        traverse (dynamic_cast<Type&> (n));
      }
    };
  }
}

#include "XSCRT/Traversal.ipp"
#include "XSCRT/Traversal.tpp"

#endif  // XSCRT_TRAVERSAL_HPP
