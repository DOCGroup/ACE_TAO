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

          ABSTRACT   ("abstract"   ),
          ATTRIBUTE  ("attribute"  ),
          CONST      ("const"      ),
          ENUM       ("enum"       ),
          EXCEPTION  ("exception"  ),
          FACTORY    ("factory"    ),
          IN         ("in"         ),
          INCLUDE    ("include"    ),
          INOUT      ("inout"      ),
          INTERFACE  ("interface"  ),
          LOCAL      ("local"      ),
          MODULE     ("module"     ),
          ONEWAY     ("oneway"     ),
          OUT        ("out"        ),
          RAISES     ("raises"     ),
          SEQUENCE   ("sequence"   ),
          SINCLUDE   ("sinclude"   ),
          STRUCT     ("struct"     ),
          SUPPORTS   ("supports"   ),
          TYPEDEF    ("typedef"    ),
          TYPEID     ("typeid"     ),
          TYPEPREFIX ("typeprefix" ),

          COLON  (":"),
          COMMA  (","),
          LBRACE ("{"),
          RBRACE ("}"),
          LPAREN ("("),
          RPAREN (")"),
          LT     ("<"),
          GT     (">"),
          SEMI   (";"),


          ADD ("+"),
          AND ("&"),
          COM ("~"),
          DIV ("/"),
          EQ  ("="),
          LSH ("<<"),
          MUL ("*"),
          OR  ("|"),
          REM ("%"),
          SUB ("-"),
          RSH (">>"),
          XOR ("^"),


          // Attribute
          //
          //
          act_attribute_type (
            f.attribute (), &SemanticAction::Attribute::type),

          act_attribute_name (
            f.attribute (), &SemanticAction::Attribute::name),

          // Enum
          //
          //
          act_enum_begin (
            f.enum_ (), &SemanticAction::Enum::begin),

          act_enum_enumerator (
            f.enum_ (), &SemanticAction::Enum::enumerator),

          act_enum_end (
            f.enum_ (), &SemanticAction::Enum::end),


          // Exception
          //
          //
          act_exception_begin (
            f.exception (), &SemanticAction::Exception::begin),

          act_exception_open_scope (
            f.exception (), &SemanticAction::Scope::open_scope),

          act_exception_close_scope (
            f.exception (), &SemanticAction::Scope::close_scope),

          act_exception_end (
            f.exception (), &SemanticAction::Exception::end),

          // Include
          //
          //
          act_include_begin (f.include (), &SemanticAction::Include::begin),
          act_include_end (f.include (), &SemanticAction::Include::end),

          act_system_include_begin (
            f.system_include (), &SemanticAction::SystemInclude::begin),

          act_system_include_end (
            f.system_include (), &SemanticAction::SystemInclude::end),

          // Interface
          //
          //
          act_abstract_interface_begin_def (
            f.interface (), &SemanticAction::Interface::begin_abstract_def),

          act_abstract_interface_begin_fwd (
            f.interface (), &SemanticAction::Interface::begin_abstract_fwd),

          act_local_interface_begin_def (
            f.interface (), &SemanticAction::Interface::begin_local_def),

          act_local_interface_begin_fwd (
            f.interface (), &SemanticAction::Interface::begin_local_fwd),

          act_unconstrained_interface_begin_def (
            f.interface (),
            &SemanticAction::Interface::begin_unconstrained_def),

          act_unconstrained_interface_begin_fwd (
            f.interface (),
            &SemanticAction::Interface::begin_unconstrained_fwd),

          act_interface_inherits (
            f.interface (), &SemanticAction::Interface::inherits),

          act_interface_open_scope (
            f.interface (), &SemanticAction::Scope::open_scope),

          act_interface_close_scope (
            f.interface (), &SemanticAction::Scope::close_scope),

          act_interface_end (f.interface (), &SemanticAction::Interface::end),


          // Member
          //
          //
          act_member_type (
            f.member (), &SemanticAction::Member::type),

          act_member_name (
            f.member (), &SemanticAction::Member::name),


          // Module
          //
          //
          act_module_begin (f.module (), &SemanticAction::Module::begin),

          act_module_open_scope (
            f.module (), &SemanticAction::Scope::open_scope),

          act_module_close_scope (
            f.module (), &SemanticAction::Scope::close_scope),

          act_module_end (f.module (), &SemanticAction::Module::end),


          // Operation
          //
          //
          act_operation_one_way (
            f.operation (), &SemanticAction::Operation::one_way),

          act_operation_two_way (
            f.operation (), &SemanticAction::Operation::two_way),

          act_operation_type (
            f.operation (), &SemanticAction::Operation::type),

          act_operation_name (
            f.operation (), &SemanticAction::Operation::name),

          act_operation_parameter (
            this, &Parser::act_operation_parameter_core),

          act_operation_raises (
            f.operation (), &SemanticAction::Operation::raises),


          // Struct
          //
          //
          act_struct_begin_def (
            f.struct_ (), &SemanticAction::Struct::begin_def),

          act_struct_begin_fwd (
            f.struct_ (), &SemanticAction::Struct::begin_fwd),

          act_struct_open_scope (
            f.struct_ (), &SemanticAction::Scope::open_scope),

          act_struct_close_scope (
            f.struct_ (), &SemanticAction::Scope::close_scope),

          act_struct_end (f.struct_ (), &SemanticAction::Struct::end),


          // Typedef
          //
          //
          act_typedef_begin (
            f.typedef_ (), &SemanticAction::Typedef::begin),

          act_typedef_begin_seq (
            f.typedef_ (), &SemanticAction::Typedef::begin_seq),

          act_typedef_declarator (
            f.typedef_ (), &SemanticAction::Typedef::declarator),

          act_typedef_end (
            f.typedef_ (), &SemanticAction::Typedef::end),


          // TypeId
          //
          //
          act_type_id_begin (f.type_id (), &SemanticAction::TypeId::begin),
          act_type_id_end (f.type_id (), &SemanticAction::TypeId::end),


          // TypePrefix
          //
          //
          act_type_prefix_begin (f.type_prefix (),
                                 &SemanticAction::TypePrefix::begin),

          act_type_prefix_end (f.type_prefix (),
                               &SemanticAction::TypePrefix::end)
    {
      language =
        guard
        (
          *import >> +declaration >> EOS

        )[root_error_handler]
        ;


      /*
      language =
        guard
        (
          +(*import >> (+declaration)) >> EOS
        )
        ;
      */

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
        >> string_literal[act_system_include_begin]
        >> SEMI[act_system_include_end]
        ;

      declaration =
          abstract_type_decl
        | const_decl
        | enum_decl
        | exception_decl
        | extension
        | local_type_decl
        | module_decl
        | struct_decl
        | typedef_
        | type_id
        | type_prefix
        | unconstrained_interface_decl
        ;

      type_id =
           TYPEID
        >> (
               identifier
            >> string_literal
           )[act_type_id_begin]
        >> SEMI[act_type_id_end]
        ;

      type_prefix =
           TYPEPREFIX
        >> (
                identifier
             >> string_literal
           )[act_type_prefix_begin]
        >> SEMI[act_type_prefix_end]
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

      // const
      //
      //
      const_decl =
           CONST
        >> identifier
        >> simple_identifier
        >> EQ
        >> const_expr
        >> SEMI
        ;


      // const expression
      //
      //

      const_expr =
          boolean_const_expr
        | character_const_expr
        | integer_const_expr
        | string_const_expr
        ;

      boolean_const_expr = boolean_literal;

      character_const_expr = character_literal;

      //@@ I may have recognision problem on unary operators or
      //   even worse on LPAREN.
      //

      integer_const_expr = integer_or_expr;

      integer_or_expr =
           integer_xor_expr
        >> *(OR >> integer_xor_expr)
        ;

      integer_xor_expr =
           integer_and_expr
        >> *(XOR >> integer_and_expr)
        ;


      integer_and_expr =
           integer_shift_expr
        >> *(AND >> integer_shift_expr)
        ;

      integer_shift_expr =
           integer_add_expr
        >> *(
                (RSH >> integer_add_expr)
              | (LSH >> integer_add_expr)
            )
        ;

      integer_add_expr =
           integer_mul_expr
        >> *(
                (ADD >> integer_mul_expr)
              | (SUB >> integer_mul_expr)
            )
        ;

      integer_mul_expr =
           integer_unary_expr
        >> *(
                (MUL >> integer_unary_expr)
              | (DIV >> integer_unary_expr)
              | (REM >> integer_unary_expr)
            )
        ;

      integer_unary_expr =
          integer_primary_expr
        | ADD >> integer_primary_expr
        | SUB >> integer_primary_expr
        | COM >> integer_primary_expr
        ;

      integer_primary_expr =
          identifier
        | integer_literal
        | LPAREN >> integer_const_expr >> RPAREN
        ;

      string_const_expr = string_literal;


      // enum
      //
      //
      enum_decl =
           ENUM
        >> simple_identifier[act_enum_begin]
        >> LBRACE
        >> enumerator_decl >> *(COMMA >> enumerator_decl)
        >> RBRACE
        >> SEMI[act_enum_end]
        ;

      enumerator_decl = identifier[act_enum_enumerator];

      // interface
      //
      //
      abstract_interface_decl =
        guard
        (
            (
                 simple_identifier
              >> SEMI
            )[act_abstract_interface_begin_fwd][act_interface_end]
          |
            (
                 (
                   simple_identifier
                   >> COLON
                 )[act_abstract_interface_begin_def]

              >> interface_inheritance_spec
              >> LBRACE[act_interface_open_scope]
              >> interface_def_trailer
            )
          |
            (
                 (
                      simple_identifier
                   >> LBRACE
                 )[act_abstract_interface_begin_def][act_interface_open_scope]

              >> interface_def_trailer
            )
        )[error_handler]
        ;


      local_interface_decl =
        guard
        (
            (
                 simple_identifier
              >> SEMI
            )[act_local_interface_begin_fwd][act_interface_end]
          |
            (
                 (
                   simple_identifier
                   >> COLON
                 )[act_local_interface_begin_def]

              >> interface_inheritance_spec
              >> LBRACE[act_interface_open_scope]
              >> interface_def_trailer
            )
          |
            (
                 (
                      simple_identifier
                   >> LBRACE
                 )[act_local_interface_begin_def][act_interface_open_scope]

              >> interface_def_trailer
            )
        )[error_handler]
        ;


      unconstrained_interface_decl =
        guard
        (
             INTERFACE
          >> (
                 (
                      simple_identifier
                   >> SEMI
                 )[act_unconstrained_interface_begin_fwd][act_interface_end]
               |
                 (
                      (
                           simple_identifier
                        >> COLON
                      )[act_unconstrained_interface_begin_def]

                   >> interface_inheritance_spec
                   >> LBRACE[act_interface_open_scope]
                   >> interface_def_trailer
                 )
               |
                 (
                      (
                           simple_identifier
                        >> LBRACE
                      )[act_unconstrained_interface_begin_def][act_interface_open_scope]

                   >> interface_def_trailer
                 )
             )
        )[error_handler]
        ;


      interface_def_trailer =
           interface_body
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
        ;

      /*

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
      */


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


      // attribute
      //
      //
      attribute_decl =
           ATTRIBUTE
        >> identifier[act_attribute_type]
        >> simple_identifier[act_attribute_name]
        >> SEMI
        ;


      // exception
      //
      //
      exception_decl =
           EXCEPTION
        >> simple_identifier[act_exception_begin]
        >> LBRACE[act_exception_open_scope]
        >> exception_body
        >> RBRACE[act_exception_close_scope]
        >> SEMI[act_exception_end]
        ;

      exception_body =
        *member_decl
        ;


      // direction specifier
      //
      //
      direction_specifier =
           IN
        |  OUT
        |  INOUT
        ;


      // member
      //
      //
      member_decl =
           identifier[act_member_type]
        >> member_declarator_list
        >> SEMI
        ;


      member_declarator_list =
           identifier[act_member_name]
        >> *(
                 COMMA
              >> identifier[act_member_name]
            )
        ;


      // operation
      //
      //
      operation_decl =
           (
               (
                    ONEWAY[act_operation_one_way]
                 >> identifier[act_operation_type]
               )
             |
               (
                 identifier[act_operation_two_way][act_operation_type]
               )
           )
        >> operation_decl_trailer
        ;


      operation_decl_trailer =
           simple_identifier[act_operation_name]
        >> LPAREN
        >> operation_parameter_list
        >> RPAREN
        >> !(RAISES >> LPAREN >> operation_raises_list >> RPAREN)
        >> SEMI
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

      operation_raises_list =
           identifier[act_operation_raises]
        >> *(COMMA >> identifier[act_operation_raises])
        ;


      // struct
      //
      //
      struct_decl =
        guard
        (
             STRUCT
          >> (
                 (
                      simple_identifier
                   >> SEMI
                 )[act_struct_begin_fwd][act_struct_end]
               |
                 (
                      (
                           simple_identifier
                        >> LBRACE
                      )[act_struct_begin_def][act_struct_open_scope]
                   >> struct_def_trailer
                 )
             )
        )[error_handler]
        ;

      /*
      struct_decl =
           STRUCT
        >> simple_identifier
        >> LBRACE
        >> struct_body
        >> RBRACE
        >> SEMI
        ;
      */


      struct_def_trailer =
           struct_body
        >> assertion ("member or \'}\' expected",
                      f.struct_ (),
                      &SemanticAction::Struct::close_scope,
                      &SemanticAction::Struct::end,
                      RecoveryMethod::STANDARD,
                      DiagnosticType::BEFORE)
           (
             RBRACE[act_struct_close_scope]
           )
        >> assertion ("\';\' is missing",
                      f.struct_ (),
                      &SemanticAction::Struct::end,
                      RecoveryMethod::NONE)
           (
             SEMI[act_struct_end]
           )
        ;


      struct_body =
        +member_decl
        ;


      // typedef
      //
      //

      typedef_ =
           TYPEDEF
        >> typedef_type_spec
        >> typedef_declarator_list
        >> SEMI[act_typedef_end]
        ;

      typedef_type_spec =
          identifier[act_typedef_begin]
        | SEQUENCE >> LT >> identifier[act_typedef_begin_seq] >> GT
        ;

      typedef_declarator_list =
           identifier[act_typedef_declarator]
        >> *(
                 COMMA
              >> identifier[act_typedef_declarator]
            )
        ;
    }
  }
}
