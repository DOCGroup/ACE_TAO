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
        NodePtr n = TypeDecl::dynamic_type (ti);

        if (n != 0) return n;

        // Try virtual type.
        //

        NodePtr v (virtual_type ());

        if (v != 0) return v->dynamic_type (ti);
        else return v;
      }

      NodePtr TypedefDecl::
      virtual_type ()
      {
        // Try to return TypeDef if there is one, otherwise TypeDecl.
        //
        try
        {
          TypeDefPtr def (table ().lookup<TypeDef> (type_));

          return def->clone_temporary (name ().simple (),
                                       order (),
                                       scope ());
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
            TypeDeclPtr decl (table ().lookup<TypeDecl> (type_));

            return decl->clone_temporary (name ().simple (),
                                          order (),
                                          scope ());
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


      bool TypedefDecl::
      defined () const
      {
        return is_a (TypeDef::static_type_info ());
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
