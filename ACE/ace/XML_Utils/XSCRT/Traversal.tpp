// file      : XSCRT/Traversal.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

namespace XSCRT
{
  namespace Traversal
  {
    template <typename B>
    void DispatcherBase<B>::dispatch (B &n)
    {
      LevelMap levels;

      ExtendedTypeInfo const &ti = extended_type_info (n);

      std::size_t const max (compute_levels (ti, 0, levels));

      for (std::size_t l{}; l <= max; ++l)
      {
        TypeInfoSet dispatched;

        for (typename LevelMap::value_type const &level_entry : levels)
        {
          if (level_entry.second != l)
            continue;

          typename TraversalMap::const_iterator const v =
            traversal_map_.find (level_entry.first.type_id ());
          if (v == traversal_map_.end ())
            continue;

          for (TraverserBase<B> *traverser : v->second)
            traverser->trampoline (n);

          flatten_tree (level_entry.first, dispatched);
        }

        // Remove traversed types from the level map.
        for (ExtendedTypeInfo const &type_info : dispatched)
          levels.erase (type_info);
      }
    }

    template <typename B>
    void DispatcherBase<B>::dispatch (B const &n)
    {
      LevelMap levels;

      ExtendedTypeInfo const &ti = extended_type_info (n);

      std::size_t const max (compute_levels (ti, 0, levels));

      for (std::size_t l{}; l <= max; ++l)
      {
        TypeInfoSet dispatched;

        for (typename LevelMap::value_type const &level_entry : levels)
        {
          if (level_entry.second != l)
            continue;

          typename TraversalMap::const_iterator const v =
            traversal_map_.find (level_entry.first.type_id ());
          if (v == traversal_map_.end ())
            continue;

          for (TraverserBase<B> *traverser : v->second)
            traverser->trampoline (n);

          flatten_tree (level_entry.first, dispatched);
        }

        // Remove traversed types from the level map.
        for (ExtendedTypeInfo const &type_info : dispatched)
          levels.erase (type_info);
      }
    }

    template <typename B>
    std::size_t DispatcherBase<B>::compute_levels (ExtendedTypeInfo const &ti, std::size_t cur, LevelMap &map)
    {
      std::size_t ret (cur);

      if (map.find (ti) == map.end () || map[ti] < cur)
        map[ti] = cur;

      for (ExtendedTypeInfo::BaseIterator i = ti.begin_base (); i != ti.end_base (); ++i)
      {
        std::size_t const t (compute_levels (i->type_info (), cur + 1, map));
        if (t > ret)
          ret = t;
      }

      return ret;
    }

    template <typename B>
    void DispatcherBase<B>::flatten_tree (ExtendedTypeInfo const &ti, TypeInfoSet &set)
    {
      set.insert (ti);

      for (ExtendedTypeInfo::BaseIterator i = ti.begin_base (); i != ti.end_base (); ++i)
      {
        flatten_tree (i->type_info (), set);
      }
    }
  }
}
