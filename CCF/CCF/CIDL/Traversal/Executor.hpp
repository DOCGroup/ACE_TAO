// file      : CCF/CIDL/Traversal/Executor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_TRAVERSAL_EXECUTOR_HPP
#define CCF_CIDL_TRAVERSAL_EXECUTOR_HPP

#include "CCF/CIDL/SemanticGraph/Executor.hpp"

#include "CCF/CIDL/Traversal/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      //
      //
      //
      struct Executor : Node<SemanticGraph::Executor>
      {
        virtual void
        traverse (Type& e);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        implements (Type&, EdgeDispatcherBase&);

        virtual void
        implements (Type&);

        virtual void
        implements_pre (Type&);

        virtual void
        implements_post (Type&);

        virtual void
        post (Type&);
      };


      //
      //
      //
      struct ComponentExecutor : Node<SemanticGraph::ComponentExecutor>
      {
        virtual void
        traverse (Type& e);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        implements (Type&, EdgeDispatcherBase&);

        virtual void
        implements (Type&);

        virtual void
        implements_pre (Type&);

        virtual void
        implements_post (Type&);

        virtual void
        post (Type&);
      };


      //
      //
      //
      struct HomeExecutor : Node<SemanticGraph::HomeExecutor>
      {
        virtual void
        traverse (Type& e);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        implements (Type&, EdgeDispatcherBase&);

        virtual void
        implements (Type&);

        virtual void
        implements_pre (Type&);

        virtual void
        implements_post (Type&);

        virtual void
        manages (Type&, EdgeDispatcherBase&);

        virtual void
        manages (Type&);

        virtual void
        manages_pre (Type&);

        virtual void
        manages_post (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_CIDL_TRAVERSAL_EXECUTOR_HPP
