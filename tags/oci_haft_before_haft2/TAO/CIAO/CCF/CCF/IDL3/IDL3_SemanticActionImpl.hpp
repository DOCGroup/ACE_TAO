// $Id$
#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_SyntaxTree.hpp"
#include "CCF/IDL3/IDL3_SyntaxTree_EventType.hpp"

#include "CCF/IDL3/IDL3_SemanticAction.hpp"
#include "CCF/IDL2/IDL2_SemanticActionImpl.hpp"

//@@ needed for include handling
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

namespace IDL3
{
  namespace SemanticAction
  {
    namespace Impl
    {
      using IDL2::SemanticAction::Impl::ScopeBase;

      // Note: overriding IDL2 include to allow inclusiion of IDL3 files
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

          using namespace SyntaxTree;

          try
          {
            fs::path file_path (sl->value ());

            if(!include_file_set_.insert (file_path).second)
            {
              cerr << "warning: skipping already included file \'"
                   << sl << "\'" << endl;
              return;
            }

            fs::ifstream ifs;
            ifs.exceptions (ios_base::badbit | ios_base::failbit);
            ifs.open (file_path, std::ios_base::in);

            //@@ for some reason ifs throws exception if I don't reset it to
            //   original state. It probably has something to do with call to
            //   get after eof.
            ifs.exceptions (ios_base::iostate (0));

            TranslationRegionPtr r (
              new IncludeTranslationRegion (file_path,
                                            stack_.top ()->table (),
                                            stack_.top ()->create_order ()));

            stack_.top ()->insert (r);
            stack_.push (r);

            cerr << "push file scope: old scope " << scope_->order ();

            scope_ = stack_.top ()->scope ();

            cerr << "; new scope " << scope_->order () << endl;

            //@@ this code is highly experimental
            CCF::InputStreamAdapter isa (ifs);
            CCF::Preprocessor pp (isa);
            IDL3::LexicalAnalyzer lexer (pp);

            TokenStream token_stream;

            //@@ bad token comparison
            for (TokenPtr token = lexer.next ();
                 token.in () != lexer.eos.in ();
                 token = lexer.next ())
            {
              token_stream.push_back (token);
            }

            if (token_stream.size () != 0)
            {
              IDL3::Parser parser (lexer, action_factory_);

            /*bool result = */Details::parse (token_stream.begin (),
                                              token_stream.end (),
                                              parser.start ());
            }

            cerr << "pop file scope: old scope " << scope_->order ();

            stack_.pop ();
            scope_ = stack_.top ()->scope ();

            cerr << "; new scope " << scope_->order () << endl;
          }
          catch (fs::filesystem_error const&)
          {
            cerr << sl << ": error: unable to open in read mode" << endl;
          }
          catch (ios_base::failure const&)
          {
            cerr << sl << ": error: unable to open in read mode" << endl;
          }
        }

        virtual void
        end ()
        {
          cerr << "end" << endl;
        }

      public:
        SemanticActionFactory& action_factory_;
        SyntaxTree::ScopePtr& scope_;
        std::stack<SyntaxTree::TranslationRegionPtr> stack_;
        struct FilePathComparator
        {
          bool operator () (fs::path const& x, fs::path const& y) const
          {
            return x.string () < y.string ();
          }
        };

        std::set<fs::path, FilePathComparator> include_file_set_;
      };


