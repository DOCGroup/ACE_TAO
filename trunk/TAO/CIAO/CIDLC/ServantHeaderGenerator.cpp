// $Id$
#include "ServantHeaderGenerator.hpp"

#include "Literals.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"

#include <cctype>
#include <algorithm>
#include <ostream>

using namespace StringLiterals;
using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;

namespace
{
  // On some platforms toupper can be something else than a
  // function.
  int
  upcase (int c)
  {
    return std::toupper (c);
  }
}

namespace
{
  // Generates the name of an operation's return type.
  //
  //
  class ReturnTypeNameEmitter : public HeaderEmitterBase,
                                public Traversal::Void,
                                public Traversal::Boolean,
                                public Traversal::Long,
                                public Traversal::String,
                                public Traversal::LocalInterfaceDecl
  {
  public:
    ReturnTypeNameEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
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
  class INArgTypeNameEmitter : public HeaderEmitterBase,
                               public Traversal::Boolean,
                               public Traversal::Long,
                               public Traversal::String
  {
  public:
    INArgTypeNameEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
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
  class OUTArgTypeNameEmitter : public HeaderEmitterBase,
                                public Traversal::Boolean,
                                public Traversal::Long,
                                public Traversal::String
  {
  public:
    OUTArgTypeNameEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
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
  class INOUTArgTypeNameEmitter : public HeaderEmitterBase,
                                  public Traversal::Boolean,
                                  public Traversal::Long,
                                  public Traversal::String
  {
  public:
    INOUTArgTypeNameEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
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

  // Generates parts of the accessor operation for an attribute.
  //
  //
  class GetAttributeEmitter : public HeaderEmitterBase,
                              public Traversal::AttributeDecl
  {
  public:
    GetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : HeaderEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    pre (AttributeDeclPtr const&)
    {
      os << "virtual ";
    }

    virtual void
    post (AttributeDeclPtr const& a)
    {
      os << endl
         << a->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };



  // Generates parts of the mutator operation for an attribute.
  //
  //
  class SetAttributeEmitter : public HeaderEmitterBase,
                              public Traversal::AttributeDecl
  {
  public:
    SetAttributeEmitter (ostream& os_,
                         ReturnTypeNameEmitter* return_type_name_emitter)
      : HeaderEmitterBase (os_),
        AttributeDecl (return_type_name_emitter)
    {
    }

    virtual void
    pre (AttributeDeclPtr const& a)
    {
      os << "virtual void " << endl
         << a->name ().simple () << " (" << endl;
    }

    virtual void
    post (AttributeDeclPtr const&)
    {
      os << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };

  //
  //
  //
  class OperationEmitter : public HeaderEmitterBase,
                           public Traversal::UnconstrainedInterfaceDef,
                           public Traversal::OperationParameter,
                           public Traversal::Comma,
                           public Traversal::OperationDecl
  {
  public:
    OperationEmitter (ostream& os_,
                      ReturnTypeNameEmitter* return_type_name_emitter,
                      INArgTypeNameEmitter* inarg_type_name_emitter_,
                      OUTArgTypeNameEmitter* outarg_type_name_emitter_,
                      INOUTArgTypeNameEmitter* inoutarg_type_name_emitter_)
      : HeaderEmitterBase (os_),
        OperationParameter (inarg_type_name_emitter_,
                            outarg_type_name_emitter_,
                            inoutarg_type_name_emitter_),
        OperationDecl (return_type_name_emitter)
    {
    }

    virtual void
    pre (OperationDeclPtr const&)
    {
      os << "virtual ";
    }

    virtual void
    name (OperationDeclPtr const& d)
    {
      os << endl << d->name ().simple () << " (";
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
      // @@@ (JP) Need to process exception list.
      os << endl
         << (d->begin () == d->end () ? STRS[ENV_SNGL_HDR] : STRS[ENV_HDR])
         << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };

  // Generates the operation associated with a home factory declaration.
  //
  //
  class FactoryEmitter : public HeaderEmitterBase,
                         public Traversal::HomeFactoryDecl,
                         public Traversal::OperationParameter,
                         public Traversal::Comma
  {
  public:
    FactoryEmitter (ostream& os_,
                    INArgTypeNameEmitter* inarg_type_name_emitter_)
      : HeaderEmitterBase (os_),
        OperationParameter (inarg_type_name_emitter_,
                            inarg_type_name_emitter_,
                            inarg_type_name_emitter_)
    {
    }

    virtual void
    pre (HomeFactoryDeclPtr const&)
    {
      os << "virtual ";
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
      // @@@ (JP) Need to process exception list.
      os << (f->begin () == f->end () ? STRS[ENV_SNGL_HDR] : STRS[ENV_HDR])
         << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };


  //
  // Select only the facets that are present in a provides declaration.
  //
  class FacetSelector : public HeaderEmitterBase,
                        public Traversal::UnconstrainedInterfaceDef
  {
  private:
    Declarations const& declarations_;

  public:
    FacetSelector (ostream& os_, Declarations const& declarations)
      : HeaderEmitterBase (os_),
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
  // FacetEmitter generates a facet servant class declaration.
  //
  class FacetEmitter : public HeaderEmitterBase,
                       public Traversal::UnconstrainedInterfaceDef,
                       public Traversal::AttributeDecl //@@ Will disappear
  {
  private:
    string export_macro_;

  public:
    FacetEmitter (ostream& os_,
                  string export_macro)
      : HeaderEmitterBase (os_),
        export_macro_ (export_macro)
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

      // @@@ (JP) Need export macro and prefixable scoped name.
      os << "class " << export_macro_ << " " << i->name ().simple ()
         << "_Servant" << endl
         << ": public virtual POA_" << i->name ().in_file_scope ()
         << "," << endl
         << STRS[INH_RCSB]
         << endl
         << "{" << endl
         << "public:" << endl;

      os << i->name ().simple () << "_Servant (" << endl
         << i->name ().scope () << "::CCM_" << i->name ().simple ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr ctx);" << endl << endl;

      os << "virtual ~" << i->name ().simple () << "_Servant (void);"
         << endl << endl;
    }

    virtual void
    traverse (AttributeDeclPtr const&)
    {
      // TODO
    }

    virtual void
    post (UnconstrainedInterfaceDefPtr const& i)
    {
      os << "// Get component implementation." << endl
         << "virtual CORBA::Object_ptr" << endl
         << "_get_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";"
         << endl << endl;

      os << "protected:" << endl
         << "// Facet executor." << endl
         << i->name ().scope () << "::CCM_" << i->name ().simple ()
         << "_var executor_;" << endl << endl;

      os  << "// Context object." << endl
          << "::Components::CCMContext_var ctx_;" << endl
          << "};" << endl;

      // Close the CIAO_GLUE namespace, if we opened one.
      if (i->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };

  //
  // Generates public part of component context interface declaration.
  //
  class ContextPublicEmitter : public HeaderEmitterBase,
                               public Traversal::ComponentDef,
                               public Traversal::UsesDecl,
                               public Traversal::PublishesDecl,
                               public Traversal::EmitsDecl
  {
  private:
    string export_macro_;

  public:
    ContextPublicEmitter (ostream& os_,
                          string export_macro)
      : HeaderEmitterBase (os_),
        export_macro_ (export_macro)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      // If we are at file scope, we create a namespace anyway.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << STRS[GLUE_NS] << endl
           << "{" << endl;
      }

      // @@@ (JP) Need export macro.
      os << "class " << export_macro_ << " " << c->name ().simple ()
         << "_Context" << endl
         << ": public virtual " << c->name ().scope () << "::CCM_"
         << c->name ().simple () << "_Context,"
         << endl
         << "public virtual TAO_Local_RefCounted_Object"
         << endl
         << "{" << endl
         << "public:" << endl;

      os << "// We will allow the servant glue code we generate to "
         << "access our state." << endl
         << "friend class " << c->name ().simple () << "_Servant;"
         << endl << endl;

      os << c->name ().simple () << "_Context (" << endl
         << "::Components::CCMHome_ptr home," << endl
         << "::CIAO::Session_Container *c," << endl
         << c->name ().simple () << "_Servant *sv);" << endl << endl;

      os << "virtual ~" << c->name ().simple () << "_Context (void);"
         << endl << endl;

      os << "// Operations from ::Components::CCMContext." << endl << endl;

      os << "virtual ::Components::Principal_ptr" << endl
         << "get_caller_principal (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::CCMHome_ptr" << endl
         << "get_CCM_home (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual CORBA::Boolean" << endl
         << "get_rollback_only (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "));" << endl << endl;

      os << "virtual ::Components::Transaction::UserTransaction_ptr" << endl
         << "get_user_transaction (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "));" << endl << endl;

      os << "virtual CORBA::Boolean" << endl
         << "is_caller_in_role (" << endl
         << "const char *role" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual void" << endl
         << "set_rollback_only (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "));" << endl << endl;

      os << "// Operations from ::Components::SessionContext interface."
         << endl << endl;

      os << "virtual CORBA::Object_ptr" << endl
         << "get_CCM_object (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "));" << endl << endl;

      os << "// Operations for " << c->name ().simple () << " receptacles"
         << " and event sources," << endl
         << "// defined in " << c->name ().scope () << "::CCM_"
         << c->name ().simple () << "_Context."
         << endl << endl;
    }

