// $Id$
#ifndef CCF_IDL3_PARSER_HPP
#define CCF_IDL3_PARSER_HPP


#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Parser.hpp"
#include "CCF/IDL2/IDL2_Parser.hpp"
#include "CCF/IDL3/IDL3_LexicalAnalyzer.hpp"
#include "CCF/IDL3/IDL3_SemanticAction.hpp"

namespace IDL3
{
  class Parser : public virtual IDL2::Parser
  {
  public:
    virtual
    ~Parser () throw () {}

  protected:

    //
    // Semanic action types
    //
    typedef
    SemanticAction::Component
    Component;

    typedef
    SemanticAction::Provides
    Provides;

    typedef
    SemanticAction::Uses
    Uses;

    typedef
    SemanticAction::Publishes
    Publishes;

    typedef
    SemanticAction::Emits
    Emits;

    typedef
    SemanticAction::Consumes
    Consumes;

    typedef
    SemanticAction::EventType
    EventType;

    typedef
    SemanticAction::Home
    Home;

    typedef
    SemanticAction::HomeFactory
    HomeFactory;

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

    Parser (LexicalAnalyzer const& l, SemanticActionFactory& f)
        : IDL2::Parser (l, f),

          COMPONENT (l.component),
          CONSUMES  (l.consumes),
          EMITS     (l.emits),
          EVENTTYPE (l.eventtype),
          HOME      (l.home),
          MANAGES   (l.manages),
          PROVIDES  (l.provides),
          PUBLISHES (l.publishes),
          USES      (l.uses),

          act_component_begin       (f.component (), &Component::begin),
          act_component_inherits    (f.component (), &Component::inherits),
          act_component_supports    (f.component (), &Component::supports),
          act_component_open_scope  (f.component (), &Component::open_scope),
          act_component_close_scope (f.component (), &Component::close_scope),
          act_component_end         (f.component (), &Component::end),

          act_provides_type (f.provides (), &Provides::type),
          act_provides_name (f.provides (), &Provides::name),

          act_uses_type (f.uses (), &Uses::type),
          act_uses_name (f.uses (), &Uses::name),

          act_publishes_type (f.publishes (), &Publishes::type),
          act_publishes_name (f.publishes (), &Publishes::name),

          act_emits_type (f.emits (), &Emits::type),
          act_emits_name (f.emits (), &Emits::name),

          act_consumes_type (f.consumes (), &Consumes::type),
          act_consumes_name (f.consumes (), &Consumes::name),

          act_event_type_begin       (f.event_type (), &EventType::begin),
          act_event_type_inherits    (f.event_type (), &EventType::inherits),
          act_event_type_open_scope  (f.event_type (), &EventType::open_scope),
          act_event_type_close_scope (f.event_type (), &EventType::close_scope),
          act_event_type_end         (f.event_type (), &EventType::end),

          act_home_begin       (f.home (), &Home::begin),
          act_home_inherits    (f.home (), &Home::inherits),
          act_home_supports    (f.home (), &Home::supports),
          act_home_manages     (f.home (), &Home::manages),
          act_home_open_scope  (f.home (), &Home::open_scope),
          act_home_close_scope (f.home (), &Home::close_scope),
          act_home_end         (f.home (), &Home::end),

          act_home_factory_begin     (f.home_factory (), &HomeFactory::begin),
          act_home_factory_parameter (f.home_factory (),
                                      &HomeFactory::parameter),
          act_home_factory_end       (f.home_factory (), &HomeFactory::end)