      //
      //
      //
      class Component : public virtual SemanticAction::Component,
                        public virtual ScopeBase<SyntaxTree::ComponentDeclPtr>
      {
      public:
        virtual
        ~Component () throw () {}


        Component (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::ComponentDeclPtr> (scope),
              name_ (""),
              inherits_ ("")
        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "component " << id << endl;

          name_ = SyntaxTree::SimpleName (id->value ());
        }

        virtual void
        inherits (IdentifierPtr const& id)
        {
          cerr << "inherits " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct InheritancePredicate :
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

              if (type != "component") throw IncompatibleType (type);

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
          cerr << "supports " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

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
              std::string type = d->declaration_type ();

              // Spec doesn't say anything about which interfaces
              // component can support.
              if (type != "unconstrained interface")
                throw IncompatibleType (type);

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
          cerr << "end" << endl;

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
        SyntaxTree::SimpleName name_;
        SyntaxTree::ScopedName inherits_;
        SyntaxTree::ScopedNameSet supports_;
      };


      class Provides : public virtual SemanticAction::Provides
      {
      public:
        virtual
        ~Provides () throw () {}

        Provides (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "provides " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              return strict_cast<InterfaceDecl> (d) != 0;
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
            cerr << "error: invalid provides declaration" << endl;
            cerr << "no interface type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid provides declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not an interface type declaration" << endl;
            cerr << "using non-<interface type> in provides "
                 << "declaration is illegal" << endl;
          }
        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          DeclarationPtr d (
            new SyntaxTree::ProvidesDecl (name, type_, scope_));

          scope_->insert (d);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };

      class Uses : public virtual SemanticAction::Uses
      {
      public:
        virtual
        ~Uses () throw () {}

        Uses (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "uses " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              return strict_cast<InterfaceDecl> (d) != 0;
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
            cerr << "error: invalid uses declaration" << endl;
            cerr << "no interface type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid uses declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not an interface type declaration" << endl;
            cerr << "using non-<interface type> in uses "
                 << "declaration is illegal" << endl;
          }
        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          DeclarationPtr d (
            new SyntaxTree::UsesDecl (name, type_, scope_));

          scope_->insert (d);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };


      class Publishes : public virtual SemanticAction::Publishes
      {
      public:
        virtual
        ~Publishes () throw () {}

        Publishes (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "publishes " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              return strict_cast<EventTypeDef> (d) != 0;
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
            cerr << "error: invalid publishes declaration" << endl;
            cerr << "no event type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid publishes declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not a defined event type" << endl;
            cerr << "using non-<defined event type> in publishes "
                 << "declaration is illegal" << endl;
          }
        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          DeclarationPtr d (
            new SyntaxTree::PublishesDecl (name, type_, scope_));

          scope_->insert (d);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };

      class Emits : public virtual SemanticAction::Emits
      {
      public:
        virtual
        ~Emits () throw () {}

        Emits (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "emits " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              return strict_cast<EventTypeDef> (d) != 0;
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
            cerr << "error: invalid emits declaration" << endl;
            cerr << "no event type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid emits declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not a defined event type" << endl;
            cerr << "using non-<defined event type> in emits "
                 << "declaration is illegal" << endl;
          }
        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          DeclarationPtr d (
            new SyntaxTree::EmitsDecl (name, type_, scope_));

          scope_->insert (d);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };

      class Consumes : public virtual SemanticAction::Consumes
      {
      public:
        virtual
        ~Consumes () throw () {}

        Consumes (SyntaxTree::ScopePtr& current)
            : scope_ (current),
              type_ ("")
        {
        }

        virtual void
        type (IdentifierPtr const& id)
        {
          cerr << "consumes " << id;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct Predicate : public DeclarationTable::ResolvePredicate
          {
            virtual bool
            test (DeclarationPtr const& d) throw ()
            {
              return strict_cast<EventTypeDef> (d) != 0;
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
            cerr << "error: invalid consumes declaration" << endl;
            cerr << "no event type with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid consumes declaration" << endl;
            cerr << "declaration with name \'" << name
                 << "\' visible from scope \'" << scope_->name ()
                 << "\' is not a defined event type" << endl;
            cerr << "using non-<defined event type> in consumes "
                 << "declaration is illegal" << endl;
          }
        }

        virtual void
        name (SimpleIdentifierPtr const& id)
        {
          cerr << " " << id << endl;

          using namespace SyntaxTree;

          SimpleName name (id->value ());

          DeclarationPtr d (
            new SyntaxTree::ConsumesDecl (name, type_, scope_));

          scope_->insert (d);

          type_ = ScopedName ("");
        }

      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::ScopedName type_;
      };

      //
      //
      //
      class EventType : public virtual SemanticAction::EventType,
                        public virtual ScopeBase<SyntaxTree::EventTypeDeclPtr>
      {
      public:
        virtual
        ~EventType () throw () {}

        EventType (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::EventTypeDeclPtr> (scope),
              name_ ("")
        {
        }

      public:

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "eventtype " << id << endl;
          name_ = SyntaxTree::SimpleName (id->value ());
        }

        virtual void
        inherits (IdentifierPtr const& id)
        {
          cerr << "inherits " << id << endl;
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
          cerr << "end" << endl;
          pop ();
        }

      private:
        SyntaxTree::SimpleName name_;
        SyntaxTree::ScopedNameSet inherits_;
      };


      //
      //
      //
      class Home : public virtual SemanticAction::Home,
                   public virtual ScopeBase<SyntaxTree::HomeDefPtr>
      {
      public:
        virtual
        ~Home () throw () {}

        Home (SyntaxTree::ScopePtr& scope)
            : ScopeBase<SyntaxTree::HomeDefPtr> (scope),
              name_ (""),
              inherits_ (""),
              manages_ ("")

        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "home " << id << endl;
          name_ = SyntaxTree::SimpleName (id->value ());
        }


        virtual void
        inherits (IdentifierPtr const& id)
        {
          cerr << "inherits " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct InheritancePredicate :
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

              // This is not really necessary since home cannot be
              // forward-declared (yet ;-).
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

            inherits_ = sn;
          }
          catch (InheritancePredicate::IncompatibleType const& e)
          {
            cerr << "error: invalid inheritance specification" << endl;

            cerr << "inheritance of ";
            cerr << "home \'" << name_ <<"\' from "
                 << e.type << " \'" << name << "\' is illegal" << endl;
          }
          catch (DeclarationTable::NameNotFound const&)
          {
            cerr << "error: invalid inheritance specification" << endl;
            cerr << "no home with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid inheritance specification" << endl;
            cerr << "no defined home with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
            cerr << "inheritance from forward-declared home is illegal"
                 << endl;
          }
        }


        virtual void
        supports (IdentifierPtr const& id)
        {
          cerr << "supports " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

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
              std::string type = d->declaration_type ();

              // Spec doesn't say anything about which interfaces
              // component can support.
              if (type != "unconstrained interface")
                throw IncompatibleType (type);

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
                 << "\' in home\'s support clause is illegal"
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
        manages (IdentifierPtr const& id)
        {
          cerr << "manages " << id << endl;

          using namespace SyntaxTree;

          Name name (id->value ());

          struct ManagesPredicate :
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

              if (type != "component") throw IncompatibleType (type);

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

            manages_ = sn;
          }
          catch (ManagesPredicate::IncompatibleType const& e)
          {
            cerr << "error: invalid manages specification" << endl;

            cerr << "specifying " << e.type << " \'" << name
                 << "\' in home manages clause is illegal" << endl;
          }
          catch (DeclarationTable::NameNotFound const&)
          {
            cerr << "error: invalid manages specification" << endl;
            cerr << "no component with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
          }
          catch (DeclarationTable::PredicateNotMet const&)
          {
            cerr << "error: invalid manages specification" << endl;
            cerr << "no defined component with name \'"
                 << name << "\' visible from scope \'"
                 << scope_->name () << "\'" << endl;
            cerr << "managing of forward-declared component is illegal"
                 << endl;
          }
        }

        virtual void
        open_scope ()
        {
          using namespace SyntaxTree;

          HomeDefPtr def (new HomeDef (
                            name_,
                            scope_,
                            inherits_,
                            supports_,
                            manages_));

          if (inherits_ == ScopedName (""))
          {
            def = HomeDefPtr (new HomeDef (
                                name_,
                                scope_,
                                supports_,
                                manages_));
          }
          else
          {
            def = HomeDefPtr (new HomeDef (
                                name_,
                                scope_,
                                inherits_,
                                supports_,
                                manages_));
          }


          scope_->insert (def);
          push (def);
          scope_ = def;

          name_ = SimpleName (""); //indicate that we are done
          inherits_  = ScopedName ("");
          supports_.clear ();
          manages_  = ScopedName ("");
        }

        virtual void
        close_scope ()
        {
          scope_ = scope_->scope ();
        }

        virtual void
        end ()
        {
          cerr << "end" << endl;

          using namespace SyntaxTree;

          if (name_ != SimpleName (""))
          {
            // this is not supported by spec yet
          }
          else
          {
            pop ();
          }
        }
      private:
        SyntaxTree::SimpleName name_;
        SyntaxTree::ScopedName inherits_;
        SyntaxTree::ScopedNameSet supports_;
        SyntaxTree::ScopedName manages_;
      };

