// file      : CCF/CIDL/SemanticAction/Impl/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_FACTORY_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_FACTORY_HPP

#include "CCF/CIDL/SemanticAction/Factory.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Factory.hpp"

#include "CCF/CIDL/SemanticAction/Impl/Composition.hpp"
#include "CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Factory : public virtual CIDL::SemanticAction::Factory,
                        public virtual IDL3::SemanticAction::Impl::Factory
        {
        public:
          virtual
          ~Factory () throw () {}

          Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SyntaxTree::TranslationRegionPtr const& r)
              : IDL2::SemanticAction::Impl::Factory (context, dout, r),
                IDL3::SemanticAction::Impl::Factory (context, dout, r),
                trace_ (context.get ("cidl::semantic-action::trace", false)),
                composition_ (trace_, scope_),
                home_executor_ (trace_, scope_)
          {
          }
        public:
          virtual SemanticAction::Composition&
          composition ()
          {
            return composition_;
          }

          virtual SemanticAction::HomeExecutor&
          home_executor ()
          {
            return home_executor_;
          }

        private:
          bool trace_;
          Composition composition_;
          HomeExecutor home_executor_;
        };
      }
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_FACTORY_HPP
