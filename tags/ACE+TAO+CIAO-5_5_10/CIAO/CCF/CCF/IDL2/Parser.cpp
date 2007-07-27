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

          ABSTRACT    ("abstract"   ),
          ATTRIBUTE   ("attribute"  ),
          BINCLUDE    ("__binclude" ),
          CASE        ("case"       ),
          CONST       ("const"      ),
          CUSTOM      ("custom"     ),
          DEFAULT     ("default"    ),
          ENUM        ("enum"       ),
          EXCEPTION   ("exception"  ),
          FACTORY     ("factory"    ),
          GETRAISES   ("getraises"  ),
          IN          ("in"         ),
          INOUT       ("inout"      ),
          INTERFACE   ("interface"  ),
          QINCLUDE    ("__qinclude" ),
          LOCAL       ("local"      ),
          MODULE      ("module"     ),
          NATIVE      ("native"     ),
          ONEWAY      ("oneway"     ),
          OUT         ("out"        ),
          PRIVATE     ("private"    ),
          PUBLIC      ("public"     ),
          RAISES      ("raises"     ),
          READONLY    ("readonly"   ),
          SEQUENCE    ("sequence"   ),
          SETRAISES   ("setraises"  ),
          STRING      ("string"     ),
          STRUCT      ("struct"     ),
          SUPPORTS    ("supports"   ),
          SWITCH      ("switch"     ),
          TRUNCATABLE ("truncatable"),
          TYPEDEF     ("typedef"    ),
          TYPEID      ("typeid"     ),
          TYPEPREFIX  ("typeprefix" ),
          UNION       ("union"      ),
          VALUETYPE   ("valuetype"  ),
          WSTRING     ("wstring"    ),

          COLON   (":"),
          COMMA   (","),
          LCBRACE ("{"),
          RCBRACE ("}"),
          LSBRACE ("["),
          RSBRACE ("]"),
          LPAREN  ("("),
          RPAREN  (")"),
          LT      ("<"),
          GT      (">"),
          SEMI    (";"),


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
          act_attribute_begin_ro (
            f.attribute (), &SemanticAction::Attribute::begin_ro),

          act_attribute_begin_rw (
            f.attribute (), &SemanticAction::Attribute::begin_rw),

          act_attribute_type (
            f.attribute (), &SemanticAction::Attribute::type),

          act_attribute_name (
            f.attribute (), &SemanticAction::Attribute::name),

          act_attribute_get_raises (
            f.attribute (), &SemanticAction::Attribute::get_raises),

          act_attribute_set_raises (
            f.attribute (), &SemanticAction::Attribute::set_raises),

          act_attribute_end (
            f.attribute (), &SemanticAction::Attribute::end),

          // Const
          //
          act_const_begin (
            f.const_ (), &SemanticAction::Const::begin),

          act_const_expr (
            f.const_ (), &SemanticAction::Const::expr),

          // Enum
          //
          //
          act_enum_begin (
            f.enum_ (), &SemanticAction::Enum::begin),

          act_enum_enumerator (
            f.enum_ (), &SemanticAction::Enum::enumerator),

          act_enum_end (
            f.enum_ (), &SemanticAction::Enum::end),


          // Expression
          //

          act_const_expr_flush (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::flush),

          act_numeric_expression_const (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::const_),

          act_numeric_expression_integer_literal (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::integer_literal),

          act_numeric_expression_pre (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::pre),

          act_numeric_expression_pos (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::pos),

          act_numeric_expression_neg (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::neg),

          act_numeric_expression_com (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::com),

          act_numeric_expression_mul (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::mul),

          act_numeric_expression_div (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::div),

          act_numeric_expression_rem (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::rem),

          act_numeric_expression_add (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::add),

          act_numeric_expression_sub (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::sub),

          act_numeric_expression_rsh (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::rsh),

          act_numeric_expression_lsh (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::lsh),

          act_numeric_expression_and (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::and_),

          act_numeric_expression_xor (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::xor_),

          act_numeric_expression_or (
            f.numeric_expression (),
            &SemanticAction::NumericExpression::or_),

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
          act_include_quote (
            f.include (), &SemanticAction::Include::quote),

          act_include_bracket (
            f.include (), &SemanticAction::Include::bracket),

          act_include_end (
            f.include (), &SemanticAction::Include::end),


          // Interface
          //
          //
          act_interface_begin_abstract_def (
            f.interface (), &SemanticAction::Interface::begin_abstract_def),

          act_interface_begin_abstract_fwd (
            f.interface (), &SemanticAction::Interface::begin_abstract_fwd),

          act_interface_begin_local_def (
            f.interface (), &SemanticAction::Interface::begin_local_def),

          act_interface_begin_local_fwd (
            f.interface (), &SemanticAction::Interface::begin_local_fwd),

          act_interface_begin_unconstrained_def (
            f.interface (),
            &SemanticAction::Interface::begin_unconstrained_def),

          act_interface_begin_unconstrained_fwd (
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

          act_member_end (
            f.member (), &SemanticAction::Member::end),


          // Module
          //
          //
          act_module_begin (f.module (), &SemanticAction::Module::begin),

          act_module_open_scope (
            f.module (), &SemanticAction::Scope::open_scope),

          act_module_close_scope (
            f.module (), &SemanticAction::Scope::close_scope),

          act_module_end (f.module (), &SemanticAction::Module::end),


          // Native
          //
          //
          act_native_name (
            f.native (), &SemanticAction::Native::name),

          act_native_end (
            f.native (), &SemanticAction::Native::end),


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
          act_typedef_pre (
            f.typedef_ (), &SemanticAction::Typedef::pre),

          act_typedef_begin (
            f.typedef_ (), &SemanticAction::Typedef::begin),

          act_typedef_begin_unbounded_seq (
            f.typedef_ (), &SemanticAction::Typedef::begin_unbounded_seq),

          act_typedef_begin_bounded_seq (
            f.typedef_ (), &SemanticAction::Typedef::begin_bounded_seq),

          act_typedef_begin_bounded_string (
            f.typedef_ (), &SemanticAction::Typedef::begin_bounded_string),

          act_typedef_begin_bounded_wstring (
            f.typedef_ (), &SemanticAction::Typedef::begin_bounded_wstring),

          act_typedef_begin_array (
            f.typedef_ (), &SemanticAction::Typedef::begin_array),

          act_typedef_bound (
            f.typedef_ (), &SemanticAction::Typedef::bound),

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
                               &SemanticAction::TypePrefix::end),

          // Union
          //
          //
          act_union_begin_def (
            f.union_ (), &SemanticAction::Union::begin_def),

          act_union_begin_fwd (
            f.union_ (), &SemanticAction::Union::begin_fwd),

          act_union_type (
            f.union_ (), &SemanticAction::Union::type),

          act_union_open_scope (
            f.union_ (), &SemanticAction::Scope::open_scope),

          act_union_member_type (
            f.union_ (), &SemanticAction::Union::member_type),

          act_union_member_name (
            f.union_ (), &SemanticAction::Union::member_name),

          act_union_close_scope (
            f.union_ (), &SemanticAction::Scope::close_scope),

          act_union_end (
            f.union_ (), &SemanticAction::Union::end),


          // ValueType
          //
          //
          act_value_type_begin_abstract_def (
            f.value_type (), &SemanticAction::ValueType::begin_abstract_def),

          act_value_type_begin_abstract_fwd (
            f.value_type (), &SemanticAction::ValueType::begin_abstract_fwd),

          act_value_type_begin_concrete_def (
            f.value_type (), &SemanticAction::ValueType::begin_concrete_def),

          act_value_type_begin_concrete_fwd (
            f.value_type (), &SemanticAction::ValueType::begin_concrete_fwd),

          act_value_type_inherits (
            f.value_type (), &SemanticAction::ValueType::inherits),

          act_value_type_supports (
            f.value_type (), &SemanticAction::ValueType::supports),

          act_value_type_open_scope (
            f.value_type (), &SemanticAction::Scope::open_scope),

          act_value_type_close_scope (
            f.value_type (), &SemanticAction::Scope::close_scope),

          act_value_type_end (
            f.value_type (), &SemanticAction::ValueType::end),


          // ValueTypeFactory
          //
          act_value_type_factory_name (
            f.value_type_factory (), &SemanticAction::ValueTypeFactory::name),

          act_value_type_factory_parameter (
            f.value_type_factory (), &SemanticAction::ValueTypeFactory::parameter),

          act_value_type_factory_raises (
            f.value_type_factory (), &SemanticAction::ValueTypeFactory::raises),



          // ValueTypeMember
          //
          act_value_type_member_begin_private (
            f.value_type_member (),
            &SemanticAction::ValueTypeMember::begin_private),

          act_value_type_member_begin_public (
            f.value_type_member (),
            &SemanticAction::ValueTypeMember::begin_public),

          act_value_type_member_type (
            f.value_type_member (), &SemanticAction::ValueTypeMember::type),

          act_value_type_member_name (
            f.value_type_member (), &SemanticAction::ValueTypeMember::name),

          act_value_type_member_end (
            f.value_type_member (), &SemanticAction::ValueTypeMember::end)

    {
      language =
        guard
        (
          guard
          (
               assertion ("declaration or include directive expected",
                          DiagnosticType::BEFORE)
               (
                   (+import >> *declaration)
                 | +declaration
               )
            >> assertion ("declaration or include directive expected",
                          DiagnosticType::BEFORE)
               (
                 EOS
               )
          )[error_handler]
        )[root_error_handler] // Stops bailing out (rethrowing).
        ;

      import = include_decl
        ;

      include_decl =
           (
               (QINCLUDE >> string_literal[act_include_quote])
             | (BINCLUDE >> string_literal[act_include_bracket])
           )
        >> SEMI[act_include_end]
        ;

      // There are two classes of types: The first class includes
      // interface and valuetype. The second class includes all
      // other types like struct, etc. I wonder how I can represent
      // it in the grammar.
      //

      declaration =
          abstract_type_decl
        | const_decl
        | extension
        | local_type_decl
        | module_decl
        | type_decl
        | type_id_decl
        | type_prefix_decl
        | unconstrained_interface_decl
        | concrete_value_type_decl
        ;

      type_decl =
          enum_decl
        | exception_decl
        | native_decl
        | struct_decl
        | typedef_decl
        | union_decl
        ;

      type_id_decl =
           TYPEID
        >> guard
           (
                (
                     assertion ("type name expected")
                     (
                       identifier
                     )
                  >> assertion ("string literal expected",
                                DiagnosticType::BEFORE)
                     (
                       string_literal
                     )
                )[act_type_id_begin]

             >> assertion ("';' expected",
                           f.type_id (),
                           &SemanticAction::TypeId::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_type_id_end]
                )
           )[error_handler]
        ;

      type_prefix_decl =
           TYPEPREFIX
        >> guard
           (
                (
                     assertion ("type name expected")
                     (
                       identifier
                     )
                  >> assertion ("string literal expected",
                                DiagnosticType::BEFORE)
                     (
                       string_literal
                     )
                )[act_type_prefix_begin]

             >> assertion ("';' expected",
                           f.type_prefix (),
                           &SemanticAction::TypePrefix::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_type_prefix_end]
                )
           )[error_handler]
        ;

      abstract_type_decl =
           ABSTRACT
        >> guard
           (
             assertion ("interface or valuetype declaration expected")
             (
                 (INTERFACE >> abstract_interface_decl)
               |
                 (VALUETYPE >> abstract_value_type_decl)
             )
           )[error_handler]
        ;

      local_type_decl =
           LOCAL
        >> guard
           (
             assertion ("interface declaration expected")
             (
               INTERFACE >> local_interface_decl
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
             >> assertion ("'{' expected",
                           f.module (),
                           &SemanticAction::Module::end)
                (
                  LCBRACE[act_module_open_scope]
                )
             >> assertion ("declaration expected",
                           f.module (),
                           &SemanticAction::Module::close_scope,
                           &SemanticAction::Module::end,
                           DiagnosticType::BEFORE)
                (
                  hood (+declaration)
                  [
                    handler (f.module (),
                             &SemanticAction::Module::close_scope,
                             &SemanticAction::Module::end)
                  ]
                )
             >> assertion ("declaration or '}' expected",
                           f.module (),
                           &SemanticAction::Module::close_scope,
                           &SemanticAction::Module::end,
                           DiagnosticType::BEFORE)
                (
                  RCBRACE[act_module_close_scope]
                )
             >> assertion ("';' expected",
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
        >> guard
           (
                (
                     assertion ("type name expected")
                     (
                       identifier
                     )
                  >> assertion ("const name expected",
                                 DiagnosticType::BEFORE)
                     (
                       simple_identifier
                     )
                )[act_const_begin]
             >> assertion ("'=' expected")
                (
                  EQ[act_const_expr_flush] // flush expression stacks
                )
             >> assertion ("const expression expected")
                (
                  const_expr[act_const_expr]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;


      // const expression
      //
      //

      const_expr =
          boolean_const_expr
        | character_const_expr
        | numeric_const_expr
        | string_const_expr
        ;

      boolean_const_expr = boolean_literal;

      character_const_expr = character_literal;

      string_const_expr = string_literal;


      //
      //
      numeric_const_expr =
        guard
        (
          numeric_or_expr
        )[error_handler]
        ;

      numeric_or_expr =
           numeric_xor_expr
        >> *(
                 OR[act_numeric_expression_pre]
              >> assertion ("expression expected")
                 (
                   numeric_xor_expr
                 )
            )[act_numeric_expression_or]
        ;

      numeric_xor_expr =
           numeric_and_expr
        >> *(
                 XOR[act_numeric_expression_pre]
              >> assertion ("expression expected")
                 (
                   numeric_and_expr
                 )
            )[act_numeric_expression_xor]
        ;


      numeric_and_expr =
           numeric_shift_expr
        >> *(
                 AND[act_numeric_expression_pre]
              >> assertion ("expression expected")
                 (
                   numeric_shift_expr
                 )
            )[act_numeric_expression_and]
        ;

      numeric_shift_expr =
           numeric_add_expr
        >> *(
                (
                     RSH[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_add_expr
                     )
                )[act_numeric_expression_rsh]
              |
                (
                     LSH[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_add_expr
                     )
                )[act_numeric_expression_lsh]
            )
        ;

      numeric_add_expr =
           numeric_mul_expr
        >> *(
                (    ADD[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_mul_expr
                     )
                )[act_numeric_expression_add]

              |
                (    SUB[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_mul_expr
                     )
                )[act_numeric_expression_sub]
            )
        ;

      numeric_mul_expr =
           numeric_unary_expr
        >> *(
                (    MUL[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_unary_expr
                     )
                )[act_numeric_expression_mul]

              |
                (    DIV[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_unary_expr
                     )
                )[act_numeric_expression_div]

              |
                (    REM[act_numeric_expression_pre]
                  >> assertion ("expression expected")
                     (
                       numeric_unary_expr
                     )
                )[act_numeric_expression_rem]
            )
        ;

      numeric_unary_expr =
          numeric_primary_expr

        |    ADD[act_numeric_expression_pre]
          >> assertion ("expression expected")
             (
               numeric_primary_expr[act_numeric_expression_pos]
             )

        |    SUB[act_numeric_expression_pre]
          >> assertion ("expression expected")
             (
               numeric_primary_expr[act_numeric_expression_neg]
             )

        |    COM[act_numeric_expression_pre]
          >> assertion ("expression expected")
             (
               numeric_primary_expr[act_numeric_expression_com]
             )
        ;

      numeric_primary_expr =
          identifier[act_numeric_expression_const]
        | integer_literal[act_numeric_expression_integer_literal]
        |    LPAREN
          >> assertion ("expression expected")
             (
               numeric_const_expr
             )
          >> assertion ("')' expected",
                        DiagnosticType::BEFORE)
             (
               RPAREN
             )
        ;


      // enum
      //
      //
      enum_decl =
           ENUM
        >> guard
           (
                assertion ("enum name expected")
                (
                  simple_identifier[act_enum_begin]
                )
             >> hood
                (
                     assertion ("'{' expected")
                     (
                       LCBRACE
                     )
                  >> enumerator_decl >> *(COMMA >> enumerator_decl)
                  >> assertion ("',' or '}' expected",
                                DiagnosticType::BEFORE)
                     (
                       RCBRACE
                     )
                )
                [
                  handler (f.enum_ (),
                           &SemanticAction::Enum::end)
                ]
             >> assertion ("';' expected",
                           f.enum_ (),
                           &SemanticAction::Enum::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_enum_end]
                )
           )[error_handler]
        ;

      enumerator_decl =
        assertion ("enumerator name expected")
        (
          simple_identifier[act_enum_enumerator]
        )
        ;


      // interface
      //
      //
      abstract_interface_decl =
        guard
        (
          assertion ("abstract interface declaration expected",
                     DiagnosticType::BEFORE)
          (
              (
                   simple_identifier
                >> SEMI
              )[act_interface_begin_abstract_fwd][act_interface_end]
            |
              (
                   (
                     simple_identifier
                     >> COLON
                   )[act_interface_begin_abstract_def]

                >> hood
                   (
                        interface_inheritance_spec
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_interface_open_scope]
                        )
                     >> hood (interface_body)
                        [
                          handler (f.interface (),
                                   &SemanticAction::Interface::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.interface (),
                                   &SemanticAction::Interface::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_interface_close_scope]
                        )
                   )
                   [
                     handler (f.interface (),
                              &SemanticAction::Interface::end)
                   ]

                >> assertion ("';' expected",
                              f.interface (),
                              &SemanticAction::Interface::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_interface_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> LCBRACE
                   )[act_interface_begin_abstract_def][act_interface_open_scope]

                >> hood
                   (
                        hood (interface_body)
                        [
                          handler (f.interface (),
                                   &SemanticAction::Interface::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.interface (),
                                   &SemanticAction::Interface::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_interface_close_scope]
                        )
                   )
                   [
                     handler (f.interface (),
                              &SemanticAction::Interface::end)
                   ]

                >> assertion ("';' expected",
                              f.interface (),
                              &SemanticAction::Interface::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_interface_end]
                   )
              )
          )
        )[error_handler]
        ;

      local_interface_decl =
        guard
        (
          assertion ("local interface declaration expected",
                     DiagnosticType::BEFORE)
          (
              (
                   simple_identifier
                >> SEMI
              )[act_interface_begin_local_fwd][act_interface_end]
            |
              (
                   (
                     simple_identifier
                     >> COLON
                   )[act_interface_begin_local_def]

                >> hood
                   (
                        interface_inheritance_spec
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_interface_open_scope]
                        )
                     >> hood (interface_body)
                        [
                          handler (f.interface (),
                                   &SemanticAction::Interface::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.interface (),
                                   &SemanticAction::Interface::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_interface_close_scope]
                        )
                   )
                   [
                     handler (f.interface (),
                              &SemanticAction::Interface::end)
                   ]

                >> assertion ("';' expected",
                              f.interface (),
                              &SemanticAction::Interface::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_interface_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> LCBRACE
                   )[act_interface_begin_local_def][act_interface_open_scope]

                >> hood
                   (
                        hood (interface_body)
                        [
                          handler (f.interface (),
                                   &SemanticAction::Interface::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.interface (),
                                   &SemanticAction::Interface::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_interface_close_scope]
                        )
                   )
                   [
                     handler (f.interface (),
                              &SemanticAction::Interface::end)
                   ]

                >> assertion ("';' expected",
                              f.interface (),
                              &SemanticAction::Interface::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_interface_end]
                   )
              )
          )
        )[error_handler]
        ;

      unconstrained_interface_decl =
           INTERFACE
        >> guard
           (
             assertion ("interface declaration expected",
                        DiagnosticType::BEFORE)
             (
                 (
                       simple_identifier
                    >> SEMI
                 )[act_interface_begin_unconstrained_fwd][act_interface_end]
               |
                 (
                      (
                         simple_identifier
                         >> COLON
                      )[act_interface_begin_unconstrained_def]

                   >> hood
                      (
                           interface_inheritance_spec
                        >> assertion ("'{' expected")
                           (
                             LCBRACE[act_interface_open_scope]
                           )
                        >> hood (interface_body)
                           [
                             handler (f.interface (),
                                      &SemanticAction::Interface::close_scope)
                           ]
                        >> assertion ("declaration or '}' expected",
                                      f.interface (),
                                      &SemanticAction::Interface::close_scope,
                                      DiagnosticType::BEFORE)
                           (
                             RCBRACE[act_interface_close_scope]
                           )
                      )
                      [
                        handler (f.interface (),
                                 &SemanticAction::Interface::end)
                      ]

                   >> assertion ("';' expected",
                                 f.interface (),
                                 &SemanticAction::Interface::end,
                                 RecoveryMethod::NONE)
                      (
                        SEMI[act_interface_end]
                      )
                 )
               |
                 (
                      (
                           simple_identifier
                        >> LCBRACE
                      )[act_interface_begin_unconstrained_def][act_interface_open_scope]

                   >> hood
                      (
                           hood (interface_body)
                           [
                             handler (f.interface (),
                                      &SemanticAction::Interface::close_scope)
                           ]
                        >> assertion ("declaration or '}' expected",
                                      f.interface (),
                                      &SemanticAction::Interface::close_scope,
                                      DiagnosticType::BEFORE)
                           (
                             RCBRACE[act_interface_close_scope]
                           )
                      )
                      [
                        handler (f.interface (),
                                 &SemanticAction::Interface::end)
                      ]

                   >> assertion ("';' expected",
                                 f.interface (),
                                 &SemanticAction::Interface::end,
                                 RecoveryMethod::NONE)
                      (
                        SEMI[act_interface_end]
                      )
                 )
             )
           )[error_handler]
        ;

      interface_inheritance_spec =
        guard
        (
              assertion ("base interface name expected")
              (
                identifier[act_interface_inherits]
              )
           >> *(
                    COMMA
                 >> assertion ("base eventtype name expected")
                    (
                      identifier[act_interface_inherits]
                    )
               )
        )[error_handler]
        ;

      interface_body =
        *(
             const_decl
           | type_decl
           | type_id_decl
           | type_prefix_decl

           | attribute_decl
           | operation_decl
        )
        ;


      // attribute
      //
      //
      attribute_decl =
        guard
        (
            (
                 (
                      READONLY
                   >> assertion ("'attribute' expected")
                      (
                        ATTRIBUTE
                      )
                 )[act_attribute_begin_ro]

              >> hood (attribute_ro_decl_trailer)
                 [
                   handler (f.attribute (),
                            &SemanticAction::Attribute::end)
                 ]
              >> assertion ("';' expected",
                           f.attribute (),
                           &SemanticAction::Attribute::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_attribute_end]
                )
            )
          |
            (
                 ATTRIBUTE[act_attribute_begin_rw]
              >> hood (attribute_rw_decl_trailer)
                 [
                   handler (f.attribute (),
                            &SemanticAction::Attribute::end)
                 ]
              >> assertion ("';' expected",
                           f.attribute (),
                           &SemanticAction::Attribute::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_attribute_end]
                )
            )
        )[error_handler]
        ;


      attribute_ro_decl_trailer =
           assertion ("attribute type name expected")
           (
             identifier[act_attribute_type]
           )
        >> assertion ("attribute name expected",
                      DiagnosticType::BEFORE)
           (
             simple_identifier[act_attribute_name]
           )
        >> !(
                attribute_ro_raises_spec
              |
                +(
                      COMMA
                   >> assertion ("attribute name expected")
                      (
                        simple_identifier[act_attribute_name]
                      )
                 )
           )
        ;

      attribute_rw_decl_trailer =
           assertion ("attribute type name expected")
           (
             identifier[act_attribute_type]
           )
        >> assertion ("attribute name expected",
                      DiagnosticType::BEFORE)
           (
             simple_identifier[act_attribute_name]
           )
        >> !(
                attribute_rw_raises_spec
              |
                +(
                      COMMA
                   >> assertion ("attribute name expected")
                      (
                        simple_identifier[act_attribute_name]
                      )
                 )
           )
        ;

      attribute_ro_raises_spec =
           RAISES
        >> attribute_get_raises_list;


      attribute_rw_raises_spec =
          (
               (GETRAISES >> attribute_get_raises_list)
            >> !(SETRAISES >> attribute_set_raises_list)
          )
        |
          (SETRAISES >> attribute_set_raises_list)
        ;

      attribute_get_raises_list =
           assertion ("'(' expected")
           (
             LPAREN
           )
        >> assertion ("exception name expected")
           (
             identifier[act_attribute_get_raises]
           )
        >> *(
                 COMMA
              >> assertion ("exception name expected")
                 (
                   identifier[act_attribute_get_raises]
                 )
            )
        >> assertion ("',' or ')' expected",
                      DiagnosticType::BEFORE)
           (
             RPAREN
           )
        ;

      attribute_set_raises_list =
           assertion ("'(' expected")
           (
             LPAREN
           )
        >> assertion ("exception name expected")
           (
             identifier[act_attribute_set_raises]
           )
        >> *(
                 COMMA
              >> assertion ("exception name expected")
                 (
                   identifier[act_attribute_set_raises]
                 )
            )
        >> assertion ("',' or ')' expected",
                      DiagnosticType::BEFORE)
           (
             RPAREN
           )
        ;


      // exception
      //
      //
      exception_decl =
           EXCEPTION
        >> guard
           (
                assertion ("exception name expected")
                (
                  simple_identifier[act_exception_begin]
                )
             >> hood
                (
                     assertion ("'{' expected")
                     (
                       LCBRACE[act_exception_open_scope]
                     )
                  >> hood (exception_body)
                     [
                       handler (f.exception (),
                                &SemanticAction::Exception::close_scope)
                     ]
                  >> assertion ("member declaration or '}' expected",
                                f.exception (),
                                &SemanticAction::Exception::close_scope,
                                DiagnosticType::BEFORE)
                     (
                       RCBRACE[act_exception_close_scope]
                     )
                )
                [
                  handler (f.exception (),
                           &SemanticAction::Exception::end)
                ]
             >> assertion ("';' expected",
                           f.exception (),
                           &SemanticAction::Exception::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_exception_end]
                )
           )[error_handler]
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
        >> guard
           (
                hood
                (
                     assertion ("member name expected")
                     (
                       simple_identifier[act_member_name]
                     )
                  >> *(
                           COMMA
                        >> assertion ("member name expected")
                           (
                             simple_identifier[act_member_name]
                           )
                     )
                )
                [
                  handler (f.member (),
                           &SemanticAction::Member::end)
                ]
             >> assertion ("';' expected",
                           f.member (),
                           &SemanticAction::Member::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_member_end]
                )
           )[error_handler]
        ;

      // native
      //
      //
      native_decl =
           NATIVE
        >> guard
           (
                assertion ("native name expected")
                (
                  simple_identifier[act_native_name]
                )
             >> assertion ("';' expected",
                           f.native (),
                           &SemanticAction::Native::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_native_end]
                )
           )[error_handler]
        ;


      // operation
      //
      //
      operation_decl =
        guard
        (
             (
                 (
                      ONEWAY[act_operation_one_way]
                   >> assertion ("operation return type name expected")
                      (
                        identifier[act_operation_type]
                      )
                 )
               |
                 (
                   identifier[act_operation_two_way][act_operation_type]
                 )
             )
          >> operation_decl_trailer
        )[error_handler]
        ;


      operation_decl_trailer =
           assertion ("operation name expected",
                      DiagnosticType::BEFORE)
           (
             simple_identifier[act_operation_name]
           )
        >> assertion ("'(' expected")
           (
             LPAREN
           )
        >> operation_parameter_list
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
              >> operation_raises_list
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
        ;

      operation_parameter_list =
        *(
              operation_parameter
           >> *(
                    COMMA
                 >> assertion ("parameter declaration expected")
                    (
                      operation_parameter
                    )
               )
        )
        ;

      operation_parameter =
        (
              direction_specifier
           >> assertion ("parameter type name expected")
              (
                identifier
              )
           >> assertion ("parameter name expected",
                         DiagnosticType::BEFORE)
              (
                simple_identifier
              )
        )[act_operation_parameter]
        ;

      operation_raises_list =
           assertion ("exception name expected")
           (
             identifier[act_operation_raises]
           )
        >> *(
                 COMMA
              >> assertion ("exception name expected")
                 (
                   identifier[act_operation_raises]
                 )
            )
        ;


      // struct
      //
      //
      struct_decl =
           STRUCT
        >> guard
           (
             assertion ("struct declaration expected",
                        DiagnosticType::BEFORE)
             (
                 (
                      simple_identifier
                   >> SEMI
                 )[act_struct_begin_fwd][act_struct_end]
               |
                 (
                       (
                            simple_identifier
                         >> LCBRACE
                       )[act_struct_begin_def][act_struct_open_scope]
                    >> hood
                       (
                            hood
                            (
                              assertion ("member declaration expected",
                                         DiagnosticType::BEFORE)
                              (
                                struct_body
                              )
                            )
                            [
                              handler (f.struct_ (),
                                       &SemanticAction::Struct::close_scope)
                            ]
                         >> assertion ("member declaration or '}' expected",
                                       f.struct_ (),
                                       &SemanticAction::Struct::close_scope,
                                       DiagnosticType::BEFORE)
                            (
                              RCBRACE[act_struct_close_scope]
                            )
                       )
                       [
                         handler (f.struct_ (),
                                  &SemanticAction::Struct::end)
                       ]
                    >> assertion ("';' expected",
                                  f.struct_ (),
                                  &SemanticAction::Struct::end,
                                  RecoveryMethod::NONE)
                       (
                         SEMI[act_struct_end]
                       )
                 )
             )
           )[error_handler]
        ;

      struct_body =
        +member_decl
        ;


      // typedef
      //
      //

      typedef_decl =
           TYPEDEF[act_typedef_pre]
        >> guard
           (
                assertion ("type name expected")
                (
                  typedef_type_spec
                )
             >> typedef_declarator
             >> *(COMMA >> typedef_declarator)
             >> assertion ("';' expected",
                           f.typedef_ (),
                           &SemanticAction::Typedef::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_typedef_end]
                )
           )[error_handler]
        ;

      typedef_declarator =
        (
             assertion ("typedef name expected",
                        f.typedef_ (),
                        &SemanticAction::Typedef::end)
             (
               simple_identifier
             )
          >> !(    LSBRACE[act_typedef_begin_array][act_const_expr_flush]
                >> assertion ("const expression expected",
                              f.typedef_ (),
                              &SemanticAction::Typedef::end)
                   (
                     numeric_const_expr[act_typedef_bound]
                   )
                >> assertion ("']' expected",
                              f.typedef_ (),
                              &SemanticAction::Typedef::end)
                   (
                     RSBRACE
                   )
              )
          >> *(    LSBRACE[act_const_expr_flush]
                >> assertion ("const expression expected",
                              f.typedef_ (),
                              &SemanticAction::Typedef::end)
                   (
                     numeric_const_expr[act_typedef_bound]
                   )
                >> assertion ("']' expected",
                              f.typedef_ (),
                              &SemanticAction::Typedef::end)
                   (
                     RSBRACE
                   )
              )
        )[act_typedef_declarator]
        ;

      typedef_type_spec =
          identifier[act_typedef_begin]
        |
          (
               SEQUENCE
            >> assertion ("'<' expected")
               (
                 LT
               )
            >> (
                   (identifier >> GT)[act_typedef_begin_unbounded_seq]
                 |
                   (
                        assertion ("sequence type name expected")
                        (
                          identifier[act_typedef_begin_bounded_seq]
                        )
                     >> assertion ("'>' or ',' expected",
                                   f.typedef_ (),
                                   &SemanticAction::Typedef::end)
                        (
                          COMMA[act_const_expr_flush] // flush expression stacks
                        )
                     >> assertion ("const expression expected",
                                   f.typedef_ (),
                                   &SemanticAction::Typedef::end)
                        (
                          numeric_const_expr[act_typedef_bound]
                        )
                     >> assertion ("'>' expected",
                                   f.typedef_ (),
                                   &SemanticAction::Typedef::end)
                        (
                          GT
                        )
                   )
               )
          )
        |
          (
               STRING
            >> assertion ("'<' expected")
               (
                 LT[act_const_expr_flush] // flush expression stacks
               )
            >> assertion ("const expression expected")
               (
                 numeric_const_expr[act_typedef_begin_bounded_string]
               )
            >> assertion ("'>' expected",
                          f.typedef_ (),
                          &SemanticAction::Typedef::end)
               (
                 GT
               )
          )
        |
          (
               WSTRING
            >> assertion ("'<' expected")
               (
                 LT[act_const_expr_flush] // flush expression stacks
               )
            >> assertion ("const expression expected")
               (
                 numeric_const_expr[act_typedef_begin_bounded_wstring]
               )
            >> assertion ("'>' expected",
                          f.typedef_ (),
                          &SemanticAction::Typedef::end)
               (
                 GT
               )
          )
        ;

      // union
      //
      //

      union_decl =
           UNION
        >> guard
           (
             assertion ("union declaration expected",
                        DiagnosticType::BEFORE)
             (
                 (
                      simple_identifier
                   >> SEMI
                 )[act_union_begin_fwd][act_union_end]
               |
                 (
                      (
                           simple_identifier
                        >> SWITCH
                      )[act_union_begin_def]

                   >> hood (union_def_trailer)
                      [
                        handler (f.union_ (),
                                 &SemanticAction::Union::end)
                      ]
                   >> assertion ("';' expected",
                                 f.union_ (),
                                 &SemanticAction::Union::end,
                                 RecoveryMethod::NONE)
                       (
                         SEMI[act_union_end]
                       )
                 )
             )
           )[error_handler]
        ;

      union_def_trailer =
           assertion ("'(' expected")
           (
             LPAREN
           )
        >> assertion ("discriminator type name expected")
           (
             identifier[act_union_type]
           )
        >> assertion ("')' expected",
                      DiagnosticType::BEFORE)
           (
             RPAREN
           )
        >> assertion ("'{' expected")
           (
             LCBRACE[act_union_open_scope]
           )
        >> hood
           (
             assertion ("case statement expected")
             (
               union_body
             )
           )
           [
             handler (f.union_ (),
                      &SemanticAction::Union::close_scope)
           ]
        >> assertion ("case statement or \'}\' expected",
                      f.union_ (),
                      &SemanticAction::Union::close_scope,
                      DiagnosticType::BEFORE)
           (
             RCBRACE[act_union_close_scope]
           )
        ;


      union_body =
        +guard
         (
              +union_case_label
           >> assertion ("type name expected",
                         DiagnosticType::BEFORE)
              (
                identifier[act_union_member_type]
              )
           >> assertion ("member name expected")
              (
                simple_identifier[act_union_member_name]
              )
           >> assertion ("';' expected",
                         RecoveryMethod::NONE)
              (
                SEMI
              )
         )[error_handler]
        ;

      union_case_label =
          (
               CASE[act_const_expr_flush] // flush expression stacks
            >> assertion ("const expression expected")
               (
                 const_expr
               )
            >> assertion ("':' expected")
               (
                 COLON
               )
          )
        |
          (    DEFAULT
            >> assertion ("':' expected")
               (
                 COLON
               )
          )
        ;


      // valuetype
      //
      //
      abstract_value_type_decl =
        guard
        (
          assertion ("abstract valuetype declaration expected",
                     DiagnosticType::BEFORE)
          (
              (
                   simple_identifier
                >> SEMI
              )[act_value_type_begin_abstract_fwd][act_value_type_end]
            |
              (
                   (
                        simple_identifier
                     >> COLON
                   )[act_value_type_begin_abstract_def]

                >> hood
                   (
                        value_type_inheritance_spec
                     >> !(SUPPORTS >> value_type_supports_spec)
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_value_type_open_scope]
                        )
                     >> hood (value_type_body)
                        [
                          handler (f.value_type (),
                                   &SemanticAction::ValueType::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.value_type (),
                                   &SemanticAction::ValueType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_value_type_close_scope]
                        )
                   )
                   [
                     handler (f.value_type (),
                              &SemanticAction::ValueType::end)
                   ]

                >> assertion ("';' expected",
                              f.value_type (),
                              &SemanticAction::ValueType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_value_type_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> SUPPORTS
                   )[act_value_type_begin_abstract_def]

                >> hood
                   (
                        value_type_supports_spec
                     >> assertion ("'{' expected")
                        (
                          LCBRACE[act_value_type_open_scope]
                        )
                     >> hood (value_type_body)
                        [
                          handler (f.value_type (),
                                   &SemanticAction::ValueType::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.value_type (),
                                   &SemanticAction::ValueType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_value_type_close_scope]
                        )
                   )
                   [
                     handler (f.value_type (),
                              &SemanticAction::ValueType::end)
                   ]

                >> assertion ("';' expected",
                              f.value_type (),
                              &SemanticAction::ValueType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_value_type_end]
                   )
              )
            |
              (
                   (
                        simple_identifier
                     >> LCBRACE
                   )[act_value_type_begin_abstract_def][act_value_type_open_scope]

                >> hood
                   (
                        hood (value_type_body)
                        [
                          handler (f.value_type (),
                                   &SemanticAction::ValueType::close_scope)
                        ]
                     >> assertion ("declaration or '}' expected",
                                   f.value_type (),
                                   &SemanticAction::ValueType::close_scope,
                                   DiagnosticType::BEFORE)
                        (
                          RCBRACE[act_value_type_close_scope]
                        )
                   )
                   [
                     handler (f.value_type (),
                              &SemanticAction::ValueType::end)
                   ]

                >> assertion ("';' expected",
                              f.value_type (),
                              &SemanticAction::ValueType::end,
                              RecoveryMethod::NONE)
                   (
                     SEMI[act_value_type_end]
                   )
              )
          )
        )[error_handler]
        ;


      concrete_value_type_decl =
           VALUETYPE
        >> guard
           (
             assertion ("valuetype declaration expected",
                     DiagnosticType::BEFORE)
             (
                (
                     simple_identifier
                  >> SEMI
                )[act_value_type_begin_concrete_fwd][act_value_type_end]
              |
                (
                     (
                          simple_identifier
                       >> COLON
                     )[act_value_type_begin_concrete_def]

                  >> hood
                     (
                          value_type_inheritance_spec
                       >> !(SUPPORTS >> value_type_supports_spec)
                       >> assertion ("'{' expected")
                          (
                            LCBRACE[act_value_type_open_scope]
                          )
                       >> hood (value_type_body)
                          [
                            handler (f.value_type (),
                                     &SemanticAction::ValueType::close_scope)
                          ]
                       >> assertion ("declaration or '}' expected",
                                     f.value_type (),
                                     &SemanticAction::ValueType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_value_type_close_scope]
                          )
                     )
                     [
                       handler (f.value_type (),
                                &SemanticAction::ValueType::end)
                     ]

                  >> assertion ("';' expected",
                                f.value_type (),
                                &SemanticAction::ValueType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_value_type_end]
                     )
                )
              |
                (
                     (
                          simple_identifier
                       >> SUPPORTS
                     )[act_value_type_begin_concrete_def]

                  >> hood
                     (
                          value_type_supports_spec
                       >> assertion ("'{' expected")
                          (
                            LCBRACE[act_value_type_open_scope]
                          )
                       >> hood (value_type_body)
                          [
                            handler (f.value_type (),
                                     &SemanticAction::ValueType::close_scope)
                          ]
                       >> assertion ("declaration or '}' expected",
                                     f.value_type (),
                                     &SemanticAction::ValueType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_value_type_close_scope]
                          )
                     )
                     [
                       handler (f.value_type (),
                                &SemanticAction::ValueType::end)
                     ]

                  >> assertion ("';' expected",
                                f.value_type (),
                                &SemanticAction::ValueType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_value_type_end]
                     )
                )
              |
                (
                     (
                          simple_identifier
                       >> LCBRACE
                     )[act_value_type_begin_concrete_def][act_value_type_open_scope]

                  >> hood
                     (
                          hood (value_type_body)
                          [
                            handler (f.value_type (),
                                     &SemanticAction::ValueType::close_scope)
                          ]
                       >> assertion ("declaration or '}' expected",
                                     f.value_type (),
                                     &SemanticAction::ValueType::close_scope,
                                     DiagnosticType::BEFORE)
                          (
                            RCBRACE[act_value_type_close_scope]
                          )
                     )
                     [
                       handler (f.value_type (),
                                &SemanticAction::ValueType::end)
                     ]

                  >> assertion ("';' expected",
                                f.value_type (),
                                &SemanticAction::ValueType::end,
                                RecoveryMethod::NONE)
                     (
                       SEMI[act_value_type_end]
                     )
                )
            )
          )[error_handler]
        ;

      value_type_inheritance_spec =
        guard
        (
              assertion ("base valuetype name expected")
              (
                identifier[act_value_type_inherits]
              )
           >> *(
                    COMMA
                 >> assertion ("base valuetype name expected")
                    (
                      identifier[act_value_type_inherits]
                    )
               )
        )[error_handler]
        ;

      value_type_supports_spec =
        guard
        (
              assertion ("supported interface name expected")
              (
                identifier[act_value_type_supports]
              )
           >> *(
                    COMMA
                 >> assertion ("supported interface name expected")
                    (
                      identifier[act_value_type_supports]
                    )
               )
        )[error_handler]
        ;

      value_type_body =
        *(
             const_decl
           | type_decl
           | type_id_decl
           | type_prefix_decl

           | attribute_decl
           | operation_decl
           | value_type_member_decl
           | value_type_factory_decl
        )
        ;


      // valuetype factory
      //
      //
      value_type_factory_decl =
           FACTORY
        >> guard
           (
                assertion ("factory name expected")
                (
                  simple_identifier[act_value_type_factory_name]
                )
             >> assertion ("'(' expected")
                (
                  LPAREN
                )
             >> value_type_factory_parameter_list
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
                   >> value_type_factory_raises_list
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

      value_type_factory_parameter_list =
        *(
              value_type_factory_parameter
           >> *(COMMA >> value_type_factory_parameter)
        )
        ;

      value_type_factory_parameter =
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
             )[act_value_type_factory_parameter]
        )[error_handler]
        ;

      value_type_factory_raises_list =
        guard
        (
             assertion ("exception name expected")
             (
               identifier[act_value_type_factory_raises]
             )
          >> *(
                  COMMA
               >> assertion ("exception name expected")
                  (
                    identifier[act_value_type_factory_raises]
                  )
              )
        )[error_handler]
        ;


      // valuetype member
      //
      //
      value_type_member_decl =
           (
               PUBLIC[act_value_type_member_begin_public]
             | PRIVATE[act_value_type_member_begin_private]
           )
        >> guard
           (
                assertion ("type name expected")
                (
                  identifier[act_value_type_member_type]
                )
             >> assertion ("member name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_value_type_member_name]
                )
             >> *(
                      COMMA
                   >> assertion ("member name expected")
                      (
                        simple_identifier[act_value_type_member_name]
                      )
                 )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;
    }
  }
}