    {
      IDL2::Parser::extension =
           component_decl
        |  eventtype_decl
        |  home_decl
        |  extension
        ;

      //
      // Component
      //
      component_decl =
           component_header
        >>
           (
                 SEMI[act_component_end]
              |
                 (
                       !(COLON >> component_inheritance_spec)
                    >> !(SUPPORTS >> component_support_spec)
                    >> LBRACE[act_component_open_scope]
                    >> component_body
                    >> RBRACE[act_component_close_scope]
                    >> SEMI[act_component_end]
                 )
           )
        ;

      component_header =
           COMPONENT
        >> simple_identifier[act_component_begin]
        ;

      component_inheritance_spec = identifier[act_component_inherits]
        ;

      component_support_spec =
           identifier[act_component_supports]
        >> *(COMMA >> identifier[act_component_supports])
        ;

      component_body =
        *(    provides_decl
           |  uses_decl
           |  emits_decl
           |  publishes_decl
           |  consumes_decl
           |  attribute_decl
        )
        ;

      //
      // Component body elements
      //
      provides_decl =
           PROVIDES
        >> identifier[act_provides_type]
        >> simple_identifier[act_provides_name]
        >> SEMI
        ;

      uses_decl =
           USES
        >> identifier[act_uses_type]
        >> simple_identifier[act_uses_name]
        >> SEMI
        ;

      emits_decl =
           EMITS
        >> identifier[act_emits_type]
        >> simple_identifier[act_emits_name]
        >> SEMI
        ;

      publishes_decl =
           PUBLISHES
        >> identifier[act_publishes_type]
        >> simple_identifier[act_publishes_name]
        >> SEMI
        ;

      consumes_decl =
           CONSUMES
        >> identifier[act_consumes_type]
        >> simple_identifier[act_consumes_name]
        >> SEMI
        ;

      //
      // Eventtype
      //
      eventtype_decl =
           eventtype_header
        >>
           (
                 SEMI[act_event_type_end]
              |
                 (
                       !(COLON >> eventtype_inheritance_spec)
                    >> LBRACE[act_event_type_open_scope]
//                  >> eventtype_body
                    >> RBRACE[act_event_type_close_scope]
                    >> SEMI[act_event_type_end]
                 )
           )
        ;

      eventtype_header =
           !(ABSTRACT)
        >> EVENTTYPE
        >> simple_identifier[act_event_type_begin]
        ;

      eventtype_inheritance_spec =
           identifier[act_event_type_inherits]
        >> *(COMMA >> identifier[act_event_type_inherits])
        ;

      //
      // Home
      //
      home_decl =
           home_header
        >> !(COLON >> home_inheritance_spec)
        >> !(SUPPORTS >> home_support_spec)
        >> MANAGES >> home_manage_spec
        >> LBRACE[act_home_open_scope]
        >> home_body
        >> RBRACE[act_home_close_scope]
        >> SEMI[act_home_end]
        ;

      home_header = HOME >> simple_identifier[act_home_begin]
        ;

      home_inheritance_spec = identifier[act_home_inherits]
        ;

      home_support_spec =
           identifier[act_home_supports]
        >> *(COMMA >> identifier[act_home_supports])
        ;

      home_manage_spec = identifier[act_home_manages]
        ;

      home_body =
        *(
             attribute_decl
           | operation_decl
           | home_factory_decl
        )
        ;

      //
      // Home factory
      //
      home_factory_decl =
           FACTORY
        >> simple_identifier[act_home_factory_begin]
        >> LPAREN
        >> home_factory_parameter_list
        >> RPAREN
        >> SEMI[act_home_factory_end]
        ;

      home_factory_parameter_list =
        *(
              home_factory_parameter
           >> *(COMMA >> home_factory_parameter)
        )
        ;

      home_factory_parameter =
           IN
        >> (identifier >> simple_identifier)[act_home_factory_parameter]
        ;

    }

  protected:
    //
    //
    //
    OneArgAction<SimpleIdentifierPtr, Component> act_component_begin;
    OneArgAction<IdentifierPtr, Component> act_component_inherits;
    OneArgAction<IdentifierPtr, Component> act_component_supports;
    ScopeAction act_component_open_scope;
    ScopeAction act_component_close_scope;
    NoArgAction<Component> act_component_end;

    //
    // Provides
    //
    OneArgAction<IdentifierPtr, Provides> act_provides_type;
    OneArgAction<SimpleIdentifierPtr, Provides> act_provides_name;

    //
    // Uses
    //
    OneArgAction<IdentifierPtr, Uses> act_uses_type;
    OneArgAction<SimpleIdentifierPtr, Uses> act_uses_name;

    //
    // Publishes
    //
    OneArgAction<IdentifierPtr, Publishes> act_publishes_type;
    OneArgAction<SimpleIdentifierPtr, Publishes> act_publishes_name;

    //
    // Emits
    //
    OneArgAction<IdentifierPtr, Emits> act_emits_type;
    OneArgAction<SimpleIdentifierPtr, Emits> act_emits_name;

    //
    // Consumes
    //
    OneArgAction<IdentifierPtr, Consumes> act_consumes_type;
    OneArgAction<SimpleIdentifierPtr, Consumes> act_consumes_name;

    //
    //
    //
    OneArgAction<SimpleIdentifierPtr, EventType> act_event_type_begin;
    OneArgAction<IdentifierPtr, EventType> act_event_type_inherits;
    ScopeAction act_event_type_open_scope;
    ScopeAction act_event_type_close_scope;
    NoArgAction<EventType> act_event_type_end;

    //
    //
    //
    OneArgAction<SimpleIdentifierPtr, Home> act_home_begin;
    OneArgAction<IdentifierPtr, Home> act_home_inherits;
    OneArgAction<IdentifierPtr, Home> act_home_supports;
    OneArgAction<IdentifierPtr, Home> act_home_manages;
    ScopeAction act_home_open_scope;
    ScopeAction act_home_close_scope;
    NoArgAction<Home> act_home_end;

    //
    // Home
    //
    OneArgAction<SimpleIdentifierPtr, HomeFactory> act_home_factory_begin;
    TwoArgAction<IdentifierPtr,
                 SimpleIdentifierPtr,
                 HomeFactory> act_home_factory_parameter;
    NoArgAction<HomeFactory> act_home_factory_end;

  };
}

#endif // CCF_IDL3_PARSER_HPP
