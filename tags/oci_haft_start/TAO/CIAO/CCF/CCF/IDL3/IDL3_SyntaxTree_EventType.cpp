// $Id$
#include "IDL3_Traversal.hpp"
#include "IDL3_SyntaxTree_EventType.hpp"

namespace IDL3
{
  namespace SyntaxTree
  {
    using CCF::Traversal::try_visit;

    void ConcreteEventTypeDef::
    accept (CCF::Traversal::Visitor* v)
    {
      ConcreteEventTypeDefPtr self (ReferenceCounting::add_ref (this));
      
      try_visit<Traversal::ConcreteEventTypeDef> (
        &Traversal::ConcreteEventTypeDef::visit_concrete_event_type_def,
        v,
        self);
    }
  }
}
