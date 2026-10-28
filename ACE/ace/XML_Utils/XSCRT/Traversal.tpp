// file      : XSCRT/Traversal.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

namespace XSCRT
{
  namespace Traversal
  {
    template <typename B>
    void DispatcherBase<B>::
    dispatch (B& n)
    {
      LevelMap levels;

      ExtendedTypeInfo const& ti = extended_type_info (n);

      unsigned long max = compute_levels (ti, 0, levels);


      //wcerr << "starting dispatch process for "
      //      << typeid (n).name () << " with "
      //      << max << " levels" << endl;

      for (unsigned long l = 0; l < max + 1; ++l)
      {
        TypeInfoSet dispatched;

        for (auto const& [type_info, level] : levels)
        {
          if (level != l)
            continue;

          auto const v (traversal_map_.find (type_info.type_id ()));
          if (v == traversal_map_.end ())
            continue;

          for (TraverserBase<B>* traverser : v->second)
            traverser->trampoline (n);

          flatten_tree (type_info, dispatched);
        }

        // Remove traversed types from the level map.
        for (ExtendedTypeInfo const& type_info : dispatched)
          levels.erase (type_info);
      }
    }

    template <typename B>
    void DispatcherBase<B>::
    dispatch (B const& n)
    {
      LevelMap levels;

      ExtendedTypeInfo const& ti = extended_type_info (n);

      unsigned long max = compute_levels (ti, 0, levels);


      //wcerr << "starting dispatch process for "
      //      << typeid (n).name () << " with "
      //      << max << " levels" << endl;

      //wcerr << "using " << &traversal_map_ << " traversal map" << endl;

      for (unsigned long l = 0; l < max + 1; ++l)
      {
        TypeInfoSet dispatched;

        for (auto const& [type_info, level] : levels)
        {
          if (level != l)
            continue;

          auto const v (traversal_map_.find (type_info.type_id ()));
          if (v == traversal_map_.end ())
            continue;

          for (TraverserBase<B>* traverser : v->second)
            traverser->trampoline (n);

          flatten_tree (type_info, dispatched);
        }

        // Remove traversed types from the level map.
        for (ExtendedTypeInfo const& type_info : dispatched)
          levels.erase (type_info);
      }
    }

    template <typename B>
    unsigned long DispatcherBase<B>::
    compute_levels (ExtendedTypeInfo const& ti,
                    unsigned long cur,
                    LevelMap& map)
    {
      unsigned long ret = cur;

      if (map.find (ti) == map.end () || map[ti] < cur) map[ti] = cur;

      for (ExtendedTypeInfo::BaseIterator i = ti.begin_base ();
           i != ti.end_base ();
           i++)
      {
        unsigned long t = compute_levels (i->type_info (), cur + 1, map);
        if (t > ret) ret = t;
      }

      return ret;
    }

    template <typename B>
    void DispatcherBase<B>::
    flatten_tree (ExtendedTypeInfo const& ti, TypeInfoSet& set)
    {
      set.insert (ti);

      for (ExtendedTypeInfo::BaseIterator i = ti.begin_base ();
           i != ti.end_base ();
           i++)
      {
        flatten_tree (i->type_info (), set);
      }
    }
  }
}
