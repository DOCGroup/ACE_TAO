// $Id$
#ifndef CCF_IDL2_SEMANTIC_ACTION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Token.hpp"
#include "CCF/CompilerElements/SemanticAction.hpp"

namespace IDL2
{
  namespace SemanticAction
  {
    using ::SemanticAction::Base;

    class Include : public virtual Base
    {
    public:
      virtual
      ~Include () throw () {}

      virtual void
      begin (StringLiteralPtr const& sl) = 0;

      virtual void
      end () = 0;
    };

    class Scope : public virtual Base
    {
    public:
      virtual
      ~Scope () throw () {}

      virtual void
      open_scope () = 0;

      virtual void
      close_scope () = 0;
    };

    class Module : public virtual Scope
    {
    public:
      virtual
      ~Module () throw () {}

      virtual void
      begin (SimpleIdentifierPtr const& id) = 0;

      virtual void
      end () = 0;
    };

    class Interface : public virtual Scope
    {
    public:
      virtual
      ~Interface () throw () {}

      struct Qualifier
      {
        enum Value
        {
          LOCAL,
          ABSTRACT,
          UNCONSTRAINED,
        };

        friend std::ostream&
        operator<< (std::ostream& o, Value d)
        {
          if (d == LOCAL) o << "local";
          else if (d == ABSTRACT) o << "abstract";
          else o << "unconstrained";
          return o;
        }
      };

      virtual void
      begin (Qualifier::Value qualifier, SimpleIdentifierPtr const& id) = 0;

      virtual void
      inherits (IdentifierPtr const& id) = 0;

      virtual void
      end () = 0;
    };

    class Attribute
    {
    public:
      virtual
      ~Attribute () throw () {}

      virtual void
      type (IdentifierPtr const& id) = 0;

      virtual void
      name (SimpleIdentifierPtr const& id) = 0;
    };

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
          IN,
          OUT,
          INOUT
        };

        friend std::ostream&
        operator<< (std::ostream& o, Value d)
        {
          if (d == IN) o << "in";
          else if (d == OUT) o << "out";
          else o << "inout";
          return o;
        }
      };

      virtual void
      parameter (Direction::Value direction,
                 IdentifierPtr const& type_id,
                 SimpleIdentifierPtr const& name_id) = 0;

      virtual void
      end () = 0;
    };
  }

  class SemanticActionFactory
  {
  public:
    virtual
    ~SemanticActionFactory () throw () {}

    virtual SemanticAction::Include&
    include () = 0;

    virtual SemanticAction::Module&
    module () = 0;

    virtual SemanticAction::Interface&
    interface () = 0;

    virtual SemanticAction::Attribute&
    attribute () = 0;

    virtual SemanticAction::Operation&
    operation () = 0;
  };
}

#endif // CCF_IDL2_SEMANTIC_ACTION_HPP
