// file      : CCF/IDL2/SemanticAction/Impl/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP

#include "CCF/IDL2/SemanticAction/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Operation : public virtual SemanticAction::Operation
        {
        public:

          virtual
          ~Operation () throw () {}

          Operation (bool trace,
                     SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current)
          {
          }

          bool
          lookup_type (SyntaxTree::Name const& name,
                       SyntaxTree::ScopedName& result)
          {
            using namespace SyntaxTree;

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) throw ()
              {
                return d->is_a<TypeDecl> ();
              }
            } p;

            try
            {
              result = scope_->table ().resolve (
                name,
                scope_->name (),
                scope_->peek_order (),
                p);

              return true;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid operation declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid operation declaration" << endl;
              cerr << "declaration with name \'" << name
                   << "\' visible from scope \'" << scope_->name ()
                   << "\' is not a type declaration" << endl;
              cerr << "using non-type as operation parameter type "
                   << " or return type is illegal"
                   << endl;
            }

            return false;
          }


          virtual void
          begin (IdentifierPtr const& type_id,
                 SimpleIdentifierPtr const& name_id)
          {
            if (trace_)
              cerr << "operation " << type_id << " " << name_id << endl;

            using namespace SyntaxTree;

            ScopedName type_name;

            if (lookup_type (Name (type_id->lexeme ()), type_name))
            {
              operation_ = OperationDeclPtr (
                new OperationDecl (SimpleName (name_id->lexeme ()),
                                   type_name,
                                   scope_));
            }
          }


          virtual void
          parameter (Direction::Value direction,
                     IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id)
          {
            if (trace_)
              cerr << "parameter " << direction << " " << type_id
                   << " " << name_id << endl;

            using namespace SyntaxTree;

            ScopedName type_name ("");

            if (lookup_type (Name (type_id->lexeme ()), type_name) &&
                operation_ != 0)
            {
              OperationParameter::Direction::Value d =
                OperationParameter::Direction::INOUT;

              if (direction == Direction::IN)
              {
                d = OperationParameter::Direction::IN;
              }
              else if (direction == Direction::OUT)
              {
                d = OperationParameter::Direction::OUT;
              }

              OperationParameterPtr p (
                new OperationParameter (d,
                                        type_name,
                                        SimpleName (name_id->lexeme ()),
                                        scope_->table ()));
              operation_->insert (p);
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
            if (operation_ != 0)
            {
              scope_->insert (operation_);
              operation_ = SyntaxTree::OperationDeclPtr ();
            }
          }
        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
          SyntaxTree::OperationDeclPtr operation_;
        };

      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_OPERATION_HPP
