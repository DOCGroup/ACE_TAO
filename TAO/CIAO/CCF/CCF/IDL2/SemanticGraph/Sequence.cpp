// file      : CCF/IDL2/SemanticGraph/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Sequence.hpp"

using Introspection::TypeInfo;
using Introspection::Access;

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      // Sequence
      //
      //
      namespace
      {
        TypeInfo
        sequence_init_ ()
        {
          TypeInfo ti (typeid (Sequence));
          ti.add_base (Access::PUBLIC,
                       true,
                       TypeTemplateSpecialization::static_type_info ());
          return ti;
        }

        TypeInfo sequence_ (sequence_init_ ());
      }

      TypeInfo const& Sequence::
      static_type_info () { return sequence_; }


      // UnboundedSequence
      //
      //
      namespace
      {
        TypeInfo
        unbounded_sequence_init_ ()
        {
          TypeInfo ti (typeid (UnboundedSequence));
          ti.add_base (Access::PUBLIC, true, Sequence::static_type_info ());
          return ti;
        }

        TypeInfo unbounded_sequence_ (unbounded_sequence_init_ ());
      }

      TypeInfo const& UnboundedSequence::
      static_type_info () { return unbounded_sequence_; }
    }
  }
}
