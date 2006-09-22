// file      : CCF/IDL3/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP

#include <set>
#include <stack>

#include "CCF/CompilerElements/Context.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"

#include "CCF/IDL2/SemanticGraph/Translation.hpp"
#include "CCF/IDL3/SemanticGraph/Elements.hpp"

#include "CCF/IDL2/SemanticAction/Include.hpp"
#include "CCF/IDL3/SemanticAction/Factory.hpp"

#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        // Note: overriding IDL2 include to allow inclusion of IDL3 files.
        //
        //
        struct Include : IDL2::SemanticAction::Include, Base
        {
        public:
          Include (Context& c,
                   Diagnostic::Stream& dout,
                   SemanticAction::Factory& action_factory);

          virtual void
          quote (StringLiteralPtr const& sl);

          virtual void
          bracket (StringLiteralPtr const& sl);

          virtual void
          end ();

        private:
          enum Type_
          {
            quote_,
            bracket_
          };

          virtual void
          impl (StringLiteralPtr const& sl, Type_ type);

          bool
          handle_already_included (fs::path const& path,
                                   StringLiteralPtr const& sl);
        private:
          Diagnostic::Stream& dout_;
          SemanticAction::Factory& action_factory_;

          std::stack<SemanticGraph::TranslationRegion*> stack_;
          std::stack<fs::path> abs_path_stack_, rel_path_stack_;

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
