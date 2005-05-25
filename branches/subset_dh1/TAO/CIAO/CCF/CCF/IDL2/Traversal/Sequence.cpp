// file      : CCF/IDL2/Traversal/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      void UnboundedSequence::
      traverse (Type& us)
      {
        pre (us);
        specialized (us);
        name (us);
        post (us);
      }

      void UnboundedSequence::
      pre (Type&)
      {
      }

      void UnboundedSequence::
      specialized (Type& us, EdgeDispatcherBase& d)
      {
        d.traverse (us.specialized ());
      }

      void UnboundedSequence::
      specialized (Type& us)
      {
        specialized (us, edge_traverser ());
      }

      void UnboundedSequence::
      name (Type&)
      {
      }

      void UnboundedSequence::
      post (Type&)
      {
      }
    }
  }
}
