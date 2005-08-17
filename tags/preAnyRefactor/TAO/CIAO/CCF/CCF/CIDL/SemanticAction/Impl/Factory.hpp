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
        struct Factory : virtual CIDL::SemanticAction::Factory,
                         virtual IDL3::SemanticAction::Impl::Factory
        {
          Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticGraph::TranslationUnit& tu);

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
          Composition composition_;
          HomeExecutor home_executor_;
        };
      }
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_FACTORY_HPP
