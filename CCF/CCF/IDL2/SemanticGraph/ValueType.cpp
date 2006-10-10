// file      : CCF/IDL2/SemanticGraph/ValueType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/ValueType.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // ValueType
      //
      //
      namespace
      {
        TypeInfo
        value_type_init_ ()
        {
          TypeInfo ti (typeid (ValueType));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo value_type_ (value_type_init_ ());
      }

      TypeInfo const& ValueType::
      static_type_info () { return value_type_; }

      ValueType::
      ~ValueType ()
      {
      }

      // AbstractValueType
      //
      //
      namespace
      {
        TypeInfo
        abstract_value_type_init_ ()
        {
          TypeInfo ti (typeid (AbstractValueType));
          ti.add_base (Access::PUBLIC, true, ValueType::static_type_info ());
          return ti;
        }

        TypeInfo abstract_value_type_ (abstract_value_type_init_ ());
      }

      TypeInfo const& AbstractValueType::
      static_type_info () { return abstract_value_type_; }


      // ConcreteValueType
      //
      //
      namespace
      {
        TypeInfo
        concrete_value_type_init_ ()
        {
          TypeInfo ti (typeid (ConcreteValueType));
          ti.add_base (Access::PUBLIC, true, ValueType::static_type_info ());
          return ti;
        }

        TypeInfo concrete_value_type_ (concrete_value_type_init_ ());
      }

      TypeInfo const& ConcreteValueType::
      static_type_info () { return concrete_value_type_; }


      // ValueTypeFactory
      //
      //
      namespace
      {
        TypeInfo
        value_type_factory_init_ ()
        {
          TypeInfo ti (typeid (ValueTypeFactory));
          ti.add_base (
            Access::PUBLIC, true, TwoWayOperation::static_type_info ());
          return ti;
        }

        TypeInfo value_type_factory_ (value_type_factory_init_ ());
      }

      TypeInfo const& ValueTypeFactory::
      static_type_info () { return value_type_factory_; }
    }
  }
}
