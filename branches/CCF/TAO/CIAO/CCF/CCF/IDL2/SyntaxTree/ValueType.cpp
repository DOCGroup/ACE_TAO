// file      : CCF/IDL2/SyntaxTree/ValueType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/SyntaxTree/ValueType.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // ValueDecl
      //
      //
      namespace
      {
        TypeInfo
        value_decl_init_ ()
        {
          TypeInfo ti (typeid (ValueTypeDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo value_decl_ (value_decl_init_ ());
      }

      TypeInfo const& ValueTypeDecl::
      static_type_info () { return value_decl_; }


      // ValueTypeForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        value_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (ValueTypeForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeDecl::static_type_info ());
          
          ti.add_base (Access::PUBLIC,
                       true,
                       TypeForwardDecl::static_type_info ());
          return ti;
        }

        TypeInfo value_forward_decl_ (value_forward_decl_init_ ());
      }

      TypeInfo const& ValueTypeForwardDecl::
      static_type_info () { return value_forward_decl_; }


      // ValueTypeDef
      //
      //
      namespace
      {
        TypeInfo
        value_def_init_ ()
        {
          TypeInfo ti (typeid (ValueTypeDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, TypeDef::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo value_def_ (value_def_init_ ());
      }

      TypeInfo const& ValueTypeDef::
      static_type_info () { return value_def_; }
    }
  }
}
