// file      : CCF/CIDL/SemanticAction/Impl/Factory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Module.hpp"
#include "CCF/IDL2/SemanticGraph/Interface.hpp"

#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Factory::
        ~Factory () throw ()
        {
        }

        Factory::
        Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticGraph::TranslationUnit& tu)
            : IDL2::SemanticAction::Impl::Factory (context, dout, tu),
              IDL3::SemanticAction::Impl::Factory (context, dout, tu),

              composition_ (ctx_),
              home_executor_ (ctx_)
        {
          //@@ I am still not sure what is the right way to do this.
          //   Current approach suppresses Components.idl inclusion
          //   and implicitly defines necessary types.
          //

          //@@ this region is inserted after principal which is
          //   somewhat strange. It doesn't affect anything as
          //   long as no code is generated for this region.
          //

          // Implied translation region with CCM-related types.
          //
          TranslationRegion& ctr (tu.new_node<TranslationRegion> ());
          tu.new_edge<ContainsImplied> (tu, ctr, ".components");

          Root& cr (tu.new_node<Root> ());
          tu.new_edge<ContainsRoot> (ctr, cr);

          Module& cm (tu.new_node<Module> ());
          tu.new_edge<Defines> (cr, cm, "Components");

          tu.new_edge<Defines> (
            cm, tu.new_node<LocalInterface> (), "EnterpriseComponent");
        }
      }
    }
  }
}
