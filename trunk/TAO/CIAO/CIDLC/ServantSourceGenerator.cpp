// $Id$
#include "ServantSourceGenerator.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"

#include "Literals.hpp"

#include <ostream>
#include <sstream>

using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;
using namespace StringLiterals;

namespace
{
  std::string
  servant_name (DeclarationPtr const& d)
  {
    std::stringstream os;
    os << "::CIAO_GLUE";

    if (d->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () == 0)
    {
      os << "_" << d->scope ()->name ().simple ();
    }

    os << "::" << d->name ().simple () << "_Servant";

    return os.str ();
  }

  // Generates the name of an operation's return type.
  //
  //
  class ReturnTypeNameEmitter : public SourceEmitterBase,
                                public Traversal::Void,
                                public Traversal::Boolean,
                                public Traversal::Long,
                                public Traversal::String,
                                public Traversal::LocalInterfaceDecl
  {
  public:
    ReturnTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (VoidPtr const&)
    {
      os << "void";
    }

    virtual void
    traverse (BooleanPtr const&)
    {
      os << "::CORBA::Boolean";
    }

    virtual void
    traverse (LongPtr const&)
    {
      os << "::CORBA::Long";
    }

    virtual void
    traverse (StringPtr const&)
    {
      os << "char *";
    }

    virtual void
    traverse (LocalInterfaceDeclPtr const& i)
    {
      os << i->name () << "_ptr";
    }
  };

  // Generates the typename of an IN argument.
  //
  //
  class INArgTypeNameEmitter : public SourceEmitterBase,
                               public Traversal::Boolean,
                               public Traversal::Long,
                               public Traversal::String
  {
  public:
    INArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (BooleanPtr const&)
    {
      os << "::CORBA::Boolean";
    }

    virtual void
    traverse (LongPtr const&)
    {
      os << "::CORBA::Long";
    }

    virtual void
    traverse (StringPtr const&)
    {
      os << "const char *";
    }
  };

  // Generates the typename of an OUT argument.
  //
  //
  class OUTArgTypeNameEmitter : public SourceEmitterBase,
                                public Traversal::Boolean,
                                public Traversal::Long,
                                public Traversal::String
  {
  public:
    OUTArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (BooleanPtr const&)
    {
      os << "::CORBA::Boolean_out";
    }

    virtual void
    traverse (LongPtr const&)
    {
      os << "::CORBA::Long_out";
    }

    virtual void
    traverse (StringPtr const&)
    {
      os << "::CORBA::String_out";
    }
  };

  // Generates the typename of an INOUT argument.
  //
  //
  class INOUTArgTypeNameEmitter : public SourceEmitterBase,
                                  public Traversal::Boolean,
                                  public Traversal::Long,
                                  public Traversal::String
  {
  public:
    INOUTArgTypeNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (BooleanPtr const&)
    {
      os << "::CORBA::Boolean &";
    }

    virtual void
    traverse (LongPtr const&)
    {
      os << "::CORBA::Long &";
    }

    virtual void
    traverse (StringPtr const&)
    {
      os << "char *&";
    }
  };

  // Generates an argument's identifier.
  //
  //
  class ArgNameEmitter : public SourceEmitterBase,
                         public Traversal::TypeDecl
  {
  public:
    ArgNameEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    traverse (CommaPtr const&)
    {
      os << ",";
    }
  };

