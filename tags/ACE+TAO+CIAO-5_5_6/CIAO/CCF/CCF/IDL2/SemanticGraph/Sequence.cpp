// file      : CCF/IDL2/SemanticGraph/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Sequence
      //
      //
      namespace
      {
        TypeInfo
        sequence_init_ ()
        {
          TypeInfo ti (typeid (Sequence));
          ti.add_base (
            Access::PUBLIC, true, Specialization::static_type_info ());
          return ti;
        }

        TypeInfo sequence_ (sequence_init_ ());
      }

      TypeInfo const& Sequence::
      static_type_info () { return sequence_; }

      Sequence::
      ~Sequence ()
      {
      }


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


      // BoundedSequence
      //
      //
      namespace
      {
        TypeInfo
        bounded_sequence_init_ ()
        {
          TypeInfo ti (typeid (BoundedSequence));
          ti.add_base (Access::PUBLIC, true, Sequence::static_type_info ());
          return ti;
        }

        TypeInfo bounded_sequence_ (bounded_sequence_init_ ());
      }

      TypeInfo const& BoundedSequence::
      static_type_info () { return bounded_sequence_; }
    }
  }
}
