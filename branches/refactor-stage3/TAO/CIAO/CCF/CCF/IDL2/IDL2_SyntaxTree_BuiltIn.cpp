// $Id$
#include "IDL2_SyntaxTree_BuiltIn.hpp"

#include "IDL2_Traversal.hpp"

namespace IDL2
{
  namespace SyntaxTree
  {
    using ReferenceCounting::StrictPtr;
    using CCF::Traversal::try_visit;

    //
    // BuiltInTypeDef
    //

    void BuiltInTypeDef::
    accept (CCF::Traversal::Visitor* v)
    {
      BuiltInTypeDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::BuiltInTypeDef> (&Traversal::BuiltInTypeDef::visit_built_in_type_def,
                     v,
                     self))
      {
        TypeDef::accept (v);
      }
    }


    //
    // Void
    //

    void Void::
    accept (CCF::Traversal::Visitor* v)
    {
      VoidPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::Void> (&Traversal::Void::visit_void,
                     v,
                     self))
      {
        BuiltInTypeDef::accept (v);
      }
    }

    //
    // Long
    //

    void Long::
    accept (CCF::Traversal::Visitor* v)
    {
      LongPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::Long> (&Traversal::Long::visit_long,
                     v,
                     self))
      {
        BuiltInTypeDef::accept (v);
      }
    }

    //
    // Boolean
    //

    void Boolean::
    accept (CCF::Traversal::Visitor* v)
    {
      BooleanPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::Boolean> (&Traversal::Boolean::visit_boolean,
                     v,
                     self))
      {
        BuiltInTypeDef::accept (v);
      }
    }

    //
    // String
    //

    void String::
    accept (CCF::Traversal::Visitor* v)
    {
      StringPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::String> (&Traversal::String::visit_string,
                     v,
                     self))
      {
        BuiltInTypeDef::accept (v);
      }
    }
  }
}
