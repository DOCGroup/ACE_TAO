// file      : CCF/IDL3/SemanticAction.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_HPP
#define CCF_IDL3_SEMANTIC_ACTION_HPP

#include "CCF/IDL2/SemanticAction.hpp"

#include "CCF/IDL3/SemanticAction/Component.hpp"
#include "CCF/IDL3/SemanticAction/Consumes.hpp"
#include "CCF/IDL3/SemanticAction/Emits.hpp"
#include "CCF/IDL3/SemanticAction/EventType.hpp"
#include "CCF/IDL3/SemanticAction/Home.hpp"
#include "CCF/IDL3/SemanticAction/HomeFactory.hpp"
#include "CCF/IDL3/SemanticAction/Provides.hpp"
#include "CCF/IDL3/SemanticAction/Publishes.hpp"
#include "CCF/IDL3/SemanticAction/Uses.hpp"

#include "CCF/IDL3/SemanticAction/Factory.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      using namespace IDL2::SemanticAction;
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_HPP
