// file      : CCF/IDL3/SyntaxTree/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL3/SyntaxTree/Operation.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {
      // ProvidesDecl
      //
      //
      namespace
      {
        TypeInfo
        provides_decl_init_ ()
        {
          TypeInfo ti (typeid (ProvidesDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo provides_decl_ (provides_decl_init_ ());
      }

      TypeInfo const& ProvidesDecl::
      static_type_info () { return provides_decl_; }


      // UsesDecl
      //
      //
      namespace
      {
        TypeInfo
        uses_decl_init_ ()
        {
          TypeInfo ti (typeid (UsesDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo uses_decl_ (uses_decl_init_ ());
      }

      TypeInfo const& UsesDecl::
      static_type_info () { return uses_decl_; }


      // PublishesDecl
      //
      //
      namespace
      {
        TypeInfo
        publishes_decl_init_ ()
        {
          TypeInfo ti (typeid (PublishesDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo publishes_decl_ (publishes_decl_init_ ());
      }

      TypeInfo const& PublishesDecl::
      static_type_info () { return publishes_decl_; }


      // EmitsDecl
      //
      //
      namespace
      {
        TypeInfo
        emits_decl_init_ ()
        {
          TypeInfo ti (typeid (EmitsDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo emits_decl_ (emits_decl_init_ ());
      }

      TypeInfo const& EmitsDecl::
      static_type_info () { return emits_decl_; }


      // ConsumesDecl
      //
      //
      namespace
      {
        TypeInfo
        consumes_decl_init_ ()
        {
          TypeInfo ti (typeid (ConsumesDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo consumes_decl_ (consumes_decl_init_ ());
      }

      TypeInfo const& ConsumesDecl::
      static_type_info () { return consumes_decl_; }

      
      // HomeFactoryParameter
      //
      //
      namespace
      {
        TypeInfo
        home_factory_parameter_init_ ()
        {
          TypeInfo ti (typeid (HomeFactoryParameter));
          ti.add_base (Access::PUBLIC,
                       true,
                       OperationParameter::static_type_info ());
          return ti;
        }

        TypeInfo home_factory_parameter_ (home_factory_parameter_init_ ());
      }

      TypeInfo const& HomeFactoryParameter::
      static_type_info () { return home_factory_parameter_; }



      // HomeFactoryDecl
      //
      //
      namespace
      {
        TypeInfo
        home_factory_decl_init_ ()
        {
          TypeInfo ti (typeid (HomeFactoryDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       OperationDecl::static_type_info ());
          return ti;
        }

        TypeInfo home_factory_decl_ (home_factory_decl_init_ ());
      }

      TypeInfo const& HomeFactoryDecl::
      static_type_info () { return home_factory_decl_; }

    }
  }
}
