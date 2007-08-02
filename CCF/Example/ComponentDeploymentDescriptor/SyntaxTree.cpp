// file      : CCF/Example/ComponentDeploymentDescriptor/SyntaxTree.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "SyntaxTree.hpp"

using namespace Introspection;

namespace CDD
{
  namespace SyntaxTree
  {
    // Node
    //
    //
    namespace
    {
      TypeInfo
      node_init_ ()
      {
        TypeInfo ti (typeid (Node));
        // I don't really need this information
        // ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
        return ti;
      }

      TypeInfo node_ (node_init_ ());
    }

    TypeInfo const& Node::
    static_type_info () { return node_; }


    // PortInstance
    //
    //
    namespace
    {
      TypeInfo
      port_instance_init_ ()
      {
        TypeInfo ti (typeid (PortInstance));
        ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
        return ti;
      }

      TypeInfo port_instance_ (port_instance_init_ ());
    }

    TypeInfo const& PortInstance::
    static_type_info () { return port_instance_; }


    // ComponentInstance
    //
    //
    namespace
    {
      TypeInfo
      component_instance_init_ ()
      {
        TypeInfo ti (typeid (ComponentInstance));
        ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
        return ti;
      }

      TypeInfo component_instance_ (component_instance_init_ ());
    }

    TypeInfo const& ComponentInstance::
    static_type_info () { return component_instance_; }


    // Connection
    //
    //
    namespace
    {
      TypeInfo
      connection_init_ ()
      {
        TypeInfo ti (typeid (Connection));
        ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
        return ti;
      }

      TypeInfo connection_ (connection_init_ ());
    }

    TypeInfo const& Connection::
    static_type_info () { return connection_; }


    // Descriptor
    //
    //
    namespace
    {
      TypeInfo
      descriptor_init_ ()
      {
        TypeInfo ti (typeid (Descriptor));
        ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
        return ti;
      }

      TypeInfo descriptor_ (descriptor_init_ ());
    }

    TypeInfo const& Descriptor::
    static_type_info () { return descriptor_; }
  }
}
