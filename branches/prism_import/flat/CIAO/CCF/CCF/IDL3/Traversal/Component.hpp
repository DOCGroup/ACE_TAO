// file      : CCF/IDL3/Traversal/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TRAVERSAL_COMPONENT_HPP
#define CCF_IDL3_TRAVERSAL_COMPONENT_HPP

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/Traversal/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      template <typename T>
      struct PortTemplate : Node<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        traverse (T&);
      };

      template <typename T>
      struct PortAccessorTemplate : Node<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        returns (T&, EdgeDispatcherBase&);

        virtual void
        returns (T&);

        virtual void
        name (T&);

        virtual void
        receives (T&, EdgeDispatcherBase&);

        virtual void
        receives (T&);

        virtual void
        receives_pre (T&);

        virtual void
        receives_post (T&);

        virtual void
        raises (T&, EdgeDispatcherBase&);

        virtual void
        raises (T&);

        virtual void
        raises_pre (T&);

        virtual void
        raises_post (T&);

        virtual void
        post (T&);
      };

      template <typename T>
      struct PortGetTemplate : PortAccessorTemplate<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        returns (T&);
      };

      template <typename T>
      struct PortSetTemplate : PortAccessorTemplate<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        receives (T&);
      };

      template <typename T>
      struct PortDataTemplate : Node<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        belongs (T&, EdgeDispatcherBase&);

        virtual void
        belongs (T&);

        virtual void
        name (T&);

        virtual void
        post (T&);
      };

      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::Provider>
      Provider;

      typedef
      PortGetTemplate<SemanticGraph::Provider>
      ProviderGet;

      typedef
      PortSetTemplate<SemanticGraph::Provider>
      ProviderSet;

      typedef
      PortDataTemplate<SemanticGraph::Provider>
      ProviderData;

      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::User>
      User;

      typedef
      PortGetTemplate<SemanticGraph::User>
      UserGet;

      typedef
      PortSetTemplate<SemanticGraph::User>
      UserSet;

      typedef
      PortDataTemplate<SemanticGraph::User>
      UserData;


      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::MultiUser>
      MultiUser;

      typedef
      PortGetTemplate<SemanticGraph::MultiUser>
      MultiUserGet;

      typedef
      PortSetTemplate<SemanticGraph::MultiUser>
      MultiUserSet;

      typedef
      PortDataTemplate<SemanticGraph::MultiUser>
      MultiUserData;


      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::SingleUser>
      SingleUser;

      typedef
      PortGetTemplate<SemanticGraph::SingleUser>
      SingleUserGet;

      typedef
      PortSetTemplate<SemanticGraph::SingleUser>
      SingleUserSet;

      typedef
      PortDataTemplate<SemanticGraph::SingleUser>
      SingleUserData;


      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::Publisher>
      Publisher;

      typedef
      PortGetTemplate<SemanticGraph::Publisher>
      PublisherGet;

      typedef
      PortSetTemplate<SemanticGraph::Publisher>
      PublisherSet;

      typedef
      PortDataTemplate<SemanticGraph::Publisher>
      PublisherData;

      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::Emitter>
      Emitter;

      typedef
      PortGetTemplate<SemanticGraph::Emitter>
      EmitterGet;

      typedef
      PortSetTemplate<SemanticGraph::Emitter>
      EmitterSet;

      typedef
      PortDataTemplate<SemanticGraph::Emitter>
      EmitterData;


      //
      //
      //
      typedef
      PortTemplate<SemanticGraph::Consumer>
      Consumer;

      typedef
      PortGetTemplate<SemanticGraph::Consumer>
      ConsumerGet;

      typedef
      PortSetTemplate<SemanticGraph::Consumer>
      ConsumerSet;

      typedef
      PortDataTemplate<SemanticGraph::Consumer>
      ConsumerData;

      //
      //
      //
      struct Component : ScopeTemplate<SemanticGraph::Component>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        inherits (Type&, EdgeDispatcherBase&);

        virtual void
        inherits (Type&);

        virtual void
        inherits_pre (Type&);

        virtual void
        inherits_post (Type&);

        virtual void
        inherits_none (Type&);

        virtual void
        supports (Type&, EdgeDispatcherBase&);

        virtual void
        supports (Type&);

        virtual void
        supports_pre (Type&);

        virtual void
        supports_post (Type&);

        virtual void
        supports_none (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };
    }
  }
}

#include "CCF/IDL3/Traversal/Component.tpp"

#endif  // CCF_IDL3_TRAVERSAL_COMPONENT_HPP
