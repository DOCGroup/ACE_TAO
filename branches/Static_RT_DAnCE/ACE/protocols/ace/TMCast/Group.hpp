// file      : ACE_TMCast/Group.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

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

    Group (ACE_INET_Addr const& addr, char const* id) ACE_THROW_SPEC((Failed));

  public:
    void
    send (void const* msg, size_t size)
      ACE_THROW_SPEC((InvalidArg, Failed, Aborted));

    size_t
    recv (void* msg, size_t size)
      ACE_THROW_SPEC((Failed, InsufficienSpace));

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
