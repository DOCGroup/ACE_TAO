// file      : CCF/IDL2/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_PARSER_HPP
#define CCF_IDL2_PARSER_HPP

#include "CCF/CompilerElements/Context.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CompilerElements/FileSystem.hpp"

#include "CCF/IDL2/SemanticAction.hpp"
#include "CCF/IDL2/LexicalAnalyzer.hpp"

#include "CCF/IDL2/Parsing/Elements.hpp"
#include "CCF/IDL2/Parsing/Action.hpp"
#include "CCF/IDL2/Parsing/Recovery.hpp"

// #$%^ing MS.
//
#ifdef CONST
#undef CONST
#endif
#ifdef FACTORY
#undef FACTORY
#endif
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif
#ifdef interface
#undef interface
#endif

namespace CCF
{
  namespace IDL2
  {
    using Parsing::ActionExecutor;
    using Parsing::NoArgAction;
    using Parsing::OneArgAction;
    using Parsing::TwoArgAction;

    class Parser
    {
    public:
      virtual
      ~Parser ();

    protected:
      Diagnostic::Stream& dout_;
      LexicalAnalyzer const& lexer_;
      SemanticAction::Factory& actions_;

    protected:
      typedef
      Parsing::Iterator
      Iterator;

      //
      // Primitives
      //

      typedef
      Parsing::EndOfStreamParser
      EndOfStreamParser;

      typedef
      Parsing::KeywordParser
      KeywordParser;

      typedef
      Parsing::PunctuationParser
      PunctuationParser;

      typedef
      Parsing::OperatorParser
      OperatorParser;

      typedef
      Parsing::IdentifierParser
      IdentifierParser;

      typedef
      Parsing::SimpleIdentifierParser
      SimpleIdentifierParser;


      typedef
      Parsing::RecoveryDescriptor
      RecoveryDescriptor;

      typedef
      Parsing::Assertion
      Assertion;

      typedef
      Parsing::RecoveryStatus
      RecoveryStatus;

      typedef
      Parsing::Guard
      Guard;

      //@@ names are not thought out weel for error handling

      struct ErrorHandler
      {
        ErrorHandler (CompilerElements::Context& context,
                      Diagnostic::Stream& dout)
            : dout_ (dout),
              context_ (context)
        {
        }

        RecoveryStatus
        operator() (Parsing::Scanner const& s, Parsing::Error e) const
        {
          Iterator i = e.where;

          switch (e.descriptor->diagnostic_)
          {
          case Parsing::DiagnosticType::BEFORE:
            {
              Diagnostic::Error rec (
                context_.get<fs::path> ("file-path").native_file_string (),
                (*i)->line ());

              rec << "before \'" << (*i)->lexeme () << "\': "
                  << e.descriptor->description_;

              dout_ << rec;
              break;
            }
          case Parsing::DiagnosticType::AFTER:
            {
              i--;

              Diagnostic::Error rec (
                context_.get<fs::path> ("file-path").native_file_string (),
                (*i)->line ());

              rec << "after \'" << (*i)->lexeme () << "\': "
                  << e.descriptor->description_;

              dout_ << rec;
              break;
            }
          case Parsing::DiagnosticType::NONE:
            {
            }
          }

          if (e.descriptor->action_one_.get ())
            e.descriptor->action_one_->execute ();

          if (e.descriptor->action_two_.get ())
            e.descriptor->action_two_->execute ();

          switch (e.descriptor->recovery_)
          {
          case Parsing::RecoveryMethod::STANDARD:
            {
              unsigned long balance = 0;

              for (Iterator t = e.where; t != s.last; t++)
              {
                //std::cerr << "error recovery: skipping " << (*t)->lexeme ()
                //          << std::endl;

                if ((*t)->lexeme () == "{") balance++;
                else if ((*t)->lexeme () == "}")
                {
                  if (balance > 0) balance--;
                  else
                  {
                    t++;
                    if ((*t)->lexeme () == ";") t++;

                    int length = t - s.first;
                    s.first += length;
                    return RecoveryStatus (RecoveryStatus::accept, length);
                  }
                }

                if (balance == 0 && (*t)->lexeme () == ";")
                {
                  int length = t - s.first + 1;
                  s.first += length;
                  return RecoveryStatus (RecoveryStatus::accept, length);
                }
              }

              switch (e.descriptor->diagnostic_)
              {
              case Parsing::DiagnosticType::BEFORE:
                {
                  Diagnostic::Error rec (
                    context_.get<fs::path> ("file-path").native_file_string (),
                    (*i)->line ());

                  rec << "before \'" << (*i)->lexeme () << "\': "
                      << "unable to recover from previous error: bailing out";

                  dout_ << rec;
                  break;
                }
              case Parsing::DiagnosticType::AFTER:
                {
                  Diagnostic::Error rec (
                    context_.get<fs::path> ("file-path").native_file_string (),
                    (*i)->line ());

                  rec << "after \'" << (*i)->lexeme () << "\': "
                      << "unable to recover from previous error: bailing out";

                  dout_ << rec;
                  break;
                }
              case Parsing::DiagnosticType::NONE:
                {
                }
              }

              e.descriptor->recovery_ = Parsing::RecoveryMethod::BAIL_OUT;
              e.descriptor->diagnostic_ = Parsing::DiagnosticType::NONE;

              e.descriptor->action_one_.reset ();
              e.descriptor->action_two_.reset ();

              // return RecoveryStatus (RecoveryStatus::rethrow);
              throw e;
            }
          case Parsing::RecoveryMethod::NONE:
          default:
            {
              int length = e.where - s.first;
              s.first += length;
              return RecoveryStatus (RecoveryStatus::accept, length);
            }
          case Parsing::RecoveryMethod::BAIL_OUT:
            {
              e.descriptor->recovery_ = Parsing::RecoveryMethod::BAIL_OUT;
              e.descriptor->diagnostic_ = Parsing::DiagnosticType::NONE;

              e.descriptor->action_one_.reset ();
              e.descriptor->action_two_.reset ();

              // return RecoveryStatus (RecoveryStatus::rethrow);
              throw e;
            }
          }
        }

