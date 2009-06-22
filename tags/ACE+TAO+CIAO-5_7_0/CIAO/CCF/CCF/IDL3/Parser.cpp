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
    using  IDL2::Parsing::DiagnosticType;
    using  IDL2::Parsing::RecoveryMethod;

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
            f.component (), &SemanticAction::Scope::open_scope),

          act_component_close_scope (
            f.component (), &SemanticAction::Scope::close_scope),

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
            f.event_type (), &SemanticAction::Scope::open_scope),

          act_event_type_close_scope (
            f.event_type (), &SemanticAction::Scope::close_scope),

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
            f.home (), &SemanticAction::Scope::open_scope),

          act_home_close_scope (
            f.home (), &SemanticAction::Scope::close_scope),

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
                 (INTERFACE >> abstract_interface_decl)
               |
                 (VALUETYPE >> abstract_value_type_decl)
               |
                 (EVENTTYPE >> abstract_event_type_decl)
             )
           )[error_handler]
        ;

      // component
      //
      //

      component_decl =
           COMPONENT
        >> guard
           (
             assertion ("component declaration expected",
                     DiagnosticType::BEFORE)
             (
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

                   >> hood
                      (
                           component_inheritance_spec
                        >> !(SUPPORTS >> component_support_spec)
                        >> assertion ("'{' expected")
                           (
                             LCBRACE[act_component_open_scope]
                           )
                        >> hood (component_body)
                           [
                             handler (f.component (),
                                      &SemanticAction::Component::close_scope)
                           ]
                        >> assertion ("'}' expected",
                                      f.component (),
                                      &SemanticAction::Component::close_scope,
                                      DiagnosticType::BEFORE)
                           (
                             RCBRACE[act_component_close_scope]
                           )
                      )
                      [
                        handler (f.component (),
                                 &SemanticAction::Component::end)
                      ]

                   >> assertion ("';' expected",
                              f.component (),
                              &SemanticAction::Component::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_component_end]
                   )
                 )
               |
                 (
                      (
                           simple_identifier
                        >> SUPPORTS
                      )[act_component_begin_def]

                   >> hood
                      (
                           component_support_spec
                        >> assertion ("'{' expected")
                           (
                             LCBRACE[act_component_open_scope]
                           )
                        >> hood (component_body)
                           [
                             handler (f.component (),
                                      &SemanticAction::Component::close_scope)
                           ]
                        >> assertion ("'}' expected",
                                      f.component (),
                                      &SemanticAction::Component::close_scope,
                                      DiagnosticType::BEFORE)
                           (
                             RCBRACE[act_component_close_scope]
                           )
                      )
                      [
                        handler (f.component (),
                                 &SemanticAction::Component::end)
                      ]

                   >> assertion ("';' expected",
                              f.component (),
                              &SemanticAction::Component::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_component_end]
                   )
                 )
               |
                 (
                      (
                           simple_identifier
                        >> LCBRACE
                      )[act_component_begin_def][act_component_open_scope]

                   >> hood
                      (
                           hood (component_body)
                           [
                             handler (f.component (),
                                      &SemanticAction::Component::close_scope)
                           ]
                        >> assertion ("'}' expected",
                                      f.component (),
                                      &SemanticAction::Component::close_scope,
                                      DiagnosticType::BEFORE)
                           (
                             RCBRACE[act_component_close_scope]
                           )
                      )
                      [
                        handler (f.component (),
                                 &SemanticAction::Component::end)
                      ]

                   >> assertion ("';' expected",
                              f.component (),
                              &SemanticAction::Component::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_component_end]
                   )
                 )
             )
           )[error_handler]
        ;

      component_inheritance_spec =
        guard
        (
          assertion ("base component name expected")
          (
            identifier[act_component_inherits]
          )
        )[error_handler]
        ;

      component_support_spec =
        guard
        (
              assertion ("supported interface name expected")
              (
                identifier[act_component_supports]
              )
           >> *(
                    COMMA
                 >> assertion ("supported interface name expected")
                    (
                      identifier[act_component_supports]
                    )
               )
        )[error_handler]
        ;

      component_body =
        *(   provides_decl
           | uses_decl
           | emits_decl
           | publishes_decl
           | consumes_decl
           | attribute_decl
        )
        ;

      // ports
      //
      //
      provides_decl =
           PROVIDES
        >> guard
           (
                assertion ("interface name expected")
                (
                  identifier[act_provides_type]
                )
             >> assertion ("provides name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_provides_name]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      uses_decl =
           USES
        >> guard
           (
                !(MULTIPLE[act_uses_multiple])
             >> assertion ("interface name expected")
                (
                  identifier[act_uses_type]
                )
             >> assertion ("uses name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_uses_name]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      emits_decl =
           EMITS
        >> guard
           (
                assertion ("eventtype name expected")
                (
                  identifier[act_emits_type]
                )
             >> assertion ("emits name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_emits_name]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      publishes_decl =
           PUBLISHES
        >> guard
           (
                assertion ("eventtype name expected")
                (
                  identifier[act_publishes_type]
                )
             >> assertion ("publishes name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_publishes_name]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      consumes_decl =
           CONSUMES
        >> guard
           (
                assertion ("eventtype name expected")
                (
                  identifier[act_consumes_type]
                )
             >> assertion ("consumes name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_consumes_name]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;


      // eventtype
      //
      //
      abstract_event_type_decl =
        guard
        (
          assertion ("abstract eventtype declaration expected",
                     DiagnosticType::BEFORE)
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

                >> hood
                   (
                        event_type_inheritance_spec
                     >> !(SUPPORTS >> event_type_supports_spec)
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_event_type_open_scope]
                        )
                     >> hood (event_type_body)
                        [
                          handler (f.event_type (),
                                   &SemanticAction::EventType::close_scope)
                        ]
                     >> assertion ("'}' expected",
                                   f.event_type (),
                                   &SemanticAction::EventType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_event_type_close_scope]
                        )
                   )
                   [
                     handler (f.event_type (),
                              &SemanticAction::EventType::end)
                   ]

                >> assertion ("';' expected",
                              f.event_type (),
                              &SemanticAction::EventType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_event_type_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> SUPPORTS
                   )[act_event_type_begin_abstract_def]

                >> hood
                   (
                        event_type_supports_spec
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_event_type_open_scope]
                        )
                     >> hood (event_type_body)
                        [
                          handler (f.event_type (),
                                   &SemanticAction::EventType::close_scope)
                        ]
                     >> assertion ("'}' expected",
                                   f.event_type (),
                                   &SemanticAction::EventType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_event_type_close_scope]
                        )
                   )
                   [
                     handler (f.event_type (),
                              &SemanticAction::EventType::end)
                   ]

                >> assertion ("';' expected",
                              f.event_type (),
                              &SemanticAction::EventType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_event_type_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> LCBRACE
                   )[act_event_type_begin_abstract_def][act_event_type_open_scope]

                >> hood
                   (
                        hood (event_type_body)
                        [
                          handler (f.event_type (),
                                   &SemanticAction::EventType::close_scope)
                        ]
                     >> assertion ("'}' expected",
                                   f.event_type (),
                                   &SemanticAction::EventType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_event_type_close_scope]
                        )
                   )
                   [
                     handler (f.event_type (),
                              &SemanticAction::EventType::end)
                   ]

                >> assertion ("';' expected",
                              f.event_type (),
                              &SemanticAction::EventType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_event_type_end]
                   )
              )
          )
        )[error_handler]
        ;


      concrete_event_type_decl =
           EVENTTYPE
        >> guard
           (
             assertion ("eventtype declaration expected",
                     DiagnosticType::BEFORE)
             (
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

                  >> hood
                     (
                          event_type_inheritance_spec
                       >> !(SUPPORTS >> event_type_supports_spec)
                       >> assertion ("'{' expected")
                          (
                            LCBRACE[act_event_type_open_scope]
                          )
                       >> hood (event_type_body)
                          [
                            handler (f.event_type (),
                                     &SemanticAction::EventType::close_scope)
                          ]
                       >> assertion ("'}' expected",
                                     f.event_type (),
                                     &SemanticAction::EventType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_event_type_close_scope]
                          )
                     )
                     [
                       handler (f.event_type (),
                                &SemanticAction::EventType::end)
                     ]

                  >> assertion ("';' expected",
                                f.event_type (),
                                &SemanticAction::EventType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_event_type_end]
                     )
                )
              |
                (
                     (
                          simple_identifier
                       >> SUPPORTS
                     )[act_event_type_begin_concrete_def]

                  >> hood
                     (
                          event_type_supports_spec
                       >> assertion ("'{' expected")
                          (
                            LCBRACE[act_event_type_open_scope]
                          )
                       >> hood (event_type_body)
                          [
                            handler (f.event_type (),
                                     &SemanticAction::EventType::close_scope)
                          ]
                       >> assertion ("'}' expected",
                                     f.event_type (),
                                     &SemanticAction::EventType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_event_type_close_scope]
                          )
                     )
                     [
                       handler (f.event_type (),
                                &SemanticAction::EventType::end)
                     ]

                  >> assertion ("';' expected",
                                f.event_type (),
                                &SemanticAction::EventType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_event_type_end]
                     )
                )
              |
                (
                     (
                          simple_identifier
                       >> LCBRACE
                     )[act_event_type_begin_concrete_def][act_event_type_open_scope]

                  >> hood
                     (
                          hood (event_type_body)
                          [
                            handler (f.event_type (),
                                     &SemanticAction::EventType::close_scope)
                          ]
                       >> assertion ("'}' expected",
                                     f.event_type (),
                                     &SemanticAction::EventType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_event_type_close_scope]
                          )
                     )
                     [
                       handler (f.event_type (),
                                &SemanticAction::EventType::end)
                     ]

                  >> assertion ("';' expected",
                                f.event_type (),
                                &SemanticAction::EventType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_event_type_end]
                     )
                )
            )
          )[error_handler]
        ;

      event_type_inheritance_spec =
        guard
        (
              assertion ("base eventtype name expected")
              (
                identifier[act_event_type_inherits]
              )
           >> *(
                    COMMA
                 >> assertion ("base eventtype name expected")
                    (
                      identifier[act_event_type_inherits]
                    )
               )
        )[error_handler]
        ;

      event_type_supports_spec =
        guard
        (
              assertion ("supported interface name expected")
              (
                identifier[act_event_type_supports]
              )
           >> *(
                    COMMA
                 >> assertion ("supported interface name expected")
                    (
                      identifier[act_event_type_supports]
                    )
               )
        )[error_handler]
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
        >> guard
           (
                assertion ("factory name expected")
                (
                  simple_identifier[act_event_type_factory_name]
                )
             >> assertion ("'(' expected")
                (
                  LPAREN
                )
             >> event_type_factory_parameter_list
             >> assertion ("parameter declaration or ')' expected",
                           DiagnosticType::BEFORE)
                (
                  RPAREN
                )
             >> !(
                      RAISES
                   >> assertion ("'(' expected")
                      (
                        LPAREN
                      )
                   >> event_type_factory_raises_list
                   >> assertion ("',' or ')' expected",
                                 DiagnosticType::BEFORE)
                      (
                        RPAREN
                      )
                 )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      event_type_factory_parameter_list =
        *(
              event_type_factory_parameter
           >> *(COMMA >> event_type_factory_parameter)
        )
        ;

      event_type_factory_parameter =
           IN
        >> guard
           (
             (
                   assertion ("type name expected")
                   (
                     identifier
                   )
                >> assertion ("parameter name expected",
                              DiagnosticType::BEFORE)
                   (
                     simple_identifier
                   )
             )[act_event_type_factory_parameter]
        )[error_handler]
        ;

      event_type_factory_raises_list =
        guard
        (
             assertion ("exception name expected")
             (
               identifier[act_event_type_factory_raises]
             )
          >> *(
                  COMMA
               >> assertion ("exception name expected")
                  (
                    identifier[act_event_type_factory_raises]
                  )
              )
        )[error_handler]
        ;

      //
      // home
      //
      home_decl =
           HOME
        >> guard
           (
                assertion ("home name expected")
                (
                  simple_identifier[act_home_begin]
                )
             >> hood
                (
                     !(COLON >> home_inheritance_spec)
                  >> !(SUPPORTS >> home_support_spec)
                  >> assertion ("'manages' expected",
                                DiagnosticType::BEFORE)
                     (
                       MANAGES
                     )
                  >> assertion ("executor name expected")
                     (
                       identifier[act_home_manages]
                     )
                  >> assertion ("'{' expected")
                     (
                       LCBRACE[act_home_open_scope]
                     )
                  >> hood (home_body)
                     [
                       handler (f.home (),
                                &SemanticAction::Home::close_scope)
                     ]
                  >> assertion ("'}' expected",
                                f.home (),
                                &SemanticAction::Home::close_scope,
                                DiagnosticType::BEFORE)
                     (
                       RCBRACE[act_home_close_scope]
                     )
                )
                [
                  handler (f.home (),
                           &SemanticAction::Home::end)
                ]
             >> assertion ("';' expected",
                           f.home (),
                           &SemanticAction::Home::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_home_end]
                )
           )[error_handler]
        ;

      home_inheritance_spec =
        guard
        (
          assertion ("base home name expected")
          (
            identifier[act_home_inherits]
          )
        )[error_handler]
        ;

      home_support_spec =
        guard
        (
              assertion ("supported interface name expected")
              (
                identifier[act_home_supports]
              )
           >> *(
                    COMMA
                 >> assertion ("supported interface name expected")
                    (
                      identifier[act_home_supports]
                    )
               )
        )[error_handler]
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
        >> guard
           (
                assertion ("factory name expected")
                (
                  simple_identifier[act_home_factory_name]
                )
             >> assertion ("'(' expected")
                (
                  LPAREN
                )
             >> home_factory_parameter_list
             >> assertion ("parameter declaration or ')' expected",
                           DiagnosticType::BEFORE)
                (
                  RPAREN
                )
             >> !(
                      RAISES
                   >> assertion ("'(' expected")
                      (
                        LPAREN
                      )
                   >> home_factory_raises_list
                   >> assertion ("',' or ')' expected",
                                 DiagnosticType::BEFORE)
                      (
                        RPAREN
                      )
                 )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      home_factory_parameter_list =
        *(
              home_factory_parameter
           >> *(COMMA >> home_factory_parameter)
        )
        ;

      home_factory_parameter =
           IN
        >> guard
           (
             (
                   assertion ("type name expected")
                   (
                     identifier
                   )
                >> assertion ("parameter name expected",
                              DiagnosticType::BEFORE)
                   (
                     simple_identifier
                   )
             )[act_home_factory_parameter]
        )[error_handler]
        ;

      home_factory_raises_list =
        guard
        (
             assertion ("exception name expected")
             (
               identifier[act_home_factory_raises]
             )
          >> *(
                  COMMA
               >> assertion ("exception name expected")
                  (
                    identifier[act_home_factory_raises]
                  )
              )
        )[error_handler]
        ;


      // home finder
      //
      //
      home_finder_decl =
           FINDER
        >> guard
           (
                assertion ("finder name expected")
                (
                  simple_identifier[act_home_finder_name]
                )
             >> assertion ("'(' expected")
                (
                  LPAREN
                )
             >> home_finder_parameter_list
             >> assertion ("parameter declaration or ')' expected",
                           DiagnosticType::BEFORE)
                (
                  RPAREN
                )
             >> !(
                      RAISES
                   >> assertion ("'(' expected")
                      (
                        LPAREN
                      )
                   >> home_finder_raises_list
                   >> assertion ("',' or ')' expected",
                                 DiagnosticType::BEFORE)
                      (
                        RPAREN
                      )
                 )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;

      home_finder_parameter_list =
        *(
              home_finder_parameter
           >> *(COMMA >> home_finder_parameter)
        )
        ;

      home_finder_parameter =
           IN
        >> guard
           (
             (
                   assertion ("type name expected")
                   (
                     identifier
                   )
                >> assertion ("parameter name expected",
                              DiagnosticType::BEFORE)
                   (
                     simple_identifier
                   )
             )[act_home_finder_parameter]
        )[error_handler]
        ;

      home_finder_raises_list =
        guard
        (
             assertion ("exception name expected")
             (
               identifier[act_home_finder_raises]
             )
          >> *(
                  COMMA
               >> assertion ("exception name expected")
                  (
                    identifier[act_home_finder_raises]
                  )
              )
        )[error_handler]
        ;
    }
  }
}
