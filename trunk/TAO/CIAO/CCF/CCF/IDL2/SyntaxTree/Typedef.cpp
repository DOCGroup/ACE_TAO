// file      : CCF/IDL2/SyntaxTree/Typedef.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Typedef.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      TypeDeclPtr TypedefDecl::
      clone_typedef_temporary (SimpleName const& name,
                               Order const& order,
                               ScopePtr const& scope)
      {
        return underlying_type ()->clone_typedef_temporary (
          name,
          order,
          scope);
      }


      //@@ It seems throwing exception in DeclarationTable::lookup ()
      //   in case declaration not found or type mismatched is not
      //   not right.
      //
      bool TypedefDecl::
      is_a (Introspection::TypeInfo const& ti) const
      {
        if (TypeDecl::is_a (ti)) return true;

        try
        {
          table ().lookup (type_, ti);
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

      NodePtr TypedefDecl::
      dynamic_type (Introspection::TypeInfo const& ti)
      {
        if (TypeDecl::is_a (ti))
        {
          return NodePtr (ReferenceCounting::add_ref (this));
        }

        // Try virtual type.
        //

        NodePtr v (virtual_type ());

        if (v != 0) return v->dynamic_type (ti);
        else return v;
      }

      TypeDeclPtr TypedefDecl::
      underlying_type () const
      {
        // Try to return TypeDef if there is one, otherwise TypeDecl.
        //
        try
        {
          return table ().lookup<TypeDef> (type_);
        }
        catch (DeclarationTable::DeclarationNotFound const&)
        {
          //@@ ICE
          abort ();
        }
        catch (DeclarationTable::TypeMismatch const&)
        {
          try
          {
            return table ().lookup<TypeDecl> (type_);
          }
          catch (DeclarationTable::DeclarationNotFound const&)
          {
            //@@ maybe it's a goofd idea to just let these exception
            //   go out of the function. And somewhere in the driver
            //   they should be cought and reported as ICE.

            //@@ ICE
            abort ();
          }
          catch (DeclarationTable::TypeMismatch const&)
          {
            //@@ ICE
            abort ();
          }
        }
      }


      NodePtr TypedefDecl::
      virtual_type ()
      {
        return underlying_type ()->clone_typedef_temporary (
          name ().simple (),
          order (),
          scope ());
      }

      namespace
      {
        TypeInfo
        typedef_decl_init_ ()
        {
          TypeInfo ti (typeid (TypedefDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo typedef_decl_ (typedef_decl_init_ ());
      }

      TypeInfo const& TypedefDecl::
      static_type_info () { return typedef_decl_; }
    }
  }
}
