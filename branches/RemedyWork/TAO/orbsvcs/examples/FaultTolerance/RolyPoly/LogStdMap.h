// file      : RolyPoly/LogStdMap.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef LOG_STD_MAP_H
#define LOG_STD_MAP_H

#include <map>

template <typename RI, typename RV>
class Log
{
public:
  typedef RI RecordIdType;
  typedef RV RecordValueType;

private:
  typedef
  std::map <RecordIdType, RecordValueType>
  Map_;

public:
  class Duplicate {};
  class NotFound {};

  void
  insert (RecordIdType const& ri, RecordValueType const& rv)
  {
    if (!map_.insert (std::make_pair (ri, rv)).second)
    {
      throw Duplicate ();
    }
  }

  bool
  contains (RecordIdType const& ri) const
  {
    return map_.count (ri) != 0;
  }


  RecordValueType const&
  lookup (RecordIdType const& ri) const
  {
    typename Map_::const_iterator i = map_.find (ri);

    if (i != map_.end ()) return i->second;
    else throw NotFound ();
  }

private:
  Map_ map_;
};

#endif // LOG_STD_MAP_H
