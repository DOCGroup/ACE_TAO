// $Id$
#ifndef CCF_CIDL_PARSER_HPP
#define CCF_CIDL_PARSER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_Parser.hpp"
#include "CCF/CIDL/CIDL_LexicalAnalyzer.hpp"
#include "CCF/CIDL/CIDL_SemanticAction.hpp"

namespace CIDL
{
  class Parser : public virtual IDL3::Parser
  {
  public:
    virtual
    ~Parser () throw () {}

  protected:

    //
    // Semanic action types
    //
    typedef
    SemanticAction::Composition
    Composition;

    typedef
    SemanticAction::HomeExecutor
    HomeExecutor;

  protected:
    LexicalAnalyzer const& lexer_;
    SemanticActionFactory& actions_;

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

    Parser (LexicalAnalyzer const& l, SemanticActionFactory& f)
        : IDL2::Parser (l, f),
          IDL3::Parser (l, f),
          lexer_ (l),
          actions_ (f),

          COMPOSITION (l.composition),
          ENTITY      (l.entity),
          EXECUTOR    (l.executor),
          IMPLEMENTS  (l.implements),
          PROCESS     (l.process),
          SERVICE     (l.service),
          SESSION     (l.session),

          act_composition_begin (this, &Parser::act_composition_begin_core),
          act_composition_open_scope (f.composition (), &Scope::open_scope),
          act_composition_close_scope (f.composition (), &Scope::close_scope),
          act_composition_end (f.composition (), &Composition::end),

          act_home_executor_begin        (f.home_executor (),
                                          &HomeExecutor::begin),
          act_home_executor_open_scope   (f.home_executor (),
                                          &Scope::open_scope),
          act_home_executor_implements   (f.home_executor (),
                                          &HomeExecutor::implements),
          act_home_executor_manages      (f.home_executor (),
                                          &HomeExecutor::manages),
          act_home_executor_close_scope  (f.home_executor (),
                                         &Scope::open_scope),
          act_home_executor_end          (f.home_executor (),
                                          &HomeExecutor::end)

    {
      IDL3::Parser::extension =
           composition_decl
        |  extension
        ;

      //
      // Composition
      //
      composition_decl =
           composition_header
        >> LBRACE[act_composition_open_scope]
        >> home_executor_decl
        >> RBRACE[act_composition_close_scope]
        >> SEMI[act_composition_end]
        ;

      composition_header =
           COMPOSITION
        >> (composition_category >> simple_identifier)[act_composition_begin]
        ;

      composition_category =
          ENTITY
        | PROCESS
        | SERVICE
        | SESSION
        ;

      //
      // Home executor
      //
      home_executor_decl =
           home_executor_header
        >> LBRACE[act_home_executor_open_scope]
        >> home_executor_home_impl_decl
        >> home_executor_executor_decl
        >> RBRACE[act_home_executor_close_scope]
        >> SEMI[act_home_executor_end]
        ;

      home_executor_header =
           HOME
        >> EXECUTOR
        >> simple_identifier[act_home_executor_begin]
        ;

      home_executor_home_impl_decl =
           IMPLEMENTS
        >> identifier[act_home_executor_implements]
        >> SEMI
        ;

      home_executor_executor_decl =
           MANAGES
        >> simple_identifier[act_home_executor_manages]
        >> SEMI
        ;
    }

  protected:
    void
    act_composition_begin_core (Iterator begin, Iterator end) const
    {
      Composition::Category::Value c;

      if(identical (lexer_.entity, *begin))
      {
        c = Composition::Category::ENTITY;
      }
      else if (identical (lexer_.process, *begin))
      {
        c = Composition::Category::PROCESS;
      }
      else if (identical (lexer_.service, *begin))
      {
        c = Composition::Category::SERVICE;
      }
      else
      {
        c = Composition::Category::SESSION;
      }

      begin++;

      actions_.composition ().begin (typical<SimpleIdentifierPtr>(*begin), c);
    }

    ActionExecutor<Parser> act_composition_begin;
    ScopeAction act_composition_open_scope;
    ScopeAction act_composition_close_scope;
    NoArgAction<Composition> act_composition_end;

    OneArgAction<SimpleIdentifierPtr, HomeExecutor> act_home_executor_begin;
    ScopeAction act_home_executor_open_scope;
    OneArgAction<IdentifierPtr, HomeExecutor> act_home_executor_implements;
    OneArgAction<SimpleIdentifierPtr, HomeExecutor> act_home_executor_manages;
    ScopeAction act_home_executor_close_scope;
    NoArgAction<HomeExecutor> act_home_executor_end;
  };
}

#endif // CCF_CIDL_PARSER_HPP
