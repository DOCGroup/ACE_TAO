// file      : CCF/IDL3/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP

#include "CCF/IDL2/SemanticAction/Include.hpp"

//@@ needed for include handling
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        // Note: overriding IDL2 include to allow inclusiion of IDL3 files
        //
        //
        class Include : public virtual IDL2::SemanticAction::Include
        {
        public:
          virtual
          ~Include () throw () {}

          Include (bool trace,
                   CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticAction::Factory& action_factory,
                   SyntaxTree::TranslationRegionPtr const& region,
                   SyntaxTree::ScopePtr& scope)
              : trace_ (trace),
                context_ (context),
                dout_ (dout),
                action_factory_ (action_factory),
                scope_ (scope)
          {
            stack_.push (region);
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            if (trace_) cerr << "include \"" << sl << "\"" << endl;

            using namespace SyntaxTree;

            try
            {
              fs::path file_path (sl->lexeme ());

              if(!include_file_set_.insert (file_path).second)
              {
                cerr << "warning: skipping already included file \'"
                     << sl << "\'" << endl;
                return;
              }

              fs::ifstream ifs;
              ifs.exceptions (ios_base::badbit | ios_base::failbit);
              ifs.open (file_path);

              //@@ for some reason ifs throws exception if I don't reset it to
              //   original state. It probably has something to do with call to
              //   get after eof.
              ifs.exceptions (ios_base::iostate (0));

              TranslationRegionPtr r (new UserIncludeTranslationRegion (
                                        file_path,
                                        stack_.top ()->table (),
                                        stack_.top ()->create_order ()));

              stack_.top ()->insert (r);

              stack_.push (r);
              context_.set ("file-path", stack_.top ()->file_path ());

              if (trace_) cerr << "push file scope: old scope "
                               << scope_->order ();

              scope_ = stack_.top ()->scope ();

              if (trace_) cerr << "; new scope " << scope_->order () << endl;

              //@@ this code is highly experimental
              InputStreamAdapter isa (ifs);
              Preprocessor pp (isa);
              IDL3::LexicalAnalyzer lexer (pp);

              TokenList token_stream;

              //@@ bad token comparison
              for (TokenPtr token = lexer.next ();; token = lexer.next ())
              {
                token_stream.push_back (token);
                if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0)
                  break;
              }

              if (token_stream.size () > 1)
              {
                IDL3::Parser parser (context_, dout_, lexer, action_factory_);
                
                IDL2::Parsing::parse (token_stream.begin (),
                                      token_stream.end (),
                                      parser.start ());
              }

              if (trace_) cerr << "pop file scope: old scope "
                               << scope_->order ();

              stack_.pop ();

              scope_ = stack_.top ()->scope ();
              context_.set ("file-path", stack_.top ()->file_path ());

              if (trace_) cerr << "; new scope " << scope_->order () << endl;
            }
            catch (fs::filesystem_error const&)
            {
              cerr << sl << ": error: unable to open in read mode" << endl;
            }
            catch (ios_base::failure const&)
            {
              cerr << sl << ": error: unable to open in read mode" << endl;
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
          }

        public:
          bool trace_;
          CompilerElements::Context& context_;
          Diagnostic::Stream& dout_;
          SemanticAction::Factory& action_factory_;
          SyntaxTree::ScopePtr& scope_;

          std::stack<SyntaxTree::TranslationRegionPtr> stack_;

          struct FilePathComparator
          {
            bool operator () (fs::path const& x, fs::path const& y) const
            {
              return x.string () < y.string ();
            }
          };

          std::set<fs::path, FilePathComparator> include_file_set_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
