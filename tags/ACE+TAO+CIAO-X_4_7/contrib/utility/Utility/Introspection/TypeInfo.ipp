// file      : Utility/Introspection/TypeInfo.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace Introspection
  {
    // BaseInfo
    //
    //

    inline BaseInfo::
    BaseInfo (Access::Value access, bool virtual_base, TypeInfo const& ti)
        : ti_ (&ti),
          virtual_base_ (virtual_base),
          access_ (access)
    {
    }

    inline TypeInfo const& BaseInfo::
    type_info () const
    {
      return *ti_;
    }


    inline Access::Value BaseInfo::
    access () const
    {
      return access_;
    }

    inline bool BaseInfo::
    virtual_base () const
    {
      return virtual_base_;
    }


    // TypeInfo
    //
    //
    inline TypeInfo::
    TypeInfo (TypeId const& tid)
        : tid_ (tid)
    {
    }

    inline TypeId TypeInfo::
    type_id () const
    {
      return tid_;
    }

    inline TypeInfo::BaseIterator TypeInfo::
    begin_base () const
    {
      return base_.begin ();
    }


    inline TypeInfo::BaseIterator TypeInfo::
    end_base () const
    {
      return base_.end ();
    }

    inline void TypeInfo::
    add_base (Access::Value access, bool virtual_base, TypeInfo const& ti)
    {
      base_.push_back (BaseInfo (access, virtual_base, ti));
    }
  }
}
//$Id$
