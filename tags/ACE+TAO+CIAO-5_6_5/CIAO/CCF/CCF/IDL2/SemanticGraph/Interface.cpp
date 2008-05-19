// file      : CCF/IDL2/SemanticGraph/Interface.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Interface.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;


      // Interface
      //
      //
      namespace
      {
        TypeInfo
        interface_init_ ()
        {
          TypeInfo ti (typeid (Interface));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo interface_ (interface_init_ ());
      }

      TypeInfo const& Interface::
      static_type_info () { return interface_; }

      Interface::
      ~Interface ()
      {
      }

      // Supports
      //
      //
      namespace
      {
        TypeInfo
        supports_init_ ()
        {
          TypeInfo ti (typeid (Supports));
          ti.add_base (Access::PUBLIC, true, IsA::static_type_info ());
          return ti;
        }

        TypeInfo supports_ (supports_init_ ());
      }

      TypeInfo const& Supports::
      static_type_info () { return supports_; }


      // AbstractInterface
      //
      //
      namespace
      {
        TypeInfo
        abstract_interface_init_ ()
        {
          TypeInfo ti (typeid (AbstractInterface));
          ti.add_base (Access::PUBLIC, true, Interface::static_type_info ());
          return ti;
        }

        TypeInfo abstract_interface_ (abstract_interface_init_ ());
      }

      TypeInfo const& AbstractInterface::
      static_type_info () { return abstract_interface_; }


      // LocalInterface
      //
      //
      namespace
      {
        TypeInfo
        local_interface_init_ ()
        {
          TypeInfo ti (typeid (LocalInterface));
          ti.add_base (Access::PUBLIC, true, Interface::static_type_info ());
          return ti;
        }

        TypeInfo local_interface_ (local_interface_init_ ());
      }

      TypeInfo const& LocalInterface::
      static_type_info () { return local_interface_; }


      // UnconstrainedInterface
      //
      //
      namespace
      {
        TypeInfo
        unconstrained_interface_init_ ()
        {
          TypeInfo ti (typeid (UnconstrainedInterface));
          ti.add_base (Access::PUBLIC, true, Interface::static_type_info ());
          return ti;
        }

        TypeInfo unconstrained_interface_ (unconstrained_interface_init_ ());
      }

      TypeInfo const& UnconstrainedInterface::
      static_type_info () { return unconstrained_interface_; }
    }
  }
}
