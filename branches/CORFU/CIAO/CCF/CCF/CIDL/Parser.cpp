// file      : CCF/CIDL/Parser.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/Parser.hpp"

//
// Note: DO NOT run emacs indenter (or any other indentation tool) over
//       this file because it will most likely break BNF indentation.
//

namespace CCF
{
  namespace CIDL
  {
    using  IDL2::Parsing::DiagnosticType;
    using  IDL2::Parsing::RecoveryMethod;

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

          act_home_executor_implements (
            f.home_executor (), &SemanticAction::HomeExecutor::implements),

          act_home_executor_manages (
            f.home_executor (), &SemanticAction::HomeExecutor::manages),

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
           COMPOSITION
        >> guard
           (
                (
                     assertion ("composition category expected",
                                DiagnosticType::BEFORE)
                     (
                       composition_category
                     )
                  >> assertion ("composition name expected",
                                DiagnosticType::BEFORE)
                     (
                       simple_identifier
                     )
                )[act_composition_begin]

             >> assertion ("'{' expected",
                           f.composition (),
                           &SemanticAction::Composition::end)
                (
                  LCBRACE[act_composition_open_scope]
                )

             >> assertion ("home executor declaration expected",
                           f.composition (),
                           &SemanticAction::Composition::close_scope,
                           &SemanticAction::Composition::end,
                           DiagnosticType::BEFORE)
                (
                  hood (home_executor_decl)
                  [
                    handler (f.composition (),
                             &SemanticAction::Composition::close_scope,
                             &SemanticAction::Composition::end)
                  ]
                )

             >> assertion ("'}' expected",
                           f.composition (),
                           &SemanticAction::Composition::close_scope,
                           &SemanticAction::Composition::end,
                           DiagnosticType::BEFORE)
                (
                  RCBRACE[act_composition_close_scope]
                )

             >> assertion ("';' expected",
                           f.composition (),
                           &SemanticAction::Composition::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_composition_end]
                )

           )[error_handler]
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
           HOME
        >> EXECUTOR
        >> guard
           (
                assertion ("home executor name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_home_executor_begin]
                )

             >> assertion ("'{' expected",
                           f.home_executor (),
                           &SemanticAction::HomeExecutor::end)
                (
                  LCBRACE
                )

             >> assertion ("implements declaration expected",
                           f.home_executor (),
                           &SemanticAction::HomeExecutor::end,
                           DiagnosticType::BEFORE)
                (
                  hood (home_executor_home_impl_decl)
                  [
                    handler (f.home_executor (),
                             &SemanticAction::HomeExecutor::end)
                  ]
                )

             >> assertion ("manages declaration expected",
                           f.home_executor (),
                           &SemanticAction::HomeExecutor::end,
                           DiagnosticType::BEFORE)
                (
                  hood (home_executor_executor_decl)
                  [
                    handler (f.home_executor (),
                             &SemanticAction::HomeExecutor::end)
                  ]
                )

             >> assertion ("'}' expected",
                           f.home_executor (),
                           &SemanticAction::HomeExecutor::end,
                           DiagnosticType::BEFORE)
                (
                  RCBRACE
                )

             >> assertion ("';' expected",
                           f.home_executor (),
                           &SemanticAction::HomeExecutor::end,
                           RecoveryMethod::NONE)
                (
                  SEMI[act_home_executor_end]
                )

           )[error_handler]
        ;


      home_executor_home_impl_decl =
           IMPLEMENTS
        >> guard
           (
                assertion ("home name expected",
                           DiagnosticType::BEFORE)
                (
                  identifier[act_home_executor_implements]
                )
             >> assertion ("';' expected",
                           RecoveryMethod::NONE)
                (
                  SEMI
                )
           )[error_handler]
        ;


      home_executor_executor_decl =
           MANAGES
        >> guard
           (
                assertion ("executor name expected",
                           DiagnosticType::BEFORE)
                (
                  simple_identifier[act_home_executor_manages]
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
