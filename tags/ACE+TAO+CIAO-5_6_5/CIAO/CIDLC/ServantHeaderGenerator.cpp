// file      : CIDLC/ServantHeaderGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantHeaderGenerator.hpp"
#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"
#include "AttributeHeaderEmitters.hpp"
#include "OperationHeaderEmitters.hpp"
#include "ParameterEmitter_T.hpp"
#include "CompositionEmitter.hpp"
#include "ModuleEmitter.hpp"
#include "InterfaceEmitter.hpp"
#include "Literals.hpp"
#include "Upcase.hpp"

#include <ostream>
#include <sstream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using std::endl;
using std::string;

using namespace StringLiterals;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
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
      if (i.context ().count ("facet_hdr_gen")) return;

      ScopedName scoped (i.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      // We need to escape C++ keywords before flattening the name.
      //
      string name;
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << scoped.scope_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }

      /// Open a namespace made from the interface scope's name.
      os << "namespace " << STRS[FACET_PREFIX] << name
         << "{";

      os << "template <typename T>" << endl
         << "class " << i.name () << "_Servant_T" << endl
         << ": public virtual POA_" << stripped << endl
         << "{"
         << "public:" << endl
         << i.name () << "_Servant_T (" << endl
         << i.scoped_name ().scope_name () << "::CCM_" << i.name ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr ctx);" << endl
         << "virtual ~" << i.name () << "_Servant_T (void);" << endl;

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        // Works for both read/write and readonly attributes.
        AttributeHeaderEmitter attribute_emitter (ctx);
        defines_.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        inherits (i, inherits_);
        names (i, defines_);
      }

      os << "// Get component implementation." << endl
         << "virtual CORBA::Object_ptr" << endl
         << "_get_component ();" << endl
         << "protected:" << endl
         << "// Facet executor." << endl
         << i.scoped_name ().scope_name ()<< "::CCM_" << i.name ()
         << "_var executor_;" << endl
         << "// Context object." << endl
         << "::Components::CCMContext_var ctx_;" << endl
         << "};";

      os << "typedef " << i.name () << "_Servant_T<int> "
         << i.name () << "_Servant;";

      // Close the facet servant's namespace.
      os << "}";

      i.context ().set ("facet_hdr_gen", true);
    }
  };

  struct ContextEmitter : Traversal::Component, EmitterBase
  {
    ContextEmitter (Context& c, CommandLine const& cl)
      : EmitterBase (c),
        cl_ (cl)
    {}

  // Nested classes used by this emitter.
  private:
    struct PortsEmitterPublic : Traversal::EmitterData,
                                Traversal::SingleUserData,
                                Traversal::MultiUserData,
                                Traversal::PublisherData,
                                EmitterBase
    {
      PortsEmitterPublic (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
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

        os << "ev);" << endl;
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "virtual ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << endl
            << "get_connection_" << u.name ()
            << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "virtual " << u.scoped_name () << "Connections *" << endl
           << "get_connections_" << u.name ()
           << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual void" << endl
          << "push_" << p.name ()
          << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "ev);" << endl;
      }

    private:
      INArgTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PortsEmitterProtected : Traversal::EmitterData,
                                   Traversal::SingleUserData,
                                   Traversal::MultiUserData,
                                   Traversal::PublisherData,
                                   EmitterBase
    {
      PortsEmitterProtected (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "virtual void" << endl
          << "connect_" << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c);" << endl;

        os << "virtual ";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << "disconnect_" << e.name () << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "virtual void" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr);" << endl;

        os << "virtual ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "virtual ::Components::Cookie *" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr);" << endl;

        os << "virtual ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " (" << endl
           << "::Components::Cookie * ck);" << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual " << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c);" << endl;

        os << "// CIAO-specific." << endl
           << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << "_generic (" << endl
           << STRS[COMP_ECB] << "_ptr c);" << endl;

        os << "virtual ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << "unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck);" << endl;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PortsEmitterMembers : Traversal::EmitterData,
                                 Traversal::SingleUserData,
                                 Traversal::MultiUserData,
                                 Traversal::PublisherData,
                                 EmitterBase
    {
      PortsEmitterMembers (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_var" << endl
           << "ciao_emits_" << e.name () << "_consumer_;"
           << endl;
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "// Simplex " << u.name () << " connection." << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_var" << endl
           << "ciao_uses_" << u.name () << "_;" << endl;
     }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << endl
           << "// Multiplex " << u.name () << " connection." << endl
           << "typedef ACE_Array_Map<ptrdiff_t," << endl
           << "                      ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var>" << endl
           << "  ";

        string uc_port_name (u.name ().str ());
        str_upcase (uc_port_name);

        os << uc_port_name << "_TABLE;"
           << uc_port_name << "_TABLE ciao_uses_"
           << u.name () << "_;";

        if (! ctx.cl ().get_value ("static-config", false))
        {
          os << "TAO_SYNCH_MUTEX " << u.name () << "_lock_;";
        }
     }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << endl
           << "typedef ACE_Array_Map<ptrdiff_t," << endl
           << "                      ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>" << endl
           << "  ";

        string uc_port_name (p.name ().str ());
        str_upcase (uc_port_name);

        os << uc_port_name << "_TABLE;"
           << uc_port_name << "_TABLE ciao_publishes_"
           << p.name () << "_;";

        if (! ctx.cl ().get_value ("static-config", false))
        {
          os << "TAO_SYNCH_MUTEX " << p.name () << "_lock_;";
        }

        os << endl
           << "typedef ACE_Array_Map<ptrdiff_t," << endl
           << "                      " << STRS[COMP_ECB]
           << "_var>" << endl
           << "  " << uc_port_name << "_GENERIC_TABLE;"
           << uc_port_name << "_GENERIC_TABLE ciao_publishes_"
           << p.name () << "_generic_;";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << "class " << t.name () << "_Servant;" << endl;

      string swap_option = cl_.get_value ("custom-container", "");
      bool swapping = (swap_option == "upgradeable");

      os << "class " << ctx.export_macro () << " " << t.name ()
         << "_Context" << endl
         << "  : public virtual CIAO::"
         << (swapping ? "Upgradeable_" : "") << "Context_Impl<" << endl
         << "      " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_Context," << endl
         << "      " << t.name () << "_Servant," << endl
         << "      " << t.scoped_name () << endl
         << "    >" << endl
         << "{"
         << "public:" << endl;

      os << "// We will allow the servant glue code we generate to "
         << "access our state." << endl
         << "friend class " << t.name () << "_Servant;" << endl;

      os << t.name () << "_Context (" << endl
         << "::Components::CCMHome_ptr h," << endl
         << "::CIAO::Session_Container *c," << endl
         << t.name () << "_Servant *sv);" << endl;

      os << "virtual ~" << t.name () << "_Context (void);"
         << endl;

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

      // Extra *_Context methods for swapping container.
      if (swapping)
      {
        os << "// Operations defined in " << t.scoped_name ().scope_name ()
           << "::CCM_" << t.name () << "_Context" << endl
           << "// that enable component swapping in the container"
           << endl << endl;

        os << "virtual " << STRS[COMP_CD] << " *" << endl
           << "get_registered_consumers (const char *publisher_name);"
           << endl;
      }

      os << "// CIAO-specific." << endl << endl;

      os << "static " << t.name () << "_Context *" << endl
         << "_narrow ( " << STRS[COMP_SC] << "_ptr p);" << endl;

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
    post (Type&)
    {
      // Component context class closer.
      os << "};";
    }

  private:
    CommandLine const& cl_;
  };

  struct ServantEmitter : Traversal::Component, EmitterBase
  {
    ServantEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by this emitter.
  private:
    struct PortsEmitterPublic : Traversal::EmitterData,
                                Traversal::SingleUserData,
                                Traversal::MultiUserData,
                                Traversal::PublisherData,
                                Traversal::ConsumerData,
                                Traversal::ProviderData,
                                EmitterBase
    {
      PortsEmitterPublic (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          stripped_type_name_emitter_ (c)
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
           << "provide_" << p.name () << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "virtual void" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr c);" << endl;

        os << "virtual ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " ();" << endl;

        os << "virtual ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "get_connection_" << u.name () << " (" << endl
           << ");" << endl;
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "virtual ::Components::Cookie *" << endl
           << "connect_" << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr c);" << endl;

        os << "virtual ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << "disconnect_" << u.name () << " (" << endl
           << "::Components::Cookie * ck);" << endl;

        os << "virtual " << u.scoped_name () << "Connections *" << endl
           << "get_connections_" << u.name () << " (" << endl
           << ");" << endl;
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

        os << "Consumer" << endl
           << "{"
           << "public:" << endl;

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name () << "_Servant (" << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name () << "_ptr executor,"
           << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context_ptr c);" << endl;

        os << "virtual ~";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_"
           << c.name () << "_Servant (void);" << endl;

        os << "virtual void" << endl
           << "push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os  << " (" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *evt);" << endl;

        os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "virtual void" << endl
           << "push_event ( ::Components::EventBase *ev);" << endl;

        // @@ GD added below code.

        os << "// CIAO-specific in " << STRS[COMP_ECB] << "." << endl
           << "virtual void" << endl
           << "ciao_push_event ( ::Components::EventBase *ev," << endl
           << "const char * source_id," << endl
           << "::CORBA::TypeCode_ptr tc);" << endl;

        os << "// CIAO-specific in " << STRS[COMP_ECB] << "." << endl
           << "virtual CORBA::Boolean" << endl
           << "ciao_is_substitutable (const char *event_repo_id);"
           << endl;

        os << "// Get component implementation." << endl
           << "virtual CORBA::Object_ptr" << endl
           << "_get_component ();" << endl;

        os << "protected:" << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_var" << endl
           << "executor_;" << endl;

        os << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context_var" << endl
           << "ctx_;" << endl;

        os << "};";

        os << "virtual ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_ptr" << endl
           << "get_consumer_" << c.name () << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "virtual void" << endl
           << "connect_" << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c);" << endl;

        os << "virtual ";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << "disconnect_" << e.name () << " ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "virtual " << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c);" << endl;

        os << "// CIAO-specific." << endl
           << STRS[COMP_CK] << " *" << endl
           << "subscribe_" << p.name () << "_generic (" << endl
           << STRS[COMP_ECB] << "_ptr c);" << endl;

        os << "virtual ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << "unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck);" << endl;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
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
          type_name_emitter_ (c)
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
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PortsEmitterPrivate : Traversal::EmitterData,
                                 Traversal::SingleUserData,
                                 Traversal::MultiUserData,
                                 Traversal::PublisherData,
                                 Traversal::ConsumerData,
                                 Traversal::ProviderData,
                                 EmitterBase
    {
      PortsEmitterPrivate (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          stripped_type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        stripped_belongs_.node_traverser (stripped_type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Provider& p)
      {
        os << "::CORBA::Object_ptr" << endl
           << "provide_" << p.name () << "_i ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::SingleUser&)
      {
      }

      virtual void
      traverse (SemanticGraph::MultiUser&)
      {
      }

      virtual void
      traverse (SemanticGraph::Consumer& c)
      {
        os << "::Components::EventConsumerBase_ptr" << endl
           << "get_consumer_" << c.name () << "_i ();" << endl;
      }

      virtual void
      traverse (SemanticGraph::Emitter&)
      {
      }

      virtual void
      traverse (SemanticGraph::Publisher&)
      {
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      StrippedTypeNameEmitter stripped_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs stripped_belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      ScopedName scoped (t.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      os << "class " << ctx.export_macro () << " " << t.name ()
         << "_Servant" << endl
         << "  : public virtual CIAO::Servant_Impl<" << endl
         << "      POA_" << stripped << "," << endl
         << "      " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "," << endl
         << "      " << t.name () << "_Context" << endl
         << "    >" << endl
         << "{"
         << "public:" << endl << endl;

      os << "typedef " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << " _exec_type;" << endl;

      os << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr executor," << endl
         << "::Components::CCMHome_ptr h," << endl
         << "const char *ins_name," << endl
         << "::CIAO::Home_Servant_Impl_Base *hs," << endl
         << "::CIAO::Session_Container *c);" << endl;

      os << "virtual ~" << t.name () << "_Servant (void);" << endl;

      os << "virtual void" << endl
         << "set_attributes (const ::Components::ConfigValues &descr"
         << endl
         << ");" << endl;

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

        // Works for both read/write and readonly attributes.
        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
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

        // Works for both read/write and readonly attributes.
        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        component_emitter.traverse (t);
      }

      os << "// Operations for Receptacles interface." << endl << endl;

      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "connect (const char *name," << endl
         << "CORBA::Object_ptr connection);" << endl;

      os << "virtual CORBA::Object_ptr" << endl
         << "disconnect (const char *name," << endl
         << STRS[COMP_CK] << " *ck);" << endl;

      os << "virtual " << STRS[COMP_RD] << " *" << endl
         << "get_all_receptacles ();" << endl;

      os << "// Operations for Events interface." << endl << endl;

      os << "virtual " << STRS[COMP_CK] << " *" << endl
         << "subscribe (const char *publisher_name," << endl
         << STRS[COMP_ECB] << "_ptr subscriber);" << endl;

      os << "virtual " << STRS[COMP_ECB] << "_ptr" << endl
         << "unsubscribe (const char *publisher_name," << endl
         << STRS[COMP_CK] << " *ck);" << endl;

      os << "virtual void" << endl
         << "connect_consumer (const char *emitter_name," << endl
         << STRS[COMP_ECB] << "_ptr consumer);" << endl;

      os << "virtual " << STRS[COMP_ECB] << "_ptr" << endl
         << "disconnect_consumer (const char *source_name);" << endl;

      os << "virtual " << STRS[COMP_PD] << " *" << endl
         << "get_all_publishers ();" << endl;

      os << "virtual " << STRS[COMP_ED] << " *" << endl
         << "get_all_emitters ();" << endl;

      os << "// CIAO specific operations on the servant. " << endl
         << "CORBA::Object_ptr" << endl
         << "get_facet_executor (const char *name);" << endl;

      os << "private:" << endl << endl;

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

      os << "const char *ins_name_;" << endl;

      os << "private:" << endl << endl
         << "void" << endl
         << "populate_port_tables ();" << endl;

      // Generate private operations for ports.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        PortsEmitterPrivate ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        component_emitter.traverse (t);
      }
    }

    virtual void
    post (Type&)
    {
      // Component servant class closer.
      os << "};";
    }
  };

  struct HomeEmitter : Traversal::Home, EmitterBase
  {
    HomeEmitter (Context& c, CommandLine const& cl)
      : EmitterBase (c),
        cl_ (cl),
        type_name_emitter_ (c),
        simple_type_name_emitter_ (c),
        enclosing_type_name_emitter_ (c)
    {
      manages_.node_traverser (type_name_emitter_);
      simple_manages_.node_traverser (simple_type_name_emitter_);
      enclosing_manages_.node_traverser (enclosing_type_name_emitter_);
    }

    virtual void pre (Type& t)
    {
      os << "class " << ctx.export_macro () << " " << t.name ()
         << "_Servant" << endl
         << "  : public virtual" << endl
         << "      ::CIAO::";

      string swap_option = cl_.get_value ("custom-container", "");
      bool swapping = (swap_option == "upgradeable");

      ScopedName scoped (t.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      os << (swapping ? "Swapping_" : "") << "Home_Servant_Impl<" << endl
         << "          ::POA_" << stripped << "," << endl
         << "          " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "," << endl
         << "          ";

      Traversal::Home::manages (t, simple_manages_);

      os << "_Servant" << endl
         << "        >" << endl
         << "{"
         << "public:" << endl << endl;
      os << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "const char *ins_name," << endl
         << "::CIAO::Session_Container *c);" << endl;

      os << "virtual ~" << t.name () << "_Servant (void);" << endl;

      // Generate home operations.
      os << "// Home operations." << endl;

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

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      // Generate home factory operations. This is a separate traversal
      // stack because we want to generate the factory operations
      // of ancestors with the return type of the base component.
      os << "// Home factory and finder operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        FactoryOperationEmitter factory_operation_emitter (ctx);
        FinderOperationEmitter finder_operation_emitter (ctx);
        defines.node_traverser (factory_operation_emitter);
        defines.node_traverser (finder_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        factory_operation_emitter.edge_traverser (receives);
        factory_operation_emitter.edge_traverser (returns);
        factory_operation_emitter.edge_traverser (raises);
        finder_operation_emitter.edge_traverser (receives);
        finder_operation_emitter.edge_traverser (returns);
        finder_operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
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

        // Works for both read/write and readonly attributes.
        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        home_emitter.traverse (t);
      }

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

        // Works for both read/write and readonly attributes.
        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      // Home servant class closer.
      os << "};";

      string name;

      // We need to escape C++ keywords before flattening the name.
      //
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << t.scoped_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::PortableServer::Servant" << endl
         << "create" << name << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "CIAO::Session_Container *c," << endl
         << "const char *ins_name);" << endl;
    }

  private:
    CommandLine const& cl_;
    FullTypeNameEmitter type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    Traversal::Manages manages_;
    Traversal::Manages simple_manages_;
    Traversal::Manages enclosing_manages_;
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
                           "/^(.+?)(\\.(idl|cidl|cdl))?$/$1S.h/")
         << "\"" << endl << endl;
    }

    virtual void
    traverse (SemanticGraph::BracketIncludes& bi)
    {
      os << "#include \""
         << regex::perl_s (bi.file ().string (),
                           "/^(.+?)(\\.(idl|cidl|cdl))?$/$1S.h/")
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
ServantHeaderEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_, cl_);

  /// CIAO has facet classes outside the context and servant classes.
  /// We launch it here to generate the classes inside namespaces
  /// constructed to be unique for each interface type used in a facet.
  generate_facets (u, c);

  Traversal::TranslationUnit unit;

  // Layer 1
  //
  Traversal::ContainsPrincipal contains_principal;
  unit.edge_traverser (contains_principal);

  //--
  Traversal::TranslationRegion region;
  contains_principal.node_traverser (region);

  // Layer 2
  //
  Traversal::ContainsRoot contains_root;
  Traversal::Includes includes;

  region.edge_traverser (includes);
  region.edge_traverser (contains_root);

  //--
  Traversal::Root root;
  contains_root.node_traverser (root);

  // Layer 3
  //
  Traversal::Defines defines;
  root.edge_traverser (defines);

  //--
  ModuleEmitter module (c);
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
  ContextEmitter context_emitter (c, cl_);
  ServantEmitter servant_emitter (c);
  HomeEmitter home_emitter (c, cl_);
  implements.node_traverser (context_emitter);
  implements.node_traverser (servant_emitter);
  implements.node_traverser (home_emitter);

  unit.traverse (u);

  post (u);
}

void
ServantHeaderEmitter::pre (TranslationUnit&)
{
  os << COPYRIGHT;

  string file_name ("");

  if (!file_.empty ())
  {
    file_name = file_.leaf ();
  }

  string uc_file_name = file_name;
  str_upcase (uc_file_name);

  string uc_file_suffix = cl_.get_value ("svnt-hdr-file-suffix",
                                         "_svnt.h");
  str_upcase (uc_file_suffix);

  string guard =
      "CIAO_GLUE_SESSION_"
      + regex::perl_s (uc_file_name,
                       "/^(.+?)(\\.(IDL|CIDL|CDL))?$/$1"
                       + uc_file_suffix
                       + "/");

  // Replace any remaining '.' with '_'.
  guard = regex::perl_s (guard, "/\\./_/");

  os << "#ifndef " << guard << endl
     << "#define " << guard << endl << endl
     << "#include /**/ \"ace/pre.h\"" << endl << endl;

  string export_include = cl_.get_value ("svnt-export-include", "");

  if (!export_include.empty ())
  {
    os << "#include \"" << export_include << "\"" << endl << endl;
  }

  // Get the suffix for the executor IDL file and strip off the
  // trailing .idl.
  // @@@ (JP) I guess it's a safe assumption that the tail of the
  // suffix will be .idl.
  string suffix = cl_.get_value ("lem-file-suffix", "E.idl");
  suffix = regex::perl_s (suffix, "/^(.+?)(\\.idl)?$/$1/");

  // @@@ (JP) No way of knowing if the IDL compiler had command line
  // option(s) to change C.h and/or S.h. We could add command line options
  // to the CIDL compiler for them, but the user will have to make sure
  // they are in synch with the IDL compiler's options.
  os << "#include \""
     << regex::perl_s (file_name,
                       "/^(.+?)(\\.(idl|cidl|cdl))?$/$1" + suffix + "C.h/")
     << "\""
     << endl << endl;

  os << "#if !defined (ACE_LACKS_PRAGMA_ONCE)" << endl
     << "# pragma once" << endl
     << "#endif /* ACE_LACKS_PRAGMA_ONCE */" << endl << endl;

  string swap_option = cl_.get_value ("custom-container", "");
  bool swapping = (swap_option == "upgradeable");

  os << "#include \"ciao/"
     << (swapping ? "Swapping_Container.h" : "Session_Container.h")
     << "\"" << endl
     << "#include \"ciao/"
     << (swapping ? "Upgradeable_Context_Impl_T.h"
                  : "Context_Impl_T.h")
     << "\"" << endl
     << "#include \"ciao/Servant_Impl_T.h\"" << endl
     << "#include \"ciao/"
     << (swapping ? "Swapping_Servant_Home_Impl_T.h"
                  : "Home_Servant_Impl_T.h")
     << "\"" << endl << endl;
}

void
ServantHeaderEmitter::generate_facets (TranslationUnit& u, Context& c)
{
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

  /// Includes are handled here so they will all be present
  /// before the facet servants (if any) are generated.

  Traversal::ContainsRoot contains_root;
  IncludesEmitter includes_emitter (c);

  principal_region.edge_traverser (includes_emitter);
  principal_region.edge_traverser (contains_root);

  //--
  Traversal::Root root;
  contains_root.node_traverser (root);

  // Layer 3
  //
  Traversal::Defines root_defines;
  root.edge_traverser (root_defines);

  //--
  Traversal::Module module;
  Traversal::Composition composition;
  root_defines.node_traverser (module);
  root_defines.node_traverser (composition);

  module.edge_traverser (root_defines);

  // Layer 4
  //
  Traversal::Defines composition_defines;
  composition.edge_traverser (composition_defines);

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

  // Layer 6
  //
  Traversal::Component component_emitter;
  implements.node_traverser (component_emitter);

  Traversal::Inherits component_inherits;
  component_inherits.node_traverser (component_emitter);

  Traversal::Defines component_defines;
  component_emitter.edge_traverser (component_defines);
  component_emitter.edge_traverser (component_inherits);

  // Layer 7
  //
  Traversal::Provider provider;
  component_defines.node_traverser (provider);

  Traversal::Belongs belongs;
  provider.edge_traverser (belongs);

  FacetEmitter facet_emitter (c);
  belongs.node_traverser (facet_emitter);

  unit.traverse (u);
}

void
ServantHeaderEmitter::post (TranslationUnit&)
{
  if (file_.empty ()) return;

  string uc_file_name = file_.leaf ();
  str_upcase (uc_file_name);

  string uc_file_suffix = cl_.get_value ("svnt-hdr-file-suffix",
                                         "_svnt.h");
  str_upcase (uc_file_suffix);

  string guard =
    "CIAO_GLUE_SESSION_"
    + regex::perl_s (uc_file_name,
                     "/^(.+?)(\\.(IDL|CIDL|CDL))?$/$1"
                     + uc_file_suffix
                     + "/");

  guard = regex::perl_s (guard, "/\\./_/");

  os << "#include /**/ \"ace/post.h\"" << endl << endl
     << "#endif /* " << guard << " */"
     << endl << endl;
}
