// file      : CCF/IDL2/SyntaxTree/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Sequence.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // SequnceDecl
      //
      //
      namespace
      {
        TypeInfo
        sequence_decl_init_ ()
        {
          TypeInfo ti (typeid (SequenceDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo sequence_decl_ (sequence_decl_init_ ());
      }

      TypeInfo const& SequenceDecl::
      static_type_info () { return sequence_decl_; }


      // UnboundedSequnceDecl
      //
      //
      namespace
      {
        TypeInfo
        unbounded_sequence_decl_init_ ()
        {
          TypeInfo ti (typeid (UnboundedSequenceDecl));
          ti.add_base (
            Access::PUBLIC, true, SequenceDecl::static_type_info ());

          return ti;
        }

        TypeInfo unbounded_sequence_decl_ (unbounded_sequence_decl_init_ ());
      }

      TypeInfo const& UnboundedSequenceDecl::
      static_type_info () { return unbounded_sequence_decl_; }
    }
  }
}
