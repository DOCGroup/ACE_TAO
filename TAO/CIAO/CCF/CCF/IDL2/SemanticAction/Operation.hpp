// file      : CCF/IDL2/SemanticAction/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_OPERATION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_OPERATION_HPP

#include <iosfwd>

#include "CCF/IDL2/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      //
      //
      //
      class Operation
      {
      public:

        virtual void
        begin (IdentifierPtr const& type_id,
               SimpleIdentifierPtr const& name_id) = 0;

        struct Direction
        {
          enum Value
          {
            IN = 0,
            OUT,
            INOUT
          };

          friend std::ostream&
          operator<< (std::ostream& o, Value d);
        };

        virtual void
        parameter (Direction::Value direction,
                   IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_OPERATION_HPP
