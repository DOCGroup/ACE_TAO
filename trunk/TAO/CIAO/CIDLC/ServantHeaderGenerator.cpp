// file      : CIDLC/ServantHeaderGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantHeaderGenerator.hpp"
#include "TypeNameEmitter.hpp"
#include "Literals.hpp"

#include <ostream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using std::endl;
using std::string;

using namespace StringLiterals;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
  // On some platforms toupper can be something other than a
  // function.
  int
  upcase (int c)
  {
    return std::toupper (c);
  }
}

namespace
{
  class Context
  {
  public:
    Context (std::ostream& os,
             string export_macro)
        : os_ (os),
          export_macro_ (export_macro)
    {
    }

  std::ostream&
  os ()
  {
    return os_;
  }

  string
  export_macro ()
  {
    return export_macro_;
  }

  private:
    std::ostream& os_;
    string export_macro_;
  };

  class EmitterBase
  {
  public:
    EmitterBase (Context& c)
      : ctx (c),
        os (ctx.os ())
    {
    }

  protected:
    Context& ctx;
    std::ostream& os;
  };

  struct OperationEmitter : Traversal::Operation, EmitterBase
  {
    OperationEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    pre (Type&)
    {
      os << "virtual ";
    }

    virtual void
    name (Type& o)
    {
      os << endl << o.name ();
    }

    virtual void
    receives_pre (Type&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_none (Type&)
    {
      os << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_HDR] << ")" << endl;
    }

    virtual void
    raises_pre (Type&)
    {
      os << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl;
    }

    virtual void
    raises_none (Type&)
    {
      os << STRS[EXCP_SNGL];
    }

    virtual void
    raises_post (Type&)
    {
      os << "))";
    }

    virtual void
    post (Type&)
    {
      os << ";" << endl << endl;
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }
  };

  // For generating parameter names.
  template <typename T>
  struct ParameterEmitter : T
  {
    ParameterEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    name (typename T::Type& p)
    {
      os << " " << p.name ();
    }

  private:
    std::ostream& os;
  };

  // Generic scoped typename emitter used by various other emitters.
  struct TypeNameEmitter : Traversal::Type
  {
    TypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.scoped_name ();
    }

