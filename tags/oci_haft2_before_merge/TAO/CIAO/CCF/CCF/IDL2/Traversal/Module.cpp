// file      : CCF/IDL2/Traversal/Module.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Module.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      
      // Module
      //
      //

      void Module::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void Module::
      pre (NodePtr const&)
      {
      }

      void Module::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void Module::
      post (NodePtr const&)
      {
      }
    }
  }
}
