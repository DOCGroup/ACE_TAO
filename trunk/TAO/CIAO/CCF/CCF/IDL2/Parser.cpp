// file      : CCF/IDL2/Parser.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Parser.hpp"

//
// Note: DO NOT run emacs indenter (or any other indentation tool) over
//       this file because it will most likely break BNF indentation.
//

namespace CCF
{
  namespace IDL2
  {
    using  Parsing::DiagnosticType;
    using  Parsing::RecoveryMethod;

    Parser::
    ~Parser ()
    {
    }

    Parser::
    Parser (CompilerElements::Context& context,
            Diagnostic::Stream& dout,
            LexicalAnalyzer const& l,
            SemanticAction::Factory& f)
        : dout_ (dout),
          lexer_ (l),
          actions_ (f),

          error_handler (context, dout),

          ABSTRACT  ("abstract"),
          ATTRIBUTE ("attribute"),
          FACTORY   ("factory"),
          IN        ("in"),
          INCLUDE   ("include"),
          INOUT     ("inout"),
          INTERFACE ("interface"),
          LOCAL     ("local"),
          MODULE    ("module"),
          OUT       ("out"),
          SINCLUDE  ("sinclude"),
          SUPPORTS  ("supports"),

          COLON  (":"),
          COMMA  (","),
          LBRACE ("{"),
          RBRACE ("}"),
          LPAREN ("("),
          RPAREN (")"),
          SEMI   (";"),

          // Include
          //
          //
          act_include_begin (f.include (), &SemanticAction::Include::begin),
          act_include_end (f.include (), &SemanticAction::Include::end),

          // Module
          //
          //
          act_module_begin (f.module (), &SemanticAction::Module::begin),

          act_module_open_scope (
            f.module (), &SemanticAction::Scope::open_scope),

          act_module_close_scope (
            f.module (), &SemanticAction::Scope::close_scope),

          act_module_end (f.module (), &SemanticAction::Module::end),


          // Interface
          //
          //
          act_abstract_interface_begin (
            f.interface (), &SemanticAction::Interface::begin_abstract),

          act_local_interface_begin (
            f.interface (), &SemanticAction::Interface::begin_local),

          act_unconstrained_interface_begin (
            f.interface (), &SemanticAction::Interface::begin_unconstrained),

          act_interface_inherits (
            f.interface (), &SemanticAction::Interface::inherits),

          act_interface_open_scope (
            f.interface (), &SemanticAction::Scope::open_scope),

          act_interface_close_scope (
            f.interface (), &SemanticAction::Scope::close_scope),

          act_interface_end (f.interface (), &SemanticAction::Interface::end),

          // Attribute
          //
          //
          act_attribute_type (
            f.attribute (), &SemanticAction::Attribute::type),

          act_attribute_name (
            f.attribute (), &SemanticAction::Attribute::name),

          // Operation
          //
          //
          act_operation_begin (
            f.operation (), &SemanticAction::Operation::begin),

          act_operation_parameter (
            this, &Parser::act_operation_parameter_core),

          act_operation_end (f.operation (), &SemanticAction::Operation::end)