  private:
    std::ostream& os;
  };

  // Generic local typename emitter used by various other emitters.
  struct SimpleTypeNameEmitter : Traversal::Type
  {
    SimpleTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.name ();
    }

  private:
    std::ostream& os;
  };

  // Generate name of type's enclosing scope.
  struct EnclosingTypeNameEmitter : Traversal::Type
  {
    EnclosingTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.scoped_name ().scope_name ();
    }

  private:
    std::ostream& os;
  };

  // Emits typename minus the leading double colon.
  struct StrippedTypeNameEmitter : Traversal::Type
  {
    StrippedTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      ScopedName scoped (t.scoped_name ());
      os << Name (scoped.begin () + 1, scoped.end ());
    }

  private:
    std::ostream& os;
  };

  // Generates operations associated with attributes.
  // @@@ (JP) Need to support exceptions.
  struct AttributeEmitter : Traversal::ReadWriteAttribute,
                            EmitterBase
  {
    AttributeEmitter (Context& c)
      : EmitterBase (c),
        write_type_name_emitter_ (c.os ()),
        read_type_name_emitter_ (c.os ())
    {
      write_belongs_.node_traverser (write_type_name_emitter_);
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadWriteAttribute& a)
    {
      os << "virtual ";

      Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

      os << endl
         << a.name () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "virtual void" << endl
         << a.name () << " (" << endl;

      Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

      os << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

  private:
    INArgTypeNameEmitter write_type_name_emitter_;
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs write_belongs_;
    Traversal::Belongs read_belongs_;
  };

  // Generates operations associated with readonly attributes.
  // @@@ (JP) Need to support exceptions.
  struct ReadOnlyAttributeEmitter : Traversal::ReadAttribute,
                                    EmitterBase
  {
    ReadOnlyAttributeEmitter (Context& c)
      : EmitterBase (c),
        read_type_name_emitter_ (c.os ())
    {
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadAttribute& a)
    {
      os << "virtual ";

      Traversal::ReadAttribute::belongs (a, read_belongs_);

      os << endl
         << a.name () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;
    }

  private:
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs read_belongs_;
  };

  struct InterfaceEmitter : Traversal::UnconstrainedInterface,
                            EmitterBase
  {
    InterfaceEmitter (Context& c)
      : EmitterBase (c)
    {}

    bool
    add (UnconstrainedInterface& i)
    {
      return interfaces_.insert (&i).second;
    }

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      if (add (i))
      {
        Traversal::UnconstrainedInterface::traverse (i);
      }
    }

  private:
    std::set<UnconstrainedInterface*> interfaces_;
  };

  struct FacetEmitter : Traversal::UnconstrainedInterface,
                        EmitterBase
  {
    FacetEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      ScopedName scoped (i.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      // Open a namespace.
      os << STRS[GLUE_NS]
         << regex::perl_s (i.scoped_name ().scope_name ().str (),
                           "/::/_/")
         << "{"
         << "class " << ctx.export_macro () << " " << i.name ()
         << "_Servant" << endl
         << ": public virtual POA_" << stripped << "," << endl
         << STRS[INH_RCSB] << endl
         << "{"
         << "public:" << endl
         << i.name () << "_Servant (" << endl
         << i.scoped_name ().scope_name () << "::CCM_" << i.name ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr ctx);" << endl
         << "virtual ~" << i.name () << "_Servant (void);" << endl;

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines_.node_traverser (attribute_emitter);
        defines_.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        inherits (i, inherits_);
        names (i, defines_);
      }

      os << "// Get component implementation." << endl
         << "virtual CORBA::Object_ptr" << endl
         << "_get_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl
         << "protected:" << endl
         << "// Facet executor." << endl
         << i.scoped_name ().scope_name ()<< "::CCM_" << i.name ()
         << "_var executor_;" << endl
         << "// Context object." << endl
         << "::Components::CCMContext_var ctx_;" << endl
         << "};" << endl;

      // Close the CIAO_GLUE namespace.
      os << "}" << endl;
    }
  };

  struct ContextEmitter : Traversal::Component, EmitterBase
  {
    ContextEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by this emitter.
  private:
    struct PortsEmitterPublic : Traversal::EmitterData,
                                Traversal::UserData,
                                Traversal::PublisherData,
                                EmitterBase
    {
      PortsEmitterPublic (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "virtual void" << endl
          << "push_" << e.name ()
          << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "ev" << endl
            << STRS[ENV_HDR] << ")" << endl
            << STRS[EXCP_SNGL] << ";" << endl;
      }

      virtual void
      traverse (SemanticGraph::User& u)
      {
        // @@@ (JP) Need to handle multiple connections.
        os << "virtual ";

        Traversal::UserData::belongs (u, belongs_);

        os << endl
            << "get_connection_" << u.name ()
            << " (" << endl
            << STRS[ENV_SNGL_HDR] << ")" << endl
            << STRS[EXCP_SNGL] << ";" << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual void" << endl
          << "push_" << p.name ()
          << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "ev" << endl
            << STRS[ENV_HDR] << ")" << endl
            << STRS[EXCP_SNGL] << ";" << endl;
      }

    private:
      INArgTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PortsEmitterProtected : Traversal::EmitterData,
                                   Traversal::UserData,
                                   Traversal::PublisherData,
                                   EmitterBase
    {
      PortsEmitterProtected (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "virtual void" << endl
          << "connect_" << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "));" << endl << endl;

        os << "virtual ";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << "disconnect_" << e.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "));" << endl;
      }

      virtual void
      traverse (SemanticGraph::User& u)
      {
        // @@@ (JP) Need to handle multiple connections.
        os << "virtual void" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "));" << endl << endl;

        os << "virtual ";

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "));" << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual " << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os  << "Consumer_ptr c" << endl
            << STRS[ENV_HDR] << ")" << endl
            << STRS[EXCP_START] << endl
            << STRS[EXCP_SYS] << "," << endl
            << STRS[EXCP_ECL] << "));" << endl << endl;

        os << "virtual ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << "unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "));" << endl << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PortsEmitterMembers : Traversal::EmitterData,
                                 Traversal::UserData,
                                 Traversal::PublisherData,
                                 EmitterBase
    {
      PortsEmitterMembers (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_var" << endl
           << "ciao_emits_" << e.name () << "_consumer_;"
           << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::User& u)
      {
        // @@@ (JP) Need to handle multiple connections.
        os << "// Simplex " << u.name () << " connection." << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "_var" << endl
           << "ciao_uses_" << u.name () << "_;" << endl << endl;
     }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "ACE_Active_Map_Manager<" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>" << endl
           << "ciao_publishes_" << p.name () << "_map_;"
           << endl << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      // CIAO has facet classes outside the context and servant classes.
      // We launch it here to generate the classes at the top of the file,
      // like the original hand-crafted examples, for easier checking.
      // This can be moved later to ServantEmitter::pre() or out on its own.
      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        Traversal::Provider provider;
        defines.node_traverser (provider);

        Traversal::Belongs belongs;
        provider.edge_traverser (belongs);

        FacetEmitter facet_emitter (ctx);
        belongs.node_traverser (facet_emitter);

        component_emitter.traverse (t);
      }

      os << STRS[GLUE_NS]
          << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
          << "{";

      os << "class " << ctx.export_macro () << " " << t.name ()
          << "_Context" << endl
          << ": public virtual " << t.scoped_name ().scope_name () << "::CCM_"
          << t.name () << "_Context,"
          << endl
          << "public virtual TAO_Local_RefCounted_Object"
          << endl
          << "{"
          << "public:" << endl;

      os << "// We will allow the servant glue code we generate to "
          << "access our state." << endl
          << "friend class " << t.name () << "_Servant;"
          << endl;

      os << t.name () << "_Context (" << endl
          << "::Components::CCMHome_ptr home," << endl
          << "::CIAO::Session_Container *c," << endl
          << t.name () << "_Servant *sv);" << endl;

      os << "virtual ~" << t.name () << "_Context (void);"
          << endl;

      os << "// Operations from ::Components::CCMContext." << endl << endl;

      os << "virtual ::Components::Principal_ptr" << endl
          << "get_caller_principal (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_SNGL] << ";" << endl;

      os << "virtual ::Components::CCMHome_ptr" << endl
          << "get_CCM_home (" << endl
          << STRS[ENV_SNGL_HDR_NOTUSED] << ")" << endl
          << STRS[EXCP_SNGL] << ";" << endl ;

      os << "virtual CORBA::Boolean" << endl
          << "get_rollback_only (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl
          << STRS[EXCP_IS] << "));" << endl;

      os << "virtual ::Components::Transaction::UserTransaction_ptr" << endl
          << "get_user_transaction (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl
          << STRS[EXCP_IS] << "));" << endl;

      os << "virtual CORBA::Boolean" << endl
          << "is_caller_in_role (" << endl
          << "const char *role" << endl
          << STRS[ENV_HDR] << ")" << endl
          << STRS[EXCP_SNGL] << ";" << endl;

      os << "virtual void" << endl
          << "set_rollback_only (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl
          << STRS[EXCP_IS] << "));" << endl;

      os << "// Operations from ::Components::SessionContext interface."
          << endl << endl;

      os << "virtual CORBA::Object_ptr" << endl
          << "get_CCM_object (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl
          << STRS[EXCP_IS] << "));" << endl;

      os << "// Operations for " << t.name () << " receptacles"
          << " and event sources," << endl
          << "// defined in " << t.scoped_name ().scope_name () << "::CCM_"
          << t.name () << "_Context."
          << endl << endl;

      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);
        
        PortsEmitterPublic ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }
      
      os << "// CIAO-specific." << endl << endl
         << "::CIAO::Session_Container *" << endl
         << "_ciao_the_Container (void) const;" << endl;
         
      os << "static " << t.name () << "_Context *" << endl
         << "_narrow (" << endl
         << "::Components::SessionContext_ptr p" << endl
         << STRS[ENV_HDR] << ");" << endl;

      os << "protected:" << endl
         << "// Methods that manage this component's connections"
         << " and consumers." << endl << endl;

      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        PortsEmitterProtected ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }

      os << "protected:" << endl;

      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        PortsEmitterMembers ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }
    }

    virtual void
    post (Type& t)
    {
      os << "::Components::CCMHome_var home_;" << endl << endl
         << "::CIAO::Session_Container *container_;" << endl << endl
         << t.name () << "_Servant *servant_;" << endl << endl
         << t.scoped_name () << "_var component_;" << endl << endl;

      // Component context class closer.
      os << "};" << endl;

      // Namespace closer.
      os << "}" << endl;
    }
  };

  struct ServantEmitter : Traversal::Component, EmitterBase
  {
    ServantEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by this emitter.
  private:
    struct PortsEmitterPublic : Traversal::EmitterData,
                                Traversal::UserData,
                                Traversal::PublisherData,
                                Traversal::ConsumerData,
                                Traversal::ProviderData,
                                EmitterBase
    {
      PortsEmitterPublic (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          stripped_type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        stripped_belongs_.node_traverser (stripped_type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Provider& p)
      {
        os << "virtual ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "_ptr" << endl
           << "provide_" << p.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_SNGL] << ";" << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::User& u)
      {
        // @@@ (JP) Need to handle multiple connections.
        os << "virtual void" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr c" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "));" << endl << endl;

        os << "virtual ";

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "));" << endl << endl;

        os << "virtual ";

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "get_connection_" << u.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_SNGL] << ";" << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::Consumer& c)
      {
        os << "// Servant class for the " << c.name ()
           << " consumer." << endl
           << "class " << ctx.export_macro () << " ";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_"
           << c.name () << "_Servant" << endl
           << ": public virtual POA_";

        Traversal::ConsumerData::belongs (c, stripped_belongs_);

        os << "Consumer," << endl
           << STRS[INH_RCSB] << endl
           << "{"
           << "public:" << endl;

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name () << "_Servant (" << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name () << "_ptr executor,"
           << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context_ptr c);" << endl << endl;

        os << "virtual ~";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_"
           << c.name () << "_Servant (void);" << endl << endl;

        // @@@ (JP) May need to generate this for the eventtype's ancestors
        // as well (the spec is vague on this point). If so, we need the
        // CIDL compiler to support valuetype/eventtype inheritance.
        os << "virtual void" << endl
           << "push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os  << " (" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *evt" << endl
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
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_var" << endl
           << "executor_;" << endl << endl;

        os << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context_var" << endl
           << "ctx_;" << endl;

        os << "};" << endl << endl;

        os << "virtual ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_ptr" << endl
           << "get_consumer_" << c.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_SNGL] << ";" << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "virtual void" << endl
           << "connect_" << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "));" << endl << endl;

        os << "virtual ";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << "disconnect_" << e.name () << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "));" << endl << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual " << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "));" << endl << endl;

        os << "virtual ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << "unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_HDR] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "));" << endl << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      StrippedTypeNameEmitter stripped_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs stripped_belongs_;
    };

    struct PortsEmitterProtected : Traversal::ConsumerData,
                                   Traversal::ProviderData,
                                   EmitterBase
    {
      PortsEmitterProtected (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Provider& p)
      {
        Traversal::ProviderData::belongs (p, belongs_);

        os << "_var" << endl
           << "provide_" << p.name () << "_;" << endl;
      }

      virtual void
      traverse (SemanticGraph::Consumer& c)
      {
        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_var" << endl
           << "consumes_" << c.name () << "_;" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << STRS[GLUE_NS]
          << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
          << "{";

      ScopedName scoped (t.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      os << "class " << ctx.export_macro () << " " << t.name ()
         << "_Servant" << endl
         << ": public virtual POA_" << stripped << "," << endl
         << STRS[INH_RCSB] << endl
         << "{"
         << "public:" << endl;

      os << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr executor," << endl
         << "::Components::CCMHome_ptr home," << endl
         << "::CIAO::Session_Container *c);" << endl << endl;

      os << "virtual ~" << t.name () << "_Servant (void);"
         << endl << endl;

      os << "// Supported operations." << endl << endl;

      // Generate operations for all supported interfaces.
      { 
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;                
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Supports supports_;
        component_emitter.edge_traverser (component_inherits);
        component_emitter.edge_traverser (supports_);
        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits interface_inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (interface_inherits);

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        component_emitter.traverse (t);
      }

      os << "// Public port operations." << endl << endl;

      // Generate public operations for ports, and nested classes for
      // event consumers.
      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;                
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (component_inherits);
        component_emitter.edge_traverser (defines);
        
        PortsEmitterPublic ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }

      // Generate operations for component attributes.
      os << "// Component attribute operations." << endl << endl;

      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;                
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        component_emitter.traverse (t);
      }

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
          << STRS[ENV_SNGL_HDR_NOTUSED] << ")" << endl
          << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl
          << STRS[EXCP_ICF] << "));" << endl << endl;

      os << "virtual void" << endl
          << "remove (" << endl
          << STRS[ENV_SNGL_HDR_NOTUSED] << ")" << endl
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

      os << "protected:" << endl
          << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
          << "_var" << endl
          << "executor_;" << endl << endl;

      os << t.name () << "_Context *" << endl
          << "context_;" << endl << endl;

      os << "::CIAO::Session_Container *" << endl
          << "container_;" << endl;

      // Generate protected operations for facets and event sinks.
      {
        Traversal::Component component_emitter;
        
        Traversal::Inherits component_inherits;                
        component_inherits.node_traverser (component_emitter);
        
        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        PortsEmitterProtected ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }
    }

    virtual void
    post (Type& t)
    {
      // Component servant class closer.
      os << "};" << endl;

      // Namespace closer.
      os << "}" << endl;
    }
  };

  struct HomeEmitter : Traversal::Home, EmitterBase
  {
    HomeEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by this emitter.
  private:
    struct ManagesEmitter : Traversal::Component, EmitterBase
    {
      ManagesEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& c)
      {
        os << c.scoped_name ();
      }
    };

    struct ManagesSimpleEmitter : Traversal::Component, EmitterBase
    {
      ManagesSimpleEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& c)
      {
        os << c.name ();
      }
    };

    struct ManagesScopeEmitter : Traversal::Component, EmitterBase
    {
      ManagesScopeEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& c)
      {
        os << c.scoped_name ().scope_name ();
      }
    };
    
    // HomeFactory and HomeFinder are tied to Operation in
    // the front end. Since we want to treat them differently
    // than regular operations in a home (we don't want to
    // generate anything for base class factory operations,
    // example), we use this class for regular home operations
    // that overrides HomeFactory and HomeFinder traversals
    // to do nothing.
    struct HomeOperationEmitter : OperationEmitter,
                                  Traversal::HomeFactory,
                                  Traversal::HomeFinder
    {
      HomeOperationEmitter (Context& c)
        : OperationEmitter (c)
      {}

      virtual void
      traverse (SemanticGraph::HomeFactory&)
      {
      }

      virtual void
      traverse (SemanticGraph::HomeFinder&)
      {
      }
    };

    struct FactoryOperationEmitter : Traversal::HomeFactory,
                                     Traversal::Home,
                                     EmitterBase
    {
      FactoryOperationEmitter (Context& c, SemanticGraph::Home& home)
        : EmitterBase (c),
          home_ (home)
      {}

      virtual void
      pre (SemanticGraph::HomeFactory&)
      {
        os << "virtual ";
      }

      virtual void
      returns (SemanticGraph::HomeFactory& hf)
      {
        ReturnTypeNameEmitter returns_emitter (os);
        Traversal::Returns returns_;
        returns_.node_traverser (returns_emitter);

        Traversal::HomeFactory::returns (hf, returns_);

        os << endl;
      }

      virtual void
      name (SemanticGraph::HomeFactory& hf)
      {
        os << hf.name ();
      }

      virtual void
      receives_none (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl
           << STRS[ENV_SNGL_HDR] << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFactory&)
      {
        os << endl << STRS[ENV_HDR] << ")" << endl;
      }

      virtual void
      raises_none (SemanticGraph::HomeFactory&)
      {
        os << STRS[EXCP_SNGL];
      }

      virtual void
      raises_pre (SemanticGraph::HomeFactory&)
      {
        os << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl;
      }

      virtual void
      raises_post (SemanticGraph::HomeFactory&)
      {
        os << "))";
      }

      virtual void
      post (SemanticGraph::HomeFactory&)
      {
        os << ";" << endl << endl;
      }

      virtual void
      comma (SemanticGraph::HomeFactory&)
      {
        os << "," << endl;
      }

    private:
      SemanticGraph::Home& home_;
    };

  public:
    virtual void pre (Type& t)
    {
      os << STRS[GLUE_NS]
          << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
          << "{";

      ScopedName scoped (t.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      os << "class " << ctx.export_macro () << " " << t.name ()
         << "_Servant" << endl
         << ": public virtual POA_" << stripped << "," << endl
         << STRS[INH_RCSB] << endl
         << "{"
         << "public:" << endl;

      os << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::CIAO::Session_Container *c);" << endl << endl;

      os << "virtual ~" << t.name () << "_Servant (void);"
         << endl << endl;

      // Generate home operations.
      os << "// Home operations." << endl << endl;

      {
        Traversal::Home home_emitter;
        
        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);
        
        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);
        home_emitter.edge_traverser (home_inherits);
        
        HomeOperationEmitter home_operation_emitter (ctx);
        defines.node_traverser (home_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        home_operation_emitter.edge_traverser (receives);
        home_operation_emitter.edge_traverser (returns);
        home_operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      // Generate home factory operations. This is a separate traversal
      // stack because we don't want to generate the factory operations
      // of ancestors.
      os << "// Home factory operations." << endl << endl;

      {
        Traversal::Home home_emitter;
        
        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);
        
        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);
        
        FactoryOperationEmitter factory_operation_emitter (ctx, t);
        defines.node_traverser (factory_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        factory_operation_emitter.edge_traverser (receives);
        factory_operation_emitter.edge_traverser (returns);
        factory_operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }
      
     // Generate operations for home attributes.
      os << "// Attribute operations." << endl << endl;

      {
        Traversal::Home home_emitter;
        
        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);
        
        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);
        home_emitter.edge_traverser (home_inherits);
        
        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        home_emitter.traverse (t);
      }

      // @@@ (JP) Need primary key support.
      os << "// Operations for keyless home interface." << endl << endl;

      os << "virtual ::Components::CCMObject_ptr" << endl
         << "create_component (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << "::Components::CreateFailure));" << endl << endl ;

      os << "// Operations for implicit home interface." << endl << endl;

      os << "virtual ";

      {
        Traversal::Manages manages_;
        ManagesEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "_ptr" << endl
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

      // Generate operations for all supported interfaces.
      {
        Traversal::Home home_emitter;
        
        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);
        
        Traversal::Supports supports_;
        home_emitter.edge_traverser (supports_);
        home_emitter.edge_traverser (home_inherits);
        
        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (inherits);

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      os << "protected:" << endl
         << "// CIAO-specific operations." << endl << endl;

      {
        Traversal::Manages manages_;
        ManagesEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "_ptr" << endl
         << "_ciao_activate_component (" << endl;

      {
        Traversal::Manages manages_;
        ManagesScopeEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "::CCM_";

      {
        Traversal::Manages manages_;
        ManagesSimpleEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "_ptr exe" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "void" << endl
         << "_ciao_passivate_component (" << endl;

      {
        Traversal::Manages manages_;
        ManagesEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "_ptr comp" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl << endl;

      os << "protected:" << endl;

      os << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_var" << endl
         << "executor_;" << endl << endl;

      os << "::CIAO::Session_Container *" << endl
         << "container_;" << endl << endl;

      os << "ACE_Hash_Map_Manager_Ex<" << endl
         << "PortableServer::ObjectId," << endl;

      {
        Traversal::Manages manages_;
        ManagesSimpleEmitter manages_emitter (ctx);
        manages_.node_traverser (manages_emitter);

        manages (t, manages_);
      }

      os << "_Servant *," << endl
         << "TAO_ObjectId_Hash," << endl
         << "ACE_Equal_To<PortableServer::ObjectId>," << endl
         << "ACE_SYNCH_MUTEX>" << endl
         << "component_map_;" << endl;

      // Home servant class closer.
      os << "};" << endl << endl;

      os << "extern \"C\" " << ctx.export_macro () << " ::PortableServer::Servant"
         << endl
         << "create" << t.name () << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "CIAO::Session_Container *c" << endl
         << STRS[ENV_HDR] << ");" << endl;
    }

    virtual void post (Type& t)
    {
      // Namespace closer.
      os << "}" << endl;
    }
  };


  struct CompositionEmitter : Traversal::Composition, EmitterBase
  {
    CompositionEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << endl 
         << "namespace " << t.name () << "{";
    }

    virtual void
    post (Type& t)
    {
      os << "}";
    }
  };
  
  struct IncludesEmitter : Traversal::QuoteIncludes,
                           Traversal::BracketIncludes,
                           EmitterBase
  {
    IncludesEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (SemanticGraph::QuoteIncludes& qi)
    {
      os << "#include \""
         << regex::perl_s (qi.file ().string (),
                           "/(\\.(idl|cidl))?$/S.h/")
         << "\""
         << endl;
    }

    virtual void
    traverse (SemanticGraph::BracketIncludes& bi)
    {
      os << "#include \""
         << regex::perl_s (bi.file ().string (),
                           "/(\\.(idl|cidl))?$/S.h/")
         << "\""
         << endl;  
    }
  };
}

ServantHeaderEmitter::ServantHeaderEmitter (std::ostream& os_,
                                            CommandLine const& cl,
                                            string export_macro,
                                            fs::path const& file)
  : os (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    file_ (file)
{}

void
ServantHeaderEmitter::pre (TranslationUnit& u)
{
  os << COPYRIGHT;

  string file_name ("");

  if (!file_.empty ())
  {
    file_name = file_.leaf ();
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
     << "#define " << guard << endl << endl
     << "#include /**/ \"ace/pre.h\"" << endl << endl;

  string export_include = cl_.get_value ("export-include", "");

  if (!export_include.empty ())
  {
    os << "#include \"" << export_include << "\"" << endl << endl;
  }

  // Get the suffix for the executor IDL file and strip off the
  // trailing .idl.
  // @@@ (JP) I guess it's a safe assumption that the tail of the
  // suffix will be .idl.
  string suffix = cl_.get_value ("lem-file-suffix", "E.idl");
  suffix = regex::perl_s (suffix,
                          "/(\\.idl)?$//");

  // @@@ (JP) No way of knowing if the IDL compiler had command line
  // option(s) to change C.h and/or S.h. We could add command line options
  // to the CIDL compiler for them, but the user will have to make sure
  // they are in synch with the IDL compiler's options.
  os << "#include \""
     << regex::perl_s (file_name,
                       "/(\\.(idl|cidl))?$/" + suffix + "C.h/")
     << "\""
     << endl << endl;

  os << "#if !defined (ACE_LACKS_PRAGMA_ONCE)" << endl
     << "# pragma once" << endl
     << "#endif /* ACE_LACKS_PRAGMA_ONCE */" << endl << endl;

  os << "#include \"ciao/Container_Base.h\"" << endl
     << "#include \"tao/LocalObject.h\"" << endl
     << "#include \"tao/PortableServer/Key_Adapters.h\"" << endl
     << "#include \"ace/Active_Map_Manager_T.h\"" << endl << endl;
}

void
ServantHeaderEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_);

  Traversal::TranslationUnit unit;

  // Layer 1
  //
  Traversal::ContainsPrincipal contains_principal;
  unit.edge_traverser (contains_principal);

  //--
  Traversal::TranslationRegion principal_region;
  contains_principal.node_traverser (principal_region);

  // Layer 2
  //
  Traversal::TranslationRegion included_region;

  // Inclusion handling is somewhat tricky because we want
  // to print only top-level #includes.

  Traversal::ContainsRoot contains_root;
  Traversal::QuoteIncludes quote_includes;
  Traversal::BracketIncludes bracket_includes;
  IncludesEmitter includes_emitter (c);

  principal_region.edge_traverser (includes_emitter);
  principal_region.edge_traverser (quote_includes);
  principal_region.edge_traverser (bracket_includes);
  principal_region.edge_traverser (contains_root);

  included_region.edge_traverser (quote_includes);
  included_region.edge_traverser (bracket_includes);
  included_region.edge_traverser (contains_root);

  //--
  Traversal::Root root;

  contains_root.node_traverser (root);
  quote_includes.node_traverser (included_region);
  bracket_includes.node_traverser (included_region);

  // Layer 3
  //
  Traversal::Defines defines;
  root.edge_traverser (defines);

  //--
  Traversal::Module module;
  CompositionEmitter composition (c);
  defines.node_traverser (module);
  defines.node_traverser (composition);

  // Layer 4
  //
  Traversal::Defines composition_defines;
  composition.edge_traverser (composition_defines);
  
  module.edge_traverser (defines);

  //--
  Traversal::ComponentExecutor component_executor;
  Traversal::HomeExecutor home_executor;
  composition_defines.node_traverser (component_executor);
  composition_defines.node_traverser (home_executor);

  // Layer 5
  //
  Traversal::Implements implements;
  component_executor.edge_traverser (implements);
  home_executor.edge_traverser (implements);

  //--
  ContextEmitter context_emitter (c);
  ServantEmitter servant_emitter (c);
  HomeEmitter home_emitter (c);
  implements.node_traverser (context_emitter);
  implements.node_traverser (servant_emitter);
  implements.node_traverser (home_emitter);

  unit.traverse (u);

  post (u);
}

void
ServantHeaderEmitter::post (TranslationUnit& u)
{
  if (file_.empty ()) return;

  string uc_file_name = file_.leaf ();

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

  guard = regex::perl_s (guard, "/\\./_/");

  os << endl
     << "#include /**/ \"ace/post.h\"" << endl << endl
     << "#endif /* " << guard << " */"
     << endl << endl;
}