      //
      //
      //
      //@@ Name is inconsistent with HomeFactoryDecl
      class HomeFactory : public virtual SemanticAction::HomeFactory
      {
      public:
        virtual
        ~HomeFactory () throw ()
        {
        }

        HomeFactory (SyntaxTree::ScopePtr& current)
            : scope_ (current)
        {
        }

        virtual void
        begin (SimpleIdentifierPtr const& id)
        {
          cerr << "home factory " << id << endl;

          using namespace SyntaxTree;

          HomeDefPtr home (scope_->dynamic_type<HomeDef> ());

          if (home == 0)
          {
            //@@ internal compiler error
          }

          ComponentDefPtr component (home->manages ());

          home_factory_ = HomeFactoryDeclPtr (
            new HomeFactoryDecl (SimpleName (id->value ()),
                                 component->name (),
                                 scope_));
        }

        virtual void
        parameter (IdentifierPtr const& type_id,
                   SimpleIdentifierPtr const& name_id)
        {
          cerr << "parameter " << type_id << " " << name_id << endl;

          using namespace IDL3::SyntaxTree;

          Name type_name (type_id->value ());

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
            ScopedName sn = scope_->table ().resolve (
              type_name,
              scope_->name (),
              scope_->peek_order (),
              p);

            HomeFactoryParameterPtr p (
              new HomeFactoryParameter (sn,
                                        SimpleName (name_id->value ()),
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
          cerr << "end" << endl;
          scope_->insert (home_factory_);
          home_factory_ = SyntaxTree::HomeFactoryDeclPtr ();
        }
      private:
        SyntaxTree::ScopePtr& scope_;
        SyntaxTree::HomeFactoryDeclPtr home_factory_;
      };
    }
  }

