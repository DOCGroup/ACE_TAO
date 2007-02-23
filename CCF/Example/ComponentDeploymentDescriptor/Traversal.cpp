// file      : CCF/Example/ComponentDeploymentDescriptor/Traversal.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "Traversal.hpp"

using namespace Introspection;

namespace CDD
{
  namespace Traversal
  {

    // Dispatcher
    //
    //

    struct TypeInfoComparator
    {
      bool
      operator () (TypeInfo const& x, TypeInfo const& y) const
      {
        return x.type_id () < y.type_id ();
      }
    };

    typedef
    std::map<TypeInfo, unsigned long, TypeInfoComparator>
    LevelMap;

    typedef
    std::set<TypeInfo, TypeInfoComparator>
    TypeInfoSet;

    unsigned long
    compute_levels (TypeInfo const& ti, unsigned long cur, LevelMap& map)
    {
      unsigned long ret = cur;

      if (map.find (ti) == map.end () || map[ti] < cur) map[ti] = cur;

      for (TypeInfo::BaseIterator i = ti.begin_base ();
           i != ti.end_base ();
           i++)
      {
        unsigned long t = compute_levels (i->type_info (), cur + 1, map);
        if (t > ret) ret = t;
      }

      return ret;
    }

    void
    flatten_tree (TypeInfo const& ti, TypeInfoSet& set)
    {
      set.insert (ti);

      for (TypeInfo::BaseIterator i = ti.begin_base ();
           i != ti.end_base ();
           i++)
      {
        flatten_tree (i->type_info (), set);
      }
    }

    void Dispatcher::
    dispatch (SyntaxTree::NodePtr const& n)
    {
      LevelMap levels;

      unsigned long max = compute_levels (n->type_info (), 0, levels);

      //cerr << "starting dispatch process for "
      //     << n->type_info ().type_id () << " with "
      //     << max << " levels" << endl;

      for (unsigned long l = 0; l < max + 1; l++)
      {
        TypeInfoSet dispatched;

        for (LevelMap::const_iterator i = levels.begin ();
             i != levels.end ();
             i++)
        {
          if (i->second == l)
          {
            TraversalMap::const_iterator v =
              traversal_map_.find (i->first.type_id ());

            if (v != traversal_map_.end () && !(v->second.suppressed))
            {
              //cerr << "dispatching traverser for "
              //     << n->type_info ().type_id () << " as "
              //     << i->first.type_id () << endl;

              v->second.traverser->traverse (n);
              flatten_tree (i->first, dispatched);
            }
          }
        }

        // Remove traversed types from level map.
        for (TypeInfoSet::const_iterator i = dispatched.begin ();
             i != dispatched.end ();
             i++)
        {
          levels.erase (*i);
        }
      }
    }
  }
}
