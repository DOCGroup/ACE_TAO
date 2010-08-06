// file      : RolyPoly/LogACE_RB_Tree.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef LOG_ACE_RB_TREE_H
#define LOG_ACE_RB_TREE_H

#include <ace/RB_Tree.h>
#include <ace/Synch.h>

template <typename RI, typename RV>
class Log
{
public:
  typedef RI RecordIdType;
  typedef RV RecordValueType;

private:
  typedef
  ACE_RB_Tree <RecordIdType,
               RecordValueType,
               ACE_Less_Than<RecordIdType>,
               ACE_Null_Mutex>
  Map_;

public:
  class Duplicate {};
  class NotFound {};

  void
  insert (RecordIdType const& ri, RecordValueType const& rv)
  {
    if (map_.bind (ri, rv) != 0) throw Duplicate ();
  }

  bool
  contains (RecordIdType const& ri) const
  {
    // Guess what: ACE_RB_Tree::find() is not const.
    //
    Map_& m = const_cast<Map_&> (map_);

    RecordValueType tmp;

    return m.find (ri, tmp) == 0;
  }


  RecordValueType const&
  lookup (RecordIdType const& ri) const
  {
    Map_& m = const_cast<Map_&> (map_);

    typename Map_::ENTRY* entry;

    if (m.find (ri, entry) != 0) throw NotFound ();

    return entry->item ();
  }

private:
  Map_ map_;
};

#endif // LOG_ACE_RB_TREE_H