    virtual void
    traverse (UsesDeclPtr const& d)
    {
      // @@@ (JP) Need to handle multiple connections.
      os << "virtual " << d->type ()->name () << "_ptr" << endl
         << "get_connection_" << d->name ().simple ()
         << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

    virtual void
    traverse (PublishesDeclPtr const& d)
    {
      os << "virtual void" << endl
         << "push_" << d->name ().simple ()
         << " (" << endl
         << d->type ()->name () << " *ev" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

    virtual void
    traverse (EmitsDeclPtr const& d)
    {
      os << "virtual void" << endl
         << "push_" << d->name ().simple ()
         << " (" << endl
         << d->type ()->name () << " *ev" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };

  //
  // Generates protected methods of component context interface declaration.
  //
  class ContextProtectedMethodEmitter
    : public HeaderEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::UsesDecl,
      public Traversal::PublishesDecl,
      public Traversal::EmitsDecl
  {
  public:
    ContextProtectedMethodEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      os << "protected:" << endl
         << "// Methods that manage this component's connections"
         << " and consumers." << endl << endl;
    }

    virtual void
    traverse (UsesDeclPtr const& d)
    {
      // @@@ (JP) Need to handle multiple connections.
      os << "virtual void" << endl
         << "connect_" << d->name ().simple () << " (" << endl
         << d->type ()->name () << "_ptr" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;

      os << "virtual " << d->type ()->name () << "_ptr" << endl
         << "disconnect_" << d->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;
    }

    virtual void
    traverse (EmitsDeclPtr const& d)
    {
      os << "virtual void" << endl
         << "connect_" << d->name ().simple () << " (" << endl
         << d->type ()->name () << "Consumer_ptr c" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual " << d->type ()->name () << "_ptr" << endl
         << "disconnect_" << d->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
    {
      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "subscribe_" << p->name ().simple () << " (" << endl
         << p->type ()->name () << "Consumer_ptr c" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_ECL] << "));" << endl << endl;

      os << "virtual " << p->type ()->name () << "Consumer_ptr" << endl
         << "unsubscribe_" << p->name ().simple () << " (" << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;
    }
  };

  //
  // Generates protected members of component context interface declaration.
  //
  class ContextProtectedMemberEmitter
    : public HeaderEmitterBase,
      public Traversal::ComponentDef,
      public Traversal::UsesDecl,
      public Traversal::PublishesDecl,
      public Traversal::EmitsDecl
  {
  public:
    ContextProtectedMemberEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      os << "protected:" << endl;
    }

    virtual void
    traverse (UsesDeclPtr const& d)
    {
      // @@@ (JP) Need to handle multiple connections.
      os << "// Simplex " << d->name ().simple () << " connection." << endl
         << d->type ()->name () << "_var" << endl
         << "ciao_uses_" << d->name ().simple () << "_;" << endl << endl;
    }

    virtual void
    traverse (EmitsDeclPtr const& d)
    {
      os << d->type ()->name () << "Consumer_var" << endl
         << "ciao_emits_" << d->name ().simple () << "_consumer_;"
         << endl << endl;
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
    {
      os << "ACE_Active_Map_Manager<" << endl
         << p->type ()->name () << "Consumer_var>" << endl
         << "ciao_publishes_" << p->name ().simple () << "_map_;"
         << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "::Components::CCMHome_var" << endl
         << "home_;" << endl << endl;

      os << "::CIAO::Session_Container *" << endl
         << "container_;" << endl << endl;

      os << c->name ().simple () << "_Servant *" << endl
         << "servant_;" << endl << endl;

      os << c->name () << "_var" << endl
         << "component_;" << endl;

      os << "};" << endl;

      // Close the CIAO_GLUE namespace, if we opened one.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << "}" << endl;
      }

      os << endl;
    }
  };



