// file      : CCF/CIDL/Traversal/Executor.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/Traversal/Executor.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      // Executor
      //
      //
      void Executor::
      traverse (Type& e)
      {
        pre (e);
        name (e);
        implements (e);
        post (e);
      }

      void Executor::
      pre (Type&)
      {
      }

      void Executor::
      name (Type&)
      {
      }

      void Executor::
      implements (Type& e, EdgeDispatcherBase& d)
      {
        d.traverse (e.implements ());
      }

      void Executor::
      implements (Type& e)
      {
        implements_pre (e);
        implements (e, edge_traverser ());
        implements_post (e);
      }

      void Executor::
      implements_pre (Type&)
      {
      }

      void Executor::
      implements_post (Type&)
      {
      }

      void Executor::
      post (Type&)
      {
      }


      // ComponentExecutor
      //
      //
      void ComponentExecutor::
      traverse (Type& e)
      {
        pre (e);
        name (e);
        implements (e);
        post (e);
      }

      void ComponentExecutor::
      pre (Type&)
      {
      }

      void ComponentExecutor::
      name (Type&)
      {
      }

      void ComponentExecutor::
      implements (Type& e, EdgeDispatcherBase& d)
      {
        d.traverse (e.implements ());
      }

      void ComponentExecutor::
      implements (Type& e)
      {
        implements_pre (e);
        implements (e, edge_traverser ());
        implements_post (e);
      }

      void ComponentExecutor::
      implements_pre (Type&)
      {
      }

      void ComponentExecutor::
      implements_post (Type&)
      {
      }

      void ComponentExecutor::
      post (Type&)
      {
      }



      // HomeExecutor
      //
      //
      void HomeExecutor::
      traverse (Type& e)
      {
        pre (e);
        name (e);
        implements (e);
        manages (e);
        post (e);
      }

      void HomeExecutor::
      pre (Type&)
      {
      }

      void HomeExecutor::
      name (Type&)
      {
      }

      void HomeExecutor::
      implements (Type& e, EdgeDispatcherBase& d)
      {
        d.traverse (e.implements ());
      }

      void HomeExecutor::
      implements (Type& e)
      {
        implements_pre (e);
        implements (e, edge_traverser ());
        implements_post (e);
      }

      void HomeExecutor::
      implements_pre (Type&)
      {
      }

      void HomeExecutor::
      implements_post (Type&)
      {
      }

      void HomeExecutor::
      manages (Type& e, EdgeDispatcherBase& d)
      {
        d.traverse (e.manages ());
      }

      void HomeExecutor::
      manages (Type& e)
      {
        manages_pre (e);
        manages (e, edge_traverser ());
        manages_post (e);
      }

      void HomeExecutor::
      manages_pre (Type&)
      {
      }

      void HomeExecutor::
      manages_post (Type&)
      {
      }

      void HomeExecutor::
      post (Type&)
      {
      }
    }
  }
}