  class SemanticActionFactoryImpl :
    public virtual IDL3::SemanticActionFactory,
    public virtual IDL2::SemanticActionFactoryImpl
  {
  public:
    virtual
    ~SemanticActionFactoryImpl () throw () {}

    SemanticActionFactoryImpl (SyntaxTree::TranslationRegionPtr const& r)
        : IDL2::SemanticActionFactoryImpl (r),

          include_ (*this, r, scope_),
          component_ (scope_),
          provides_ (scope_),
          uses_ (scope_),
          publishes_ (scope_),
          emits_ (scope_),
          consumes_ (scope_),
          event_type_ (scope_),
          home_ (scope_),
          home_factory_ (scope_)
    {

    }

  public:

    virtual SemanticAction::Include&
    include ()
    {
      return include_;
    }

    virtual SemanticAction::Component&
    component ()
    {
      return component_;
    }

    virtual SemanticAction::Provides&
    provides ()
    {
      return provides_;
    }

    virtual SemanticAction::Uses&
    uses ()
    {
      return uses_;
    }

    virtual SemanticAction::Publishes&
    publishes ()
    {
      return publishes_;
    }

    virtual SemanticAction::Emits&
    emits ()
    {
      return emits_;
    }

    virtual SemanticAction::Consumes&
    consumes ()
    {
      return consumes_;
    }


    virtual SemanticAction::EventType&
    event_type ()
    {
      return event_type_;
    }

    virtual SemanticAction::Home&
    home ()
    {
      return home_;
    }

    virtual SemanticAction::HomeFactory&
    home_factory ()
    {
      return home_factory_;
    }


  private:

    SemanticAction::Impl::Include include_;

    SemanticAction::Impl::Component component_;
    SemanticAction::Impl::Provides provides_;
    SemanticAction::Impl::Uses uses_;
    SemanticAction::Impl::Publishes publishes_;
    SemanticAction::Impl::Emits emits_;
    SemanticAction::Impl::Consumes consumes_;
    SemanticAction::Impl::EventType event_type_;
    SemanticAction::Impl::Home home_;
    SemanticAction::Impl::HomeFactory home_factory_;
  };
}

#endif // CCF_IDL3_SEMANTIC_ACTION_IMPL_HPP
