// file      : CCF/IDL3/SemanticAction/Impl/Include.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticAction/Impl/Include.hpp"

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

#include "CCF/IDL3/LexicalAnalyzer.hpp"
#include "CCF/IDL3/Parser.hpp"

#include <vector>
#include <string>
#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Include::
        Include (Context& c,
                 Diagnostic::Stream& dout,
                 SemanticAction::Factory& action_factory)
            : Base (c),
              dout_ (dout),
              action_factory_ (action_factory)
        {
          abs_path_stack_.push (
            fs::normalize (
              fs::complete (
                ctx.parsing_context ().get<fs::path> ("file-path"))));
        }

        void Include::
        quote (StringLiteralPtr const& sl)
        {
          impl (sl, quote_);
        }

        void Include::
        bracket (StringLiteralPtr const& sl)
        {
          impl (sl, bracket_);
        }


        void Include::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }

        void Include::
        impl (StringLiteralPtr const& sl, Type_ type)
        {
          CompilerElements::Context& pctx (ctx.parsing_context ());

          std::string prefix;

          if (type == quote_)
          {
            prefix = std::string ("include") + " \"" + sl->value () + "\"";
          }
          else
          {
            prefix = std::string ("include") + " <" + sl->value () + ">";
          }

          if (ctx.trace ()) cerr << prefix << endl;

          try
          {
            fs::ifstream ifs;
            ifs.exceptions (std::ios_base::badbit | std::ios_base::failbit);

            fs::path include_path (sl->value ());
            fs::path complete_path (include_path);

            if (ctx.trace ()) cerr << prefix << ": staring resolution."
                                   << endl;

            if (complete_path.is_complete ())
            {
              complete_path = fs::normalize (complete_path);

              if (handle_already_included (complete_path, sl)) return;

              ifs.open (complete_path, std::ios_base::in);
            }
            else
            {
              bool found (false);

              if (type == quote_)
              {
                fs::path rel_path (abs_path_stack_.top ().branch_path ());

                complete_path = fs::normalize (rel_path / include_path);

                if (ctx.trace ()) cerr << prefix << ": considering "
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
                typedef std::vector<fs::path> SearchPaths;

                SearchPaths const& search_paths (
                  pctx.get<SearchPaths> ("include-search-paths"));

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

                  if (ctx.trace ()) cerr << prefix << ": considering "
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
                  cerr << ctx.file () << ":" << sl->line () << ": error: "
                       << "'" << sl << "': file not found" << endl;
                  return;
                }
              }
            }

            if (ctx.trace ()) cerr << prefix << ": resolved to "
                                   << complete_path.string () << endl;

            //@@ for some reason ifs throws exception if I don't reset it
            //   to original state. It probably has something to do with
            //   call to get after eof.
            //
            ifs.exceptions (std::ios_base::iostate (0));

            TranslationRegion& r (
              ctx.tu ().new_node<TranslationRegion> (include_path, 0));

            if (type == quote_)
            {
              ctx.tu ().new_edge<QuoteIncludes> (
                ctx.region (), r, include_path);
            }
            else
            {
              ctx.tu ().new_edge<BracketIncludes> (
                ctx.region (), r, include_path);
            }

            // Set new current region.
            //
            stack_.push (&ctx.region ());
            ctx.region (r);

            // Create Root scope for new region.
            //
            Root& root (ctx.tu ().new_node<Root> (include_path, 0));
            ctx.tu ().new_edge<ContainsRoot> (r, root);
            ctx.scope (root);

            // Store previous relative path and current absolute.
            //
            rel_path_stack_.push (pctx.get<fs::path> ("file-path"));
            pctx.set("file-path", include_path);

            abs_path_stack_.push (complete_path);


            // Parse the file.
            //
            //

            //@@ this code is highly experimental
            CompilerElements::InputStreamAdapter isa (ifs);

            CompilerElements::CPP::Symbols const& symbols (
              pctx.get<CompilerElements::CPP::Symbols> ("cpp-symbols"));
            CompilerElements::CPP::Preprocessor pp (isa, symbols);

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
              IDL3::Parser parser (pctx, dout_, lexer, action_factory_);

              IDL2::Parsing::parse (token_stream.begin (),
                                    token_stream.end (),
                                    parser.start ());
            }

            // Restore paths
            //
            abs_path_stack_.pop ();

            pctx.set("file-path", rel_path_stack_.top ());
            rel_path_stack_.pop ();


            // Restore region.
            //
            ctx.region (*stack_.top ());
            stack_.pop ();

            // Create new Root scope.
            //
            {
              Root& root (ctx.tu ().new_node<Root> (ctx.file (), 0));
              ctx.tu ().new_edge<ContainsRoot> (ctx.region (), root);
              ctx.scope (root);
            }
          }
          catch (fs::filesystem_error const&)
          {
            cerr << ctx.file () << ":" << sl->line () << ": error: "
                 << "'" << sl << "': unable to open in read mode" << endl;
          }
          catch (std::ios_base::failure const&)
          {
            cerr << ctx.file () << ":" << sl->line () << ": error: "
                 << "'" << sl << "': unable to open in read mode" << endl;
          }
        }

        bool Include::
        handle_already_included (fs::path const& path,
                                 StringLiteralPtr const&)
        {
          if(!include_file_set_.insert (path).second)
          {
            // cerr << "warning: skipping already included file " << sl
            //      << endl;
            return true;
          }
          return false;
        }
      }
    }
  }
}
