// $Id$
#include "ServantSourceGenerator.hpp"

#include "CCF/CIDL/CIDL_SyntaxTree.hpp"
#include "CCF/CIDL/CIDL_Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IDLStream.hpp"

#include "Literals.hpp"

#include <ostream>

using namespace CIDL;
using namespace CIDL::SyntaxTree;
using namespace StringLiterals;

namespace
{
  // Generates the name of an operation's return type.
  //
  //
  class ReturnTypeNameEmitter : public SourceEmitterBase,
                                public virtual Traversal::Void,
                                public virtual Traversal::Boolean,
                                public virtual Traversal::Long,
                                public virtual Traversal::String,
                                public virtual Traversal::LocalInterfaceDecl
  {
  public:
    ReturnTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_void (VoidPtr const&)
    {
      os << "void";
    }

    virtual void
    visit_boolean (BooleanPtr const&)
    {
      os << "::CORBA::Boolean";
    }

    virtual void
    visit_long (LongPtr const&)
    {
      os << "::CORBA::Long";
    }

    virtual void
    visit_string (StringPtr const&)
    {
      os << "char *";
    }

    virtual void
    visit_local_interface_decl (LocalInterfaceDeclPtr const& i)
    {
      os << i->name () << "_ptr";
    }
  };

  // Generates the typename of an IN argument.
  //
  //
  class INArgTypeNameEmitter : public SourceEmitterBase,
                               public virtual Traversal::Boolean,
                               public virtual Traversal::Long,
                               public virtual Traversal::String
  {
  public:
    INArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_boolean (BooleanPtr const&)
    {
      os << "::CORBA::Boolean";
    }

    virtual void
    visit_long (LongPtr const&)
    {
      os << "::CORBA::Long";
    }

    virtual void
    visit_string (StringPtr const&)
    {
      os << "const char *";
    }
  };

  // Generates the typename of an OUT argument.
  //
  //
  class OUTArgTypeNameEmitter : public SourceEmitterBase,
                                public virtual Traversal::Boolean,
                                public virtual Traversal::Long,
                                public virtual Traversal::String
  {
  public:
    OUTArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_boolean (BooleanPtr const&)
    {
      os << "::CORBA::Boolean_out";
    }

    virtual void
    visit_long (LongPtr const&)
    {
      os << "::CORBA::Long_out";
    }

    virtual void
    visit_string (StringPtr const&)
    {
      os << "::CORBA::String_out";
    }
  };

  // Generates the typename of an INOUT argument.
  //
  //
  class INOUTArgTypeNameEmitter : public SourceEmitterBase,
                                  public virtual Traversal::Boolean,
                                  public virtual Traversal::Long,
                                  public virtual Traversal::String
  {
  public:
    INOUTArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_boolean (BooleanPtr const&)
    {
      os << "::CORBA::Boolean &";
    }

    virtual void
    visit_long (LongPtr const&)
    {
      os << "::CORBA::Long &";
    }

    virtual void
    visit_string (StringPtr const&)
    {
      os << "char *&";
    }
  };

  // Generates an argument's identifier.
  //
  //
  class ArgNameEmitter : public SourceEmitterBase,
                         public virtual Traversal::TypeDecl
  {
  public:
    ArgNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_operation_parameter_pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    visit_operation_parameter_post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    visit_comma (CommaPtr const&)
    {
      os << ",";
    }
  };