        Diagnostic::Stream& dout_;
        CompilerElements::Context& context_;
      };

      struct RootErrorHandler
      {
        RecoveryStatus
        operator() (Parsing::Scanner const&, Parsing::Error&) const
        {
          return RecoveryStatus (RecoveryStatus::fail);
        }
      };

      struct handler
      {
        template<typename Object>
        handler (Object& obj,
                 void (Object::*action_one)())
            : action_one_ (Parsing::call_thunk (obj, action_one)),
              action_two_ (0)
        {
        }

        template<typename Object>
        handler (Object& obj,
                 void (Object::*action_one)(),
                 void (Object::*action_two)())
            : action_one_ (Parsing::call_thunk (obj, action_one)),
              action_two_ (Parsing::call_thunk (obj, action_two))
        {
        }

        handler (handler const& h)
            : action_one_ (h.action_one_.get () ? h.action_one_->clone () : 0),
              action_two_ (h.action_two_.get () ? h.action_two_->clone () : 0)
        {
        }

        RecoveryStatus
        operator() (Parsing::Scanner const&, Parsing::Error e) const
        {
          /*
          assert (
            e.descriptor->diagnostic_ == Parsing::DiagnosticType::NONE &&
            e.descriptor->recovery_ == Parsing::RecoveryMethod::BAIL_OUT
          );
          */

          if (action_one_.get ()) action_one_->execute ();
          if (action_two_.get ()) action_two_->execute ();

          // return RecoveryStatus (RecoveryStatus::rethrow);
          throw e;
        }

        std::auto_ptr<Parsing::Thunk> action_one_;
        std::auto_ptr<Parsing::Thunk> action_two_;
      };

      Guard guard;
      Guard hood;
      Assertion assertion;
      ErrorHandler error_handler;
      RootErrorHandler root_error_handler;

      // End of stream parser.
      //
      EndOfStreamParser EOS;

