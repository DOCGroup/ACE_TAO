// file      : CCF/IDL2/SyntaxTree/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Operation.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // AttributeDecl
      //
      //
      namespace
      {
        TypeInfo
        attribute_decl_init_ ()
        {
          TypeInfo ti (typeid (AttributeDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo attribute_decl_ (attribute_decl_init_ ());
      }

      TypeInfo const& AttributeDecl::
      static_type_info () { return attribute_decl_; }


      // OperationParameter
      //
      //
      namespace
      {
        TypeInfo
        operation_parameter_init_ ()
        {
          TypeInfo ti (typeid (OperationParameter));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo operation_parameter_ (operation_parameter_init_ ());
      }

      TypeInfo const& OperationParameter::
      static_type_info () { return operation_parameter_; }


      // OperationDecl
      //
      //
      namespace
      {
        TypeInfo
        operation_decl_init_ ()
        {
          TypeInfo ti (typeid (OperationDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo operation_decl_ (operation_decl_init_ ());
      }

      TypeInfo const& OperationDecl::
      static_type_info () { return operation_decl_; }
    }
  }
}