  //
  // Generates public part of component servant interface declaration.
  //
  class ServantPublicEmitter : public HeaderEmitterBase,
                               public Traversal::ComponentDef,
                               public Traversal::ProvidesDecl,
                               public Traversal::UsesDecl,
                               public Traversal::PublishesDecl,
                               public Traversal::ConsumesDecl,
                               public Traversal::EmitsDecl
  {
  private:
    string export_macro_;

    OperationEmitter& operation_emitter_;

  public:
    ServantPublicEmitter (ostream& os_,
                          string export_macro,
                          OperationEmitter& operation_emitter)
      : HeaderEmitterBase (os_),
        export_macro_ (export_macro),
        operation_emitter_ (operation_emitter)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      // If we are at file scope, we create a namespace anyway.
      if (c->scope ()->dynamic_type<IDL2::SyntaxTree::FileScope> () != 0)
      {
        os << STRS[GLUE_NS] << endl
           << "{" << endl;
      }

      // @@@ (JP) Need export macro and prefixable scoped name.
      os << "class " << export_macro_ << " " << c->name ().simple ()
         << "_Servant" << endl
         << ": public virtual POA_" << c->name ().in_file_scope ()
         << "," << endl
         << STRS[INH_RCSB] << endl
         << "{" << endl
         << "public:" << endl;

      os << c->name ().simple () << "_Servant (" << endl
         << c->name ().scope () << "::CCM_" << c->name ().simple ()
         << "_ptr executor," << endl
         << "::Components::CCMHome_ptr home," << endl
         << "::CIAO::Session_Container *c);" << endl << endl;

      os << "virtual ~" << c->name ().simple () << "_Servant (void);"
         << endl << endl;

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        operation_emitter_.dispatch (i->resolve ());
      }
    }

    virtual void
    traverse (ProvidesDeclPtr const& p)
    {
      os << "virtual " << p->type ()->name () << "_ptr" << endl
         << "provide_" << p->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

    virtual void
    traverse (UsesDeclPtr const& p)
    {
      // @@@ (JP) Need to handle multiple connections.
      os << "virtual void" << endl
         << "connect_" << p->name ().simple () << " (" << endl
         << p->type ()->name () << "_ptr c" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;

      os << "virtual " << p->type ()->name () << "_ptr" << endl
         << "disconnect_" << p->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;

      os << "virtual " << p->type ()->name () << "_ptr" << endl
         << "get_connection_" << p->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

    virtual void
    traverse (ConsumesDeclPtr const& c)
    {
      // @@@ (JP) Need export macro.
      os << "// Servant class for the " << c->name ().simple ()
         << " consumer." << endl
         << "class " << export_macro_ << " "
         << c->type ()->name ().simple () << "Consumer_"
         << c->name ().simple () << "_Servant" << endl
         << ": public virtual POA_" << c->type ()->name ().in_file_scope ()
         << "Consumer," << endl
         << STRS[INH_RCSB]
         << endl
         << "{" << endl
         << "public:" << endl;

      os << c->type ()->name ().simple () << "Consumer_" << c->name ().simple ()
         << "_Servant (" << endl
         << c->scope ()->name ().scope () << "::CCM_"
         << c->scope ()->name ().simple () << "_ptr executor,"
         << endl
         << c->scope ()->name ().scope () << "::CCM_"
         << c->scope ()->name ().simple ()
         << "_Context_ptr c);" << endl << endl;

      os << "virtual ~" << c->type ()->name ().simple () << "Consumer_"
         << c->name ().simple () << "_Servant (void);" << endl << endl;

      // @@@ (JP) May need to generate this for the eventtype's ancestors
      // as well (the spec is vague on this point). If so, we need the
      // CIDL compiler to support valuetype/eventtype inheritance.
      os << "virtual void" << endl
         << "push_" << c->type ()->name ().simple () << " (" << endl
         << c->type ()->name () << " *evt" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
         << "virtual void" << endl
         << "push_event (::Components::EventBase *ev" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_BET] << "));" << endl << endl;

      os << "// Get component implementation." << endl
         << "virtual CORBA::Object_ptr" << endl
         << "_get_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "protected:" << endl
         << c->scope ()->name ().scope () << "::CCM_"
         << c->scope ()->name ().simple ()
         << "_var" << endl
         << "executor_;" << endl << endl;

      os << c->scope ()->name ().scope () << "::CCM_"
         << c->scope ()->name ().simple ()
         << "_Context_var" << endl
         << "ctx_;" << endl;

      os << "};" << endl << endl;

      os << "virtual " << c->type ()->name () << "Consumer_ptr" << endl
         << "get_consumer_" << c->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

    virtual void
    traverse (EmitsDeclPtr const& e)
    {
      os << "virtual void" << endl
         << "connect_" << e->name ().simple () << " (" << endl
         << e->type ()->name () << "Consumer_ptr c" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_AC] << "));" << endl << endl;

      os << "virtual " << e->type ()->name () << "Consumer_ptr" << endl
         << "disconnect_" << e->name ().simple () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;
    }

    virtual void
    traverse (PublishesDeclPtr const& p)
    {
      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "subscribe_" << p->name ().simple () << " (" << endl
         << p->type ()->name () << "Consumer_ptr c" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_ECL] << "));" << endl << endl;

      os << "virtual " << p->type ()->name () << "Consumer_ptr" << endl
         << "unsubscribe_" << p->name ().simple () << " (" << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "// Operations for Navigation interface." << endl << endl;

      os << "virtual CORBA::Object_ptr" << endl
         << "provide_facet (" << endl
         << "const char *name" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual ::Components::FacetDescriptions *" << endl
         << "get_all_facets (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::FacetDescriptions *" << endl
         << "get_named_facets (" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual CORBA::Boolean" << endl
         << "same_component (" << endl
         << "CORBA::Object_ptr object_ref" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "// Operations for Receptacles interface." << endl << endl;

      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "connect (" << endl
         << "const char *name," << endl
         << "CORBA::Object_ptr connection" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_ECL] << "));" << endl << endl;

      os << "virtual CORBA::Object_ptr" << endl
         << "disconnect (" << endl
         << "const char *name," << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_CR] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;

      os << "virtual ::Components::ConnectionDescriptions *" << endl
         << "get_connections (" << endl
         << "const char *name" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual ::Components::ReceptacleDescriptions *" << endl
         << "get_all_receptacles (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::ReceptacleDescriptions *" << endl
         << "get_named_receptacles (" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "// Operations for Events interface." << endl << endl;

      os << "virtual " << STRS[COMP_ECB] << "_ptr" << endl
         << "get_consumer (" << endl
         << "const char *sink_name" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "subscribe (" << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_ECB] << "_ptr subscriber" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_ECL] << "));" << endl << endl;

      os << "virtual " << STRS[COMP_ECB] << "_ptr" << endl
         << "unsubscribe (" << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;

      os << "virtual void" << endl
         << "connect_consumer (" << endl
         << "const char *emitter_name," << endl
         << STRS[COMP_ECB] << "_ptr consumer" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "));" << endl << endl;

      os << "virtual " << STRS[COMP_ECB] << "_ptr" << endl
         << "disconnect_consumer (" << endl
         << "const char *source_name" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_NC] << "));" << endl << endl;

      os << "virtual ::Components::ConsumerDescriptions *" << endl
         << "get_all_consumers (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::ConsumerDescriptions *" << endl
         << "get_named_consumers (" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual ::Components::EmitterDescriptions *" << endl
         << "get_all_emitters (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::EmitterDescriptions *" << endl
         << "get_named_emitters(" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "virtual ::Components::PublisherDescriptions *" << endl
         << "get_all_publishers (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::PublisherDescriptions *" << endl
         << "get_named_publishers(" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "));" << endl << endl;

      os << "// Operations for CCMObject interface." << endl << endl;

      os << "virtual CORBA::IRObject_ptr" << endl
         << "get_component_def (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::CCMHome_ptr" << endl
         << "get_ccm_home (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::Components::PrimaryKeyBase *" << endl
         << "get_primary_key (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NKA] << "));" << endl << endl;

      os << "virtual void" << endl
         << "configuration_complete (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_ICF] << "));" << endl << endl;

      os << "virtual void" << endl
         << "remove (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_RF] << "));" << endl << endl;

      os << "virtual ::Components::ComponentPortDescription *" << endl
         << "get_all_ports(" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "// Get component implementation." << endl
         << "virtual CORBA::Object_ptr" << endl
         << "_get_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "// CIAO-specific operations." << endl << endl;

      os << "void" << endl
         << "_ciao_activate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "void" << endl
         << "_ciao_passivate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }
  };

  //
  // Generates public part of component servant interface declaration.
  //
  class ServantProtectedEmitter : public HeaderEmitterBase,
                                  public Traversal::ComponentDef,
                                  public Traversal::ProvidesDecl,
                                  public Traversal::ConsumesDecl
  {
  public:
    ServantProtectedEmitter (ostream& os_)
      : HeaderEmitterBase (os_)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      os << "protected:" << endl
         << c->name ().scope () << "::CCM_" << c->name ().simple ()
         << "_var" << endl
         << "executor_;" << endl << endl;

      os << c->name ().simple () << "_Context *" << endl
         << "context_;" << endl << endl;

      os << "::CIAO::Session_Container *" << endl
         << "container_;" << endl << endl;
    }

    virtual void
    traverse (ProvidesDeclPtr const& p)
    {
      os << p->type ()->name () << "_var" << endl
         << "provide_" << p->name ().simple () << "_;" << endl << endl;
    }

    virtual void
    traverse (ConsumesDeclPtr const& c)
    {
      os << c->type ()->name () << "Consumer_var" << endl
         << "consumes_" << c->name ().simple () << "_;" << endl << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "};" << endl;

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
  class HomeEmitter : public HeaderEmitterBase,
                      public Traversal::HomeDef,
                      public Traversal::AttributeDecl //@@ Will disappear
  {
  private:
    string export_macro_;

    OperationEmitter& operation_emitter_;

  public:
    HomeEmitter (ostream& os_,
                 string export_macro,
                 OperationEmitter& operation_emitter)
      : HeaderEmitterBase (os_),
        export_macro_ (export_macro),
        operation_emitter_ (operation_emitter)
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

      // @@@ (JP) Need export macro and prefixable scoped name.
      os << "class " << export_macro_ << " " << h->name ().simple ()
         << "_Servant" << endl
         << ": public virtual POA_" << h->name ().in_file_scope ()
         << "," << endl
         << STRS[INH_RCSB] << endl
         << "{" << endl
         << "public:" << endl;

      os << h->name ().simple () << "_Servant (" << endl
         << h->name ().scope () << "::CCM_" << h->name ().simple ()
         << "_ptr exe," << endl
         << "::CIAO::Session_Container *c);" << endl << endl;

      os << "virtual ~" << h->name ().simple () << "_Servant (void);"
         << endl << endl;

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
      // @@@ (JP) Need primary key support.
      os << "// Operations for keyless home interface." << endl << endl;

      os << "virtual ::Components::CCMObject_ptr" << endl
         << "create_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << "::Components::CreateFailure));" << endl << endl;

      os << "// Operations for implicit home interface." << endl << endl;

      os << "virtual " << h->manages ()->name () << "_ptr" << endl
         << "create (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << "::Components::CreateFailure));" << endl << endl;

      os << "// Operations for CCMHome interface." << endl << endl;

      os << "virtual ::CORBA::IRObject_ptr" << endl
         << "get_component_def (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual ::CORBA::IRObject_ptr" << endl
         << "get_home_def (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual void" << endl
         << "remove_component (" << endl
         << "::Components::CCMObject_ptr comp" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_RF] << "));" << endl << endl;

      os << "// Supported operations." << endl << endl;

      // @@@ (JP) Must include ancestors' supported interfaces as well.
      for (SyntaxTree::HomeDef::Iterator i = h->supports_begin ();
           i != h->supports_end ();
           i++)
      {
        operation_emitter_.dispatch (i->resolve ());
      }

      os << "protected:" << endl
         << "// CIAO-specific operations." << endl << endl;

      os << h->manages ()->name () << "_ptr" << endl
         << "_ciao_activate_component (" << endl
         << h->manages ()->name ().scope () << "::CCM_"
         << h->manages ()->name ().simple () << "_ptr exe" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "void" << endl
         << "_ciao_passivate_component (" << endl
         << h->manages ()->name () << "_ptr comp" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "protected:" << endl;

      os << h->name ().scope () << "::CCM_"
         << h->name ().simple () << "_var" << endl
         << "executor_;" << endl << endl;

      os << "::CIAO::Session_Container *" << endl
         << "container_;" << endl << endl;

      os << "ACE_Hash_Map_Manager_Ex<" << endl
         << "PortableServer::ObjectId," << endl
         << h->manages ()->name ().simple () << "_Servant *," << endl
         << "TAO_ObjectId_Hash," << endl
         << "ACE_Equal_To<PortableServer::ObjectId>," << endl
         << "ACE_SYNCH_MUTEX>" << endl
         << "component_map_;" << endl;

      os << "};" << endl << endl;

      os << "extern \"C\" " << export_macro_ << " ::PortableServer::Servant"
         << endl
         << "create" << h->name ().simple () << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "CIAO::Session_Container *c" << endl
         << STRS[ENV_HDR] << ");" << endl;

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
  class NamespaceEmitter : public HeaderEmitterBase,
                           public Traversal::Module
  {
  private:
    Declarations const& declarations_;

  public:
    NamespaceEmitter (ostream& os_, Declarations const& declarations)
      : HeaderEmitterBase (os_),
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


}



