// file      : CCF/IDL3/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_PARSER_HPP
#define CCF_IDL3_PARSER_HPP

#include "CCF/IDL2/Parser.hpp"

#include "CCF/IDL3/LexicalAnalyzer.hpp"
#include "CCF/IDL3/SemanticAction.hpp"

namespace CCF
{
  namespace IDL3
  {
    using IDL2::ActionExecutor;
    using IDL2::NoArgAction;
    using IDL2::OneArgAction;
    using IDL2::TwoArgAction;

    class Parser : public virtual IDL2::Parser
    {
    protected:
      //
      // Primitives (alphabetic order).
      //

      KeywordParser COMPONENT;
      KeywordParser CONSUMES;
      KeywordParser EMITS;
      KeywordParser EVENTTYPE;
      KeywordParser HOME;
      KeywordParser MANAGES;
      KeywordParser PROVIDES;
      KeywordParser PUBLISHES;
      KeywordParser USES;


      //
      // Language
      //

      Rule extension;

      // Component
      Rule component_decl;
      Rule component_header;
      Rule component_inheritance_spec;
      Rule component_support_spec;
      Rule component_body;

      // Component body elements
      Rule provides_decl;
      Rule uses_decl;
      Rule emits_decl;
      Rule publishes_decl;
      Rule consumes_decl;

      // Eventtype
      Rule eventtype_decl;
      Rule eventtype_header;
      Rule eventtype_inheritance_spec;
      Rule eventtype_body;

      // Home
      Rule home_decl;
      Rule home_header;
      Rule home_inheritance_spec;
      Rule home_support_spec;
      Rule home_manage_spec;
      Rule home_body;

      Rule home_factory_decl;
      Rule home_factory_parameter_list;
      Rule home_factory_parameter;


    public:
      Parser (CompilerElements::Context& context,
              Diagnostic::Stream& dout,
              LexicalAnalyzer const& l,
              SemanticAction::Factory& f);

    protected:
      // Component
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Component>
      act_component_begin;

      OneArgAction<IdentifierPtr, SemanticAction::Component>
      act_component_inherits;

      OneArgAction<IdentifierPtr, SemanticAction::Component>
      act_component_supports;

      ScopeAction
      act_component_open_scope;

      ScopeAction
      act_component_close_scope;

      NoArgAction<SemanticAction::Component>
      act_component_end;

      // Provides
      //
      OneArgAction<IdentifierPtr, SemanticAction::Provides>
      act_provides_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Provides>
      act_provides_name;

      // Uses
      //
      OneArgAction<IdentifierPtr, SemanticAction::Uses>
      act_uses_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Uses>
      act_uses_name;

      // Publishes
      //
      OneArgAction<IdentifierPtr, SemanticAction::Publishes>
      act_publishes_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Publishes>
      act_publishes_name;

      // Emits
      //
      OneArgAction<IdentifierPtr, SemanticAction::Emits>
      act_emits_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Emits>
      act_emits_name;

      // Consumes
      //
      OneArgAction<IdentifierPtr, SemanticAction::Consumes>
      act_consumes_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Consumes>
      act_consumes_name;

      // EventType
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::EventType>
      act_event_type_begin;

      OneArgAction<IdentifierPtr, SemanticAction::EventType>
      act_event_type_inherits;

      ScopeAction
      act_event_type_open_scope;

      ScopeAction
      act_event_type_close_scope;

      NoArgAction<SemanticAction::EventType>
      act_event_type_end;

      // Home
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Home>
      act_home_begin;

      OneArgAction<IdentifierPtr, SemanticAction::Home>
      act_home_inherits;

      OneArgAction<IdentifierPtr, SemanticAction::Home>
      act_home_supports;

      OneArgAction<IdentifierPtr, SemanticAction::Home>
      act_home_manages;

      ScopeAction
      act_home_open_scope;

      ScopeAction
      act_home_close_scope;

      NoArgAction<SemanticAction::Home>
      act_home_end;

      // HomeFactory
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::HomeFactory>
      act_home_factory_begin;

      TwoArgAction<IdentifierPtr,
                   SimpleIdentifierPtr,
                   SemanticAction::HomeFactory>
      act_home_factory_parameter;

      NoArgAction<SemanticAction::HomeFactory>
      act_home_factory_end;

    };
  }
}

#endif  // CCF_IDL3_PARSER_HPP
