// file      : CCF/CIDL/SemanticGraph/Composition.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticGraph/Composition.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;


      // Composition
      //
      //
      namespace
      {
        TypeInfo
        composition_init_ ()
        {
          TypeInfo ti (typeid (Composition));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo composition_ (composition_init_ ());
      }

      TypeInfo const& Composition::
      static_type_info () { return composition_; }

      Composition::
      ~Composition ()
      {
      }


      // EntityComposition
      //
      //
      namespace
      {
        TypeInfo
        entity_composition_init_ ()
        {
          TypeInfo ti (typeid (EntityComposition));
          ti.add_base (
            Access::PUBLIC, true, Composition::static_type_info ());
          return ti;
        }

        TypeInfo entity_composition_ (entity_composition_init_ ());
      }

      TypeInfo const& EntityComposition::
      static_type_info () { return entity_composition_; }


      // ProcessComposition
      //
      //
      namespace
      {
        TypeInfo
        process_composition_init_ ()
        {
          TypeInfo ti (typeid (ProcessComposition));
          ti.add_base (
            Access::PUBLIC, true, Composition::static_type_info ());
          return ti;
        }

        TypeInfo process_composition_ (process_composition_init_ ());
      }

      TypeInfo const& ProcessComposition::
      static_type_info () { return process_composition_; }


      // ServiceComposition
      //
      //
      namespace
      {
        TypeInfo
        service_composition_init_ ()
        {
          TypeInfo ti (typeid (ServiceComposition));
          ti.add_base (
            Access::PUBLIC, true, Composition::static_type_info ());
          return ti;
        }

        TypeInfo service_composition_ (service_composition_init_ ());
      }

      TypeInfo const& ServiceComposition::
      static_type_info () { return service_composition_; }


      // SessionComposition
      //
      //
      namespace
      {
        TypeInfo
        session_composition_init_ ()
        {
          TypeInfo ti (typeid (SessionComposition));
          ti.add_base (
            Access::PUBLIC, true, Composition::static_type_info ());
          return ti;
        }

        TypeInfo session_composition_ (session_composition_init_ ());
      }

      TypeInfo const& SessionComposition::
      static_type_info () { return session_composition_; }
    }
  }
}