  // Generates an attribute's accessor operation.
  //
  //
  class GetAttributeEmitter : public SourceEmitterBase,
                              public Traversal::AttributeDecl
  {
  public:
    GetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : SourceEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    post (AttributeDeclPtr const& a)
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
                              public Traversal::AttributeDecl
  {
  public:
    SetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : SourceEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    pre (AttributeDeclPtr const& a)
    {
      os << "void " << endl
         << a->name ().scope ().simple ()
         << "_Servant::" << a->name ().simple () << " (" << endl;
    }

    virtual void
    post (AttributeDeclPtr const& a)
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
                               public Traversal::OperationParameter,
                               public Traversal::Comma,
                               public Traversal::OperationDecl
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
    pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    post (OperationParameterPtr const& op)
    {
      os << op->name ();
    }

    virtual void
    traverse (CommaPtr const&)
    {
      os << ",";
    }
  };

  // Generates an operation definition.
  //
  //
  class OperationEmitter : public SourceEmitterBase,
                           public Traversal::UnconstrainedInterfaceDef,
                           public virtual Traversal::OperationParameter,
                           public Traversal::Comma,
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
    name (OperationDeclPtr const& d)
    {
      os << endl
         << d->name ().scope ().simple () << "_Servant::"
         << d->name ().simple () << " (";
    }

    virtual void
    pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    traverse (CommaPtr const&)
    {
      os << ",";
    }

    virtual void
    post (OperationDeclPtr const& d)
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
      operation_exec_emitter.dispatch (d);

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
                         public Traversal::HomeFactoryDecl,
                         public Traversal::OperationParameter,
                         public Traversal::Comma
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
    type (HomeFactoryDeclPtr const& f)
    {
      HomeDefPtr def (f->scope ()->dynamic_type<SyntaxTree::HomeDef> ());
      os << def->manages ()->name () << "_ptr";
    }

    virtual void
    name (HomeFactoryDeclPtr const& f)
    {
      os << endl
         << f->name ().scope ().simple () << "_Servant::"
         << f->name ().simple () << " (";
    }

    virtual void
    pre (OperationParameterPtr const&)
    {
      os << endl;
    }

    virtual void
    post (OperationParameterPtr const& op)
    {
      os << " " << op->name ();
    }

    virtual void
    traverse (CommaPtr const&)
    {
      os << ",";
    }

    virtual void
    post (HomeFactoryDeclPtr const& f)
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
      operation_exec_emitter.dispatch (f);

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
  class SupportedOperationEmitter : public OperationEmitter,
                                    public virtual Traversal::OperationParameter,
                                    public virtual Traversal::OperationDecl
  {
  private:
    SimpleName context_;

  public:
    SupportedOperationEmitter (ostream& os_,
                               ReturnTypeNameEmitter* return_type_name_emitter,
                               INArgTypeNameEmitter* inarg_type_name_emitter,
                               OUTArgTypeNameEmitter* outarg_type_name_emitter,
                               INOUTArgTypeNameEmitter* inoutarg_type_name_emitter,
                               SimpleName context)
      : OperationEmitter (os_,
                          return_type_name_emitter,
                          inarg_type_name_emitter,
                          outarg_type_name_emitter,
                          inoutarg_type_name_emitter),
        OperationParameter (inarg_type_name_emitter,
                            outarg_type_name_emitter,
                            inoutarg_type_name_emitter),
        OperationDecl (return_type_name_emitter),
        context_ (context)
    {
    }

    virtual void
    name (OperationDeclPtr const& d)
    {
      os << endl
         << context_ << "_Servant::"
         << d->name ().simple () << " (";
    }
  };


  //
  // Select only the facets that are present in a provides declaration.
  //
  class FacetSelector : public SourceEmitterBase,
                        public Traversal::UnconstrainedInterfaceDef
  {
  private:
    Declarations const& declarations_;

  public:
    FacetSelector (ostream& os_, Declarations const& declarations)
      : SourceEmitterBase (os_),
        declarations_ (declarations)
    {
    }

    void
    traverse (UnconstrainedInterfaceDefPtr const& i)
    {
      if (declarations_.find (i))
      {
        Traversal::UnconstrainedInterfaceDef::traverse (i);
      }
    }
  };

  //
  // FacetServantEmitter generates facet servant class definitions.
  //
  class FacetServantEmitter : public SourceEmitterBase,
                              public Traversal::UnconstrainedInterfaceDef,
                              public Traversal::AttributeDecl
  {
  public:
    FacetServantEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (UnconstrainedInterfaceDefPtr const& i)
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
    traverse (AttributeDeclPtr const& a)
    {
      // TODO
    }

    virtual void
    post (UnconstrainedInterfaceDefPtr const& i)
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
                         public Traversal::ComponentDef,
                         public Traversal::UsesDecl,
                         public Traversal::PublishesDecl,
                         public Traversal::EmitsDecl
  {
  public:
    ContextEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
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
    traverse (UsesDeclPtr const& d)
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
    traverse (PublishesDeclPtr const& d)
    {
      os << "void" << endl
         << d->scope ()->name ().simple () << "_Context::push_"
         << d->name ().simple () << " (" << endl
         << d->type ()->name () << " *ev" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{" << endl
         << "ACE_Active_Map_Manager<" << endl //@@ gcc bug
         << d->type ()->name ()
         << "Consumer_var>::iterator end =" << endl
         << "this->ciao_publishes_" << d->name ().simple ()
         << "_map_.end ();" << endl << endl
         << "for (ACE_Active_Map_Manager<" << endl //@@ gcc bug
         << d->type ()->name ()
         << "Consumer_var>::iterator iter =" << endl
         << "this->ciao_publishes_" << d->name ().simple ()
         << "_map_.begin ();" << endl
         << "iter != end;" << endl
         << "++iter)" << endl
         << "{" << endl
         << "ACE_Active_Map_Manager<" << endl //@@ gcc bug
         << d->type ()->name ()
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
    traverse (EmitsDeclPtr const& d)
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
    post (ComponentDefPtr const& c)
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
  // Generates component servant interface definition, constructor &
  // destructor.
  //
  class ServantFabricEmitter : public SourceEmitterBase,
                               public Traversal::ComponentDef
  {
  public:
    ServantFabricEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (ComponentDefPtr const& c)
    {
      // If we are at file scope, we create a namespace anyway.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "namespace CIAO_GLUE" << endl
             << "{" << endl;
      }

      // Servant Constructor
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

      // Servant Destructor
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
  };

  //
  // Generates generic navigation interface for a component.  The
  // concrete provide_XXX operations will be generated by the
  // FacetProvidesEmitter class.
  //
  class NavigationEmitter : public SourceEmitterBase,
                            public Traversal::ComponentDef,
                            public Traversal::ProvidesDecl
  {
  public:
    NavigationEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
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
    }

    virtual void
    traverse (ProvidesDeclPtr const& p)
    {
      os << "if (ACE_OS::strcmp (name, \""
         << p->name ().simple () << "\") == 0)" << endl
         << "{" << endl
         << "return this->provide_" << p->name ().simple ()
         << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
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
    }
  };


  //
  // Generates each of the provide_XXX operations.
  // class.
  //
  class FacetProvidesEmitter : public SourceEmitterBase,
                               public Traversal::ProvidesDecl
  {
  public:
    FacetProvidesEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (ProvidesDeclPtr const& p)
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
         << servant_name (p->type ()) << " *svt =" << endl
         << "new " << servant_name (p->type ()) << " (" << endl
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
  };

  //
  // Generates generic receptacles connection interface for a component.
  // The concrete connect_XXX, disconnect_XXX and get_connection_XXX
  // operations will be generated by the ConnectionEmitter class.
  //
  class ReceptaclesGenericConnectionEmitter
    : public SourceEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::UsesDecl
  {
  public:
    ReceptaclesGenericConnectionEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
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
    }

    virtual void
    traverse (UsesDeclPtr const& u)
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

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
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
    }
  };

  //
  // Generates generic receptacles disconnection interface for a component.
  //
  class ReceptaclesGenericDisconnectionEmitter
    : public SourceEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::UsesDecl
  {
  public:
    ReceptaclesGenericDisconnectionEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
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
    }

    virtual void
    traverse (UsesDeclPtr const& u)
    {
      os << "if (ACE_OS::strcmp (name, \""
         << u->name ().simple () << "\") == 0)" << endl
         << "{" << endl
         << "// Simplex disconnect." << endl
         << "return this->disconnect_" << u->name ().simple ()
         << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl << endl;
    }
  };

  //
  // Generates specific component receptacles connection & disconnection
  // operations.
  //
  class ReceptaclesEmitter : public SourceEmitterBase,
                             public Traversal::UsesDecl
  {
  public:
    ReceptaclesEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (UsesDeclPtr const& p)
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
  };

  //
  // Generates generic Event consumer operations for a component.
  //
  class ConsumerGenericEmitter : public SourceEmitterBase,
                                 public Traversal::ComponentDef,
                                 public Traversal::ConsumesDecl
  {
  public:
    ConsumerGenericEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
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
    }

    virtual void
    traverse (ConsumesDeclPtr const& c)
    {
      os << "if (ACE_OS::strcmp (sink_name, \""
         << c->name ().simple () << "\") == 0)" << endl
         << "{" << endl
         << "return this->get_consumer_" << c->name ().simple ()
         << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
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
    }
  };

