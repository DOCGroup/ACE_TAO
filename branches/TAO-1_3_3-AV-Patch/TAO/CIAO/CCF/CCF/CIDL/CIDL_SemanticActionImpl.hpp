// $Id$
#ifndef CCF_CIDL_SEMANTIC_ACTION_IMPL_HPP
#define CCF_CIDL_SEMANTIC_ACTION_IMPL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_SemanticAction.hpp"
#include "CCF/IDL3/IDL3_SemanticActionImpl.hpp"

namespace CIDL
{
  namespace SemanticAction
  {
    namespace Impl
    {
      using IDL2::SemanticAction::Impl::ScopeBase;

      class Composition : public virtual SemanticAction::Composition,
                          public virtual ScopeBase<SyntaxTree::CompositionPtr>
      {
      public:
        virtual
        ~Composition () throw () {}

        Composition (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::CompositionPtr> (scope)
        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id, Category::Value c)
        {
          cerr << "composition " << c << " " << id << endl;

          SyntaxTree::SimpleName name (id->value ());

          SyntaxTree::Composition::Category::Value category =
            SyntaxTree::Composition::Category::SESSION;

          if (c == Category::ENTITY)
          {
            category = SyntaxTree::Composition::Category::ENTITY;
          }
          else if (c == Category::PROCESS)
          {
            category = SyntaxTree::Composition::Category::PROCESS;
          }
          else if (c == Category::SERVICE)
          {
            category = SyntaxTree::Composition::Category::SERVICE;
          }

          SyntaxTree::CompositionPtr cp (
            new SyntaxTree::Composition (name, category, scope_));

          scope_->insert (cp);
          push (cp);
        }

        virtual void
        open_scope ()
        {
          scope_ = top ();
        }

        virtual void
        close_scope ()
        {
          scope_ = scope_->scope ();
        }

        virtual void
        end ()
        {
          pop ();
          cerr << "end" << endl;
        }
      };

      class HomeExecutor :
        public virtual SemanticAction::HomeExecutor,
        public virtual ScopeBase<SyntaxTree::HomeExecutorPtr>
      {
      public:
        virtual
        ~HomeExecutor () throw () {}

        HomeExecutor (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::HomeExecutorPtr> (scope),
              name_ (""),
              implements_ (""),
              manages_ ("")

        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "home executor " << id << endl;
          name_ = SyntaxTree::SimpleName (id->value ());
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
          cerr << "implements " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct ImplementsPredicate :
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
              std::string type = d->declaration_type ();

              if (type != "home") throw IncompatibleType (type);

              return strict_cast<TypeDecl> (d)->defined ();
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
          cerr << "manages " << id << endl;
          manages_ = SyntaxTree::SimpleName (id->value ());
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
          cerr << "end" << endl;

          using namespace SyntaxTree;

          HomeExecutorPtr he (new SyntaxTree::HomeExecutor (
                                name_,
                                scope_,
                                implements_,
                                manages_));
          scope_->insert (he);
        }
      private:
        SyntaxTree::SimpleName name_;
        SyntaxTree::ScopedName implements_;
        SyntaxTree::SimpleName manages_;
      };
    }
  }

  //@@ Maybe I should bring this class back in to Impl namespace.
  class SemanticActionFactoryImpl :
    public virtual CIDL::SemanticActionFactory,
    public virtual IDL3::SemanticActionFactoryImpl
  {
  public:
    virtual
    ~SemanticActionFactoryImpl () throw () {}

    SemanticActionFactoryImpl (SyntaxTree::TranslationRegionPtr const& r)
        : IDL2::SemanticActionFactoryImpl (r),
          IDL3::SemanticActionFactoryImpl (r),
          composition_ (scope_),
          home_executor_ (scope_)
    {
    }
  public:
    virtual SemanticAction::Composition&
    composition ()
    {
      return composition_;
    }

    virtual SemanticAction::HomeExecutor&
    home_executor ()
    {
      return home_executor_;
    }

  private:
    SemanticAction::Impl::Composition composition_;
    SemanticAction::Impl::HomeExecutor home_executor_;
  };
}

#endif // CCF_CIDL_SEMANTIC_ACTION_IMPL_HPP
