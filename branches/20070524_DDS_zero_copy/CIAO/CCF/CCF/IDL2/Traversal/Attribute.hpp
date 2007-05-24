// file      : CCF/IDL2/Traversal/Attribute.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_ATTRIBUTE_HPP
#define CCF_IDL2_TRAVERSAL_ATTRIBUTE_HPP

#include "CCF/IDL2/SemanticGraph/Attribute.hpp"
#include "CCF/IDL2/Traversal/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct GetRaises : Edge<SemanticGraph::GetRaises>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.exception ());
        }
      };


      //
      //
      //
      struct SetRaises : Edge<SemanticGraph::SetRaises>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.exception ());
        }
      };


      //
      //
      //
      struct Attribute : Node<SemanticGraph::Attribute>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        belongs (Type&, EdgeDispatcherBase&);

        virtual void
        belongs (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };

      //
      //
      //
      struct ReadAttribute : Node<SemanticGraph::ReadAttribute>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        belongs (Type&, EdgeDispatcherBase&);

        virtual void
        belongs (Type&);

        virtual void
        name (Type&);

        virtual void
        get_raises (Type&, EdgeDispatcherBase&);

        virtual void
        get_raises (Type&);

        virtual void
        get_raises_pre (Type&);

        virtual void
        get_raises_post (Type&);

        virtual void
        get_raises_none (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };


      //
      //
      //
      struct ReadWriteAttribute : Node<SemanticGraph::ReadWriteAttribute>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        belongs (Type&, EdgeDispatcherBase&);

        virtual void
        belongs (Type&);

        virtual void
        name (Type&);

        virtual void
        get_raises (Type&, EdgeDispatcherBase&);

        virtual void
        get_raises (Type&);

        virtual void
        get_raises_pre (Type&);

        virtual void
        get_raises_post (Type&);

        virtual void
        get_raises_none (Type&);

        virtual void
        set_raises (Type&, EdgeDispatcherBase&);

        virtual void
        set_raises (Type&);

        virtual void
        set_raises_pre (Type&);

        virtual void
        set_raises_post (Type&);

        virtual void
        set_raises_none (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_ATTRIBUTE_HPP