      // Keyword parsers (alphabetic order).
      //
      KeywordParser ABSTRACT;
      KeywordParser ATTRIBUTE;
      KeywordParser BINCLUDE;
      KeywordParser CASE;
      KeywordParser CONST;
      KeywordParser CUSTOM;
      KeywordParser DEFAULT;
      KeywordParser ENUM;
      KeywordParser EXCEPTION;
      KeywordParser FACTORY;
      KeywordParser GETRAISES;
      KeywordParser IN;
      KeywordParser INOUT;
      KeywordParser INTERFACE;
      KeywordParser QINCLUDE;
      KeywordParser LOCAL;
      KeywordParser MODULE;
      KeywordParser NATIVE;
      KeywordParser ONEWAY;
      KeywordParser OUT;
      KeywordParser PRIVATE;
      KeywordParser PUBLIC;
      KeywordParser RAISES;
      KeywordParser READONLY;
      KeywordParser SEQUENCE;
      KeywordParser SETRAISES;
      KeywordParser STRING;
      KeywordParser STRUCT;
      KeywordParser SUPPORTS;
      KeywordParser SWITCH;
      KeywordParser TRUNCATABLE;
      KeywordParser TYPEDEF;
      KeywordParser TYPEID;
      KeywordParser TYPEPREFIX;
      KeywordParser UNION;
      KeywordParser VALUETYPE;
      KeywordParser WSTRING;

      // Punctuation parsers (alphabetic group order).
      //
      PunctuationParser COLON;
      PunctuationParser COMMA;
      PunctuationParser LCBRACE;
      PunctuationParser RCBRACE;
      PunctuationParser LSBRACE;
      PunctuationParser RSBRACE;
      PunctuationParser LPAREN;
      PunctuationParser RPAREN;
      PunctuationParser LT;
      PunctuationParser GT;
      PunctuationParser SEMI;

      OperatorParser ADD;
      OperatorParser AND;
      OperatorParser COM;
      OperatorParser DIV;
      OperatorParser EQ;
      OperatorParser LSH;
      OperatorParser MUL;
      OperatorParser OR;
      OperatorParser REM;
      OperatorParser SUB;
      OperatorParser RSH;
      OperatorParser XOR;


      IdentifierParser       identifier;
      SimpleIdentifierParser simple_identifier;

      Parsing::BooleanLiteralParser   boolean_literal;
      Parsing::CharacterLiteralParser character_literal;
      Parsing::IntegerLiteralParser   integer_literal;
      Parsing::StringLiteralParser    string_literal;

      //
      // Language
      //
      typedef
      Parsing::Rule
      Rule;

      Rule language;
      Rule import;
      Rule declaration;
      Rule extension;

      Rule include_decl;

      Rule type_decl;

      Rule module_decl;

      Rule const_decl;
      Rule const_expr;

      Rule enum_decl;
      Rule enumerator_decl;

      Rule boolean_const_expr;
      Rule character_const_expr;
      Rule numeric_const_expr;

      Rule numeric_or_expr;
      Rule numeric_xor_expr;
      Rule numeric_and_expr;
      Rule numeric_shift_expr;
      Rule numeric_add_expr;
      Rule numeric_mul_expr;
      Rule numeric_unary_expr;
      Rule numeric_primary_expr;

      Rule string_const_expr;

      Rule abstract_type_decl;
      Rule local_type_decl;

      Rule abstract_interface_decl;
      Rule local_interface_decl;
      Rule unconstrained_interface_decl;
      Rule interface_inheritance_spec;
      Rule interface_body;

      Rule attribute_decl;
      Rule attribute_ro_decl_trailer;
      Rule attribute_rw_decl_trailer;
      Rule attribute_ro_raises_spec;
      Rule attribute_rw_raises_spec;
      Rule attribute_get_raises_list;
      Rule attribute_set_raises_list;

      Rule exception_decl;
      Rule exception_body;

      Rule direction_specifier;

      Rule member_decl;

      Rule native_decl;

      Rule operation_decl;
      Rule operation_decl_trailer;
      Rule operation_parameter_list;
      Rule operation_parameter;
      Rule operation_raises_list;

      Rule struct_decl;
      Rule struct_body;

      Rule typedef_decl;
      Rule typedef_declarator;
      Rule typedef_type_spec;

      Rule type_id_decl;
      Rule type_prefix_decl;

      Rule union_decl;
      Rule union_def_trailer;
      Rule union_body;
      Rule union_case_label;

      Rule abstract_value_type_decl;
      Rule concrete_value_type_decl;
      Rule value_type_inheritance_spec;
      Rule value_type_supports_spec;
      Rule value_type_body;

      Rule value_type_factory_decl;
      Rule value_type_factory_parameter_list;
      Rule value_type_factory_parameter;
      Rule value_type_factory_raises_list;

