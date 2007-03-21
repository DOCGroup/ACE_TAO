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
      struct Operation
      {
        virtual ~Operation ()
        {
        }
        
        virtual void
        one_way () = 0;

        virtual void
        two_way () = 0;

        virtual void
        type (IdentifierPtr const& id) = 0;

        virtual void
        name (SimpleIdentifierPtr const& id) = 0;

        struct Direction
        {
          enum Value
          {
            in = 0,
            out,
            inout
          };
        };

        virtual void
        parameter (Direction::Value direction,
                   IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id) = 0;

        virtual void
        raises (IdentifierPtr const& id) = 0;
      };
    }
  }
}

std::ostream&
operator<< (std::ostream& o,
            CCF::IDL2::SemanticAction::Operation::Direction::Value d);

#endif  // CCF_IDL2_SEMANTIC_ACTION_OPERATION_HPP
