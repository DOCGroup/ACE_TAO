// file      : CCF/IDL2/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP

#include <stack>

#include "CCF/IDL2/SyntaxTree/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Include : public virtual SemanticAction::Include
        {
        public:
          virtual
          ~Include () throw () {}

          Include (bool trace,
                   SemanticAction::Factory& action_factory,
                   SyntaxTree::TranslationRegionPtr const& region,
                   SyntaxTree::ScopePtr& scope)
              : trace_ (trace),
                action_factory_ (action_factory),
                scope_ (scope)
          {
            stack_.push (region);
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            if (trace_) cerr << "include \"" << sl << "\"" << endl;

            /*
              @@ this code is experimentla and temporarlily disabled
              @@ see relevant code in IDL3 for more information

              using namespace SyntaxTree;

              std::ifstream ifs (sl->lexeme ().c_str ());
              if (!ifs.is_open ())
              {
              cerr << "error: unable to open included file \'"
              << sl << "\'" << endl;
              }

              TranslationRegionPtr r (
              new TranslationRegion (sl->lexeme (),
              stack_.top ()->table (),
              stack_.top ()->create_order ()));

              stack_.top ()->insert (r);
              stack_.push (r);

              scope_ = stack_.top ()->scope ();

              //@@ this code is highly experimental
              IDL2::LexicalAnalyzer lexer (ifs);

              TokenStream token_stream;

              //@@ bad token comparison
              for (TokenPtr token = lexer.next ();
              token.in () != lexer.eos.in ();
              token = lexer.next ())
              {
              token_stream.push_back (token);
              }

              if (token_stream.size () == 0)
              {
              // Empty file
              return;
              }

              IDL2::Parser parser (lexer, action_factory_);

              bool result = Details::parse (token_stream.begin (),
              token_stream.end (),
              parser.start ());
            */
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;

            stack_.pop ();
            scope_ = stack_.top ()->scope ();
          }

        public:
          bool trace_;
          SemanticAction::Factory& action_factory_;
          SyntaxTree::ScopePtr& scope_;
          std::stack<SyntaxTree::TranslationRegionPtr> stack_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
