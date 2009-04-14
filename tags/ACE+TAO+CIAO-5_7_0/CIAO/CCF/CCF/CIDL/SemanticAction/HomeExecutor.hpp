// file      : CCF/CIDL/SemanticAction/HomeExecutor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_HOME_EXECUTOR_HPP
#define CCF_CIDL_SEMANTIC_ACTION_HOME_EXECUTOR_HPP

#include "CCF/CIDL/SemanticAction/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      struct HomeExecutor
      {
        virtual ~HomeExecutor ()
        {
        }
        
        virtual void
        begin (SimpleIdentifierPtr const& id) = 0;

        virtual void
        implements (IdentifierPtr const& id) = 0;

        virtual void
        manages (SimpleIdentifierPtr const& id) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_HOME_EXECUTOR_HPP
