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
        // Note: overriding IDL2 include to allow inclusion of IDL3 files
        //
        //
        class IncludeImpl
        {
        protected:
          virtual
          ~IncludeImpl () throw () {}

          enum Type
          {
            system,
            user
          };

          IncludeImpl (bool trace,
                       CompilerElements::Context& context,
                       Diagnostic::Stream& dout,
                       SemanticAction::Factory& action_factory,
                       SyntaxTree::TranslationRegionPtr const& region,
                       SyntaxTree::ScopePtr& scope,
                       Type type)
              : trace_ (trace),
                context_ (context),
                dout_ (dout),
                action_factory_ (action_factory),
                scope_ (scope),
                type_ (type)
          {
            stack_.push (region);
            path_stack_.push (
              fs::normalize (
                fs::complete (context_.get<fs::path> ("file-path"))));
          }

          virtual void
          begin_impl (StringLiteralPtr const& sl)
          {
            std::string prefix;

            if (type_ == user)
            {
              prefix = std::string ("include") + " \"" + sl->lexeme () + "\"";
            }
            else
            {
              prefix = std::string ("include") + " <" + sl->lexeme () + ">";
            }

            if (trace_) cerr << prefix << endl;

            using namespace SyntaxTree;

            try
            {
              fs::ifstream ifs;
              ifs.exceptions (ios_base::badbit | ios_base::failbit);

              fs::path include_path (sl->lexeme ());
              fs::path complete_path (include_path);

              if (trace_) cerr << prefix << ": staring resolution." << endl;

              if (complete_path.is_complete ())
              {
                complete_path = fs::normalize (complete_path);

                if (handle_already_included (complete_path, sl)) return;

                ifs.open (complete_path, std::ios_base::in);
              }
              else
              {
                bool found (false);

                if (type_ == user)
                {
                  fs::path rel_path (path_stack_.top ().branch_path ());

                  complete_path = fs::normalize (rel_path / include_path);

                  if (trace_) cerr << prefix << ": considering "
                                   << complete_path.string () << endl;

                  if (fs::exists (complete_path))
                  {
                    if (handle_already_included (complete_path, sl)) return;
                    ifs.open (complete_path, std::ios_base::in);
                    found = true;
                  }
                }

                if (!found)
                {
                  typedef
                  std::vector<fs::path>
                  SearchPaths;

                  SearchPaths const& search_paths (
                    context_.get<SearchPaths> ("include-search-paths"));

                  for (SearchPaths::const_iterator
                         i (search_paths.begin ()),
                         e (search_paths.end ()); i != e && !found; ++i)
                  {
                    complete_path  = *i / include_path;

                    if (!complete_path.is_complete ())
                    {
                      complete_path = complete (complete_path);
                    }

                    complete_path = fs::normalize (complete_path);

                    if (trace_) cerr << prefix << ": considering "
                                     << complete_path.string () << endl;

                    if (fs::exists (complete_path))
                    {
                      if (handle_already_included (complete_path, sl)) return;
                      ifs.open (complete_path, std::ios_base::in);
                      found = true;
                    }
                  }

                  if (!found)
                  {
                    cerr << sl << ": error: file not found." << endl;
                    return;
                  }
                }
              }

              if (trace_) cerr << prefix << ": resolved to "
                               << complete_path.string () << endl;

              //@@ for some reason ifs throws exception if I don't reset it
              //   to original state. It probably has something to do with
              //   call to get after eof.
              //
              ifs.exceptions (ios_base::iostate (0));

              TranslationRegionPtr r (new UserIncludeTranslationRegion (
                                        include_path,
                                        stack_.top ()->table (),
                                        stack_.top ()->create_order ()));

              stack_.top ()->insert (r);

              stack_.push (r);
              path_stack_.push (complete_path);

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

              path_stack_.pop ();
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
          end_impl ()
          {
            if (trace_) cerr << "end" << endl;
          }

        private:

          bool
          handle_already_included (fs::path const& path,
                                   StringLiteralPtr const& sl)
          {
            if(!include_file_set_.insert (path).second)
            {
              cerr << "warning: skipping already included file " << sl
                   << endl;
              return true;
            }
            return false;
          }

        private:
          bool trace_;
          CompilerElements::Context& context_;
          Diagnostic::Stream& dout_;
          SemanticAction::Factory& action_factory_;
          SyntaxTree::ScopePtr& scope_;
          Type type_;

          std::stack<SyntaxTree::TranslationRegionPtr> stack_;
          std::stack<fs::path> path_stack_;

          struct FilePathComparator
          {
            bool operator () (fs::path const& x, fs::path const& y) const
            {
              return x.string () < y.string ();
            }
          };

          std::set<fs::path, FilePathComparator> include_file_set_;
        };


        //
        //
        //
        struct Include : virtual IDL2::SemanticAction::Include, IncludeImpl
        {
          virtual
          ~Include () throw ()
          {
          }

          Include (bool trace,
                   CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticAction::Factory& action_factory,
                   SyntaxTree::TranslationRegionPtr const& region,
                   SyntaxTree::ScopePtr& scope)
              : IncludeImpl (trace,
                             context,
                             dout,
                             action_factory,
                             region,
                             scope,
                             IncludeImpl::user)
          {
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            begin_impl (sl);
          }

          virtual void
          end ()
          {
            end_impl ();
          }
        };

        //
        //
        //
        struct SystemInclude : virtual IDL2::SemanticAction::SystemInclude,
                               IncludeImpl
        {
          virtual
          ~SystemInclude () throw ()
          {
          }

          SystemInclude (bool trace,
                         CompilerElements::Context& context,
                         Diagnostic::Stream& dout,
                         SemanticAction::Factory& action_factory,
                         SyntaxTree::TranslationRegionPtr const& region,
                         SyntaxTree::ScopePtr& scope)
              : IncludeImpl (trace,
                             context,
                             dout,
                             action_factory,
                             region,
                             scope,
                             IncludeImpl::system)
          {
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            //@@ This is a really dirty hack. Need to think of
            //   a mechanism to address this issue.
            //
            if (sl->lexeme () != "Components.idl")
            {
              begin_impl (sl);
            }
          }

          virtual void
          end ()
          {
            end_impl ();
          }
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
