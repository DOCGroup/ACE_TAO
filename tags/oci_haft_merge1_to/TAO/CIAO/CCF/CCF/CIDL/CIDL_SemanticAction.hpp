// $Id$
#ifndef CCF_CIDL_SEMANTIC_ACTION_HPP
#define CCF_CIDL_SEMANTIC_ACTION_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_SemanticAction.hpp"

namespace CIDL
{
  namespace SemanticAction
  {
    using namespace IDL3::SemanticAction;

    class Composition : public virtual Scope
    {
    public:
      struct Category
      {
        enum Value
        {
          ENTITY,
          PROCESS,
          SERVICE,
          SESSION
        };

        friend std::ostream&
        operator<< (std::ostream& o, Value v)
        {
          if (v == ENTITY) o << "entity";
          else if (v == PROCESS) o << "process";
          else if (v == SERVICE) o << "service";
          else o << "session";
          return o;
        }
      };

      virtual void
      begin (SimpleIdentifierPtr const& id, Category::Value category) = 0;

      virtual void
      end () = 0;
    };

    class HomeExecutor : public virtual Scope
    {
    public:
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

  class SemanticActionFactory : public virtual IDL3::SemanticActionFactory
  {
  public:
    virtual SemanticAction::Composition&
    composition () = 0;

    virtual SemanticAction::HomeExecutor&
    home_executor () = 0;
  };
}

#endif // CCF_CIDL_SEMANTIC_ACTION_HPP