  //
  // Generates Event consumers for a component.
  //
  class ConsumerEmitter : public SourceEmitterBase,
                          public Traversal::ConsumesDecl
  {
  public:
    ConsumerEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (ConsumesDeclPtr const& c)
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

  };


  //
  // Generates generic publishes subscription interface for a component.
  //
  class PublishesGenericSubscribeEmitter
    : public SourceEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::PublishesDecl
  {
  public:
    PublishesGenericSubscribeEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
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
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
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

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}" << endl << endl;
    }
  };

  //
  // Generates generic publishes unsubscription interface for a component.
  //
  class PublishesGenericUnsubscribeEmitter
    : public SourceEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::PublishesDecl
  {
  public:
    PublishesGenericUnsubscribeEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
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
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
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

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
         << "}" << endl << endl;

      // @@ (diego) These are not implemented. Whenever they are,
      // they'll require a treatment as all the other ports above.

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

    }
  };

  //
  // Generates publishes subscription & unsubscription interface
  // the published events of a component.
  //
  class PublishesEmitter : public SourceEmitterBase,
                           public Traversal::PublishesDecl
  {
  public:
    PublishesEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
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
  };

  //
  // Generates publishes subscription & unsubscription interface
  // the published events of a component.
  //
  class EmitsEmitter : public SourceEmitterBase,
                       public Traversal::ComponentDef,
                       public Traversal::EmitsDecl
  {
  public:
    EmitsEmitter (ostream& os_)
      : SourceEmitterBase (os_)
    {
    }

    virtual void
    traverse (EmitsDeclPtr const& e)
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
    post (ComponentDefPtr const& c)
    {
      // @@ (diego) These are not implemented. Whenever they are,
      // they'll require a treatment as all the other ports above.

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
    }
  };

  //
  // Generates component final common part.
  //
  class ServantCommonFinalizingEmitter
    : public SourceEmitterBase,
      public Traversal::ComponentDef
  {
  private:
    ReturnTypeNameEmitter* return_type_name_emitter_;
    INArgTypeNameEmitter* inarg_type_name_emitter_;
    OUTArgTypeNameEmitter* outarg_type_name_emitter_;
    INOUTArgTypeNameEmitter* inoutarg_type_name_emitter_;

  public:
    ServantCommonFinalizingEmitter
    (ostream& os_,
     ReturnTypeNameEmitter* return_type_name_emitter,
     INArgTypeNameEmitter* inarg_type_name_emitter,
     OUTArgTypeNameEmitter* outarg_type_name_emitter,
     INOUTArgTypeNameEmitter* inoutarg_type_name_emitter)
      : SourceEmitterBase (os_),
        return_type_name_emitter_ (return_type_name_emitter),
        inarg_type_name_emitter_ (inarg_type_name_emitter),
        outarg_type_name_emitter_ (outarg_type_name_emitter),
        inoutarg_type_name_emitter_ (inoutarg_type_name_emitter)
    {
    }

    virtual void
    traverse (ComponentDefPtr const& c)
    {
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
          return_type_name_emitter_,
          inarg_type_name_emitter_,
          outarg_type_name_emitter_,
          inoutarg_type_name_emitter_,
          c->name ().simple ());

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        supported_operation_emitter.dispatch (i->resolve ());
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
                      public Traversal::HomeDef,
                      public Traversal::AttributeDecl //@@ will disappear
  {
  private:
    ReturnTypeNameEmitter* return_type_name_emitter_;
    INArgTypeNameEmitter* inarg_type_name_emitter_;
    OUTArgTypeNameEmitter* outarg_type_name_emitter_;
    INOUTArgTypeNameEmitter* inoutarg_type_name_emitter_;

  public:
    HomeEmitter (ostream& os_,
                 ReturnTypeNameEmitter* return_type_name_emitter,
                 INArgTypeNameEmitter* inarg_type_name_emitter,
                 OUTArgTypeNameEmitter* outarg_type_name_emitter,
                 INOUTArgTypeNameEmitter* inoutarg_type_name_emitter)
      : SourceEmitterBase (os_),
        return_type_name_emitter_ (return_type_name_emitter),
        inarg_type_name_emitter_ (inarg_type_name_emitter),
        outarg_type_name_emitter_ (outarg_type_name_emitter),
        inoutarg_type_name_emitter_ (inoutarg_type_name_emitter)
    {
    }

    virtual void
    pre (HomeDefPtr const& h)
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

      os << "// Home factory and other operations." << endl << endl;
    }

    virtual void
    traverse (AttributeDeclPtr const& a)
    {
      // TODO
    }

    virtual void
    post (HomeDefPtr const& h)
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
          return_type_name_emitter_,
          inarg_type_name_emitter_,
          outarg_type_name_emitter_,
          inoutarg_type_name_emitter_,
          h->name ().simple ());

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::HomeDef::Iterator i = h->supports_begin ();
           i != h->supports_end ();
           i++)
      {
        supported_operation_emitter.dispatch (i->resolve ());
      }

      // Close the CIAO_GLUE namespace, if we opened one.
      if (h->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };


  //
  // Generates needed namespaces for modules
  //
  class NamespaceEmitter : public SourceEmitterBase,
                           public Traversal::Module
  {
  private:
    Declarations const& declarations_;

  public:
    NamespaceEmitter (ostream& os_, Declarations const& declarations)
      : SourceEmitterBase (os_),
        declarations_ (declarations)
    {
    }

    void
    pre (ModulePtr const& m)
    {
      if (declarations_.contains_suborder (m->order ()))
      {
        os << STRS[GLUE_NS] << "_" << m->name ().simple () << endl
           << "{" << endl;
      }
    }

    void
    post (ModulePtr const& m)
    {
      if (declarations_.contains_suborder (m->order ()))
      {
        os << "}" << endl << endl;
      }
    }
  };


  class ServantSourceFinalizingEmitter
    : public SourceEmitterBase,
      public Traversal::TranslationUnit,
      public Traversal::TranslationRegion,
      public Traversal::FileScope,
      public Traversal::Module,
      public Traversal::HomeDef
  {
  private:
    string export_macro_;

  public:
    ServantSourceFinalizingEmitter (ostream& os_,
                                    string export_macro)
      : SourceEmitterBase (os_),
        export_macro_ (export_macro)
    {
    }

    virtual void
    traverse (HomeDefPtr const& h)
    {
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
         << "return new" << endl;

      os << servant_name (h) << " (" << endl
         << "x.in ()," << endl
         << "c);" << endl
         << "}" << endl << endl;
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
  string export_macro,
  CommandLine const& cl,
  Declarations const& declarations)
  : SourceEmitterBase (os_),
    export_macro_ (export_macro),
    cl_ (cl),
    declarations_ (declarations)
{
}

void
ServantSourceEmitter::pre (TranslationUnitPtr const& u)
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
                             "/(\\.(idl|cidl))?$/"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"Cookies.h\"" << endl << endl;
}

