// file      : CCF/IDL2/SyntaxTree/TypeId.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/TypeId.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      //  TypeId
      //
      //
      namespace
      {
        TypeInfo
        typeid_init_ ()
        {
          TypeInfo ti (typeid (TypeId));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo typeid_ (typeid_init_ ());
      }

      TypeInfo const& TypeId::
      static_type_info () { return typeid_; }


      //  Typeprefix
      //
      //
      namespace
      {
        TypeInfo
        typeprefix_init_ ()
        {
          TypeInfo ti (typeid (TypePrefix));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo typeprefix_ (typeprefix_init_ ());
      }

      TypeInfo const& TypePrefix::
      static_type_info () { return typeprefix_; }
    }
  }
}
