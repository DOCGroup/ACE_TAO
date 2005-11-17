// file      : Utility/Introspection/Object.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace Introspection
  {
    inline TypeInfo const& Object::
    type_info () const throw ()
    {
      return *type_info_;
    }

    inline Object::
    ~Object ()
    {
    }

    inline Object::
    Object () throw ()
    {
      type_info (static_type_info ());
    }


    inline Object::
    Object (Object const&) throw ()
    {
      type_info (static_type_info ());
    }


    inline Object& Object::
    operator= (Object const&) throw ()
    {
      return *this;
    }

    inline void Object::
    type_info (TypeInfo const& tid) throw ()
    {
      type_info_ = &tid;
    }
  }
}
//$Id$
