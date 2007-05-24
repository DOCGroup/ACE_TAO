// file      : CCF/IDL2/SemanticGraph/Attribute.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Attribute.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Attribute
      //
      //
      namespace
      {
        TypeInfo
        attribute_init_ ()
        {
          TypeInfo ti (typeid (Attribute));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo attribute_ (attribute_init_ ());
      }

      TypeInfo const& Attribute::
      static_type_info () { return attribute_; }

      Attribute::
      ~Attribute ()
      {
      }


      // GetRaises
      //
      //
      namespace
      {
        TypeInfo
        get_raises_init_ ()
        {
          TypeInfo ti (typeid (GetRaises));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo get_raises_ (get_raises_init_ ());
      }

      TypeInfo const& GetRaises::
      static_type_info () { return get_raises_; }


      // ReadAttribute
      //
      //
      namespace
      {
        TypeInfo
        read_attribute_init_ ()
        {
          TypeInfo ti (typeid (ReadAttribute));
          ti.add_base (Access::PUBLIC, true, Attribute::static_type_info ());
          return ti;
        }

        TypeInfo read_attribute_ (read_attribute_init_ ());
      }

      TypeInfo const& ReadAttribute::
      static_type_info () { return read_attribute_; }


      // SetRaises
      //
      //
      namespace
      {
        TypeInfo
        set_raises_init_ ()
        {
          TypeInfo ti (typeid (SetRaises));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo set_raises_ (set_raises_init_ ());
      }

      TypeInfo const& SetRaises::
      static_type_info () { return set_raises_; }


      // WriteAttribute
      //
      //
      namespace
      {
        TypeInfo
        write_attribute_init_ ()
        {
          TypeInfo ti (typeid (WriteAttribute));
          ti.add_base (Access::PUBLIC, true, Attribute::static_type_info ());
          return ti;
        }

        TypeInfo write_attribute_ (write_attribute_init_ ());
      }

      TypeInfo const& WriteAttribute::
      static_type_info () { return write_attribute_; }


      // ReadWriteAttribute
      //
      //
      namespace
      {
        TypeInfo
        read_write_attribute_init_ ()
        {
          TypeInfo ti (typeid (ReadWriteAttribute));
          ti.add_base (
            Access::PUBLIC, true, ReadAttribute::static_type_info ());
          ti.add_base (
            Access::PUBLIC, true, WriteAttribute::static_type_info ());
          return ti;
        }

        TypeInfo read_write_attribute_ (read_write_attribute_init_ ());
      }

      TypeInfo const& ReadWriteAttribute::
      static_type_info () { return read_write_attribute_; }
    }
  }
}
