// $Id$
#ifndef CCF_IDL3_TRAVERSAL_HPP
#define CCF_IDL3_TRAVERSAL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_Traversal.hpp"
#include "CCF/IDL3/IDL3_SyntaxTreeFwd.hpp"

namespace IDL3
{
  namespace Traversal
  {
    // Import visitors from IDL2
    using namespace IDL2::Traversal;

    //
    //
    //
    struct ComponentDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ComponentDeclPtr
      NodePtr;

      virtual void visit_component_decl (NodePtr const& c);
      virtual void visit_component_decl_pre (NodePtr const& c);
      virtual void visit_component_decl_post (NodePtr const& c);
    };

    //
    //
    //
    struct ComponentDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ComponentDefPtr
      NodePtr;

      virtual void visit_component_def (NodePtr const& c);
      virtual void visit_component_def_pre (NodePtr const& c);
      virtual void visit_component_def_scope (NodePtr const& c);
      virtual void visit_component_def_post (NodePtr const& c);
    };

    //
    //
    //
    struct ProvidesDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ProvidesDeclPtr
      NodePtr;

      virtual void visit_provides_decl (NodePtr const& c)
      {
      }
    };

    //
    //
    //
    struct UsesDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::UsesDeclPtr
      NodePtr;

      virtual void visit_uses_decl (NodePtr const& c)
      {
      }
    };

    //
    //
    //
    struct PublishesDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::PublishesDeclPtr
      NodePtr;

      virtual void visit_publishes_decl (NodePtr const& c)
      {
      }
    };

    //
    //
    //
    struct EmitsDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::EmitsDeclPtr
      NodePtr;

      virtual void visit_emits_decl (NodePtr const& c)
      {
      }
    };

    //
    //
    //
    struct ConsumesDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ConsumesDeclPtr
      NodePtr;

      virtual void visit_consumes_decl (NodePtr const& c)
      {
      }
    };

    //
    //
    //
    struct HomeDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::HomeDefPtr
      NodePtr;

      virtual void visit_home_def (NodePtr const& h);
      virtual void visit_home_def_pre (NodePtr const& h);
      virtual void visit_home_def_scope (NodePtr const& h);
      virtual void visit_home_def_post (NodePtr const& h);

    };

    //
    //
    //
    struct HomeFactoryDecl : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::HomeFactoryDeclPtr
      NodePtr;

      HomeFactoryDecl (CCF::Traversal::Visitor* type = 0)
          : type_ (type)
      {
      }

      virtual void visit_home_factory_decl (NodePtr const& n);

      virtual void visit_home_factory_decl_pre (NodePtr const& n);
      virtual void visit_home_factory_decl_type (NodePtr const& n);
      virtual void visit_home_factory_decl_name (NodePtr const& n);
      virtual void visit_home_factory_decl_parameters (NodePtr const& n);
      virtual void visit_home_factory_decl_post (NodePtr const& n);

    private:
      CCF::Traversal::Visitor* type_;
    };

    //
    //
    //
    struct ConcreteEventTypeDef : virtual CCF::Traversal::Visitor
    {
      typedef
      SyntaxTree::ConcreteEventTypeDefPtr
      NodePtr;

      virtual void visit_concrete_event_type_def (NodePtr const& et);
      virtual void visit_concrete_event_type_def_pre (NodePtr const& et);
      virtual void visit_concrete_event_type_def_scope (NodePtr const& et);
      virtual void visit_concrete_event_type_def_post (NodePtr const& et);
    };
  }
}

#endif // CCF_IDL3_TRAVERSAL_HPP
