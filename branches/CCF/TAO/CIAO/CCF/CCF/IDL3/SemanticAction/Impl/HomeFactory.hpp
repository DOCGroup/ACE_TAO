// file      : CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP

#include "CCF/IDL3/SemanticAction/HomeFactory.hpp"

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
        class HomeFactory : public virtual SemanticAction::HomeFactory
        {
        public:
          virtual
          ~HomeFactory () throw ()
          {
          }

          HomeFactory (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current)
          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "home factory " << id << endl;

            using namespace SyntaxTree;

            HomeDefPtr home (scope_->dynamic_type<HomeDef> ());

            if (home == 0)
            {
              //@@ internal compiler error
            }

            ComponentDefPtr component (home->manages ());

            home_factory_ = HomeFactoryDeclPtr (
              new HomeFactoryDecl (SimpleName (id->lexeme ()),
                                   component->name (),
                                   scope_));
          }

          virtual void
          parameter (IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id)
          {
            if (trace_) cerr << "parameter " << type_id << " "
                             << name_id << endl;

            using namespace IDL3::SyntaxTree;

            Name type_name (type_id->lexeme ());

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
              ScopedName sn = scope_->table ().resolve (
                type_name,
                scope_->name (),
                scope_->peek_order (),
                p);

              HomeFactoryParameterPtr p (
                new HomeFactoryParameter (sn,
                                          SimpleName (name_id->lexeme ()),
                                          scope_->table ()));

              home_factory_->insert (p);
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid home factory declaration" << endl;
              cerr << "no type with name \'"
                   << type_name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid home factory declaration" << endl;
              cerr << "declaration with name \'" << type_name
                   << "\' visible from scope \'" << scope_->name ()
                   << "\' is not a type declaration" << endl;
              cerr << "using non-type as home factory parameter type is illegal"
                   << endl;
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;
            scope_->insert (home_factory_);
            home_factory_ = SyntaxTree::HomeFactoryDeclPtr ();
          }
        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
          SyntaxTree::HomeFactoryDeclPtr home_factory_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP
