// file      : CCF/IDL3/SemanticAction/Impl/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_HPP

#include "CCF/IDL3/SemanticAction/EventType.hpp"

#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class EventType : public virtual SemanticAction::EventType,
                          public virtual ScopeBase<SyntaxTree::EventTypeDeclPtr>
        {
        public:
          virtual
          ~EventType () throw () {}

          EventType (bool trace, SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::EventTypeDeclPtr> (scope),
                trace_ (trace),
                name_ ("")
          {
          }

        public:

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "eventtype " << id << endl;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          inherits (IdentifierPtr const& id)
          {
            if (trace_) cerr << "inherits " << id << endl;
          }

          virtual void
          open_scope ()
          {
            using namespace SyntaxTree;

            ConcreteEventTypeDefPtr et( new ConcreteEventTypeDef (
                                          name_,
                                          scope_,
                                          inherits_));
            scope_->insert (et);
            push (et);
            scope_ = et;

            name_ = SimpleName (""); //indicate that we are done
            inherits_.clear ();
          }

          virtual void
          close_scope ()
          {
            scope_ = scope_->scope ();
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
            pop ();
          }

        private:
          bool trace_;
          SyntaxTree::SimpleName name_;
          SyntaxTree::ScopedNameSet inherits_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_EVENT_TYPE_HPP
