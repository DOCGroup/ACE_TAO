// file      : CCF/CIDL/Traversal/HomeExecutor.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/CIDL/Traversal/HomeExecutor.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      // HomeExecutor
      //
      //
      void HomeExecutor::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void HomeExecutor::
      pre (NodePtr const&)
      {
      }

      void HomeExecutor::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void HomeExecutor::
      post (NodePtr const&)
      {
      }
    }
  }
}
