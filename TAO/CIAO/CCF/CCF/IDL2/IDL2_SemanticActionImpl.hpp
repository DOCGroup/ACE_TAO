// $Id$
#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SemanticAction.hpp"
#include "CCF/IDL2/IDL2_SyntaxTree.hpp"
#include "CCF/IDL2/IDL2_SyntaxTree_BuiltIn.hpp"

#include <stack>

// needed for include handling
#include <fstream>

//@@tmp
#include <iostream>
using std::cerr;
using std::endl;

namespace IDL2
{
  namespace SemanticAction
  {
    namespace Impl
    {

      /*
        Design notes:

        (1) I am using stack-based approach to handle cases like this:

            struct A
            {
              struct B
              {
                struct C
                {
                  ...
                } c;
              } b;
            };

            Even though you can't write the same let's say for interface
            I am still using stack approach for consistency and possible
            meta-coding.

      */

      using ReferenceCounting::strict_cast;

      //
      //
      //
      template <typename NodeTypePtr>
      class ScopeBase : public virtual SemanticAction::Scope
      {
      protected:
        virtual
        ~ScopeBase () throw () {}

        ScopeBase (SyntaxTree::ScopePtr& scope)
            : scope_ (scope)
        {
        }

      protected:
        void
        push (NodeTypePtr const& t)
        {
          stack_.push (t);
        }

        void
        pop ()
        {
          stack_.pop ();
        }

        NodeTypePtr&
        top ()
        {
          return stack_.top ();
        }

      protected:
        SyntaxTree::ScopePtr& scope_;
        std::stack<NodeTypePtr> stack_;
      };


      //
      //
      //
      class Include : public virtual SemanticAction::Include
      {
      public:
        virtual
        ~Include () throw () {}

        Include (SemanticActionFactory& action_factory,
                 SyntaxTree::TranslationRegionPtr const& region,
                 SyntaxTree::ScopePtr& scope)
            : action_factory_ (action_factory),
              scope_ (scope)
        {
          stack_.push (region);
        }

        virtual void
        begin (StringLiteralPtr const& sl)
        {
          cerr << "include \"" << sl << "\"" << endl;

          /*
            @@ this code is experimentla and temporarlily disabled
          using namespace SyntaxTree;

          std::ifstream ifs (sl->value ().c_str ());
          if (!ifs.is_open ())
          {
            cerr << "error: unable to open included file \'"
                 << sl << "\'" << endl;
          }

          TranslationRegionPtr r (
            new TranslationRegion (sl->value (),
                                   stack_.top ()->table (),
                                   stack_.top ()->create_order ()));

          stack_.top ()->insert (r);
          stack_.push (r);

          scope_ = stack_.top ()->scope ();

          //@@ this code is highly experimental
          IDL2::LexicalAnalyzer lexer (ifs);

          TokenStream token_stream;

          //@@ bad token comparison
          for (TokenPtr token = lexer.next ();
               token.in () != lexer.eos.in ();
               token = lexer.next ())
          {
            token_stream.push_back (token);
          }

          if (token_stream.size () == 0)
          {
            // Empty file
            return;
          }

          IDL2::Parser parser (lexer, action_factory_);

          bool result = Details::parse (token_stream.begin (),
                                        token_stream.end (),
                                        parser.start ());
          */
        }

        virtual void
        end ()
        {
          cerr << "end" << endl;

          stack_.pop ();
          scope_ = stack_.top ()->scope ();
        }

      public:
        SemanticActionFactory& action_factory_;
        SyntaxTree::ScopePtr& scope_;
        std::stack<SyntaxTree::TranslationRegionPtr> stack_;
      };