void
ServantSourceEmitter::generate (TranslationUnitPtr const& u)
{
  // This method will use the ServantHeaderEmitter class itself
  // as a entry point for the traversal of the tree

  ReturnTypeNameEmitter return_type_name (os);
  INArgTypeNameEmitter inarg_type_name (os);
  OUTArgTypeNameEmitter outarg_type_name (os);
  INOUTArgTypeNameEmitter inoutarg_type_name (os);

  ArgNameEmitter arg_name (os);

  GetAttributeEmitter get_attribute_emitter (os, &return_type_name);
  SetAttributeEmitter set_attribute_emitter (os, &return_type_name);

  /* @@ (diego) (used internally)
  OperationExecEmitter operation_exec_emitter (os, &arg_name);
  */

  OperationEmitter operation_emitter (os,
                                      &return_type_name,
                                      &inarg_type_name,
                                      &outarg_type_name,
                                      &inoutarg_type_name);

  FactoryEmitter factory_emitter (os, &inarg_type_name);

  ServantFabricEmitter servant_fabric_emitter (os);

  NavigationEmitter navigation_emitter (os);

  FacetProvidesEmitter facet_provides_emitter (os);

  ReceptaclesGenericConnectionEmitter
      receptacles_generic_connection_emitter (os);

  ReceptaclesGenericDisconnectionEmitter
      receptacles_generic_disconnection_emitter (os);

  ReceptaclesEmitter receptacles_emitter (os);

  ConsumerGenericEmitter consumer_generic_emitter (os);

  ConsumerEmitter consumer_emitter (os);

  PublishesGenericSubscribeEmitter publishes_generic_subscribe_emitter (os);

  PublishesGenericUnsubscribeEmitter
      publishes_generic_unsubscribe_emitter (os);

  PublishesEmitter publishes_emitter (os);

  EmitsEmitter emits_emitter (os);

  FacetServantEmitter facet_servant_emitter (os);
  facet_servant_emitter.add_scope_delegate (&operation_emitter);

  FacetSelector facet_selector (os, declarations_);
  facet_selector.add_delegate (&facet_servant_emitter);

  ContextEmitter context_emitter (os);

  ServantCommonFinalizingEmitter
      servant_common_finalizing_emitter (os,
                                         &return_type_name,
                                         &inarg_type_name,
                                         &outarg_type_name,
                                         &inoutarg_type_name);
  servant_common_finalizing_emitter.add_scope_delegate (&get_attribute_emitter);
  servant_common_finalizing_emitter.add_scope_delegate (&set_attribute_emitter);

  // Discriminator for operation and factory
  // This is needed to select always the most-derived type of traversal
  // for any element of the scope of a Home
  Traversal::Discriminator d;
  d.add (&operation_emitter);
  d.add (&factory_emitter);

  HomeEmitter home_emitter (os,
                            &return_type_name,
                            &inarg_type_name,
                            &outarg_type_name,
                            &inoutarg_type_name);
  home_emitter.add_scope_delegate (&d);

  Traversal::ComponentDef component;
  component.add_scope_delegate (&facet_provides_emitter);
  component.add_scope_delegate (&receptacles_emitter);
  component.add_scope_delegate (&consumer_emitter);
  component.add_scope_delegate (&publishes_emitter);

  Traversal::FileScope fs;
  NamespaceEmitter m (os, declarations_);;

  fs.add_scope_delegate (&m);

  fs.add_scope_delegate (&context_emitter);
  fs.add_scope_delegate (&servant_fabric_emitter);
  fs.add_scope_delegate (&navigation_emitter);
  fs.add_scope_delegate (&component);
  fs.add_scope_delegate (&receptacles_generic_connection_emitter);
  fs.add_scope_delegate (&receptacles_generic_disconnection_emitter);
  fs.add_scope_delegate (&consumer_generic_emitter);
  fs.add_scope_delegate (&publishes_generic_subscribe_emitter);
  fs.add_scope_delegate (&publishes_generic_unsubscribe_emitter);
  fs.add_scope_delegate (&emits_emitter);
  fs.add_scope_delegate (&home_emitter);
  fs.add_scope_delegate (&facet_selector);
  fs.add_scope_delegate (&servant_common_finalizing_emitter);

  m.add_scope_delegate (&context_emitter);
  m.add_scope_delegate (&servant_fabric_emitter);
  m.add_scope_delegate (&navigation_emitter);
  m.add_scope_delegate (&component);
  m.add_scope_delegate (&receptacles_generic_connection_emitter);
  m.add_scope_delegate (&receptacles_generic_disconnection_emitter);
  m.add_scope_delegate (&consumer_generic_emitter);
  m.add_scope_delegate (&publishes_generic_subscribe_emitter);
  m.add_scope_delegate (&publishes_generic_unsubscribe_emitter);
  m.add_scope_delegate (&emits_emitter);
  m.add_scope_delegate (&home_emitter);
  m.add_scope_delegate (&facet_selector);
  m.add_scope_delegate (&servant_common_finalizing_emitter);

  Traversal::TranslationRegion region (&fs);
  add_content_delegate (&region);

  // Start processing
  dispatch (u);

  // Start the finalizing phase
  ServantSourceFinalizingEmitter finalizing_emitter (os, export_macro_);
  finalizing_emitter.dispatch (u);
}


/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