    {
      language =
        guard
        (
          *import >> +declaration >> EOS

        )[root_error_handler]
        ;

      import =
          include_decl
        | system_include_decl
        ;

      include_decl =
           INCLUDE
        >> string_literal[act_include_begin]
        >> SEMI[act_include_end]
        ;

      system_include_decl =
           SINCLUDE
        >> string_literal
        >> SEMI[act_include_end]
        ;

      declaration =
          abstract_type_decl
        | local_type_decl
        | module_decl
        | unconstrained_interface_decl
        | extension
        ;

      abstract_type_decl =
           ABSTRACT
        >> guard
           (
             assertion ("interface or valuetype declaration expected")
             (
               INTERFACE >> assertion ()(abstract_interface_decl)
             )
           )[error_handler]
        ;

      local_type_decl =
           LOCAL
        >> guard
           (
             assertion ("interface declaration expected")
             (
               INTERFACE >> assertion ()(local_interface_decl)
             )
           )[error_handler]
        ;

      module_decl =
           MODULE
        >> guard
           (
                assertion ("module name expected")
                (
                  simple_identifier[act_module_begin]
                )
             >> assertion ("\'{\' expected",
                           f.module (),
                           &SemanticAction::Module::end)
                (
                  LBRACE[act_module_open_scope]
                )
             >> assertion ("declaration expected",
                           f.module (),
                           &SemanticAction::Module::close_scope,
                           &SemanticAction::Module::end,
                           RecoveryMethod::STANDARD,
                           DiagnosticType::BEFORE)
                (
                  hood (+declaration)
                  [
                    handler (f.module (),
                             &SemanticAction::Module::close_scope,
                             &SemanticAction::Module::end)
                  ]
                )
             >> assertion ("declaration or \'}\' expected",
                           f.module (),
                           &SemanticAction::Module::close_scope,
                           &SemanticAction::Module::end,
                           RecoveryMethod::STANDARD,
                           DiagnosticType::BEFORE)
                (
                  RBRACE[act_module_close_scope]
                )
             >> assertion ("\';\' is missing",
                           f.module (),
                           &SemanticAction::Module::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_module_end]
                )
           )[error_handler]
        ;


      //
      // interface
      //
      abstract_interface_decl =
        guard
        (
             assertion ("interface name expected")
             (
               simple_identifier[act_abstract_interface_begin]
             )
          >> interface_decl_trailer

        )[error_handler]
        ;


      local_interface_decl =
        guard
        (
             assertion ("interface name expected")
             (
               simple_identifier[act_local_interface_begin]
             )
          >> interface_decl_trailer

        )[error_handler]
        ;


      unconstrained_interface_decl =
        guard
        (
             INTERFACE
          >> assertion ("interface name expected")
             (
               simple_identifier[act_unconstrained_interface_begin]
             )
          >> interface_decl_trailer

        )[error_handler]
        ;


      interface_decl_trailer =
        assertion ("\';\', \'{\' or inheritance specification expected",
                   f.interface (),
                   &SemanticAction::Interface::end)
        (
            SEMI[act_interface_end]
          |
            (
                 !(COLON >> interface_inheritance_spec)
              >> LBRACE[act_interface_open_scope]
              >> interface_body
              >> assertion ("declaration or \'}\' expected",
                            f.interface (),
                            &SemanticAction::Interface::close_scope,
                            &SemanticAction::Interface::end,
                            RecoveryMethod::STANDARD,
                            DiagnosticType::BEFORE)
                 (
                   RBRACE[act_interface_close_scope]
                 )
              >> assertion ("\';\' is missing",
                            f.interface (),
                            &SemanticAction::Interface::end,
                            RecoveryMethod::NONE)
                 (
                   SEMI[act_interface_end]
                 )
            )
        )
        ;


      interface_inheritance_spec =
           assertion ("interface name expected",
                      f.interface (),
                      &SemanticAction::Interface::end)
           (
             identifier[act_interface_inherits]
           )
        >> *(
                 COMMA
              >> assertion ("interface name expected",
                            f.interface (),
                            &SemanticAction::Interface::end)
                 (
                   identifier[act_interface_inherits]
                 )
            )
        ;

      interface_body =
        *(
             attribute_decl
           | operation_decl
        )
        ;

      //
      // attribute
      //
      attribute_decl =
           ATTRIBUTE
        >> identifier[act_attribute_type]
        >> simple_identifier[act_attribute_name]
        >> SEMI
        ;


      //
      // direction specifier
      //
      direction_specifier =
           IN
        |  OUT
        |  INOUT
        ;


      //
      // operation
      //
      operation_decl =
           (identifier >> simple_identifier)[act_operation_begin]
        >> LPAREN
        >> operation_parameter_list
        >> RPAREN
        >> SEMI[act_operation_end]
        ;

      operation_parameter_list =
        *(
              operation_parameter
           >> *(COMMA >> operation_parameter)
        )
        ;

      operation_parameter =
        (
              direction_specifier
           >> identifier
           >> simple_identifier
        )[act_operation_parameter]
        ;
    }
  }
}