  // Generates an attribute's accessor operation.
  //
  //
  class GetAttributeEmitter : public SourceEmitterBase,
                              public virtual Traversal::AttributeDecl,
                              public virtual Traversal::ComponentDef
  {
  public:
    GetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : SourceEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    visit_attribute_post (AttributeDeclPtr const& a)
    {
      os << endl
         << a->name ().scope ().simple ()
         << "_Servant::" << a->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{" << endl
         << "return this->executor_->" << a->name ().simple ()
         << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl << endl;
    }
  };

  // Generates an attribute's mutator operation.
  //
  //
  class SetAttributeEmitter : public SourceEmitterBase,
                              public virtual Traversal::AttributeDecl
  {
  public:
    SetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : SourceEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    visit_attribute_pre (AttributeDeclPtr const& a)
    {
      os << "void " << endl
         << a->name ().scope ().simple ()
         << "_Servant::" << a->name ().simple () << " (" << endl;
    }

    virtual void
    visit_attribute_post (AttributeDeclPtr const& a)
    {
      os << " val" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{" << endl
         << "this->executor_->" << a->name ().simple () << " (" << endl
         << "val" << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl << endl;
    }
  };

  // Generates the statement that delegates the operation
  // to the executor.
  //
  class OperationExecEmitter : public SourceEmitterBase,
                               public virtual Traversal::OperationParameter,
                               public virtual Traversal::Comma,
                               public virtual Traversal::OperationDecl
  {
  public:
    OperationExecEmitter (ostream& os_,
                          ArgNameEmitter* arg_name_emitter_)
      : SourceEmitterBase (os_),
        OperationParameter (arg_name_emitter_,
                            arg_name_emitter_,
                            arg_name_emitter_)
    {
    }

    virtual void
    visit_operation_parameter_pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    visit_operation_parameter_post (OperationParameterPtr const& op)
    {
      os << op->name ();
    }

    virtual void
    visit_comma (CommaPtr const&)
    {
      os << ",";
    }
  };

  // Generates an operation definition.
  //
  //
  class OperationEmitter : public SourceEmitterBase,
                           public virtual Traversal::OperationParameter,
                           public virtual Traversal::Comma,
                           public virtual Traversal::UnconstrainedInterfaceDef,
                           public virtual Traversal::OperationDecl
  {
  public:
    OperationEmitter (ostream& os_,
                      ReturnTypeNameEmitter* return_type_name_emitter,
                      INArgTypeNameEmitter* inarg_type_name_emitter_,
                      OUTArgTypeNameEmitter* outarg_type_name_emitter_,
                      INOUTArgTypeNameEmitter* inoutarg_type_name_emitter_)
      : SourceEmitterBase (os_),
        OperationParameter (inarg_type_name_emitter_,
                            outarg_type_name_emitter_,
                            inoutarg_type_name_emitter_),
        OperationDecl (return_type_name_emitter)
    {
    }

    virtual void
    visit_operation_decl_name (OperationDeclPtr const& d)
    {
      os << endl
         << d->name ().scope ().simple () << "_Servant::"
         << d->name ().simple () << " (";
    }

    virtual void
    visit_operation_parameter_pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    visit_operation_parameter_post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    visit_comma (CommaPtr const&)
    {
      os << ",";
    }

    virtual void
    visit_operation_decl_post (OperationDeclPtr const& d)
    {
      bool void_return_type =
        (d->type ()->dynamic_type<IDL2::SyntaxTree::Void> () != 0);

      // @@@ (JP) Must add support for user exceptions.
      os << endl
         << (d->begin () == d->end () ? STRS[ENV_SNGL_SRC] : STRS[ENV_SRC])
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{" << endl
         << (void_return_type ? "" : "return ")
         << "this->executor_->" << d->name ().simple () << " (";

      ArgNameEmitter arg_name_emitter (os);
      OperationExecEmitter operation_exec_emitter (os,
                                                   &arg_name_emitter);
      d->accept (&operation_exec_emitter);

      os << endl
           << (d->begin () == d->end () ? STRS[ENV_SNGL_ARG] : STRS[ENV_ARG])
           << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates the operation associated with a home factory definition.
  //
  //
  class FactoryEmitter : public SourceEmitterBase,
                         public virtual Traversal::HomeFactoryDecl,
                         public virtual Traversal::OperationParameter,
                         public virtual Traversal::Comma
  {
  public:
    FactoryEmitter (ostream& os_,
                    INArgTypeNameEmitter* inarg_type_name_emitter_)
      : SourceEmitterBase (os_),
        OperationParameter (inarg_type_name_emitter_,
                            inarg_type_name_emitter_,
                            inarg_type_name_emitter_)
    {
    }

    virtual void
    visit_home_factory_decl_type (HomeFactoryDeclPtr const& f)
    {
      HomeDefPtr def (f->scope ()->dynamic_type<SyntaxTree::HomeDef> ());
      os << def->manages ()->name () << "_ptr";
    }

    virtual void
    visit_home_factory_decl_name (HomeFactoryDeclPtr const& f)
    {
      os << endl
         << f->name ().scope ().simple () << "_Servant::"
         << f->name ().simple () << " (";
    }

    virtual void
    visit_operation_parameter_pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    visit_operation_parameter_post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    visit_comma (CommaPtr const&)
    {
      os << ",";
    }

    virtual void
    visit_home_factory_decl_post (HomeFactoryDeclPtr const& f)
    {
      os << endl
           << (f->begin () == f->end () ? STRS[ENV_SNGL_SRC] : STRS[ENV_SRC])
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::Components::EnterpriseComponent_var _ciao_ec =" << endl
           << "this->executor_->" << f->name ().simple () << " (";

      ArgNameEmitter arg_name_emitter (os);
      OperationExecEmitter operation_exec_emitter (os,
                                                   &arg_name_emitter);
      f->accept (&operation_exec_emitter);

      // If this cast is 0, a syntax error has slipped through.
      HomeDefPtr def (f->scope ()->dynamic_type<SyntaxTree::HomeDef> ());
      ScopedName retval = def->manages ()->name ();

      os << endl
           << (f->begin () == f->end () ? STRS[ENV_SNGL_ARG] : STRS[ENV_ARG])
           << ");" << endl
           << "ACE_CHECK_RETURN (" << retval << "::_nil ());"
           << endl << endl
           << retval.scope () << "::CCM_" << retval.simple ()
           << "_var _ciao_comp =" << endl
           << retval.scope () << "::CCM_" << retval.simple ()
           << "::_narrow (" << endl
           << "_ciao_ec.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl << endl
           << "return this->_ciao_activate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an operation of a supported interface.
  //
  //
  class SupportedOperationEmitter : public virtual OperationEmitter,
                                    public virtual Traversal::OperationParameter,
                                    public virtual Traversal::OperationDecl
  {
  private:
    SimpleName context_;

  public:
    SupportedOperationEmitter (ostream& os_,
                               ReturnTypeNameEmitter* return_type_name_emitter,
                               INArgTypeNameEmitter* inarg_type_name_emitter_,
                               OUTArgTypeNameEmitter* outarg_type_name_emitter_,
                               INOUTArgTypeNameEmitter* inoutarg_type_name_emitter_,
                               SimpleName context)
      : OperationEmitter (os_,
                          return_type_name_emitter,
                          inarg_type_name_emitter_,
                          outarg_type_name_emitter_,
                          inoutarg_type_name_emitter_),
        OperationParameter (inarg_type_name_emitter_,
                            outarg_type_name_emitter_,
                            inoutarg_type_name_emitter_),
        OperationDecl (return_type_name_emitter),
        context_ (context)
    {
    }

    virtual void
    visit_operation_decl_name (OperationDeclPtr const& d)
    {
      os << endl
           << context_ << "_Servant::"
           << d->name ().simple () << " (";
    }
  };

  // Generates an IF statement that compares the publisher name
  // argument with an item from an iterator, and calls the
  // appropriate subscribe operation upon success.
  class SubscribeEmitter : public SourceEmitterBase,
                           public virtual Traversal::ComponentDef,
                           public virtual Traversal::PublishesDecl
  {
  public:
    SubscribeEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_publishes_decl (PublishesDeclPtr const& p)
    {
      os << "if (ACE_OS::strcmp (publisher_name, \""
           << p->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << p->type ()->name () << "Consumer_var _ciao_consumer =" << endl
           << p->type ()->name () << "Consumer::_narrow (" << endl
           << "subscribe" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "if (::CORBA::is_nil (_ciao_consumer.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}" << endl << endl
           << "return this->subscribe_" << p->name ().simple ()
           << " (" << endl
           << "_ciao_consumer.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an IF statement that compares the publisher name
  // argument with an item from an iterator, and calls the
  // appropriate unsubscribe operation upon success.
  class UnsubscribeEmitter : public SourceEmitterBase,
                             public virtual Traversal::ComponentDef,
                             public virtual Traversal::PublishesDecl
  {
  public:
    UnsubscribeEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_publishes_decl (PublishesDeclPtr const& p)
    {
      os << "if (ACE_OS::strcmp (publisher_name, \""
           << p->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << "return this->unsubscribe_" << p->name ().simple ()
           << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an IF statement that compares the receptacle name
  // argument with an item from an iterator, and calls the
  // appropriate connect operation upon success.
  class ConnectEmitter : public SourceEmitterBase,
                         public virtual Traversal::ComponentDef,
                         public virtual Traversal::UsesDecl
  {
  public:
    ConnectEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_uses_decl (UsesDeclPtr const& u)
    {
      os << "if (ACE_OS::strcmp (name, \""
           << u->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << u->type ()->name () << "_var _ciao_conn =" << endl
           << u->type ()->name () << "::_narrow (" << endl
           << "connection" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "if (::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}" << endl << endl
           << "// Simplex connect." << endl
           << "this->connect_" << u->name ().simple () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl << endl
           << "return 0;" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an IF statement that compares the receptacle name
  // argument with an item from an iterator, and calls the
  // appropriate disconnect operation upon success.
  class DisconnectEmitter : public SourceEmitterBase,
                            public virtual Traversal::ComponentDef,
                            public virtual Traversal::UsesDecl
  {
  public:
    DisconnectEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_uses_decl (UsesDeclPtr const& u)
    {
      os << "if (ACE_OS::strcmp (name, \""
           << u->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << "// Simplex disconnect." << endl
           << "return this->disconnect_" << u->name ().simple ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an IF statement that compares the consumer name
  // argument with an item from an iterator, and calls the
  // appropriate get_consumer operation upon success.
  class GetConsumerEmitter : public SourceEmitterBase,
                             public virtual Traversal::ComponentDef,
                             public virtual Traversal::ConsumesDecl
  {
  public:
    GetConsumerEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_consumes_decl (ConsumesDeclPtr const& c)
    {
      os << "if (ACE_OS::strcmp (sink_name, \""
           << c->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << "return this->get_consumer_" << c->name ().simple ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  // Generates an IF statement that compares the facet name
  // argument with an item from an iterator, and calls the
  // appropriate provide operation upon success.
  class ProvidesFacetEmitter : public SourceEmitterBase,
                               public virtual Traversal::ComponentDef,
                               public virtual Traversal::ProvidesDecl
  {
  public:
    ProvidesFacetEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_provides_decl (ProvidesDeclPtr const& p)
    {
      os << "if (ACE_OS::strcmp (name, \""
           << p->name ().simple () << "\") == 0)" << endl
           << "{" << endl
           << "return this->provide_" << p->name ().simple ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;
    }
  };

  //
  // FacetEmitter generates facet servant class definitions.
  //
  class FacetEmitter : public SourceEmitterBase,
                       public virtual Traversal::UnconstrainedInterfaceDef,
                       public virtual Traversal::OperationDecl,
                       public virtual Traversal::AttributeDecl
  {
  private:
    ReturnTypeNameEmitter return_type_name_emitter_;
    INArgTypeNameEmitter inarg_type_name_emitter_;
    INOUTArgTypeNameEmitter inoutarg_type_name_emitter_;
    OUTArgTypeNameEmitter outarg_type_name_emitter_;

    OperationEmitter operation_emitter_;

  public:
    FacetEmitter (ostream& os_)
      : SourceEmitterBase (os_),
        return_type_name_emitter_ (os_),
        inarg_type_name_emitter_ (os_),
        inoutarg_type_name_emitter_ (os_),
        outarg_type_name_emitter_ (os_),
        operation_emitter_ (os_,
                            &return_type_name_emitter_,
                            &inarg_type_name_emitter_,
                            &outarg_type_name_emitter_,
                            &inoutarg_type_name_emitter_)
    {
    }

    virtual void
    visit_unconstrained_interface_def_pre (
      UnconstrainedInterfaceDefPtr const& i)
    {
      // If we are at file scope, we create a namespace anyway.
      if (i->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << STRS[GLUE_NS] << endl
              << "{" << endl;
      }

      os << i->name ().simple () << "_Servant::" << i->name ().simple ()
           << "_Servant (" << endl
           << i->name ().scope () << "::CCM_" << i->name ().simple ()
           << "_ptr executor," << endl
           << "::Components::CCMContext_ptr c)" << endl
           << ": executor_ (" << i->name ().scope () << "::CCM_"
           << i->name ().simple () << "::_duplicate (executor))," << endl
           << "ctx_ (::Components::CCMContext::_duplicate (c))" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << i->name ().simple () << "_Servant::~" << i->name ().simple ()
           << "_Servant (void)" << endl
           << "{" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_operation_decl (OperationDeclPtr const& d)
    {
      d->accept (&operation_emitter_);
    }

    virtual void
    visit_attribute_decl (AttributeDeclPtr const& a)
    {
      // TODO
    }

    virtual void
    visit_unconstrained_interface_def_post (
      UnconstrainedInterfaceDefPtr const& i)
    {
      os << "CORBA::Object_ptr" << endl
           << i->name ().simple () << "_Servant::_get_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << "ACE_THROW_SPEC ((CORBA::SystemException))" << endl
           << "{" << endl
           << "::Components::SessionContext_var sc =" << endl
           << "::Components::SessionContext::_narrow (" << endl
           << "this->ctx_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl << endl
           << "if (! CORBA::is_nil (sc.in ()))" << endl
           << "{" << endl
           << "return sc->get_CCM_object (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl
           << "::Components::EntityContext_var ec =" << endl
           << "::Components::EntityContext::_narrow (" << endl
           << "this->ctx_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl << endl
           << "if (! CORBA::is_nil (ec.in ()))" << endl
           << "{" << endl
           << "return ec->get_CCM_object (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl
           << "ACE_THROW_RETURN (CORBA::INTERNAL (), 0);" << endl
           << "}" << endl;

      // Close the CIAO_GLUE namespace, if we opened one.
      if (i->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };

  //
  // Generates component context interface definitions.
  //
  class ContextEmitter : public SourceEmitterBase,
                         public virtual Traversal::ComponentDef,
                         public virtual Traversal::UsesDecl,
                         public virtual Traversal::PublishesDecl,
                         public virtual Traversal::EmitsDecl
  {
  public:
    ContextEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    visit_component_def_pre (ComponentDefPtr const& c)
    {
      // If we are at file scope, we create a namespace anyway.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "namespace CIAO_GLUE" << endl
              << "{" << endl;
      }

      os << c->name ().simple () << "_Context::"
           << c->name ().simple () << "_Context (" << endl
           << "::Components::CCMHome_ptr home," << endl
           << "::CIAO::Session_Container *c," << endl
           << c->name ().simple () << "_Servant *sv)" << endl
           << ": home_ (::Components::CCMHome::_duplicate (home))," << endl
           << "container_ (c)," << endl
           << "servant_ (sv)" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << c->name ().simple () << "_Context::~"
           << c->name ().simple () << "_Context (void)" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << "// Operations from ::Components::CCMContext." << endl << endl;

      os << "::Components::Principal_ptr" << endl
           << c->name ().simple () << "_Context::"
           << "get_caller_principal (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl
           << "::Components::Principal::_nil ());" << endl
           << "}" << endl << endl;

      os << "::Components::CCMHome_ptr" << endl
           << c->name ().simple () << "_Context::"
           << "get_CCM_home (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "return ::Components::CCMHome::_duplicate (this->home_.in ());"
           << endl
           << "}" << endl << endl;

      os << "CORBA::Boolean" << endl
           << c->name ().simple () << "_Context::"
           << "get_rollback_only (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IS] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::Transaction::UserTransaction_ptr" << endl
           << c->name ().simple () << "_Context::"
           << "get_user_transaction (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IS] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl
           << "::Components::Transaction::UserTransaction::_nil ());" << endl
           << "}" << endl << endl;

      os << "CORBA::Boolean" << endl
           << c->name ().simple () << "_Context::"
           << "is_caller_in_role (" << endl
           << "const char *role" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_UNUSED_ARG (role);" << endl
           << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->name ().simple () << "_Context::"
           << "set_rollback_only (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IS] << "))" << endl
           << "{" << endl
           << "ACE_THROW (CORBA::NO_IMPLEMENT ());" << endl
           << "}" << endl << endl;

      os << "// Operations from ::Components::SessionContext interface."
           << endl << endl;

      os << "CORBA::Object_ptr" << endl
           << c->name ().simple () << "_Context::"
           << "get_CCM_object (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IS] << "))" << endl
           << "{" << endl
           << "if (CORBA::is_nil (this->component_.in ()))" << endl
           << "{" << endl
           << "CORBA::Object_var obj =" << endl
           << "this->container_->get_objref (" << endl
           << "this->servant_" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl << endl
           << "this->component_ =" << endl
           << c->name () << "::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl << endl
           << "if (CORBA::is_nil (this->component_.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl
           << "}" << endl << endl
           << "return " << c->name () << "::_duplicate (" << endl
           << "this->component_.in ());" << endl
           << "}" << endl << endl;

      os << "// Operations for " << c->name ().simple () << " receptacles"
           << " and event sources," << endl
           << "// defined in " << c->name ().scope () << "::CCM_"
           << c->name ().simple () << "_Context."
           << endl << endl;
    }

    virtual void
    visit_uses_decl (UsesDeclPtr const& d)
    {
      os << d->type ()->name () << "_ptr" << endl
           << d->scope ()->name ().simple () << "_Context::get_connection_"
           << d->name ().simple ()
           << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "return " << d->type ()->name () << "::_duplicate (" << endl
           << "this->ciao_uses_" << d->name ().simple ()
           << "_.in ());" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << d->scope ()->name ().simple () << "_Context::connect_"
           << d->name ().simple () << " (" << endl
           << d->type ()->name () << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << "if (! CORBA::is_nil (this->ciao_uses_"
           << d->name ().simple () << "_.in ()))" << endl
           << "{" << endl
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}" << endl << endl
           << "if (CORBA::is_nil (c))" << endl
           << "{" << endl
           << "ACE_THROW (" << STRS[EXCP_IC] << " ());" << endl
           << "}" << endl << endl
           << "this->ciao_uses_" << d->name ().simple () << "_ =" << endl
           << d->type ()-> name () << "::_duplicate (c);" << endl
           << "}" << endl << endl;

      os << d->type ()->name () << "_ptr" << endl
           << d->scope ()->name ().simple () << "_Context::disconnect_"
           << d->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "if (CORBA::is_nil (this->ciao_uses_"
           << d->name ().simple () << "_.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl
           << d->type ()->name () << "::_nil ());" << endl
           << "}" << endl << endl
           << "return this->ciao_uses_" << d->name ().simple ()
           << "_._retn ();" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_publishes_decl (PublishesDeclPtr const& d)
    {
      os << "void" << endl
           << d->scope ()->name ().simple () << "_Context::push_"
           << d->name ().simple () << " (" << endl
           << d->type ()->name () << " *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_Active_Map_Manager<" << d->type ()->name ()
           << "Consumer_var>::iterator end =" << endl
           << "this->ciao_publishes_" << d->name ().simple ()
           << "_map_.end ();" << endl << endl
           << "for (ACE_Active_Map_Manager<" << d->type ()->name ()
           << "Consumer_var>::iterator iter =" << endl
           << "this->ciao_publishes_" << d->name ().simple ()
           << "_map_.begin ();" << endl
           << "iter != end;" << endl
           << "++iter)" << endl
           << "{" << endl
           << "ACE_Active_Map_Manager<" << d->type ()->name ()
           << "Consumer_var>::ENTRY &entry = *iter;" << endl
           << d->type ()->name () << "Consumer_var c =" << endl
           << d->type ()->name () << "Consumer::_narrow (" << endl
           << "entry.int_id_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << "entry.int_id_->push_" << d->type ()->name ().simple ()
           << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl
           << "}" << endl
           << "}" << endl << endl;

      os << STRS[COMP_CK] << " *" << endl
           << d->scope ()->name ().simple () << "_Context::subscribe_"
           << d->name ().simple () << " (" << endl
           << d->type ()->name () << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{" << endl
           << "if (CORBA::is_nil (c))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << endl
           << "}" << endl << endl
           << d->type ()->name () << "Consumer_var sub =" << endl
           << d->type ()->name () << "Consumer::_duplicate (c);"
           << endl << endl
           << "ACE_Active_Map_Manager_Key key;" << endl
           << "this->ciao_publishes_" << d->name ().simple ()
           << "_map_.bind (sub.in (), key);" << endl << endl
           << "sub._retn ();" << endl << endl
           << STRS[COMP_CK] << "_var retv = "
           << "new ::CIAO::Map_Key_Cookie (key);"
           << endl
           << "return retv._retn ();" << endl
           << "}" << endl << endl;

      os << d->type ()->name () << "Consumer_ptr" << endl
           << d->scope ()->name ().simple () << "_Context::unsubscribe_"
           << d->name ().simple () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << d->type ()->name () << "Consumer_var retv;" << endl
           << "ACE_Active_Map_Manager_Key key;" << endl << endl
           << "if (ck == 0 || ::CIAO::Map_Key_Cookie::extract (ck, key) == -1)"
           << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl
           << d->type ()->name () << "Consumer::_nil ());" << endl
           << "}" << endl << endl
           << "if (this->ciao_publishes_" << d->name ().simple ()
           << "_map_.unbind (key, retv) != 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl
           << d->type ()->name () << "Consumer::_nil ());" << endl
           << "}" << endl << endl
           << "return retv._retn ();" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_emits_decl (EmitsDeclPtr const& d)
    {
      os << "void" << endl
           << d->scope ()->name ().simple () << "_Context::push_"
           << d->name ().simple () << " (" << endl
           << d->type ()->name () << " *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "this->ciao_emits_" << d->name ().simple ()
           << "_consumer_->push_" << d->name ().simple () << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << d->scope ()->name ().simple () << "_Context::connect_"
           << d->name ().simple () << " (" << endl
           << d->type ()-> name () << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{" << endl
           << "if (CORBA::is_nil (c))" << endl
           << "{" << endl
           << "ACE_THROW (CORBA::BAD_PARAM ());" << endl
           << "}" << endl << endl
           << "if (! CORBA::is_nil (this->ciao_emits_" << d->name ().simple ()
           << "_consumer_.in ()))" << endl
           << "{" << endl
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}" << endl << endl
           << "this->ciao_emits_" << d->name ().simple ()
           << "_consumer_ = c;" << endl
           << "}" << endl << endl;

      os << d->type ()->name () << "Consumer_ptr" << endl
           << d->scope ()->name ().simple () << "_Context::disconnect_"
           << d->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "if (CORBA::is_nil (this->ciao_emits_" << d->name ().simple ()
           << "_consumer_.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl
           << d->type ()->name () << "Consumer::_nil ());" << endl
           << "}" << endl << endl
           << "return this->ciao_emits_" << d->name ().simple ()
           << "_consumer_._retn ();" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_component_def_post (ComponentDefPtr const& c)
    {
      // Close the CIAO_GLUE namespace, if we opened one.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
 };

  //
  // Generates component servant interface definitions.
  //
  class ServantEmitter : public SourceEmitterBase,
                         public virtual Traversal::ComponentDef,
                         public virtual Traversal::ProvidesDecl,
                         public virtual Traversal::UsesDecl,
                         public virtual Traversal::PublishesDecl,
                         public virtual Traversal::ConsumesDecl,
                         public virtual Traversal::EmitsDecl,
                         public virtual Traversal::AttributeDecl
  {
  private:
    ReturnTypeNameEmitter return_type_name_emitter_;
    INArgTypeNameEmitter inarg_type_name_emitter_;
    INOUTArgTypeNameEmitter inoutarg_type_name_emitter_;
    OUTArgTypeNameEmitter outarg_type_name_emitter_;

    OperationEmitter operation_emitter_;
    GetAttributeEmitter get_attribute_emitter_;
    SetAttributeEmitter set_attribute_emitter_;

  public:
    ServantEmitter (ostream& os_)
      : SourceEmitterBase (os_),
        return_type_name_emitter_ (os_),
        inarg_type_name_emitter_ (os_),
        inoutarg_type_name_emitter_ (os_),
        outarg_type_name_emitter_ (os_),
        operation_emitter_ (os_,
                            &return_type_name_emitter_,
                            &inarg_type_name_emitter_,
                            &outarg_type_name_emitter_,
                            &inoutarg_type_name_emitter_),
        get_attribute_emitter_ (os_,
                                &return_type_name_emitter_),
        set_attribute_emitter_ (os_,
                                &return_type_name_emitter_)
    {
    }

    virtual void
    visit_component_def_pre (ComponentDefPtr const& c)
    {
      // If we are at file scope, we create a namespace anyway.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "namespace CIAO_GLUE" << endl
             << "{" << endl;
      }

      os << c->name ().simple () << "_Servant::"
           << c->name ().simple () << "_Servant (" << endl
           << c->name ().scope () << "::CCM_" << c->name ().simple ()
           << "_ptr exe," << endl
           << "::Components::CCMHome_ptr h," << endl
           << "::CIAO::Session_Container *c)" << endl
           << ": executor_ (" << c->name ().scope () << "::CCM_"
           << c->name ().simple () << "::_duplicate (exe))," << endl
           << "container_ (c)" << endl
           << "{" << endl
           << "this->context_ = "
           << "new " << c->name ().simple () << "_Context (h, c, this);"
           << endl << endl
           << "ACE_TRY_NEW_ENV" << endl
           << "{" << endl
           << "::Components::SessionComponent_var scom =" << endl
           << "::Components::SessionComponent::_narrow (" << endl
           << "exe" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_TRY_CHECK;" << endl << endl
           << "if (! ::CORBA::is_nil (scom.in ()))" << endl
           << "{" << endl
           << "scom->set_session_context (" << endl
           << "this->context_" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl
           << "}" << endl
           << "ACE_CATCHANY" << endl
           << "{" << endl
           << "}" << endl
           << "ACE_ENDTRY;" << endl
           << "}" << endl << endl;

      os << c->name ().simple () << "_Servant::~"
           << c->name ().simple () << "_Servant (void)" << endl
           << "{" << endl
           << "ACE_TRY_NEW_ENV" << endl
           << "{" << endl
           << "::Components::SessionComponent_var scom =" << endl
           << "::Components::SessionComponent::_narrow (" << endl
           << "this->executor_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_TRY_CHECK;" << endl << endl
           << "if (! ::CORBA::is_nil (scom.in ()))" << endl
           << "{" << endl
           << "scom->ccm_remove (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl
           << "}" << endl
           << "ACE_CATCHANY" << endl
           << "{" << endl
           << "}" << endl
           << "ACE_ENDTRY;" << endl << endl
           << "this->context_->_remove_ref ();" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_attribute_decl (AttributeDeclPtr const& a)
    {
      a->accept (&get_attribute_emitter_);
      a->accept (&set_attribute_emitter_);
    }

    virtual void
    visit_provides_decl (ProvidesDeclPtr const& p)
    {
      os << p->type ()->name () << "_ptr" << endl
           << p->scope ()->name ().simple ()
           << "_Servant::provide_" << p->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "if (::CORBA::is_nil (this->provide_"
           << p->name ().simple () << "_.in ()))" << endl
           << "{" << endl
           << p->name ().scope ().scope () << "::CCM_"
           << p->type ()->name ().simple ()
           << "_var fexe =" << endl
           << "this->executor_->get_" << p->name ().simple ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << p->type ()->name () << "::_nil ());"
           << endl << endl
           << "if (::CORBA::is_nil (fexe.in ()))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl
           << p->type ()->name () << "::_nil ());" << endl
           << "}" << endl << endl
           << p->type ()->name ().simple () << "_Servant *svt =" << endl
           << "new " << p->type ()->name ().simple () << "_Servant (" << endl
           << "fexe.in ()," << endl
           << "this->context_);" << endl
           << "PortableServer::ServantBase_var safe_servant (svt);"
           << endl << endl
           << "::CORBA::Object_var obj =" << endl
           << "this->container_->install_servant (" << endl
           << "svt" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << p->type ()->name () << "::_nil ());"
           << endl << endl
           << p->type ()->name () << "_var fo =" << endl
           << p->type ()->name () << "::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << p->type ()->name () << "::_nil ());"
           << endl << endl
           << "this->provide_" << p->name ().simple () << "_ = fo;" << endl
           << "}" << endl << endl
           << "return " << p->type ()->name ()
           << "::_duplicate (this->provide_"
           << p->name ().simple () << "_.in ());" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_uses_decl (UsesDeclPtr const& p)
    {
      os << "void" << endl
           << p->scope ()->name ().simple () << "_Servant::connect_"
           << p->name ().simple () << " (" << endl
           << p->type ()->name () << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << "this->context_->connect_" << p->name ().simple () << " ("
           << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << p->type ()->name () << "_ptr" << endl
           << p->scope ()->name ().simple () << "_Servant::disconnect_"
           << p->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "return this->context_->disconnect_" << p->name ().simple ()
           << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;

      os << p->type ()->name () << "_ptr" << endl
           << p->scope ()->name ().simple ()
           << "_Servant::get_connection_" << p->name ().simple ()
           << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "return this->context_->get_connection_"
           << p->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_consumes_decl (ConsumesDeclPtr const& c)
    {
      os << c->scope ()-> name ().simple  () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant::" << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant (" << endl
           << c->scope ()->name ().scope () << "::CCM_"
           << c->scope ()->name ().simple () << "_ptr executor,"
           << endl
           << c->scope ()->name ().scope () << "::CCM_"
           << c->scope ()->name ().simple ()
           << "_Context_ptr c)" << endl
           << ": executor_ (" << c->scope ()->name ().scope ()
           << "::CCM_" << c->scope ()->name ().simple ()
           << "::_duplicate (executor))," << endl
           << "ctx_ (" << c->scope ()->name ().scope ()
           << "::CCM_" << c->scope ()->name ().simple ()
           << "_Context::_duplicate (c))" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant::~" << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant (void)" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << "CORBA::Object_ptr" << endl
           << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant::_get_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "return this->ctx_->get_CCM_object "
           << "(" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant::push_" << c->type ()->name ().simple ()
           << " (" << endl
           << c->type ()->name () << " *evt" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "this->executor_->push_" << c->name ().simple ()
           << " (" << endl
           << "evt" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "void" << endl
           << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant::push_event (" << endl
           << "::Components::EventBase *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_BET] << "))" << endl
           << "{" << endl
           << c->type ()->name () << "_var ev_type =" << endl
           << c->type ()->name () << "::_downcast (ev);" << endl << endl
           << "if (ev_type != 0)" << endl
           << "{" << endl
           << "this->push_" << c->type ()->name ().simple () << " (" << endl
           << "ev_type.in ()" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl << endl
           << "return;" << endl
           << "}" << endl << endl
           << "ACE_THROW (" << STRS[EXCP_BET] << " ());" << endl
           << "}" << endl << endl;

      os << c->type ()->name () << "Consumer_ptr" << endl
           << c->scope ()-> name ().simple () << "_Servant::"
           << "get_consumer_" << c->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "if (CORBA::is_nil (this->consumes_" << c->name ().simple ()
           << "_.in ()))" << endl
           << "{" << endl
           << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant *svt =" << endl
           << "new " << c->scope ()-> name ().simple () << "_Servant::"
           << c->type ()->name ().simple ()
           << "Consumer_" << c->name ().simple ()
           << "_Servant (" << endl
           << "this->executor_.in ()," << endl
           << "this->context_);" << endl
           << "PortableServer::ServantBase_var safe_servant (svt);"
           << endl << endl
           << "CORBA::Object_var obj =" << endl
           << "this->container_->install_servant (" << endl
           << "svt" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN ("
           << c->type ()->name () << "Consumer::_nil ());" << endl << endl
           << c->type ()->name () << "Consumer_var eco =" << endl
           << c->type ()->name () << "Consumer::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN ("
           << c->type ()->name () << "Consumer::_nil ());" << endl << endl
           << "this->consumes_" << c->name ().simple () << "_ = eco;" << endl
           << "}" << endl << endl
           << "return " << c->type ()->name ()
           << "Consumer::_duplicate (this->consumes_" << c->name ().simple ()
           << "_.in ());" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_emits_decl (EmitsDeclPtr const& e)
    {
      os << "void" << endl
           << e->scope ()->name ().simple () << "_Servant::connect_"
           << e->name ().simple () << " (" << endl
           << e->type ()->name () << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{" << endl
           << "this->context_->connect_" << e->name ().simple ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << e->type ()->name () << "Consumer_ptr" << endl
           << e->scope ()->name ().simple () << "_Servant::disconnect_"
           << e->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "return this->context_->disconnect_"
           << e->name ().simple () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_publishes_decl (PublishesDeclPtr const& p)
    {
      os << STRS[COMP_CK] << " *" << endl
           << p->scope ()->name ().simple ()
           << "_Servant::subscribe_" << p->name ().simple () << " (" << endl
           << p->type ()->name () << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{" << endl
           << "return this->context_->subscribe_" << p->name ().simple ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << p->type ()->name () << "Consumer_ptr" << endl
           << p->scope ()->name ().simple ()
           << "_Servant::unsubscribe_" << p->name ().simple () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << "return this->context_->unsubscribe_"
           << p->name ().simple () << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;
   }

    virtual void
    visit_component_def_post (ComponentDefPtr const& c)
    {
      os << "// Operations for Navigation interface." << endl << endl;

      os << "CORBA::Object_ptr" << endl
           << c->name ().simple () << "_Servant::provide_facet (" << endl
           << "const char *name" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "if (name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::BAD_PARAM ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl << endl;

      ProvidesFacetEmitter provides_facet_emitter (os);
      c->accept (&provides_facet_emitter);

      os << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl << endl;

      os << "::Components::FacetDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_all_facets (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::FacetDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_named_facets (" << endl
           << "const " << STRS[COMP_NAMES] << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "CORBA::Boolean" << endl
           << c->name ().simple () << "_Servant::same_component (" << endl
           << "CORBA::Object_ptr object_ref" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "if (::CORBA::is_nil (object_ref))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::BAD_PARAM (), 0);" << endl
           << "}" << endl << endl
           << "::CORBA::Object_var the_other =" << endl
           << "object_ref->_get_component (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "::CORBA::Object_var me =" << endl
           << "this->context_->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "return me->_is_equivalent (" << endl
           << "the_other.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "// Operations for Receptacles interface." << endl << endl;

      os << STRS[COMP_CK] << " *" << endl
           << c->name ().simple () << "_Servant::connect (" << endl
           << "const char *name," << endl
           << "::CORBA::Object_ptr connection" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_IC] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{" << endl
           << "if (name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
           << "}" << endl << endl;

      ConnectEmitter connect_emitter (os);
      c->accept (&connect_emitter);

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
           << "}" << endl << endl;

      os << "CORBA::Object_ptr" << endl
           << c->name ().simple () << "_Servant::disconnect (" << endl
           << "const char *name," << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_IC] << "," << endl
           << STRS[EXCP_CR] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "if (name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl << endl;

      DisconnectEmitter disconnect_emitter (os);
      c->accept (&disconnect_emitter);

      os << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl << endl;

      os << "::Components::ConnectionDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_connections ("
           << endl
           << "const char *name" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::ReceptacleDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_all_receptacles ("
           << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::ReceptacleDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_named_receptacles ("
           << endl
           << "const " << STRS[COMP_NAMES] << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "// Operations for Events interface." << endl << endl;

      os << STRS[COMP_ECB] << "_ptr" << endl
           << c->name ().simple () << "_Servant::get_consumer (" << endl
           << "const char *sink_name" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "if (sink_name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << STRS[COMP_ECB] << "::_nil ());" << endl
           << "}" << endl << endl;

      GetConsumerEmitter get_consumer_emitter (os);
      c->accept (&get_consumer_emitter);

      os << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << STRS[COMP_ECB] << "::_nil ());" << endl
           << "}" << endl << endl;

      os << STRS[COMP_CK] << " *" << endl
           << c->name ().simple () << "_Servant::subscribe (" << endl
           << "const char *publisher_name," << endl
           << STRS[COMP_ECB] << "_ptr subscribe" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_IC] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{" << endl
           << "if (publisher_name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);"
           << endl
           << "}" << endl << endl;

      SubscribeEmitter subscribe_emitter (os);
      c->accept (&subscribe_emitter);

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
           << "}" << endl << endl;

      os << STRS[COMP_ECB] << "_ptr" << endl
           << c->name ().simple () << "_Servant::unsubscribe ("
           << endl
           << "const char *publisher_name," << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << "if (publisher_name == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << STRS[COMP_ECB] << "::_nil ());" << endl
           << "}" << endl << endl;

      UnsubscribeEmitter unsubscribe_emitter (os);
      c->accept (&unsubscribe_emitter);

      os << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IN] << " ()," << endl
           << STRS[COMP_ECB] << "::_nil ());" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->name ().simple () << "_Servant::connect_consumer ("
           << endl
           << "const char *emitter_name," << endl
           << STRS[COMP_ECB] << "_ptr consumer" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{" << endl
           << "ACE_THROW (::CORBA::NO_IMPLEMENT ());" << endl
           << "}" << endl << endl;

      os << STRS[COMP_ECB] << "_ptr" << endl
           << c->name ().simple () << "_Servant::disconnect_consumer ("
           << endl
           << "const char *source_name" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::ConsumerDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_all_consumers ("
           << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::ConsumerDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_named_consumers ("
           << endl
           << "const " << STRS[COMP_NAMES] << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::EmitterDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_all_emitters ("
           << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::EmitterDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_named_emitters ("
           << endl
           << "const " << STRS[COMP_NAMES] << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::PublisherDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_all_publishers ("
           << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "::Components::PublisherDescriptions *" << endl
           << c->name ().simple () << "_Servant::get_named_publishers ("
           << endl
           << "const " << STRS[COMP_NAMES] << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
           << "}" << endl << endl;

      os << "// Operations for CCMObject interface." << endl << endl;

      os << "CORBA::IRObject_ptr" << endl
           << c->name ().simple () << "_Servant::get_component_def (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl
           << "::CORBA::IRObject::_nil ());" << endl
           << "}" << endl << endl;

      os << "::Components::CCMHome_ptr" << endl
           << c->name ().simple () << "_Servant::get_ccm_home (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "return this->context_->get_CCM_home "
           << "(ACE_ENV_SINGLE_ARG_PARAMETER);" << endl
           << "}" << endl << endl;

      os << "::Components::PrimaryKeyBase *" << endl
           << c->name ().simple () << "_Servant::get_primary_key (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NKA] << "))" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << STRS[EXCP_NKA] << " (), 0);" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->name ().simple ()
           << "_Servant::configuration_complete (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ICF] << "))" << endl
           << "{" << endl
           << "// CIAO to-do" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->name ().simple () << "_Servant::remove (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_RF] << "))" << endl
           << "{" << endl
           << "// CIAO to-do" << endl
           << "}" << endl << endl;

      os << "::Components::ComponentPortDescription *" << endl
           << c->name ().simple () << "_Servant::get_all_ports (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::Components::ComponentPortDescription_var retv =" << endl
           << "new OBV_Components::ComponentPortDescription;" << endl << endl
           << "::Components::FacetDescriptions_var facets_desc =" << endl
           << "this->get_all_facets (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "::Components::ReceptacleDescriptions_var receptacle_desc ="
           << endl
           << "this->get_all_receptacles (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "::Components::ConsumerDescriptions_var consumer_desc =" << endl
           << "this->get_all_consumers (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "::Components::EmitterDescriptions_var emitter_desc =" << endl
           << "this->get_all_emitters (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "::Components::PublisherDescriptions_var publisher_desc ="
           << endl
           << "this->get_all_publishers (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (0);" << endl << endl
           << "retv->facets (facets_desc.in ());" << endl
           << "retv->receptacles (receptacle_desc.in ());" << endl
           << "retv->consumers (consumer_desc.in ());" << endl
           << "retv->emitters (emitter_desc.in ());" << endl
           << "retv->publishers (publisher_desc.in ());" << endl << endl
           << "return retv._retn ();" << endl
           << "}" << endl << endl;

      os << "CORBA::Object_ptr" << endl
           << c->name ().simple () << "_Servant::_get_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::Components::SessionContext_var sc =" << endl
           << "::Components::SessionContext::_narrow (" << endl
           << "this->context_" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (::CORBA::Object::_nil ());" << endl << endl
           << "if (! ::CORBA::is_nil (sc.in ()))" << endl
           << "{" << endl
           << "return sc->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "}" << endl << endl
           << "::Components::EntityContext_var ec =" << endl
           << "::Components::EntityContext::_narrow (" << endl
           << "this->context_" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (::CORBA::Object::_nil ());" << endl << endl
           << "if (! ::CORBA::is_nil (ec.in ()))" << endl
           << "{" << endl
           << "return ec->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "}" << endl << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl
           << "::CORBA::Object::_nil ());" << endl
           << "}" << endl << endl;

      os << "// CIAO-specific operations." << endl << endl;

      os << "void" << endl
           << c->name ().simple () << "_Servant::_ciao_activate (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::Components::SessionComponent_var temp =" << endl
           << "::Components::SessionComponent::_narrow (" << endl
           << "this->executor_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << "if (! ::CORBA::is_nil (temp.in ()))" << endl
           << "{" << endl
           << "temp->ccm_activate (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << c->name ().simple () << "_Servant::_ciao_passivate (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::Components::SessionComponent_var temp =" << endl
           << "::Components::SessionComponent::_narrow (" << endl
           << "this->executor_.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << "if (! ::CORBA::is_nil (temp.in ()))" << endl
           << "{" << endl
           << "temp->ccm_passivate (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl
           << "}" << endl << endl;

      os << "// Supported operations." << endl << endl;

      SupportedOperationEmitter supported_operation_emitter (
          os,
          &return_type_name_emitter_,
          &inarg_type_name_emitter_,
          &outarg_type_name_emitter_,
          &inoutarg_type_name_emitter_,
          c->name ().simple ());

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        i->resolve ()->accept (&supported_operation_emitter);
      }

      // Close the CIAO_GLUE namespace, if we opened one.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };

  //
  // Generates the component home servant interface.
  //
  class HomeEmitter : public SourceEmitterBase,
                      public virtual Traversal::HomeDef,
                      public virtual Traversal::OperationDecl,
                      public virtual Traversal::AttributeDecl,
                      public virtual Traversal::HomeFactoryDecl
  {
  private:
    ReturnTypeNameEmitter return_type_name_emitter_;
    INArgTypeNameEmitter inarg_type_name_emitter_;
    INOUTArgTypeNameEmitter inoutarg_type_name_emitter_;
    OUTArgTypeNameEmitter outarg_type_name_emitter_;

    OperationEmitter operation_emitter_;
    FactoryEmitter factory_emitter_;

  public:
    HomeEmitter (ostream& os_)
      : SourceEmitterBase (os_),
        return_type_name_emitter_ (os_),
        inarg_type_name_emitter_ (os_),
        inoutarg_type_name_emitter_ (os_),
        outarg_type_name_emitter_ (os_),
        operation_emitter_ (os_,
                            &return_type_name_emitter_,
                            &inarg_type_name_emitter_,
                            &outarg_type_name_emitter_,
                            &inoutarg_type_name_emitter_),
        factory_emitter_ (os_,
                          &inarg_type_name_emitter_)
    {
    }

    virtual void
    visit_home_def_pre (HomeDefPtr const& h)
    {
      // If we are at file scope, we create a namespace anyway.
      if (h->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << STRS[GLUE_NS] << endl
              << "{" << endl;
      }

      os << h->name ().simple () << "_Servant::"
           << h->name ().simple () << "_Servant (" << endl
           << h->name ().scope () << "::CCM_" << h->name ().simple ()
           << "_ptr exe," << endl
           << "::CIAO::Session_Container *c)" << endl
           << ": executor_ (" << h->name ().scope () << "::CCM_"
           << h->name ().simple () << "::_duplicate (exe))," << endl
           << "container_ (c)" << endl
           << "{" << endl
           << "}" << endl << endl;

      os << h->name ().simple () << "_Servant::~"
           << h->name ().simple () << "_Servant (void)" << endl
           << "{" << endl
           << "}" << endl << endl;
    }

    virtual void
    visit_operation_decl (OperationDeclPtr const& d)
    {
      os << "// Home factory and other operations." << endl << endl;

      d->accept (&operation_emitter_);
    }

    virtual void
    visit_home_factory_decl (HomeFactoryDeclPtr const& f)
    {
      f->accept (&factory_emitter_);
    }

    virtual void
    visit_attribute_decl (AttributeDeclPtr const& a)
    {
      // TODO
    }

    virtual void
    visit_home_def_post (HomeDefPtr const& h)
    {

      os << "// Operations for keyless home interface." << endl << endl;

      os << "::Components::CCMObject_ptr" << endl
           << h->name ().simple () << "_Servant::create_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << "::Components::CreateFailure))" << endl
           << "{" << endl
           << "return this->create (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "// Operations for implicit home interface." << endl << endl;

      os << h->manages ()->name () << "_ptr" << endl
           << h->name ().simple () << "_Servant::create (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << "::Components::CreateFailure))" << endl
           << "{" << endl
           << "if (this->executor_.in () == 0)" << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl
           << h->manages ()->name () << "::_nil ());" << endl
           << "}" << endl << endl
           << "Components::EnterpriseComponent_var _ciao_ec =" << endl
           << "this->executor_->create (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name ()
           << "::_nil ());" << endl << endl
           << h->manages ()->name ().scope () << "::CCM_"
           << h->manages ()->name ().simple () << "_var _ciao_comp =" << endl
           << h->manages ()->name ().scope () << "::CCM_"
           << h->manages ()->name ().simple () << "::_narrow (" << endl
           << "_ciao_ec.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name ()
           << "::_nil ());" << endl << endl
           << "return this->_ciao_activate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "// Operations for CCMHome interface." << endl << endl;

      os << "::CORBA::IRObject_ptr" << endl
           << h->name ().simple () << "_Servant::get_component_def (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl
           << "::CORBA::IRObject::_nil ());" << endl
           << "}" << endl << endl;

      os << "::CORBA::IRObject_ptr" << endl
           << h->name ().simple () << "_Servant::get_home_def (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl
           << "::CORBA::IRObject::_nil ());" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << h->name ().simple () << "_Servant::remove_component (" << endl
           << "::Components::CCMObject_ptr comp" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_RF] << "))" << endl
           << "{" << endl
           << h->manages ()->name () << "_var _ciao_comp =" << endl
           << h->manages ()->name () << "::_narrow (" << endl
           << "comp" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << "if (CORBA::is_nil (_ciao_comp.in ()))" << endl
           << "{" << endl
           << "ACE_THROW (CORBA::INTERNAL ());" << endl
           << "}" << endl << endl
           << "_ciao_comp->remove (" << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << "this->_ciao_passivate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl << endl;

      os << "// CIAO-specific operations." << endl << endl;

      os << h->manages ()->name () << "_ptr" << endl
           << h->name ().simple ()
           << "_Servant::_ciao_activate_component (" << endl
           << h->manages ()->name ().scope () << "::CCM_"
           << h->manages ()->name ().simple () << "_ptr exe" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "::CORBA::Object_var hobj =" << endl
           << "this->container_->get_objref (" << endl
           << "this" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name () << "::_nil ());"
           << endl << endl
           << "::Components::CCMHome_var home =" << endl
           << "::Components::CCMHome::_narrow (" << endl
           << "hobj" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name () << "::_nil ());"
           << endl << endl
           << h->manages ()->name ().simple () << "_Servant *svt =" << endl
           << "new " << h->manages ()->name ().simple () << "_Servant ("
           << endl
           << "exe," << endl
           << "home.in ()," << endl
           << "this->container_);" << endl << endl
           << "PortableServer::ServantBase_var safe (svt);" << endl
           << "PortableServer::ObjectId_var oid;" << endl << endl
           << "CORBA::Object_var objref =" << endl
           << "this->container_->install_component (" << endl
           << "svt," << endl
           << "oid.out ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name () << "::_nil ());"
           << endl << endl
           << "svt->_ciao_activate (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name () << "::_nil ());"
           << endl << endl
           << h->manages ()->name () << "_var ho =" << endl
           << h->manages ()->name () << "::_narrow (" << endl
           << "objref.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK_RETURN (" << h->manages ()->name () << "::_nil ());"
           << endl << endl
           << "if (this->component_map_.bind (oid.in (), svt) == 0)" << endl
           << "{" << endl
           << "safe._retn ();" << endl
           << "}" << endl << endl
           << "return ho._retn ();" << endl
           << "}" << endl << endl;

      os << "void" << endl
           << h->name ().simple ()
           << "_Servant::_ciao_passivate_component (" << endl
           << h->manages ()->name () << "_ptr comp" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{" << endl
           << "PortableServer::ObjectId_var oid;" << endl << endl
           << "this->container_->uninstall_component (" << endl
           << "comp," << endl
           << "oid.out ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "ACE_CHECK;" << endl << endl
           << h->manages ()->name ().simple () << "_Servant *servant = 0;"
           << endl << endl
           << "if (this->component_map_.unbind (oid.in (), servant) == 0)"
           << endl
           << "{" << endl
           << "PortableServer::ServantBase_var safe (servant);" << endl
           << "servant->_ciao_passivate (" << STRS[ENV_SNGL_ARG] << ");"
           << endl
           << "ACE_CHECK;" << endl
           << "}" << endl
           << "}" << endl << endl;

      os << "// Supported operations." << endl << endl;

      SupportedOperationEmitter supported_operation_emitter (
          os,
          &return_type_name_emitter_,
          &inarg_type_name_emitter_,
          &outarg_type_name_emitter_,
          &inoutarg_type_name_emitter_,
          h->name ().simple ());

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::HomeDef::Iterator i = h->supports_begin ();
           i != h->supports_end ();
           i++)
      {
        i->resolve ()->accept (&supported_operation_emitter);
      }

      // Close the CIAO_GLUE namespace, if we opened one.
      if (h->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };
}

SourceEmitterBase::SourceEmitterBase (ostream& os_)
  : os (os_)
{
}

// ===========================================================

ServantSourceEmitter::ServantSourceEmitter (
  ostream& os_,
  CommandLine const& cl,
  UnconstrainedInterfaceDefSet const& interface_set,
  ComponentDefSet const& component_set,
  HomeDefSet const& home_set)
  : SourceEmitterBase (os_),
    cl_ (cl),
    component_set_ (component_set),
    home_set_ (home_set),
    interface_set_ (interface_set)
{
}

bool
ServantSourceEmitter::contains_element (ModulePtr const& m) const
{
  for (UnconstrainedInterfaceDefSet::const_iterator i =
          interface_set_.begin ();
        i != interface_set_.end ();
        i++)
  {
    if (m->order ().suborder ((*i)->order ())) return true;
  }

  for (ComponentDefSet::const_iterator i = component_set_.begin ();
        i != component_set_.end ();
        i++)
  {
    if (m->order ().suborder ((*i)->order ())) return true;
  }

  for (HomeDefSet::const_iterator i = home_set_.begin ();
        i != home_set_.end ();
        i++)
  {
    if (m->order ().suborder ((*i)->order ())) return true;
  }

  return false;
}

void
ServantSourceEmitter::visit_translation_unit_pre (TranslationUnitPtr const& u)
{
  os << COPYRIGHT << endl << endl;

  fs::path file_path = u->principal_translation_region ()->file_path ();
  string file_name ("");

  if (! file_path.empty ())
  {
    file_name = file_path.leaf ();
  }
    
  string file_suffix = cl_.get_value ("hdr-file-suffix", 
                                      "_svnt.h");

  file_name = regex::perl_s (file_name,
                             "/^(.*?)(\\.(idl|cidl))?$/$1"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"Cookies.h\"" << endl << endl;
}

void
ServantSourceEmitter::visit_module_pre (ModulePtr const& m)
{
  if (contains_element (m))
  {
    os << STRS[GLUE_NS] << "_" << m->name ().simple () << endl
       << "{" << endl;
  }
}

void
ServantSourceEmitter::visit_unconstrained_interface_def (
  UnconstrainedInterfaceDefPtr const& i)
{
  if (interface_set_.find (i) != interface_set_.end ())
  {
    FacetEmitter facet_emitter (os);
    i->accept (&facet_emitter);
  }
}

void
ServantSourceEmitter::visit_component_def (ComponentDefPtr const& c)
{
  {
    ContextEmitter context_emitter (os);
    c->accept (&context_emitter);
  }

  {
    ServantEmitter servant_emitter (os);
    c->accept (&servant_emitter);
  }
}

void
ServantSourceEmitter::visit_home_def (HomeDefPtr const& h)
{
  HomeEmitter home_emitter (os);
  h->accept (&home_emitter);
}

void
ServantSourceEmitter::visit_module_post (ModulePtr const& m)
{
  if (contains_element (m))
  {
    os << "}" << endl << endl;
  }
}

// ===========================================================

ServantSourceFinalizingEmitter::ServantSourceFinalizingEmitter (
  ostream& os_,
  string export_macro)
  : SourceEmitterBase (os_),
    export_macro_ (export_macro)
{
}

void
ServantSourceFinalizingEmitter::visit_home_def (HomeDefPtr const& h)
{
  bool global =
    (h->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0);

  os << "extern \"C\" " << export_macro_ << " ::PortableServer::Servant"
        << endl
        << "create" << h->name ().simple () << "_Servant (" << endl
        << "::Components::HomeExecutorBase_ptr p," << endl
        << "CIAO::Session_Container *c" << endl
        << STRS[ENV_SRC] << ")" << endl
        << "{" << endl
        << "if (p == 0)" << endl
        << "{" << endl
        << "return 0;" << endl
        << "}" << endl << endl
        << h->name ().scope () << "::CCM_"
        << h->name ().simple () << "_var x =" << endl
        << h->name ().scope () << "::CCM_" << h->name ().simple ()
        << "::_narrow (" << endl
        << "p" << endl
        << STRS[ENV_ARG] << ");" << endl
        << "ACE_CHECK_RETURN (0);" << endl << endl
        << "if (::CORBA::is_nil (x.in ()))" << endl
        << "{" << endl
        << "return 0;" << endl
        << "}" << endl << endl
        << "return new" << endl
        << "::CIAO_GLUE";

  if (!global) os << "_" << h->name ().scope ().simple ();

  os << "::" << h->name ().simple () << "_Servant (" << endl
        << "x.in ()," << endl
        << "c);" << endl
        << "}" << endl << endl;
}
