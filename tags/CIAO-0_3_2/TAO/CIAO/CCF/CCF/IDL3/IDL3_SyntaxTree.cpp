// $Id$
#include "IDL3_Traversal.hpp"
#include "IDL3_SyntaxTree.hpp"

namespace IDL3
{
  namespace SyntaxTree
  {
    using CCF::Traversal::try_visit;

    namespace
    {
      //@@ repetition of what's already in IDL2
      bool
      try_visit_scope (CCF::Traversal::Visitor* v,
                       ScopePtr const& node)
      {
        return try_visit<Traversal::Scope> (
          &Traversal::Scope::visit_scope,
          v,
          node);
      }

      bool
      try_visit_component_decl (CCF::Traversal::Visitor* v,
                                ComponentDeclPtr const& node)
      {
        return try_visit<Traversal::ComponentDecl> (
          &Traversal::ComponentDecl::visit_component_decl,
                          v,
                          node);
      }
    }


    //
    // ComponentDecl
    //

    void ComponentDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      ComponentDeclPtr self (ReferenceCounting::add_ref (this));

      try_visit_component_decl (v, self);
    }

    //
    // ComponentForwardDecl
    //

    void ComponentForwardDecl::
    accept (CCF::Traversal::Visitor* v)
    {
    }

    //
    // ComponentDef
    //

    void ComponentDef::
    accept (CCF::Traversal::Visitor* v)
    {
      ComponentDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::ComponentDef> (
           &Traversal::ComponentDef::visit_component_def,
           v,
           self))
      {
        try_visit_component_decl (v, self);
        try_visit_scope (v, self);
      }
    }

    //
    // ProvidesDecl
    //

    void ProvidesDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      ProvidesDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::ProvidesDecl> (
        &Traversal::ProvidesDecl::visit_provides_decl,
        v,
        self);
    }

    //
    // UsesDecl
    //

    void UsesDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      UsesDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::UsesDecl> (
        &Traversal::UsesDecl::visit_uses_decl,
        v,
        self);
    }

    //
    // PublishesDecl
    //

    void PublishesDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      PublishesDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::PublishesDecl> (
        &Traversal::PublishesDecl::visit_publishes_decl,
        v,
        self);
    }

    //
    // EmitsDecl
    //

    void EmitsDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      EmitsDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::EmitsDecl> (
        &Traversal::EmitsDecl::visit_emits_decl,
        v,
        self);
    }

    //
    // ConsumesDecl
    //

    void ConsumesDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      ConsumesDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::ConsumesDecl> (
        &Traversal::ConsumesDecl::visit_consumes_decl,
        v,
        self);
    }


    //
    // HomeDef
    //

    void HomeDef::
    accept (CCF::Traversal::Visitor* v)
    {
      HomeDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::HomeDef> (
           &Traversal::HomeDef::visit_home_def,
           v,
           self))
      {
        try_visit_scope (v, self);
      }
    }

    //
    // HomeFactoryDecl
    //

    void HomeFactoryDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      HomeFactoryDeclPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::HomeFactoryDecl> (
           &Traversal::HomeFactoryDecl::visit_home_factory_decl,
                     v,
                     self))
      {
        try_visit<Traversal::OperationDecl> (
          &Traversal::OperationDecl::visit_operation_decl,
          v,
          self);
      }
    }
  }
}
