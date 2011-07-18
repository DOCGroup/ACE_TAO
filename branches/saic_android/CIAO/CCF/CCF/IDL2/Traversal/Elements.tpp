// file      : CCF/IDL2/Traversal/Elements.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Traverser
      //
      //

      template<typename T>
      Traverser<T>::
      ~Traverser ()
      {
      }

      // Dispatcher
      //
      //

      template <typename T>
      Dispatcher<T>::
      ~Dispatcher ()
      {
      }

      template <typename T>
      void Dispatcher<T>::
      traverse (T& n)
      {
        LevelMap levels;

        unsigned long max = compute_levels (n.type_info (), 0, levels);


        //cerr << "starting dispatch process for "
        //     << n.type_info ().type_id () << " with "
        //     << max << " levels" << endl;

        for (unsigned long l = 0; l < max + 1; ++l)
        {
          TypeInfoSet dispatched;

          for (typename LevelMap::const_iterator
                 i (levels.begin ()), e (levels.end ());
               i != e; ++i)
          {
            if (i->second == l)
            {
              typename TraversalMap::const_iterator v (
                traversal_map_.find (i->first.type_id ()));

              if (v != traversal_map_.end ())
              {
                //cerr << "dispatching traversers for "
                //     << n.type_info ().type_id () << " as "
                //     << i->first.type_id () << endl;

                Traversers const& traversers (v->second);
                for (typename Traversers::const_iterator
                       ti (traversers.begin ()), te (traversers.end ());
                     ti != te; ++ti)
                {
                  (*ti)->trampoline (n);
                }

                flatten_tree (i->first, dispatched);
              }
            }
          }

          // Remove traversed types from level map.
          //
          for (typename TypeInfoSet::const_iterator i = dispatched.begin ();
               i != dispatched.end ();
               ++i)
          {
            levels.erase (*i);
          }
        }
      }

      template <typename T>
      unsigned long Dispatcher<T>::
      compute_levels (Introspection::TypeInfo const& ti,
                      unsigned long cur,
                      LevelMap& map)
      {
        unsigned long ret = cur;

        if (map.find (ti) == map.end () || map[ti] < cur) map[ti] = cur;

        for (Introspection::TypeInfo::BaseIterator i = ti.begin_base ();
             i != ti.end_base ();
             i++)
        {
          unsigned long t = compute_levels (i->type_info (), cur + 1, map);
          if (t > ret) ret = t;
        }

        return ret;
      }

      template <typename T>
      void Dispatcher<T>::
      flatten_tree (Introspection::TypeInfo const& ti, TypeInfoSet& set)
      {
        set.insert (ti);

        for (Introspection::TypeInfo::BaseIterator i = ti.begin_base ();
             i != ti.end_base ();
             i++)
        {
          flatten_tree (i->type_info (), set);
        }
      }
    }
  }
}
