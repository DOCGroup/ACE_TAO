// $Id$
#ifndef CCF_IDL3_SEMANTIC_ACTION_HPP
#define CCF_IDL3_SEMANTIC_ACTION_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SemanticAction.hpp"

namespace IDL3
{
  namespace SemanticAction
  {
    using namespace IDL2::SemanticAction;

    class Component : public virtual Scope
    {
    public:

      virtual void
      begin (SimpleIdentifierPtr const& id) = 0;

      virtual void
      inherits (IdentifierPtr const& id) = 0;

      virtual void
      supports (IdentifierPtr const& id) = 0;

      virtual void
      end () = 0;
    };

    class Provides
    {
    public:
      virtual
      ~Provides () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

    class Uses
    {
    public:
      virtual
      ~Uses () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

    class Publishes
    {
    public:
      virtual
      ~Publishes () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

    class Emits
    {
    public:
      virtual
      ~Emits () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

    class Consumes
    {
    public:
      virtual
      ~Consumes () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

    class EventType : public virtual Scope
    {
    public:

      virtual void
      begin (SimpleIdentifierPtr const& id) = 0;

      virtual void
      inherits (IdentifierPtr const& id) = 0;

      virtual void
      end () = 0;
    };

    class Home : public virtual Scope
    {
    public:

      virtual void
      begin (SimpleIdentifierPtr const& id) = 0;

      virtual void
      inherits (IdentifierPtr const& id) = 0;

      virtual void
      supports (IdentifierPtr const& id) = 0;

      virtual void
      manages (IdentifierPtr const& id) = 0;

      virtual void
      end () = 0;
    };

    class HomeFactory
    {
    public:

      virtual void
      begin (SimpleIdentifierPtr const& id) = 0;

      virtual void
      parameter (IdentifierPtr const& type,
                 SimpleIdentifierPtr const& name) = 0;

      virtual void
      end () = 0;
    };
  }

  class SemanticActionFactory : public virtual IDL2::SemanticActionFactory
  {
  public:
    virtual SemanticAction::Component&
    component () = 0;

    virtual SemanticAction::Provides&
    provides () = 0;

    virtual SemanticAction::Uses&
    uses () = 0;

    virtual SemanticAction::Publishes&
    publishes () = 0;

    virtual SemanticAction::Emits&
    emits () = 0;

    virtual SemanticAction::Consumes&
    consumes () = 0;

    virtual SemanticAction::EventType&
    event_type () = 0;

    virtual SemanticAction::Home&
    home () = 0;

    virtual SemanticAction::HomeFactory&
    home_factory () = 0;
  };
}

#endif // CCF_IDL3_SEMANTIC_ACTION_HPP