      //
      //
      //
      class Module : public virtual SemanticAction::Module,
                     public virtual ScopeBase<SyntaxTree::ModulePtr>
      {
      public:
        virtual
        ~Module () throw () {}


        Module (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::ModulePtr> (scope)
        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "module " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          ModulePtr m (new SyntaxTree::Module (name, scope_));

          scope_->insert (m);
          push (m);
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


      //
      //
      //
      class Interface : public virtual SemanticAction::Interface,
                        public virtual ScopeBase<SyntaxTree::InterfaceDeclPtr>
      {
      public:
        virtual
        ~Interface () throw () {}

        Interface (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::InterfaceDeclPtr> (scope),
              name_ ("")
        {
        }

        virtual void
        begin (Qualifier::Value qualifier, SimpleIdentifierPtr const& id)
        {
          cerr << qualifier << " interface " << id << endl;

          qualifier_ = qualifier;
          name_ = SyntaxTree::SimpleName (id->value ());
        }

        virtual void
        inherits (IdentifierPtr const& id)
        {
          using namespace SyntaxTree;

          cerr << " inherits: " << id << endl;

          Name name (id->value ());

          struct InheritancePredicate :
             public DeclarationTable::ResolvePredicate
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
              std::string type = d->declaration_type ();

              bool passed = false;

              switch (q_)
              {
              case Qualifier::UNCONSTRAINED:
                {
                  if (type == "unconstrained interface" ||
                      type == "abstract interface") passed = true;
                  break;
                }
              case Qualifier::LOCAL:
                {
                  if (type == "unconstrained interface" ||
                      type == "abstract interface" ||
                      type == "local interface") passed = true;
                  break;
                }
              case Qualifier::ABSTRACT:
                {
                  if (type == "abstract interface") passed = true;
                    break;
                }
              }

              if (!passed) throw IncompatibleType (type);

              return strict_cast<TypeDecl> (d)->defined ();
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
        }

        virtual void
        end ()
        {
          using namespace SyntaxTree;

          cerr << "end" << endl;

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
        Qualifier::Value qualifier_;
        SyntaxTree::SimpleName name_;
        SyntaxTree::ScopedNameSet inherits_;
      };


      class Attribute : public virtual SemanticAction::Attribute
      {
      public:
        virtual
        ~Attribute () throw () {}

        Attribute (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

      public:
        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "attribute " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              //@@ need extension to declaration type system
              //   e.g. something like this:
              //   "long;built-in type def;type def;type decl;declaration"
              return strict_cast<TypeDecl> (d) != 0;
            }
          } p;

          try
          {
            ScopedName sn = scope_->table ().resolve (
              name,
              scope_->name (),
              scope_->peek_order (),
              p);

            type_ = sn;
          }
          catch (DeclarationTable::NameNotFound const&)
          {
            cerr << "error: invalid attribute declaration" << endl;
            cerr << "no type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid attribute declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not a type declaration" << endl;
            cerr << "using non-type as an attribute type is illegal"
                 << endl;
          }

        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;
          SimpleName name (id->value ());

          AttributeDeclPtr a (
            new SyntaxTree::AttributeDecl (name, type_, scope_));

          scope_->insert (a);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };


      //
      //
      //
      class Operation : public virtual SemanticAction::Operation
      {
      public:

        virtual
        ~Operation () throw () {}

        Operation (SyntaxTree::ScopePtr& current)
            : scope_ (current)
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
              return strict_cast<TypeDecl> (d) != 0;
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
          cerr << "operation " << type_id << " " << name_id << endl;

          using namespace SyntaxTree;

          //@@ Why don't I have a default c-tor?
          ScopedName type_name ("");

          if (lookup_type (Name (type_id->value ()), type_name))
          {
            operation_ = OperationDeclPtr (
              new OperationDecl (SimpleName (name_id->value ()),
                                 type_name,
                                 scope_));
          }
        }


        virtual void
        parameter (Direction::Value direction,
                   IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          cerr << "parameter "
               << direction << " " << type_id << " " << name_id << endl;

          using namespace SyntaxTree;

          ScopedName type_name ("");

          if (lookup_type (Name (type_id->value ()), type_name) &&
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
                                      SimpleName (name_id->value ()),
                                      scope_->table ()));
            operation_->insert (p);
          }
        }

        virtual void
        end ()
        {
          cerr << "end" << endl;
          if (operation_ != 0)
          {
            scope_->insert (operation_);
            operation_ = SyntaxTree::OperationDeclPtr ();
          }
        }
      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::OperationDeclPtr operation_;
      };
    }
  }

  //
  //
  //
  class SemanticActionFactoryImpl : public virtual SemanticActionFactory
  {
  public:
    virtual
    ~SemanticActionFactoryImpl () throw () {}

    SemanticActionFactoryImpl (SyntaxTree::TranslationRegionPtr const& r)
        : scope_ (r->scope ()),

          include_ (*this, r, scope_),
          module_ (scope_),
          interface_ (scope_),
          attribute_ (scope_),
          operation_ (scope_)
    {
    }

    virtual SemanticAction::Include&
    include ()
    {
      return include_;
    }

    virtual SemanticAction::Module&
    module ()
    {
      return module_;
    }

    virtual SemanticAction::Interface&
    interface ()
    {
      return interface_;
    }

    virtual SemanticAction::Attribute&
    attribute ()
    {
      return attribute_;
    }

    virtual SemanticAction::Operation&
    operation ()
    {
      return operation_;
    }

  protected:
    SyntaxTree::ScopePtr scope_;
    SyntaxTree::TranslationRegionPtr region_;

    SemanticAction::Impl::Include include_;
    SemanticAction::Impl::Module module_;
    SemanticAction::Impl::Interface interface_;
    SemanticAction::Impl::Attribute attribute_;
    SemanticAction::Impl::Operation operation_;
  };
}

#endif // CCF_IDL2_SEMANTIC_ACTION_IMPL_HPP
