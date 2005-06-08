// file      : Example/Introspection/Traversal/Traversal.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Traversal.hpp"

#include <set>
#include <map>

using namespace Utility::Introspection;

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
  dispatch (SyntaxTree::Node* n)
  {
    LevelMap levels;

    unsigned long max = compute_levels (n->type_info (), 0, levels);

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

          if (v != traversal_map_.end ())
          {
            v->second->traverse (n);
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
//$Id$
