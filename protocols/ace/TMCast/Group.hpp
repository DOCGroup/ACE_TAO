// $Id$
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#ifndef TMCAST_GROUP_HPP
#define TMCAST_GROUP_HPP

#include <ace/Auto_Ptr.h>
#include <ace/INET_Addr.h>

#include "Export.hpp"

namespace ACE_TMCast
{
  class ACE_TMCast_Export Group
  {
  public:
    class Aborted {};
    class Failed {};
    class InvalidArg {};
    class InsufficienSpace {};

  public:
    ~Group ();

    Group (ACE_INET_Addr const& addr, char const* id);

  public:
    void
    send (void const* msg, size_t size);

    size_t
    recv (void* msg, size_t size);

  private:
    bool
    failed ();

  private:
    class GroupImpl;
    auto_ptr<GroupImpl> pimpl_;

  private:
    Group (Group const&);

    Group&
    operator= (Group const&);
  };
}

#endif // TMCAST_GROUP_HPP
