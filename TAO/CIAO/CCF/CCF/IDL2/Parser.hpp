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
      Parsing::IdentifierParser
      IdentifierParser;

      typedef
      Parsing::SimpleIdentifierParser
      SimpleIdentifierParser;

      typedef
      Parsing::StringLiteralParser
      StringLiteralParser;


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
        operator() (Parsing::Scanner const& s, Parsing::Error& e) const
        {
          Iterator i = e.where;

          switch (e.descriptor.diagnostic_)
          {
          case Parsing::DiagnosticType::BEFORE:
            {
              Diagnostic::Error rec (
                context_.get<fs::path> ("file-path").native_file_string (),
                (*i)->line ());

              rec << "before \'" << (*i)->lexeme () << "\': "
                  << e.descriptor.description_;

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
                  << e.descriptor.description_;

              dout_ << rec;
              break;
            }
          case Parsing::DiagnosticType::NONE:
            {
            }
          }

          if (e.descriptor.action_one_.get ())
            e.descriptor.action_one_->execute ();

          if (e.descriptor.action_two_.get ())
            e.descriptor.action_two_->execute ();

          switch (e.descriptor.recovery_)
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

              switch (e.descriptor.diagnostic_)
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

              e.descriptor.recovery_ = Parsing::RecoveryMethod::BAIL_OUT;
              e.descriptor.diagnostic_ = Parsing::DiagnosticType::NONE;

              e.descriptor.action_one_.reset ();
              e.descriptor.action_two_.reset ();

              return RecoveryStatus (RecoveryStatus::rethrow);
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
              e.descriptor.recovery_ = Parsing::RecoveryMethod::BAIL_OUT;
              e.descriptor.diagnostic_ = Parsing::DiagnosticType::NONE;

              e.descriptor.action_one_.reset ();
              e.descriptor.action_two_.reset ();

              return RecoveryStatus (RecoveryStatus::rethrow);
            }
          }
        }

        Diagnostic::Stream& dout_;
        CompilerElements::Context& context_;
      };

      struct RootErrorHandler
      {
        RecoveryStatus
        operator() (Parsing::Scanner const& s, Parsing::Error& e) const
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
        operator() (Parsing::Scanner const& s, Parsing::Error& e) const
        {
          assert (
            e.descriptor.diagnostic_ == Parsing::DiagnosticType::NONE &&
            e.descriptor.recovery_ == Parsing::RecoveryMethod::BAIL_OUT
          );

          if (action_one_.get ()) action_one_->execute ();
          if (action_two_.get ()) action_two_->execute ();

          return RecoveryStatus (RecoveryStatus::rethrow);
        }

        std::auto_ptr<Parsing::Thunk> action_one_;
        std::auto_ptr<Parsing::Thunk> action_two_;
      };

      Guard guard;
      Guard hood;
      Assertion assertion;
      ErrorHandler error_handler;
      RootErrorHandler root_error_handler;

      // End of stream parser
      EndOfStreamParser EOS;

      // Keyword parsers (alphabetic order).
      KeywordParser ABSTRACT;
      KeywordParser ATTRIBUTE;
      KeywordParser FACTORY;
      KeywordParser IN;
      KeywordParser INCLUDE;
      KeywordParser INOUT;
      KeywordParser INTERFACE;
      KeywordParser LOCAL;
      KeywordParser MODULE;
      KeywordParser OUT;
      KeywordParser SINCLUDE;
      KeywordParser SUPPORTS;

      // Punctuation parsers (alphabetic group order).
      PunctuationParser COLON;
      PunctuationParser COMMA;
      PunctuationParser LBRACE;
      PunctuationParser RBRACE;
      PunctuationParser LPAREN;
      PunctuationParser RPAREN;
      PunctuationParser SEMI;

      IdentifierParser       identifier;
      SimpleIdentifierParser simple_identifier;
      StringLiteralParser    string_literal;


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
      Rule system_include_decl;

      Rule module_decl;

      Rule abstract_type_decl;
      Rule local_type_decl;

      Rule abstract_interface_decl;
      Rule local_interface_decl;
      Rule unconstrained_interface_decl;
      Rule interface_decl_trailer;
      Rule interface_inheritance_spec;
      Rule interface_body;

      Rule attribute_decl;

      Rule direction_specifier;

      Rule operation_decl;
      Rule operation_parameter_list;
      Rule operation_parameter;

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

      //
      // Include
      //
      OneArgAction<StringLiteralPtr, SemanticAction::Include>
      act_include_begin;

      NoArgAction<SemanticAction::Include>
      act_include_end;

      //
      // Module
      //

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Module>
      act_module_begin;

      ScopeAction
      act_module_open_scope;

      ScopeAction
      act_module_close_scope;

      NoArgAction<SemanticAction::Module>
      act_module_end;

      //
      // Interface
      //

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Interface>
      act_abstract_interface_begin;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Interface>
      act_local_interface_begin;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Interface>
      act_unconstrained_interface_begin;

      OneArgAction<IdentifierPtr, SemanticAction::Interface>
      act_interface_inherits;

      ScopeAction
      act_interface_open_scope;

      ScopeAction
      act_interface_close_scope;

      NoArgAction<SemanticAction::Interface>
      act_interface_end;

      //
      // Attribute
      //
      OneArgAction<IdentifierPtr, SemanticAction::Attribute>
      act_attribute_type;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::Attribute>
      act_attribute_name;

      //
      // Operation
      //
      TwoArgAction<IdentifierPtr,
                   SimpleIdentifierPtr,
                   SemanticAction::Operation>
      act_operation_begin;

      void
      act_operation_parameter_core (Iterator begin,
                                    Iterator end) const
      {
        SemanticAction::Operation::Direction::Value d;

        if((*begin)->lexeme () == "in")
        {
          d = SemanticAction::Operation::Direction::IN;
        }
        else if((*begin)->lexeme () == "out")
        {
          d = SemanticAction::Operation::Direction::OUT;
        }
        else
        {
          d = SemanticAction::Operation::Direction::INOUT;
        }

        begin++;

        actions_.operation ().parameter (
          d,
          ReferenceCounting::strict_cast<Identifier>(*begin),
          ReferenceCounting::strict_cast<SimpleIdentifier>(*(begin + 1)));
      }

      ActionExecutor<Parser>
      act_operation_parameter;

      NoArgAction<SemanticAction::Operation>
      act_operation_end;
    };
  }
}

#endif  // CCF_IDL2_PARSER_HPP