      Rule value_type_member_decl;
    public:
      Parser (CompilerElements::Context& context,
              Diagnostic::Stream& dout,
              LexicalAnalyzer const& l,
              SemanticAction::Factory& f);

      virtual Rule const&
      start() const
      {
        return language;
      }

    protected:

      //
      // Semantic actions
      //
      typedef
      NoArgAction<SemanticAction::Scope>
      ScopeAction;


      // Attribute
      //
      //
      NoArgAction<SemanticAction::Attribute>
      act_attribute_begin_ro, act_attribute_begin_rw;

      OneArgAction<IdentifierPtr, SemanticAction::Attribute>
      act_attribute_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Attribute>
      act_attribute_name;

      OneArgAction<IdentifierPtr, SemanticAction::Attribute>
      act_attribute_get_raises;

      OneArgAction<IdentifierPtr, SemanticAction::Attribute>
      act_attribute_set_raises;

      NoArgAction<SemanticAction::Attribute>
      act_attribute_end;

      // Const
      //
      TwoArgAction<IdentifierPtr, SimpleIdentifierPtr, SemanticAction::Const>
      act_const_begin;

      NoArgAction<SemanticAction::Const>
      act_const_expr;


      // Enum
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Enum>
      act_enum_begin, act_enum_enumerator;

      NoArgAction<SemanticAction::Enum>
      act_enum_end;

      // Expression
      //
      NoArgAction<SemanticAction::NumericExpression>
      act_const_expr_flush;

      OneArgAction<IdentifierPtr, SemanticAction::NumericExpression>
      act_numeric_expression_const;

      OneArgAction<IntegerLiteralPtr, SemanticAction::NumericExpression>
      act_numeric_expression_integer_literal;

      OneArgAction<OperatorPtr, SemanticAction::NumericExpression>
      act_numeric_expression_pre;

      NoArgAction<SemanticAction::NumericExpression>
        act_numeric_expression_pos,
        act_numeric_expression_neg,
        act_numeric_expression_com,
        act_numeric_expression_mul,
        act_numeric_expression_div,
        act_numeric_expression_rem,
        act_numeric_expression_add,
        act_numeric_expression_sub,
        act_numeric_expression_rsh,
        act_numeric_expression_lsh,
        act_numeric_expression_and,
        act_numeric_expression_xor,
        act_numeric_expression_or;


      // Exception
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Exception>
      act_exception_begin;

      ScopeAction
      act_exception_open_scope;

      ScopeAction
      act_exception_close_scope;

      NoArgAction<SemanticAction::Exception>
      act_exception_end;


      // Include
      //
      //
      OneArgAction<StringLiteralPtr, SemanticAction::Include>
      act_include_quote, act_include_bracket;

      NoArgAction<SemanticAction::Include>
      act_include_end;

      // Interface
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Interface>
        act_interface_begin_abstract_def,
        act_interface_begin_abstract_fwd,
        act_interface_begin_local_def,
        act_interface_begin_local_fwd,
        act_interface_begin_unconstrained_def,
        act_interface_begin_unconstrained_fwd;

      OneArgAction<IdentifierPtr, SemanticAction::Interface>
      act_interface_inherits;

      ScopeAction
      act_interface_open_scope;

      ScopeAction
      act_interface_close_scope;

      NoArgAction<SemanticAction::Interface>
      act_interface_end;


      // Member
      //
      //
      OneArgAction<IdentifierPtr, SemanticAction::Member>
      act_member_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Member>
      act_member_name;

      NoArgAction<SemanticAction::Member>
      act_member_end;


      // Module
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Module>
      act_module_begin;

      ScopeAction
      act_module_open_scope;

      ScopeAction
      act_module_close_scope;

      NoArgAction<SemanticAction::Module>
      act_module_end;

      // Native
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Native>
      act_native_name;

      NoArgAction<SemanticAction::Native>
      act_native_end;


      // Operation
      //
      //
      NoArgAction<SemanticAction::Operation>
      act_operation_one_way, act_operation_two_way;

      OneArgAction<IdentifierPtr, SemanticAction::Operation>
      act_operation_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Operation>
      act_operation_name;

      void
      act_operation_parameter_core (Iterator begin, Iterator) const
      {
        SemanticAction::Operation::Direction::Value d;

        if((*begin)->lexeme () == "in")
        {
          d = SemanticAction::Operation::Direction::in;
        }
        else if((*begin)->lexeme () == "out")
        {
          d = SemanticAction::Operation::Direction::out;
        }
        else
        {
          d = SemanticAction::Operation::Direction::inout;
        }

        begin++;

        actions_.operation ().parameter (
          d,
          ReferenceCounting::strict_cast<Identifier>(*begin),
          ReferenceCounting::strict_cast<SimpleIdentifier>(*(begin + 1)));
      }

      ActionExecutor<Parser>
      act_operation_parameter;

      OneArgAction<IdentifierPtr, SemanticAction::Operation>
      act_operation_raises;


      // Struct
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Struct>
      act_struct_begin_def;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Struct>
      act_struct_begin_fwd;

      ScopeAction
      act_struct_open_scope;

      ScopeAction
      act_struct_close_scope;

      NoArgAction<SemanticAction::Struct>
      act_struct_end;


      // Typedef
      //
      //
      OneArgAction<KeywordPtr, SemanticAction::Typedef>
      act_typedef_pre;

      OneArgAction<IdentifierPtr, SemanticAction::Typedef>
      act_typedef_begin;

      OneArgAction<IdentifierPtr, SemanticAction::Typedef>
      act_typedef_begin_unbounded_seq;

      OneArgAction<IdentifierPtr, SemanticAction::Typedef>
      act_typedef_begin_bounded_seq;

      NoArgAction<SemanticAction::Typedef>
      act_typedef_begin_bounded_string;

      NoArgAction<SemanticAction::Typedef>
      act_typedef_begin_bounded_wstring;

      NoArgAction<SemanticAction::Typedef>
      act_typedef_begin_array;

      NoArgAction<SemanticAction::Typedef>
      act_typedef_bound;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Typedef>
      act_typedef_declarator;

      NoArgAction<SemanticAction::Typedef>
      act_typedef_end;


      // TypeId
      //
      //
      TwoArgAction<IdentifierPtr,
                   StringLiteralPtr,
                   SemanticAction::TypeId>
      act_type_id_begin;

      NoArgAction<SemanticAction::TypeId>
      act_type_id_end;


      // TypePrefix
      //
      //
      TwoArgAction<IdentifierPtr,
                   StringLiteralPtr,
                   SemanticAction::TypePrefix>
      act_type_prefix_begin;

      NoArgAction<SemanticAction::TypePrefix>
      act_type_prefix_end;

      // Union
      //
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::Union>
      act_union_begin_def, act_union_begin_fwd;

      OneArgAction<IdentifierPtr, SemanticAction::Union>
      act_union_type;

      ScopeAction
      act_union_open_scope;

      OneArgAction<IdentifierPtr, SemanticAction::Union>
      act_union_member_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Union>
      act_union_member_name;

      ScopeAction
      act_union_close_scope;

      NoArgAction<SemanticAction::Union>
      act_union_end;


      // ValueType
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::ValueType>
        act_value_type_begin_abstract_def,
        act_value_type_begin_abstract_fwd,
        act_value_type_begin_concrete_def,
        act_value_type_begin_concrete_fwd;

      OneArgAction<IdentifierPtr, SemanticAction::ValueType>
      act_value_type_inherits, act_value_type_supports;

      ScopeAction
      act_value_type_open_scope;

      ScopeAction
      act_value_type_close_scope;

      NoArgAction<SemanticAction::ValueType>
      act_value_type_end;


      // ValueTypeFactory
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::ValueTypeFactory>
      act_value_type_factory_name;

      TwoArgAction<IdentifierPtr,
                   SimpleIdentifierPtr,
                   SemanticAction::ValueTypeFactory>
      act_value_type_factory_parameter;

      OneArgAction<IdentifierPtr, SemanticAction::ValueTypeFactory>
      act_value_type_factory_raises;


      // ValueTypeMember
      //
      NoArgAction<SemanticAction::ValueTypeMember>
      act_value_type_member_begin_private;

      NoArgAction<SemanticAction::ValueTypeMember>
      act_value_type_member_begin_public;

      OneArgAction<IdentifierPtr, SemanticAction::ValueTypeMember>
      act_value_type_member_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::ValueTypeMember>
      act_value_type_member_name;

      NoArgAction<SemanticAction::ValueTypeMember>
      act_value_type_member_end;

    };
  }
}

#endif  // CCF_IDL2_PARSER_HPP
