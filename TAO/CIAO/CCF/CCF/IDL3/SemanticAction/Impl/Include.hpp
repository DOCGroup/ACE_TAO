// file      : CCF/IDL3/SemanticAction/Impl/Include.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_INCLUDE_HPP

#include <stack>
#include <set>

#include "CCF/CompilerElements/Context.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"

#include "CCF/IDL2/SemanticGraph/Translation.hpp"
#include "CCF/IDL3/SemanticGraph/Elements.hpp"

#include "CCF/IDL2/SemanticAction/Include.hpp"
#include "CCF/IDL3/SemanticAction/Elements.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

#include "CCF/IDL3/SemanticAction/Factory.hpp"

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
          ~IncludeImpl () throw ();

          enum Type
          {
            system,
            user
          };

          IncludeImpl (Context& c,
                       CompilerElements::Context& context,
                       Diagnostic::Stream& dout,
                       SemanticAction::Factory& action_factory,
                       Type type);

          virtual void
          begin_impl (StringLiteralPtr const& sl);

          virtual void
          end_impl ();

        private:
          bool
          handle_already_included (fs::path const& path,
                                   StringLiteralPtr const& sl);
        private:
          Context& ctx;

          CompilerElements::Context& context_;
          Diagnostic::Stream& dout_;
          SemanticAction::Factory& action_factory_;

          Type type_;

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


        //
        //
        //
        struct Include : virtual IDL2::SemanticAction::Include, IncludeImpl
        {
          virtual
          ~Include () throw ()
          {
          }

          Include (Context& c,
                   CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticAction::Factory& action_factory)
              : IncludeImpl (c,
                             context,
                             dout,
                             action_factory,
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

          SystemInclude (Context& c,
                         CompilerElements::Context& context,
                         Diagnostic::Stream& dout,
                         SemanticAction::Factory& action_factory)
              : IncludeImpl (c,
                             context,
                             dout,
                             action_factory,
                             IncludeImpl::system)
          {
          }

          virtual void
          begin (StringLiteralPtr const& sl)
          {
            //@@ This is a really dirty hack. Need to think of
            //   a mechanism to address this issue.
            //
            if (sl->value () != "Components.idl")
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
