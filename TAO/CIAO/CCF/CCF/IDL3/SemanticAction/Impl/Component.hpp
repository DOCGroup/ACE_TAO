// file      : CCF/IDL3/SemanticAction/Impl/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP

#include "CCF/IDL3/SemanticAction/Component.hpp"

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
        class Component :
          public virtual SemanticAction::Component,
          public virtual ScopeBase<SyntaxTree::ComponentDeclPtr>
        {
        public:
          virtual
          ~Component () throw () {}


          Component (bool trace, SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::ComponentDeclPtr> (scope),
                trace_ (trace),
                name_ (""),
                inherits_ ("")
          {
          }

          virtual void
          begin (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "component " << id << endl;

            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          inherits (IdentifierPtr const& id)
          {
            if (trace_) cerr << "inherits " << id << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct InheritancePredicate : public DeclarationTable::ResolvePredicate
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

                if (type != "component") throw IncompatibleType (type);

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

              inherits_ = sn;
            }
            catch (InheritancePredicate::IncompatibleType const& e)
            {
              cerr << "error: invalid inheritance specification" << endl;

              cerr << "inheritance of ";
              cerr << "component \'" << name_ <<"\' from "
                   << e.type << " \'" << name << "\' is illegal" << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              cerr << "no component with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              cerr << "no defined component with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
              cerr << "inheritance from forward-declared component is illegal"
                   << endl;
            }
          }


          virtual void
          supports (IdentifierPtr const& id)
          {
            if (trace_) cerr << "supports " << id << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct SupportsPredicate :
  public DeclarationTable::ResolvePredicate
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

                // Spec doesn't say anything about which interfaces
                // component can support.
                if (type != "unconstrained interface")
                  throw IncompatibleType (type);

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

              if (supports_.insert (sn).second == false)
              {
                cerr << "error: invalid support specification" << endl;
                cerr << "directly supporting interface \'"
                     << sn << "\' more than once is illegal" << endl;
              }
            }
            catch (SupportsPredicate::IncompatibleType const& e)
            {
              cerr << "error: invalid support specification" << endl;

              cerr << "specifying " << e.type << " \'" << name
                   << "\' in component\'s support clause is illegal"
                   << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid support specification" << endl;
              cerr << "no interface with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid support specification" << endl;
              cerr << "no defined interface with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
              cerr << "supporting forward-declared interface is illegal"
                   << endl;
            }
          }

          virtual void
          open_scope ()
          {
            using namespace SyntaxTree;

            ComponentDefPtr def;

            if (inherits_ == ScopedName (""))
            {
              def = ComponentDefPtr (new ComponentDef (
                                       name_,
                                       scope_,
                                       supports_));
            }
            else
            {
              def = ComponentDefPtr (new ComponentDef (
                                       name_,
                                       scope_,
                                       inherits_,
                                       supports_));
            }

            scope_->insert (def);
            push (def);
            scope_ = def;

            name_ = SimpleName (""); //indicate that we are done
            inherits_  = ScopedName ("");
            supports_.clear ();
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

            using namespace SyntaxTree;

            if (name_ != SimpleName (""))
            {
              ComponentDeclPtr decl (new ComponentForwardDecl (name_, scope_));
              scope_->insert (decl);
            }
            else
            {
              pop ();
            }
          }
        private:
          bool trace_;
          SyntaxTree::SimpleName name_;
          SyntaxTree::ScopedName inherits_;
          SyntaxTree::ScopedNameSet supports_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_COMPONENT_HPP
