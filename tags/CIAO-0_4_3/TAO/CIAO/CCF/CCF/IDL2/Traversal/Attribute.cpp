// file      : CCF/IDL2/Traversal/Attribute.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Attribute.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Attribute
      //
      //

      void Attribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        post (a);
      }

      void Attribute::
      pre (Type&)
      {
      }

      void Attribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void Attribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void Attribute::
      name (Type&)
      {
      }

      void Attribute::
      post (Type&)
      {
      }

      // ReadAttribute
      //
      //
      void ReadAttribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        post (a);
      }

      void ReadAttribute::
      pre (Type&)
      {
      }

      void ReadAttribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void ReadAttribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void ReadAttribute::
      name (Type&)
      {
      }

      void ReadAttribute::
      post (Type&)
      {
      }

      // ReadWriteAttribute
      //
      //
      void ReadWriteAttribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        post (a);
      }

      void ReadWriteAttribute::
      pre (Type&)
      {
      }

      void ReadWriteAttribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void ReadWriteAttribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void ReadWriteAttribute::
      name (Type&)
      {
      }

      void ReadWriteAttribute::
      post (Type&)
      {
      }
    }
  }
}
