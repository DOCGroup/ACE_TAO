// file      : CCF/CIDL/Parser.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/CIDL/Parser.hpp"

//
// Note: DO NOT run emacs indenter (or any other indentation tool) over
//       this file because it will most likely break BNF indentation.
//

namespace CCF
{
  namespace CIDL
  {
    Parser::
    Parser (CompilerElements::Context& context,
            Diagnostic::Stream& dout,
            LexicalAnalyzer const& l,
            SemanticAction::Factory& f)
        : IDL2::Parser (context, dout, l, f),
          IDL3::Parser (context, dout, l, f),
          lexer_ (l),
          actions_ (f),

          COMPOSITION ("composition"),
          ENTITY      ("entity"     ),
          EXECUTOR    ("executor"   ),
          IMPLEMENTS  ("implements" ),
          PROCESS     ("process"    ),
          SERVICE     ("service"    ),
          SESSION     ("session"    ),

          // Composition
          //
          act_composition_begin (
            this, &Parser::act_composition_begin_core),

          act_composition_open_scope (
            f.composition (), &SemanticAction::Scope::open_scope),

          act_composition_close_scope (
            f.composition (), &SemanticAction::Scope::close_scope),

          act_composition_end (
            f.composition (), &SemanticAction::Composition::end),


          // Home Executor
          //
          act_home_executor_begin (
            f.home_executor (), &SemanticAction::HomeExecutor::begin),

          act_home_executor_open_scope (
            f.home_executor (), &SemanticAction::Scope::open_scope),

          act_home_executor_implements (
            f.home_executor (), &SemanticAction::HomeExecutor::implements),

          act_home_executor_manages (
            f.home_executor (), &SemanticAction::HomeExecutor::manages),

          act_home_executor_close_scope (
            f.home_executor (), &SemanticAction::Scope::open_scope),

          act_home_executor_end (
            f.home_executor (), &SemanticAction::HomeExecutor::end)

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
  }
}
