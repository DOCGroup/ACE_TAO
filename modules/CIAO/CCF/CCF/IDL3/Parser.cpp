// file      : CCF/IDL3/Parser.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/Parser.hpp"

//
// Note: DO NOT run emacs indenter (or any other indentation tool) over
//       this file because it will most likely break BNF indentation.
//

namespace CCF
{
  namespace IDL3
  {
    Parser::
    Parser (CompilerElements::Context& context,
            Diagnostic::Stream& dout,
            LexicalAnalyzer const& l,
            SemanticAction::Factory& f)
        : IDL2::Parser (context, dout, l, f),

          COMPONENT ("component"),
          CONSUMES  ("consumes" ),
          EMITS     ("emits"    ),
          EVENTTYPE ("eventtype"),
          HOME      ("home"     ),
          FINDER    ("finder"   ),
          MANAGES   ("manages"  ),
          MULTIPLE  ("multiple" ),
          PROVIDES  ("provides" ),
          PUBLISHES ("publishes"),
          USES      ("uses"     ),


          // Component
          //
          act_component_begin_def (
            f.component (), &SemanticAction::Component::begin_def),

          act_component_begin_fwd (
            f.component (), &SemanticAction::Component::begin_fwd),

          act_component_inherits (
            f.component (), &SemanticAction::Component::inherits),

          act_component_supports (
            f.component (), &SemanticAction::Component::supports),

          act_component_open_scope (
            f.component (), &SemanticAction::Component::open_scope),

          act_component_close_scope (
            f.component (), &SemanticAction::Component::close_scope),

          act_component_end         (
            f.component (), &SemanticAction::Component::end),


          // Provides
          //
          act_provides_type (f.provides (), &SemanticAction::Provides::type),
          act_provides_name (f.provides (), &SemanticAction::Provides::name),


          // Uses
          //
          act_uses_multiple (f.uses (), &SemanticAction::Uses::multiple),
          act_uses_type (f.uses (), &SemanticAction::Uses::type),
          act_uses_name (f.uses (), &SemanticAction::Uses::name),


          // Publishes
          //
          act_publishes_type (
            f.publishes (), &SemanticAction::Publishes::type),

          act_publishes_name (
            f.publishes (), &SemanticAction::Publishes::name),


          // Emits
          //
          act_emits_type (f.emits (), &SemanticAction::Emits::type),
          act_emits_name (f.emits (), &SemanticAction::Emits::name),


          // Consumes
          //
          act_consumes_type (f.consumes (), &SemanticAction::Consumes::type),
          act_consumes_name (f.consumes (), &SemanticAction::Consumes::name),


          // EventType
          //
          act_event_type_begin_abstract_def (
            f.event_type (), &SemanticAction::EventType::begin_abstract_def),

          act_event_type_begin_abstract_fwd (
            f.event_type (), &SemanticAction::EventType::begin_abstract_fwd),

          act_event_type_begin_concrete_def (
            f.event_type (), &SemanticAction::EventType::begin_concrete_def),

          act_event_type_begin_concrete_fwd (
            f.event_type (), &SemanticAction::EventType::begin_concrete_fwd),

          act_event_type_inherits (
            f.event_type (), &SemanticAction::EventType::inherits),

          act_event_type_supports (
            f.event_type (), &SemanticAction::EventType::supports),

          act_event_type_open_scope (
            f.event_type (), &SemanticAction::EventType::open_scope),

          act_event_type_close_scope (
            f.event_type (), &SemanticAction::EventType::close_scope),

          act_event_type_end (
            f.event_type (), &SemanticAction::EventType::end),

          // EventTypeFactory
          //
          act_event_type_factory_name (
            f.event_type_factory (), &SemanticAction::EventTypeFactory::name),

          act_event_type_factory_parameter (
            f.event_type_factory (), &SemanticAction::EventTypeFactory::parameter),

          act_event_type_factory_raises (
            f.event_type_factory (), &SemanticAction::EventTypeFactory::raises),


          // Home
          //
          act_home_begin (
            f.home (), &SemanticAction::Home::begin),

          act_home_inherits (
            f.home (), &SemanticAction::Home::inherits),

