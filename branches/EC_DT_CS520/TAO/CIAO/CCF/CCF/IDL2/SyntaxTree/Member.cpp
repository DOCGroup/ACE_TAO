// file      : CCF/IDL2/SyntaxTree/Member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Member.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      namespace
      {
        TypeInfo
        member_decl_init_ ()
        {
          TypeInfo ti (typeid (MemberDecl));
          ti.add_base (
            Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo member_decl_ (member_decl_init_ ());
      }

      TypeInfo const& MemberDecl::
      static_type_info () { return member_decl_; }
    }
  }
}
