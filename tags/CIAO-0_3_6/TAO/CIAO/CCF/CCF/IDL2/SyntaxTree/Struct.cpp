// file      : CCF/IDL2/SyntaxTree/Struct.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Struct.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      //  StructDecl
      //
      //

      bool StructDecl::
      complete () const
      {
        try
        {
          //@@ I probably should add something like exists into
          //   DeclTable.

          //@@ gcc bug
          table ().template lookup<StructDef> (name ());

          return true;
        }
        catch (DeclarationTable::DeclarationNotFound const&)
        {
          //@@ ICE
          abort ();
        }
        catch (DeclarationTable::TypeMismatch const&)
        {
          return false;
        }
      }

      namespace
      {
        TypeInfo
        struct_decl_init_ ()
        {
          TypeInfo ti (typeid (StructDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       ForwardDeclarableTypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo struct_decl_ (struct_decl_init_ ());
      }

      TypeInfo const& StructDecl::
      static_type_info () { return struct_decl_; }


      //  StructForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        struct_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (StructForwardDecl));

          ti.add_base (
            Access::PUBLIC, true, StructDecl::static_type_info ());

          ti.add_base (
            Access::PUBLIC, true, TypeForwardDecl::static_type_info ());

          return ti;
        }

        TypeInfo struct_forward_decl_ (struct_forward_decl_init_ ());
      }

      TypeInfo const& StructForwardDecl::
      static_type_info () { return struct_forward_decl_; }


      //  StructDef
      //
      //
      namespace
      {
        TypeInfo
        struct_def_init_ ()
        {
          TypeInfo ti (typeid (StructDef));

          ti.add_base (
            Access::PUBLIC, true, StructDecl::static_type_info ());

          ti.add_base (
            Access::PUBLIC, true, TypeDef::static_type_info ());

          ti.add_base (
            Access::PUBLIC, true, Scope::static_type_info ());

          return ti;
        }

        TypeInfo struct_def_ (struct_def_init_ ());
      }

      TypeInfo const& StructDef::
      static_type_info () { return struct_def_; }

    }
  }
}
