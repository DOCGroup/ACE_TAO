// file      : CCF/CIDL/SemanticAction/Impl/HomeExecutor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP

#include "CCF/CIDL/SyntaxTree/HomeExecutor.hpp"
#include "CCF/CIDL/SemanticAction/HomeExecutor.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class HomeExecutor :
          public virtual SemanticAction::HomeExecutor,
          public virtual ScopeBase<SyntaxTree::HomeExecutorPtr>
        {
        public:
          virtual
          ~HomeExecutor () throw () {}

          HomeExecutor (bool trace, SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::HomeExecutorPtr> (scope),
                trace_ (trace),
                name_ (""),
                implements_ (""),
                manages_ ("")

          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "home executor " << id << endl;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          open_scope ()
          {
            // Note: nothing is expected to go to home executor scope
            //       so watch for grammar changes.
          }

          virtual void
          implements (IdentifierPtr const& id)
          {
            if (trace_) cerr << "implements " << id << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct ImplementsPredicate : public DeclarationTable::ResolvePredicate
            {
              struct IncompatibleType :
                public DeclarationTable::ResolutionFailure
              {
                IncompatibleType (std::string const& t) : type (t) {}
                std::string type;
              };

              virtual bool
              test (DeclarationPtr const& d)
                throw (IncompatibleType)
              {
                std::string type = d->declaration_class ();

                if (type != "home") throw IncompatibleType (type);

                return d->dynamic_type<TypeDecl> ()->defined ();
              }
            } p;

            try
            {
              ScopedName sn = scope_->table ().resolve (
                name,
                scope_->name (),
                scope_->peek_order (),
                p);

              implements_ = sn;
            }
            catch (ImplementsPredicate::IncompatibleType const& e)
            {
              cerr << "error: invalid implements specification" << endl;

              cerr << "specifying " << e.type << " \'" << name
                   << "\' in home executor implements clause is illegal"
                   << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid implements specification" << endl;
              cerr << "no home with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid implements specification" << endl;
              cerr << "no defined home with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
              cerr << "implementing forward-declared home is illegal"
                   << endl;
            }

          }

          virtual void
          manages (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "manages " << id << endl;
            manages_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          close_scope ()
          {
            // Note: nothing is expected to go to home executor scope
            //       so watch for grammar changes.
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;

            using namespace SyntaxTree;

            HomeExecutorPtr he (new SyntaxTree::HomeExecutor (
                                  name_,
                                  scope_,
                                  implements_,
                                  manages_));
            scope_->insert (he);
          }
        private:
          bool trace_;
          SyntaxTree::SimpleName name_;
          SyntaxTree::ScopedName implements_;
          SyntaxTree::SimpleName manages_;
        };
      }
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_IMPL_HOME_EXECUTOR_HPP