          act_home_supports (
            f.home (), &SemanticAction::Home::supports),

          act_home_manages (
            f.home (), &SemanticAction::Home::manages),

          act_home_open_scope (
            f.home (), &SemanticAction::Home::open_scope),

          act_home_close_scope (
            f.home (), &SemanticAction::Home::close_scope),

          act_home_end (
            f.home (), &SemanticAction::Home::end),


          // HomeFactory
          //
          act_home_factory_name (
            f.home_factory (), &SemanticAction::HomeFactory::name),

          act_home_factory_parameter (
            f.home_factory (), &SemanticAction::HomeFactory::parameter),

          act_home_factory_raises (
            f.home_factory (), &SemanticAction::HomeFactory::raises),


          // HomeFinder
          //
          act_home_finder_name (
            f.home_finder (), &SemanticAction::HomeFinder::name),

          act_home_finder_parameter (
            f.home_finder (), &SemanticAction::HomeFinder::parameter),

          act_home_finder_raises (
            f.home_finder (), &SemanticAction::HomeFinder::raises)

    {
      IDL2::Parser::extension =
           component_decl
        |  concrete_event_type_decl
        |  home_decl
        |  extension
        ;

      IDL2::Parser::abstract_type_decl =
           ABSTRACT
        >> guard
           (
             assertion ("interface, valuetype or eventtype declaration expected")
             (
                 (INTERFACE >> assertion ()(abstract_interface_decl))
               |
                 (VALUETYPE >> assertion ()(abstract_value_type_decl))
               |
                 (EVENTTYPE >> assertion ()(abstract_event_type_decl))
             )
           )[error_handler]
        ;

      // component
      //
      //

      component_decl =
           COMPONENT
        >> (
               (
                    simple_identifier
                 >> SEMI
               )[act_component_begin_fwd][act_component_end]
             |
               (
                    (
                         simple_identifier
                      >> COLON
                    )[act_component_begin_def]
                 >> component_inheritance_spec
                 >> !(SUPPORTS >> component_support_spec)
                 >> LCBRACE[act_component_open_scope]
                 >> component_def_trailer
               )
             |
               (
                    (
                         simple_identifier
                      >> SUPPORTS
                    )[act_component_begin_def]
                 >> component_support_spec
                 >> LCBRACE[act_component_open_scope]
                 >> component_def_trailer
               )
             |
               (
                    (
                         simple_identifier
                      >> LCBRACE
                    )[act_component_begin_def][act_component_open_scope]
                 >> component_def_trailer
               )

           )
        ;

      component_def_trailer =
           component_body
        >> RCBRACE[act_component_close_scope]
        >> SEMI[act_component_end]
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

      // ports
      //
      //
      provides_decl =
           PROVIDES
        >> identifier[act_provides_type]
        >> simple_identifier[act_provides_name]
        >> SEMI
        ;

      uses_decl =
           USES
        >> !(MULTIPLE[act_uses_multiple])
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


      // eventtype
      //
      //
      abstract_event_type_decl =
        guard
        (
            (
                 simple_identifier
              >> SEMI
            )[act_event_type_begin_abstract_fwd][act_event_type_end]
          |
            (
                 (
                   simple_identifier
                   >> COLON
                 )[act_event_type_begin_abstract_def]

              >> event_type_inheritance_spec
              >> !(SUPPORTS >> event_type_supports_spec)
              >> LCBRACE[act_event_type_open_scope]
              >> event_type_def_trailer
            )
          |
            (
                 (
                           simple_identifier
                        >> SUPPORTS
                 )[act_event_type_begin_abstract_def]

              >> event_type_supports_spec
              >> LCBRACE[act_event_type_open_scope]
              >> event_type_def_trailer
            )
          |
            (
                 (
                      simple_identifier
                   >> LCBRACE
                 )[act_event_type_begin_abstract_def][act_event_type_open_scope]

              >> event_type_def_trailer
            )
        )[error_handler]
        ;

      concrete_event_type_decl =
        guard
        (
             EVENTTYPE
          >> (
                 (
                      simple_identifier
                   >> SEMI
                 )[act_event_type_begin_concrete_fwd][act_event_type_end]
               |
                 (
                      (
                           simple_identifier
                        >> COLON
                      )[act_event_type_begin_concrete_def]

                   >> event_type_inheritance_spec
                   >> !(SUPPORTS >> event_type_supports_spec)
                   >> LCBRACE[act_event_type_open_scope]
                   >> event_type_def_trailer
                 )
               |
                 (
                      (
                           simple_identifier
                        >> SUPPORTS
                      )[act_event_type_begin_concrete_def]

                   >> event_type_supports_spec
                   >> LCBRACE[act_event_type_open_scope]
                   >> event_type_def_trailer
                 )
               |
                 (
                      (
                           simple_identifier
                        >> LCBRACE
                      )[act_event_type_begin_concrete_def][act_event_type_open_scope]

                   >> event_type_def_trailer
                 )
             )
        )[error_handler]
        ;


      event_type_inheritance_spec =
           identifier[act_event_type_inherits]
        >> *(
                 COMMA
              >> identifier[act_event_type_inherits]
            )
        ;

      event_type_supports_spec =
           identifier[act_event_type_supports]
        >> *(
                 COMMA
              >> identifier[act_event_type_supports]
            )
        ;

      event_type_def_trailer =
           event_type_body
        >> RCBRACE[act_event_type_close_scope]
        >> SEMI[act_event_type_end]
        ;

      event_type_body =
        *(
             const_decl
           | type_decl
           | type_id_decl
           | type_prefix_decl

           | attribute_decl
           | operation_decl
           | value_type_member_decl
           | event_type_factory_decl
        )
        ;


      // eventtype factory
      //
      //
      event_type_factory_decl =
           FACTORY
        >> simple_identifier[act_event_type_factory_name]
        >> LPAREN
        >> event_type_factory_parameter_list
        >> RPAREN
        >> !(RAISES >> LPAREN >> event_type_factory_raises_list >> RPAREN)
        >> SEMI
        ;

      event_type_factory_parameter_list =
        *(
              event_type_factory_parameter
           >> *(COMMA >> event_type_factory_parameter)
        )
        ;

      event_type_factory_parameter =
           IN
        >> (identifier >> simple_identifier)[act_event_type_factory_parameter]
        ;

      event_type_factory_raises_list =
           identifier[act_event_type_factory_raises]
        >> *(COMMA >> identifier[act_event_type_factory_raises])
        ;

      //
      // home
      //
      home_decl =
           home_header
        >> !(COLON >> home_inheritance_spec)
        >> !(SUPPORTS >> home_support_spec)
        >> MANAGES >> home_manage_spec
        >> LCBRACE[act_home_open_scope]
        >> home_body
        >> RCBRACE[act_home_close_scope]
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
             const_decl
           | type_decl
           | type_id_decl
           | type_prefix_decl

           | attribute_decl
           | operation_decl
           | home_factory_decl
           | home_finder_decl
        )
        ;

      // home factory
      //
      //
      home_factory_decl =
           FACTORY
        >> simple_identifier[act_home_factory_name]
        >> LPAREN
        >> home_factory_parameter_list
        >> RPAREN
        >> !(RAISES >> LPAREN >> home_factory_raises_list >> RPAREN)
        >> SEMI
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

      home_factory_raises_list =
           identifier[act_home_factory_raises]
        >> *(COMMA >> identifier[act_home_factory_raises])
        ;


      // home finder
      //
      //
      home_finder_decl =
           FINDER
        >> simple_identifier[act_home_finder_name]
        >> LPAREN
        >> home_finder_parameter_list
        >> RPAREN
        >> !(RAISES >> LPAREN >> home_finder_raises_list >> RPAREN)
        >> SEMI
        ;

      home_finder_parameter_list =
        *(
              home_finder_parameter
           >> *(COMMA >> home_finder_parameter)
        )
        ;

      home_finder_parameter =
           IN
        >> (identifier >> simple_identifier)[act_home_finder_parameter]
        ;

      home_finder_raises_list =
           identifier[act_home_finder_raises]
        >> *(COMMA >> identifier[act_home_finder_raises])
        ;
    }
  }
}
