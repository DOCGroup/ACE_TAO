// file      : CCF/IDL2/SemanticAction/Impl/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP

#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

#include "CCF/IDL2/SemanticAction/Interface.hpp"

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
        class Interface : public virtual SemanticAction::Interface,
                          public virtual ScopeBase<SyntaxTree::InterfaceDeclPtr>
        {
        public:
          virtual
          ~Interface () throw () {}

          Interface (bool trace,
                     SyntaxTree::ScopePtr& scope)
              : ScopeBase<SyntaxTree::InterfaceDeclPtr> (scope),
                trace_ (trace),
                name_ ("")
          {
          }

          virtual void
          begin_abstract (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "abstract interface " << id << endl;

            qualifier_ = Qualifier::ABSTRACT;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }


          virtual void
          begin_local (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "local interface " << id << endl;

            qualifier_ = Qualifier::LOCAL;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          begin_unconstrained (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "unconstrained interface " << id << endl;

            qualifier_ = Qualifier::UNCONSTRAINED;
            name_ = SyntaxTree::SimpleName (id->lexeme ());
          }

          virtual void
          inherits (IdentifierPtr const& id)
          {
            using namespace SyntaxTree;

            if (trace_) cerr << " inherits: " << id << endl;

            Name name (id->lexeme ());

            struct InheritancePredicate : public DeclarationTable::ResolvePredicate
            {
              InheritancePredicate (Qualifier::Value q) : q_ (q) {}

              struct IncompatibleType :
                public DeclarationTable::ResolutionFailure
              {
                IncompatibleType (std::string const& t) : type (t) {}
                std::string type;
              };

              virtual bool
              test (DeclarationPtr const& d) throw (IncompatibleType)
              {
                bool passed = false;

                switch (q_)
                {
                case Qualifier::UNCONSTRAINED:
                  {
                    if (d->is_a<AbstractInterfaceDecl> () ||
                        d->is_a<UnconstrainedInterfaceDecl> ()) passed = true;

                    break;
                  }
                case Qualifier::LOCAL:
                  {
                    if (d->is_a<LocalInterfaceDecl> () ||
                        d->is_a<AbstractInterfaceDecl> () ||
                        d->is_a<UnconstrainedInterfaceDecl> ()) passed = true;

                    break;
                  }
                case Qualifier::ABSTRACT:
                  {
                    if (d->is_a<AbstractInterfaceDecl> ()) passed = true;

                    break;
                  }
                }

                if (!passed) throw IncompatibleType (d->declaration_class ());

                return d->dynamic_type<TypeDecl> ()->defined ();
              }

            private:
              Qualifier::Value q_;
            } p (qualifier_);

            try
            {
              ScopedName sn = scope_->table ().resolve (
                name,
                scope_->name (),
                scope_->peek_order (),
                p);

              if (inherits_.insert (sn).second == false)
              {
                cerr << "error: invalid inheritance specification" << endl;
                cerr << "directly inheriting from interface \'"
                     << sn << "\' more than once is illegal" << endl;
              }
            }
            catch (InheritancePredicate::IncompatibleType const& e)
            {
              cerr << "error: invalid inheritance specification" << endl;

              cerr << "inheritance of ";

              switch (qualifier_)
              {
              case Qualifier::UNCONSTRAINED:
                {
                  cerr << "unconstrained ";
                  break;
                }
              case Qualifier::LOCAL:
                {
                  cerr << "local ";
                  break;
                }
              case Qualifier::ABSTRACT:
                {
                  cerr << "abstract ";
                  break;
                }
              }

              cerr << "interface \'" << name_ <<"\' from "
                   << e.type << " \'" << name << "\' is illegal" << endl;
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              cerr << "no interface with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid inheritance specification" << endl;
              cerr << "no defined interface with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
              cerr << "inheritance from forward-declared interface is illegal"
                   << endl;
            }
          }

          virtual void
          open_scope ()
          {
            if (trace_) cerr << "scope open" << endl;

            using namespace SyntaxTree;

            InterfaceDefPtr def;

            switch (qualifier_)
            {
            case Qualifier::UNCONSTRAINED:
              {
                def = new UnconstrainedInterfaceDef (name_, scope_, inherits_);
                break;
              }
            case Qualifier::LOCAL:
              {
                def = new LocalInterfaceDef (name_, scope_, inherits_);
                break;
              }
            case Qualifier::ABSTRACT:
              {
                def = new AbstractInterfaceDef (name_, scope_, inherits_);
                break;
              }
            }

            scope_->insert (def);
            push (def);
            scope_ = def;

            name_ = SimpleName (""); //indicate that we are done
            inherits_.clear ();
          }

          virtual void
          close_scope ()
          {
            scope_ = scope_->scope ();
            if (trace_) cerr << "scope close" << endl;
          }

          virtual void
          end ()
          {
            using namespace SyntaxTree;

            if (trace_) cerr << "end" << endl;

            if (name_ != SimpleName (""))
            {
              InterfaceDeclPtr decl;

              switch (qualifier_)
              {
              case Qualifier::UNCONSTRAINED:
                {
                  decl = new UnconstrainedInterfaceForwardDecl (name_, scope_);
                  break;
                }
              case Qualifier::LOCAL:
                {
                  decl = new LocalInterfaceForwardDecl (name_, scope_);
                  break;
                }
              case Qualifier::ABSTRACT:
                {
                  decl = new AbstractInterfaceForwardDecl (name_, scope_);
                  break;
                }
              }

              scope_->insert (decl);
            }
            else
            {
              pop ();
            }
          }
        private:

          struct Qualifier
          {
            enum Value
            {
              ABSTRACT,
              LOCAL,
              UNCONSTRAINED
            };
          };

          bool trace_;

          Qualifier::Value qualifier_;
          SyntaxTree::SimpleName name_;
          SyntaxTree::ScopedNameSet inherits_;
        };

      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_INTERFACE_HPP
