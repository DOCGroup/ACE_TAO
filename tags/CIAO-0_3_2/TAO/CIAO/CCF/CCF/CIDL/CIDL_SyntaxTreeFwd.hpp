// $Id$
#ifndef CCF_CIDL_SYNTAX_TREE_FWD_HPP
#define CCF_CIDL_SYNTAX_TREE_FWD_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_SyntaxTreeFwd.hpp"

namespace CIDL
{
  namespace SyntaxTree
  {

    // Import all nodes of IDL3
    using namespace IDL3::SyntaxTree;

    class Composition;
    typedef
    StrictPtr<Composition>
    CompositionPtr;

    class HomeExecutor;
    typedef
    StrictPtr<HomeExecutor>
    HomeExecutorPtr;
  }
}

#endif // CCF_CIDL_SYNTAX_TREE_FWD_HPP
