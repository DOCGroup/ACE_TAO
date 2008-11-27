// file      : CCF/IDL2/Traversal/Union.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Union.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // UnionMember
      //
      //
      void UnionMember::
      traverse (Type& m)
      {
        pre (m);
        belongs (m);
        name (m);
        post (m);
      }

      void UnionMember::
      pre (Type&)
      {
      }

      void UnionMember::
      belongs (Type& m, EdgeDispatcherBase& d)
      {
        d.traverse (m.belongs ());
      }

      void UnionMember::
      belongs (Type& m)
      {
        belongs (m, edge_traverser ());
      }

      void UnionMember::
      name (Type&)
      {
      }

      void UnionMember::
      post (Type&)
      {
      }


      // Union
      //
      //
      void Union::
      traverse (Type& s)
      {
        pre (s);
        name (s);
        names (s);
        post (s);
      }

      void Union::
      pre (Type&)
      {
      }

      void Union::
      name (Type&)
      {
      }

      void Union::
      post (Type&)
      {
      }
    }
  }
}
