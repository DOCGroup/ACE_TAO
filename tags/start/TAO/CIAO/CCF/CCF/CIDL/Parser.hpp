// file      : CCF/CIDL/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_CIDL_PARSER_HPP
#define CCF_CIDL_PARSER_HPP

#include "CCF/CompilerElements/Context.hpp"

#include "CCF/IDL3/Parser.hpp"

#include "CCF/CIDL/LexicalAnalyzer.hpp"
#include "CCF/CIDL/SemanticAction.hpp"

namespace CCF
{
  namespace CIDL
  {
    using IDL3::ActionExecutor;
    using IDL3::NoArgAction;
    using IDL3::OneArgAction;
    using IDL3::TwoArgAction;

    class Parser : public virtual IDL3::Parser
    {
    protected:
      LexicalAnalyzer const& lexer_;
      SemanticAction::Factory& actions_;

      //
      // Primitives (alphabetic order).
      //

      KeywordParser COMPOSITION;
      KeywordParser ENTITY;
      KeywordParser EXECUTOR;
      KeywordParser IMPLEMENTS;
      KeywordParser PROCESS;
      KeywordParser SERVICE;
      KeywordParser SESSION;

      //
      // Language
      //

      Rule extension;

      // Composition
      Rule composition_decl;
      Rule composition_header;
      Rule composition_category;

      // Home executor
      Rule home_executor_decl;
      Rule home_executor_header;
      Rule home_executor_body;
      Rule home_executor_home_impl_decl;
      Rule home_executor_executor_decl;

    public:
      Parser (CompilerElements::Context& context,
              Diagnostic::Stream& dout,
              LexicalAnalyzer const& l,
              SemanticAction::Factory& f);

    protected:
      void
      act_composition_begin_core (Iterator begin, Iterator end) const
      {
        SemanticAction::Composition::Category::Value c;

        if((*begin)->lexeme () == "entity")
        {
          c = SemanticAction::Composition::Category::ENTITY;
        }
        else if ((*begin)->lexeme () == "process")
        {
          c = SemanticAction::Composition::Category::PROCESS;
        }
        else if ((*begin)->lexeme () == "service")
        {
          c = SemanticAction::Composition::Category::SERVICE;
        }
        else
        {
          c = SemanticAction::Composition::Category::SESSION;
        }

        begin++;

        actions_.composition ().begin (
          ReferenceCounting::strict_cast<SimpleIdentifier> (*begin), c);
      }

      // Composition
      //
      ActionExecutor<Parser>
      act_composition_begin;

      ScopeAction
      act_composition_open_scope;

      ScopeAction
      act_composition_close_scope;

      NoArgAction<SemanticAction::Composition>
      act_composition_end;

      // Home Executor
      //
      OneArgAction<SimpleIdentifierPtr, SemanticAction::HomeExecutor>
      act_home_executor_begin;

      ScopeAction
      act_home_executor_open_scope;

      OneArgAction<IdentifierPtr, SemanticAction::HomeExecutor>
      act_home_executor_implements;

      OneArgAction<SimpleIdentifierPtr, SemanticAction::HomeExecutor>
      act_home_executor_manages;

      ScopeAction
      act_home_executor_close_scope;

      NoArgAction<SemanticAction::HomeExecutor>
      act_home_executor_end;
    };
  }
}

#endif  // CCF_CIDL_PARSER_HPP
