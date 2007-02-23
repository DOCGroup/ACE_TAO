// file      : CCF/IDL2/SemanticGraph/ValueTypeMember.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/ValueTypeMember.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;


      // ValueTypeMember
      //
      //
      namespace
      {
        TypeInfo
        value_type_member_init_ ()
        {
          TypeInfo ti (typeid (ValueTypeMember));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo
        value_type_member_ (value_type_member_init_ ());
      }

      TypeInfo const& ValueTypeMember::
      static_type_info () { return value_type_member_; }

      ValueTypeMember::
      ~ValueTypeMember ()
      {
      }


      // ValueTypePrivateMember
      //
      //
      namespace
      {
        TypeInfo
        value_type_private_member_init_ ()
        {
          TypeInfo ti (typeid (ValueTypePrivateMember));
          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeMember::static_type_info ());
          return ti;
        }

        TypeInfo
        value_type_private_member_ (value_type_private_member_init_ ());
      }

      TypeInfo const& ValueTypePrivateMember::
      static_type_info () { return value_type_private_member_; }


      // ValueTypePublicMember
      //
      //
      namespace
      {
        TypeInfo
        value_type_public_member_init_ ()
        {
          TypeInfo ti (typeid (ValueTypePublicMember));
          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeMember::static_type_info ());
          return ti;
        }

        TypeInfo
        value_type_public_member_ (value_type_public_member_init_ ());
      }

      TypeInfo const& ValueTypePublicMember::
      static_type_info () { return value_type_public_member_; }
    }
  }
}
