// $Id$
#include "IDL3_Traversal.hpp"

#include "IDL3_SyntaxTree.hpp"
#include "IDL3_SyntaxTree_EventType.hpp"

namespace IDL3
{
  namespace Traversal
  {
    namespace
    {
      //@@ code duplication
      void
      iterate_scope (SyntaxTree::ScopePtr const& s,
                     CCF::Traversal::Visitor* v)
      {
        for (SyntaxTree::Scope::Iterator i = s->begin (); i != s->end (); i++)
        {
          (*i)->accept (v);
        }
      }
    }

    //
    // ComponentDecl
    //

    void ComponentDecl::
    visit_component_decl (NodePtr const& c)
    {
      visit_component_decl_pre   (c);
      visit_component_decl_post  (c);
    }


    void ComponentDecl::
    visit_component_decl_pre (NodePtr const&)
    {
    }

    void ComponentDecl::
    visit_component_decl_post (NodePtr const&)
    {
    }

    //
    // ComponentDef
    //

    void ComponentDef::
    visit_component_def (NodePtr const& c)
    {
      visit_component_def_pre   (c);
      visit_component_def_scope (c);
      visit_component_def_post  (c);
    }


    void ComponentDef::
    visit_component_def_pre (NodePtr const&)
    {
    }

    void ComponentDef::
    visit_component_def_scope (NodePtr const& c)
    {
      iterate_scope (c, this);
    }

    void ComponentDef::
    visit_component_def_post (NodePtr const&)
    {
    }

    //
    // HomeDef
    //

    void HomeDef::
    visit_home_def (NodePtr const& h)
    {
      visit_home_def_pre   (h);
      visit_home_def_scope (h);
      visit_home_def_post  (h);
    }

    void HomeDef::
    visit_home_def_pre (NodePtr const&)
    {
    }

    void HomeDef::
    visit_home_def_scope (NodePtr const& h)
    {
      iterate_scope (h, this);
    }

    void HomeDef::
    visit_home_def_post (NodePtr const&)
    {
    }

    //
    // HomeFactoryDecl
    //
    void HomeFactoryDecl::
    visit_home_factory_decl (NodePtr const& n)
    {
      visit_home_factory_decl_pre (n);
      visit_home_factory_decl_type (n);
      visit_home_factory_decl_name (n);
      visit_home_factory_decl_parameters (n);
      visit_home_factory_decl_post (n);
    }

    void HomeFactoryDecl::
    visit_home_factory_decl_pre (NodePtr const& n)
    {
    }

    void HomeFactoryDecl::
    visit_home_factory_decl_type (NodePtr const& n)
    {
      if (type_) n->type ()->accept (type_);
      else n->type ()->accept (this);
    }

    void HomeFactoryDecl::
    visit_home_factory_decl_name (NodePtr const& n)
    {
    }

    void HomeFactoryDecl::
    visit_home_factory_decl_parameters (NodePtr const& n)
    {
      for (SyntaxTree::HomeFactoryDecl::Iterator i = n->begin ();
           i != n->end ();
           i++)
      {
        (*i)->accept (this);

        if (i + 1 != n->end ())
        {
          //@@ could be HomeFactoryParameterComma
          SyntaxTree::CommaPtr c (new SyntaxTree::Comma);
          c->accept (this);
        }
      }
    }

    void HomeFactoryDecl::
    visit_home_factory_decl_post (NodePtr const& n)
    {
    }
    
    // ConcreteEventTypeDef
    //
    //
    void ConcreteEventTypeDef::
    visit_concrete_event_type_def (NodePtr const& et)
    {
      visit_concrete_event_type_def_pre   (et);
      visit_concrete_event_type_def_scope (et);
      visit_concrete_event_type_def_post  (et);
    }

    void ConcreteEventTypeDef::
    visit_concrete_event_type_def_pre (NodePtr const&)
    {
    }

    void ConcreteEventTypeDef::
    visit_concrete_event_type_def_scope (NodePtr const& et)
    {
      iterate_scope (et, this);
    }

    void ConcreteEventTypeDef::
    visit_concrete_event_type_def_post (NodePtr const&)
    {
    }
  }
}