HeaderEmitterBase::HeaderEmitterBase (ostream& os_)
  : os (os_)
{
}

// ===========================================================

ServantHeaderEmitter::ServantHeaderEmitter (
                                            ostream& os_,
                                            CommandLine const& cl,
                                            string export_macro,
                                            Declarations const& declarations)
  : HeaderEmitterBase (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    declarations_ (declarations)
{
}

void
ServantHeaderEmitter::pre (TranslationUnitPtr const& u)
{
  os << COPYRIGHT << endl << endl;

  fs::path file_path = u->principal_translation_region ()->file_path ();
  string file_name ("");

  if (!file_path.empty ())
  {
    file_name = file_path.leaf ();
  }

  string uc_file_name = file_name;

  std::transform (uc_file_name.begin (),
                  uc_file_name.end (),
                  uc_file_name.begin (),
                  upcase);

  string uc_file_suffix = cl_.get_value ("hdr-file-suffix",
                                         "_svnt.h");

  std::transform (uc_file_suffix.begin (),
                  uc_file_suffix.end (),
                  uc_file_suffix.begin (),
                  upcase);

  string guard =
      "CIAO_GLUE_SESSION_"
      + regex::perl_s (uc_file_name,
                       "/(\\.(IDL|CIDL))?$/" +  uc_file_suffix + "/");

  // Replace any remaining '.' with '_'.
  guard = regex::perl_s (guard, "/\\./_/");

  os << "#ifndef " << guard << endl
     << "#define " << guard << endl
     << "#include \"ace/pre.h\"" << endl << endl;

  string export_include = cl_.get_value ("export-include", "");

  if (!export_include.empty ())
  {
    os << "#include \"" << export_include << "\"" << endl << endl;
  }

  // Get the suffix for the executor IDL file and strip off the
  // trailing .idl.
  // @@@ (JP) I guess it's a safe assumption that the tail of the
  // suffix will be .idl.
  string suffix = cl_.get_value ("lem-file-suffix", "_exec.idl");
  suffix = regex::perl_s (suffix,
                          "/(\\.idl)?$//");

  // @@@ (JP) No way of knowing if the IDL compiler had command line
  // option(s) to change C.h and/or S.h. We could add command line options
  // to the CIDL compiler for them, but the user will have to make sure
  // they are in synch with the IDL compiler's options.
  os << "#include \""
     << regex::perl_s (file_name,
                       "/(\\.(idl|cidl))?$/S.h/")
     << "\""
     << endl
     << "#include \""
     << regex::perl_s (file_name,
                       "/(\\.(idl|cidl))?$/" + suffix + "C.h/")
     << "\""
     << endl << endl;

  os << "#include \"ciao/Container_Base.h\"" << endl
     << "#include \"tao/LocalObject.h\"" << endl
     << "#include \"tao/PortableServer/Key_Adapters.h\"" << endl
     << "#include \"ace/Active_Map_Manager_T.h\"" << endl << endl;

  os << "#if !defined (ACE_LACKS_PRAGMA_ONCE)" << endl
     << "# pragma once" << endl
     << "#endif /* ACE_LACKS_PRAGMA_ONCE */" << endl << endl;
}


void
ServantHeaderEmitter::generate (TranslationUnitPtr const& u)
{
  // This method will use the ServantHeaderEmitter class itself
  // as a entry point for the traversal of the tree

  ReturnTypeNameEmitter return_type_name (os);
  INArgTypeNameEmitter inarg_type_name (os);
  OUTArgTypeNameEmitter outarg_type_name (os);
  INOUTArgTypeNameEmitter inoutarg_type_name (os);

  GetAttributeEmitter get_attribute_emitter (os, &return_type_name);
  SetAttributeEmitter set_attribute_emitter (os, &return_type_name);

  OperationEmitter operation_emitter (os,
                                      &return_type_name,
                                      &inarg_type_name,
                                      &outarg_type_name,
                                      &inoutarg_type_name);

  FactoryEmitter factory_emitter (os, &inarg_type_name);

  FacetEmitter facet_emitter (os, export_macro_);
  facet_emitter.add_scope_delegate (&operation_emitter);

  FacetSelector facet_selector (os, declarations_);
  facet_selector.add_delegate (&facet_emitter);


  ContextPublicEmitter context_public_emitter (os, export_macro_);

  ContextProtectedMethodEmitter context_protected_method_emitter (os);

  ContextProtectedMemberEmitter context_protected_member_emitter (os);

  ServantPublicEmitter servant_public_emitter (os,
                                               export_macro_,
                                               operation_emitter);
  servant_public_emitter.add_scope_delegate (&get_attribute_emitter);
  servant_public_emitter.add_scope_delegate (&set_attribute_emitter);

  ServantProtectedEmitter servant_protected_emitter (os);

  // Discriminator for operation and factory
  // This is needed to select always the most-derived type of traversal
  // for any element of the scope of a Home
  Traversal::Discriminator d;
  d.add (&operation_emitter);
  d.add (&factory_emitter);

  HomeEmitter home_emitter (os, export_macro_, operation_emitter);
  home_emitter.add_scope_delegate (&d);

  Traversal::FileScope fs;
  NamespaceEmitter m (os, declarations_);
  fs.add_scope_delegate (&m);

  fs.add_scope_delegate (&context_public_emitter);
  m.add_scope_delegate (&context_public_emitter);
  fs.add_scope_delegate (&context_protected_method_emitter);
  m.add_scope_delegate (&context_protected_method_emitter);
  fs.add_scope_delegate (&context_protected_member_emitter);
  m.add_scope_delegate (&context_protected_member_emitter);
  fs.add_scope_delegate (&servant_public_emitter);
  m.add_scope_delegate (&servant_public_emitter);
  fs.add_scope_delegate (&servant_protected_emitter);
  m.add_scope_delegate (&servant_protected_emitter);


  fs.add_scope_delegate (&home_emitter);
  m.add_scope_delegate (&home_emitter);
  fs.add_scope_delegate (&facet_selector);
  m.add_scope_delegate (&facet_selector);

  Traversal::TranslationRegion region (&fs);

  add_content_delegate (&region);

  // Start processing
  dispatch (u);
}

void
ServantHeaderEmitter::post (TranslationUnitPtr const& u)
{
  fs::path file_path = u->principal_translation_region ()->file_path ();

  if (file_path.empty ()) return;

  string uc_file_name = file_path.leaf ();

  std::transform (uc_file_name.begin (),
                  uc_file_name.end (),
                  uc_file_name.begin (),
                  upcase);

  string uc_file_suffix = cl_.get_value ("hdr-file-suffix",
                                         "_svnt.h");

  std::transform (uc_file_suffix.begin (),
                  uc_file_suffix.end (),
                  uc_file_suffix.begin (),
                  upcase);

  string guard = "CIAO_GLUE_SESSION_"
      + regex::perl_s (uc_file_name,
                       "/(\\.(IDL|CIDL))?$/" +  uc_file_suffix + "/");

  guard = regex::perl_s (guard, "/\\./_/");

  os << "#include \"ace/post.h\"" << endl
     << "#endif /* " << guard << " */"
     << endl << endl;
}


/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
