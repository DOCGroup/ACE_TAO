// file      : CCF/IDL3/SyntaxTree/Component.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SyntaxTree/Component.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {
      
      // ComponentDecl
      //
      //
      namespace
      {
        TypeInfo
        component_decl_init_ ()
        {
          TypeInfo ti (typeid (ComponentDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo component_decl_ (component_decl_init_ ());
      }

      TypeInfo const& ComponentDecl::
      static_type_info () { return component_decl_; }

      
      // ComponentForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        component_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (ComponentForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       ComponentDecl::static_type_info ());

          ti.add_base (Access::PUBLIC,
                       true,
                       TypeForwardDecl::static_type_info ());
          return ti;
        }

        TypeInfo component_forward_decl_ (component_forward_decl_init_ ());
      }

      TypeInfo const& ComponentForwardDecl::
      static_type_info () { return component_forward_decl_; }


      // ComponentDef
      //
      //
      namespace
      {
        TypeInfo
        component_def_init_ ()
        {
          TypeInfo ti (typeid (ComponentDef));
          ti.add_base (Access::PUBLIC, true, TypeDef::static_type_info ());
          ti.add_base (Access::PUBLIC,
                       true,
                       ComponentDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo component_def_ (component_def_init_ ());
      }

      TypeInfo const& ComponentDef::
      static_type_info () { return component_def_; }
    }
  }
}
