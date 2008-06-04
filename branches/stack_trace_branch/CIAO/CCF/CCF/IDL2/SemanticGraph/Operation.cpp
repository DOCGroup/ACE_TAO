// file      : CCF/IDL2/SemanticGraph/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Receives
      //
      //
      namespace
      {
        TypeInfo
        receives_init_ ()
        {
          TypeInfo ti (typeid (Receives));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo receives_ (receives_init_ ());
      }

      TypeInfo const& Receives::
      static_type_info () { return receives_; }


      // Returns
      //
      //
      namespace
      {
        TypeInfo
        returns_init_ ()
        {
          TypeInfo ti (typeid (Returns));
          ti.add_base (Access::PUBLIC, true, Belongs::static_type_info ());
          return ti;
        }

        TypeInfo returns_ (returns_init_ ());
      }

      TypeInfo const& Returns::
      static_type_info () { return returns_; }


      // Raises
      //
      //
      namespace
      {
        TypeInfo
        raises_init_ ()
        {
          TypeInfo ti (typeid (Raises));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo raises_ (raises_init_ ());
      }

      TypeInfo const& Raises::
      static_type_info () { return raises_; }



      // Parameter
      //
      //
      namespace
      {
        TypeInfo
        parameter_init_ ()
        {
          TypeInfo ti (typeid (Parameter));
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo parameter_ (parameter_init_ ());
      }

      TypeInfo const& Parameter::
      static_type_info () { return parameter_; }

      Parameter::
      ~Parameter ()
      {
      }


      // InParameter
      //
      //
      namespace
      {
        TypeInfo
        in_parameter_init_ ()
        {
          TypeInfo ti (typeid (InParameter));
          ti.add_base (Access::PUBLIC, true, Parameter::static_type_info ());
          return ti;
        }

        TypeInfo in_parameter_ (in_parameter_init_ ());
      }

      TypeInfo const& InParameter::
      static_type_info () { return in_parameter_; }


      // OutParameter
      //
      //
      namespace
      {
        TypeInfo
        out_parameter_init_ ()
        {
          TypeInfo ti (typeid (OutParameter));
          ti.add_base (Access::PUBLIC, true, Parameter::static_type_info ());
          return ti;
        }

        TypeInfo out_parameter_ (out_parameter_init_ ());
      }

      TypeInfo const& OutParameter::
      static_type_info () { return out_parameter_; }


      // InOutParameter
      //
      //
      namespace
      {
        TypeInfo
        in_out_parameter_init_ ()
        {
          TypeInfo ti (typeid (InOutParameter));
          ti.add_base (Access::PUBLIC, true, Parameter::static_type_info ());
          return ti;
        }

        TypeInfo in_out_parameter_ (in_out_parameter_init_ ());
      }

      TypeInfo const& InOutParameter::
      static_type_info () { return in_out_parameter_; }


      // Operation
      //
      //
      namespace
      {
        TypeInfo
        operation_init_ ()
        {
          TypeInfo ti (typeid (Operation));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo operation_ (operation_init_ ());
      }

      TypeInfo const& Operation::
      static_type_info () { return operation_; }

      Operation::
      ~Operation ()
      {
      }


      // OneWayOperation
      //
      //
      namespace
      {
        TypeInfo
        one_way_operation_init_ ()
        {
          TypeInfo ti (typeid (OneWayOperation));
          ti.add_base (Access::PUBLIC, true, Operation::static_type_info ());
          return ti;
        }

        TypeInfo one_way_operation_ (one_way_operation_init_ ());
      }

      TypeInfo const& OneWayOperation::
      static_type_info () { return one_way_operation_; }


      // TwoWayOperation
      //
      //
      namespace
      {
        TypeInfo
        two_way_operation_init_ ()
        {
          TypeInfo ti (typeid (TwoWayOperation));
          ti.add_base (Access::PUBLIC, true, Operation::static_type_info ());
          return ti;
        }

        TypeInfo two_way_operation_ (two_way_operation_init_ ());
      }

      TypeInfo const& TwoWayOperation::
      static_type_info () { return two_way_operation_; }
    }
  }
}
