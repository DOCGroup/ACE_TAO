// file      : CCF/IDL2/SyntaxTree/Interface.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/SyntaxTree/Interface.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      // InterfaceDecl
      //
      //
      namespace
      {
        TypeInfo
        interface_decl_init_ ()
        {
          TypeInfo ti (typeid (InterfaceDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo interface_decl_ (interface_decl_init_ ());
      }

      TypeInfo const& InterfaceDecl::
      static_type_info () { return interface_decl_; }


      // InterfaceForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        interface_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (InterfaceForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceDecl::static_type_info ());

          ti.add_base (Access::PUBLIC,
                       true,
                       TypeForwardDecl::static_type_info ());
          return ti;
        }

        TypeInfo interface_forward_decl_ (interface_forward_decl_init_ ());
      }

      TypeInfo const& InterfaceForwardDecl::
      static_type_info () { return interface_forward_decl_; }


      // InterfaceDef
      //
      //
      namespace
      {
        TypeInfo
        interface_def_init_ ()
        {
          TypeInfo ti (typeid (InterfaceDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, TypeDef::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo interface_def_ (interface_def_init_ ());
      }

      TypeInfo const& InterfaceDef::
      static_type_info () { return interface_def_; }


      // AbstractInterfaceDecl
      //
      //
      namespace
      {
        TypeInfo
        abstract_interface_decl_init_ ()
        {
          TypeInfo ti (typeid (AbstractInterfaceDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo abstract_interface_decl_ (abstract_interface_decl_init_ ());
      }

      TypeInfo const& AbstractInterfaceDecl::
      static_type_info () { return abstract_interface_decl_; }


      // AbstractInterfaceForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        abstract_interface_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (AbstractInterfaceForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceForwardDecl::static_type_info ());
          ti.add_base (Access::PUBLIC,
                       true,
                       AbstractInterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo abstract_interface_forward_decl_ (
          abstract_interface_forward_decl_init_ ());
      }

      TypeInfo const& AbstractInterfaceForwardDecl::
      static_type_info () { return abstract_interface_forward_decl_; }


      // AbstractInterfaceDef
      //
      //
      namespace
      {
        TypeInfo
        abstract_interface_def_init_ ()
        {
          TypeInfo ti (typeid (AbstractInterfaceDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       AbstractInterfaceDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, InterfaceDef::static_type_info ());
          return ti;
        }

        TypeInfo abstract_interface_def_ (abstract_interface_def_init_ ());
      }

      TypeInfo const& AbstractInterfaceDef::
      static_type_info () { return abstract_interface_def_; }


      // LocalInterfaceDecl
      //
      //
      namespace
      {
        TypeInfo
        local_interface_decl_init_ ()
        {
          TypeInfo ti (typeid (LocalInterfaceDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo local_interface_decl_ (local_interface_decl_init_ ());
      }

      TypeInfo const& LocalInterfaceDecl::
      static_type_info () { return local_interface_decl_; }


      // LocalInterfaceForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        local_interface_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (LocalInterfaceForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceForwardDecl::static_type_info ());
          ti.add_base (Access::PUBLIC,
                       true,
                       LocalInterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo local_interface_forward_decl_ (
          local_interface_forward_decl_init_ ());
      }

      TypeInfo const& LocalInterfaceForwardDecl::
      static_type_info () { return local_interface_forward_decl_; }


      // LocalInterfaceDef
      //
      //
      namespace
      {
        TypeInfo
        local_interface_def_init_ ()
        {
          TypeInfo ti (typeid (LocalInterfaceDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       LocalInterfaceDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, InterfaceDef::static_type_info ());
          return ti;
        }

        TypeInfo local_interface_def_ (local_interface_def_init_ ());
      }

      TypeInfo const& LocalInterfaceDef::
      static_type_info () { return local_interface_def_; }


      // UnconstrainedInterfaceDecl
      //
      //
      namespace
      {
        TypeInfo
        unconstrained_interface_decl_init_ ()
        {
          TypeInfo ti (typeid (UnconstrainedInterfaceDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo unconstrained_interface_decl_ (
          unconstrained_interface_decl_init_ ());
      }

      TypeInfo const& UnconstrainedInterfaceDecl::
      static_type_info () { return unconstrained_interface_decl_; }


      // UnconstrainedInterfaceForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        unconstrained_interface_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (UnconstrainedInterfaceForwardDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       InterfaceForwardDecl::static_type_info ());
          ti.add_base (Access::PUBLIC,
                       true,
                       UnconstrainedInterfaceDecl::static_type_info ());
          return ti;
        }

        TypeInfo unconstrained_interface_forward_decl_ (
          unconstrained_interface_forward_decl_init_ ());
      }

      TypeInfo const& UnconstrainedInterfaceForwardDecl::
      static_type_info () { return unconstrained_interface_forward_decl_; }


      // UnconstrainedInterfaceDef
      //
      //
      namespace
      {
        TypeInfo
        unconstrained_interface_def_init_ ()
        {
          TypeInfo ti (typeid (UnconstrainedInterfaceDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       UnconstrainedInterfaceDecl::static_type_info ());
          ti.add_base (Access::PUBLIC, true, InterfaceDef::static_type_info ());
          return ti;
        }

        TypeInfo unconstrained_interface_def_ (
          unconstrained_interface_def_init_ ());
      }

      TypeInfo const& UnconstrainedInterfaceDef::
      static_type_info () { return unconstrained_interface_def_; }
    }
  }
}
