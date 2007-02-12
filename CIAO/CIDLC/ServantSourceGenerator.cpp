// file      : CIDLC/ServantSourceGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantSourceGenerator.hpp"
#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"
#include "ParameterEmitter_T.hpp"
#include "AttributeSourceEmitters_T.hpp"
#include "CompositionEmitter.hpp"
#include "ModuleEmitter.hpp"
#include "InterfaceEmitter.hpp"
#include "Literals.hpp"

#include <ostream>
#include <sstream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

using std::string;

namespace
{
  struct RepoIdEmitter : Traversal::Type,
                         EmitterBase
  {
    RepoIdEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << "\"" << t.context ().get<string> (STRS[REPO_ID]) << "\"";
    }
  };

  struct ConsumerRepoIdEmitter : Traversal::Type,
                                 EmitterBase
  {
    ConsumerRepoIdEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      string holder (t.context ().get<string> (STRS[REPO_ID]));
      os << "\"" << holder.substr (0, holder.rfind (':'))
         << "Consumer:1.0\"";
    }
  };

  struct FlatNameEmitter : Traversal::Type,
                           EmitterBase
  {
    FlatNameEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      // We need to escape C++ keywords before flattening the name.
      //
      std::ostringstream ostr;
      ostr.pword (name_printer_index) = os.pword (name_printer_index);
      ostr << t.scoped_name ();
      os << regex::perl_s (ostr.str (), "/::/_/");
    }
  };

  struct OperationExecEmitter : Traversal::Operation,
                                EmitterBase
  {
    OperationExecEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    name (Type& o)
    {
      os << o.name ();
    }

    virtual void
    receives_pre (Type&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_none (Type&)
    {
      os << " ();";
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << ");";
    }

    /// If we don't do this, the comma() method just below
    /// gets used not only with the arguments, but also
    /// in raises(). Even though we are not generating the
    /// exception list here, the front end iterator still
    /// executes, and picks up the overridden comma(). So
    /// we override raises() itself to do nothing.
    virtual void
    raises (Type&)
    {
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }
  };

  struct OpExecReturnEmitter : Traversal::Type,
                               Traversal::Void
  {
    OpExecReturnEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (Void&)
    {
      os << "this->executor_->";
    }

    virtual void
    traverse (SemanticGraph::Type&)
    {
      os << "return this->executor_->";
    }

  private:
    std::ostream& os;
  };

  // For generating parameter names in operation delegation.
  template <typename T>
  struct ParameterExecEmitter : T
  {
    ParameterExecEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    name (typename T::Type& p)
    {
      os << p.name ();
    }

  private:
    std::ostream& os;
  };

  template <typename T>
  struct OperationEmitter : Traversal::Operation, EmitterBase
  {
    OperationEmitter (Context& c,
                      T& scope)
      : EmitterBase (c),
        scope_ (scope)
    {
    }

    // Overridden by facet and home operation emitters to do nothing.
    virtual void
    gen_swap_related (Type&)
    {
      string swap_option = ctx.cl ().get_value ("custom-container", "");
      bool swapping = (swap_option == "upgradeable");

      if (swapping)
        {
          os << "this->activate_component ();" << endl;
        }
    }

    virtual void
    name (Type& o)
    {
      os << endl
         << scope_.name () << "_Servant::" << o.name ();
    }

    virtual void
    receives_pre (Type&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_none (Type&)
    {
      os << " ()" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << ")" << endl;
    }

    virtual void
    raises_pre (Type&)
    {
      os << STRS[EXCP_START] << " "
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
    post (Type& o)
    {
      os << "{";

      this->gen_swap_related (o);

      OperationExecEmitter operation_emitter (ctx);

      Traversal::Receives receives;
      Traversal::Belongs returns;
      operation_emitter.edge_traverser (receives);
      operation_emitter.edge_traverser (returns);

      ParameterExecEmitter<Traversal::Parameter> param (os);
      receives.node_traverser (param);

      OpExecReturnEmitter return_emitter (os);
      returns.node_traverser (return_emitter);

      operation_emitter.traverse (o);

      os << "}";
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }

  protected:
    T& scope_;
  };

  struct FacetEmitter : Traversal::UnconstrainedInterface,
                        EmitterBase
  {
    FacetEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    struct FacetOperationEmitter
      : OperationEmitter<SemanticGraph::UnconstrainedInterface>
    {
      FacetOperationEmitter (Context& c,
                             SemanticGraph::UnconstrainedInterface& i)
        : OperationEmitter<SemanticGraph::UnconstrainedInterface> (c, i)
      {}

      // Overriden to do nothing since we don't want to generate
      // component swapping related code for facet operations.
      virtual void
      gen_swap_related (Type&)
      {
      }

      virtual void
      pre (Type&)
      {
        os << "template <typename T>" << endl;
      }

      virtual void
      name (Type& o)
      {
        os << endl
           << scope_.name () << "_Servant_T<T>::" << o.name ();
      }
    };

    struct FacetWriteAttributeEmitter
      : WriteAttributeSourceEmitter<SemanticGraph::Interface>
    {
      // Since swapping does not affect facets, we can just pass 'false'
      // to the base class constructor, and not override gen_swapping_set().
      FacetWriteAttributeEmitter (Context& c, SemanticGraph::Interface& i)
        : WriteAttributeSourceEmitter<SemanticGraph::Interface> (c,
                                                                 i,
                                                                 false,
                                                                 false)
      {}

      virtual void
      pre (SemanticGraph::ReadWriteAttribute& a)
      {
        os << "template <typename T>" << endl;

        WriteAttributeSourceEmitter<SemanticGraph::Interface>::pre (a);
      }

      virtual void
      name (SemanticGraph::ReadWriteAttribute& a)
      {
        os << scope_.name () << "_Servant_T<T>::"
           << a.name () << " (" << endl;

        Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

        os << " " << a.name () << endl
           << ")" << endl;
      }
    };

    struct FacetAttributeEmitter
      : AttributeSourceEmitter<SemanticGraph::Interface>
    {
      FacetAttributeEmitter (Context& c,
                             SemanticGraph::Interface& i)
        : AttributeSourceEmitter<SemanticGraph::Interface> (c, i)
      {}

      // ReadWriteAttribute
      //

      // No-op override because we don't want to generate any swapping
      // code for facets.
      virtual void gen_swapping_get (SemanticGraph::ReadWriteAttribute&)
      {
      }

      virtual void
      gen_write_operation (SemanticGraph::ReadWriteAttribute& a,
                           bool /* swapping */)
      {
        FacetWriteAttributeEmitter write_attribute_emitter (ctx, scope_);
        write_attribute_emitter.traverse (a);
      }

      virtual void
      pre (SemanticGraph::ReadWriteAttribute& a)
      {
        os << "template <typename T>" << endl;

        AttributeSourceEmitter<SemanticGraph::Interface>::pre (a);
      }

      virtual void
      name (SemanticGraph::ReadWriteAttribute& a)
      {
        os << scope_.name () << "_Servant_T<T>::"
           << a.name () << " (" << endl
           << ")" << endl;
      }

      // ReadAttribute
      //

      // No-op override because we don't want to generate any swapping
      // code for facets.
      virtual void gen_swapping_get (SemanticGraph::ReadAttribute&)
      {
      }

      virtual void
      pre (SemanticGraph::ReadAttribute& a)
      {
        os << "template <typename T>" << endl;

        AttributeSourceEmitter<SemanticGraph::Interface>::pre (a);
      }

      virtual void
      name (SemanticGraph::ReadAttribute& a)
      {
        os << scope_.name () << "_Servant_T<T>::"
           << a.name () << " (" << endl
           << ")" << endl;
      }
    };

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      if (i.context ().count ("facet_src_gen")) return;

      // We need to escape C++ keywords before flattening the name.
      //
      string name;
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << i.scoped_name ().scope_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }


      /// Open a namespace made from the interface scope's name.
      os << "namespace " << STRS[FACET_PREFIX] << name
         << "{";

      os << "template <typename T>" << endl
         << i.name () << "_Servant_T<T>::" << i.name ()
         << "_Servant_T (" << endl
         << i.scoped_name ().scope_name () << "::CCM_" << i.name ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr c)" << endl
         << "  : executor_ ( " << i.scoped_name ().scope_name () << "::CCM_"
         << i.name () << "::_duplicate (executor))," << endl
         << "    ctx_ ( ::Components::CCMContext::_duplicate (c))" << endl
         << "{"
         << "}";

      os << "template <typename T>" << endl
         << i.name () << "_Servant_T<T>::~" << i.name ()
         << "_Servant_T (void)" << endl
         << "{"
         << "}";

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        FacetAttributeEmitter attribute_emitter (ctx, i);
        defines_.node_traverser (attribute_emitter);

        FacetOperationEmitter operation_emitter (ctx, i);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

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

        inherits (i, inherits_);
        names (i, defines_);
      }

      os << "template <typename T>" << endl
         << "::CORBA::Object_ptr" << endl
         << i.name () << "_Servant_T<T>::_get_component (" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << STRS[COMP_SC] << "_var sc =" << endl
         << "  " << STRS[COMP_SC] << "::_narrow (" << endl
         << "  this->ctx_.in ()" << endl
         << "  " << ");"
         << endl
         << "if (! ::CORBA::is_nil (sc.in ()))" << endl
         << "{"
         << "return sc->get_CCM_object (" << endl
         << ");"
         << "}"
         << "::Components::EntityContext_var ec =" << endl
         << "::Components::EntityContext::_narrow (" << endl
         << "this->ctx_.in ()" << endl
         << ");"
         << endl
         << "if (! ::CORBA::is_nil (ec.in ()))" << endl
         << "{"
         << "return ec->get_CCM_object (" << endl
         << ");"
         << "}"
         << " throw ::CORBA::INTERNAL ();" << endl
         << "}" << endl;

      // Close the facet servant's namespace.
      os << "}";

      i.context ().set ("facet_src_gen", true);
    }
  };

  struct ContextEmitter : Traversal::Component, EmitterBase
  {
    ContextEmitter (Context& c)
      : EmitterBase (c)
    {}

    // Nested classes used by ContextEmitter.
  private:
    struct ContextPortsEmitter : Traversal::SingleUserData,
                                 Traversal::MultiUserData,
                                 Traversal::PublisherData,
                                 Traversal::EmitterData,
                                 EmitterBase
    {
      ContextPortsEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          enclosing_type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
        enclosing_belongs_.node_traverser (enclosing_type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::get_connection_"
           << u.name () << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_duplicate (" << endl
           << "this->ciao_uses_" << u.name ()
           << "_.in ());" << endl
           << "}";

        os << "void" << endl
           << scope_.name () << "_Context::connect_"
           << u.name () << " (" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if (! ::CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "throw " << STRS[EXCP_AC] << " ();"
           << "}"
           << "if ( ::CORBA::is_nil (c))" << endl
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();"
           << "}"
           << "this->ciao_uses_" << u.name () << "_ =" << endl
           << "  ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_duplicate (c);" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << u.name () << " ()" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "throw "
           << STRS[EXCP_NC] << " ();" << endl;

        os << endl
           << "}"
           << "return this->ciao_uses_" << u.name ()
           << "_._retn ();" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << u.scoped_name () << "Connections *" << endl
           << scope_.name () << "_Context::get_connections_"
           << u.name () << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << u.scoped_name () << "Connections *tmp_retv = 0;"
           << "ACE_NEW_THROW_EX ("
           << "tmp_retv," << endl
           << u.scoped_name () << "Connections (" << endl
           << "this->ciao_uses_"
           << u.name () << "_.current_size ())," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << u.scoped_name () << "Connections_var retv"
           << " = tmp_retv;" << endl
           << "retv->length (this->ciao_uses_" << u.name ()
           << "_.current_size ());" << endl;

        os << "::CORBA::ULong i = 0;" << endl;

        os << "for (ACE_Active_Map_Manager< " << endl
           << "  ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var>::iterator iter =" << endl
           << "    this->ciao_uses_" << u.name () << "_.begin ();" << endl
           << "iter != this->ciao_uses_" << u.name () << "_.end ();"
           << endl
           << "++iter)" << endl
           << "{"
           << "ACE_Active_Map_Manager< " << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var>::ENTRY & entry = *iter;" << endl
           << "retv[i].objref = ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "entry.int_id_.in ()" << endl
           << ");"
           << endl;

        os << "ACE_NEW_THROW_EX ("
           << "retv[i].ck.inout ()," << endl
           << "::CIAO::Map_Key_Cookie (entry.ext_id_)," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << "++i;" << endl
           << "}";

        os << "return retv._retn ();" << endl
           << "}";

        os << STRS[COMP_CK] << " *" << endl
           << scope_.name () << "_Context::connect_"
           << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (c))" << endl
           << "{"
           << "throw "
           << STRS[EXCP_IC] << " ();" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var conn = ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_duplicate (c);"
           << "ACE_Active_Map_Manager_Key key;" << endl;

        os << "if (this->ciao_uses_" << u.name ()
           << "_.bind (conn.in (), key) == -1)" << endl
           << "{"
           << "throw "
           << STRS[EXCP_IC] << " ();" << endl
           << "}";

        os << "conn._retn ();" << endl;

        os << STRS[COMP_CK] << " * ck = 0;"
           << "ACE_NEW_THROW_EX ("
           << "ck," << endl
           << "::CIAO::Map_Key_Cookie (key)," << endl
           << "::CORBA::NO_MEMORY ());" << endl;


        os << "return ck;" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << u.name () << " (" << endl
           << STRS[COMP_CK] << " * ck)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var retv;"
           << "ACE_Active_Map_Manager_Key key;" << endl;

        os << "if (! CIAO::Map_Key_Cookie::extract (ck, key))"
           << endl
           << "{"
           << "throw  "
           << STRS[EXCP_IC] << " ();" << endl;

        os << endl
           << "}";

        os << "if (this->ciao_uses_" << u.name ()
           << "_.unbind (key, retv) != 0)" << endl
           << "{"
           << "throw "
           << " ();" << endl;

        os << endl
           << "}";

        os << "return retv._retn ();" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "void" << endl
           << scope_.name () << "_Context::push_"
           << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << " *ev" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "for (ACE_Active_Map_Manager< " << endl
           << "  ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator iter =" << endl
           << "    this->ciao_publishes_" << p.name ()
           << "_map_.begin ();" << endl
           << "iter != this->ciao_publishes_" << p.name ()
           << "_map_.end ();" << endl
           << "++iter)" << endl
           << "{"
           << "(*iter).int_id_->push_";

        Traversal::PublisherData::belongs (p, simple_belongs_);

        os << " (" << endl
           << "ev" << endl
           << ");" << endl
           << "}";

        // @@ GD Modified Code Below

        os << endl;
        os << "ACE_CString source_id = this->_ciao_instance_id ();";
        os << "source_id += \"_\";" << endl;
        os << "source_id += \"" << p.name () << "\";//port name"
           << endl << endl;

        os << "for (ACE_Active_Map_Manager< " << endl
           << "  " << STRS[COMP_ECB] << "_var>::iterator giter =" << endl
           << "    this->ciao_publishes_" << p.name ()
           << "_generic_map_.begin ();" << endl
           << "giter != this->ciao_publishes_" << p.name ()
           << "_generic_map_.end ();" << endl
           << "++giter)" << endl
           << "{"
           << "(*giter).int_id_->ciao_push_event" << " (" << endl
           << "ev," << endl
           << "source_id.c_str ()," << endl;

        Traversal::PublisherData::belongs (p, enclosing_belongs_);

        os << "::_tc_";

        Traversal::PublisherData::belongs (p, simple_belongs_);

        os << endl
           << ");" << endl
           << "}"
           << "}";

        os << STRS[COMP_CK] << " *" << endl
           << scope_.name () << "_Context::subscribe_"
           << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (c))" << endl
           << "{"
           << "throw ::CORBA::BAD_PARAM ();" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var sub =" << endl
           << "  ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_duplicate (c);" << endl
           << "ACE_Active_Map_Manager_Key key;"
           << "this->ciao_publishes_" << p.name ()
           << "_map_.bind (sub.in (), key);"
           << "sub._retn ();" << endl
           << STRS[COMP_CK] << " * retv = 0;"
           << "ACE_NEW_THROW_EX ("
           << "retv," << endl
           << "::CIAO::Map_Key_Cookie (key)," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << "return retv;" << endl
           << "}";

        os << STRS[COMP_CK] << " *" << endl
           << scope_.name () << "_Context::subscribe_"
           << p.name () << "_generic (" << endl
           << STRS[COMP_ECB] << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (c))" << endl
           << "{"
           << "throw ::CORBA::BAD_PARAM ();" << endl
           << "}";

        os << STRS[COMP_ECB] << "_var sub =" << endl
           << "  " << STRS[COMP_ECB] << "::_duplicate (c);" << endl
           << "ACE_Active_Map_Manager_Key key;"
           << "this->ciao_publishes_" << p.name ()
           << "_generic_map_.bind (sub.in (), key);"
           << "sub._retn ();" << endl
           << STRS[COMP_CK] << " * retv = 0;"
           << "ACE_NEW_THROW_EX ("
           << "retv," << endl
           << "::CIAO::Map_Key_Cookie (key)," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << "return retv;" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Context::unsubscribe_"
           << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "ACE_Active_Map_Manager_Key key;" << endl
           << "if (ck == 0 || ::CIAO::Map_Key_Cookie::"
           << "extract (ck, key) == false)" << endl
           << "{"
           << "throw "
           << STRS[EXCP_IC] << " ();";

        os << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var retv;"
           << "if (this->ciao_publishes_" << p.name ()
           << "_map_.unbind (key, retv) == 0)" << endl
           << "{"
           << "return retv._retn ();" << endl
           << "}";

        os << STRS[COMP_ECB] << "_var ecb;" << endl;

        os << "if (this->ciao_publishes_" << p.name ()
           << "_generic_map_.unbind (key, ecb) != 0)" << endl
           << "{"
           << "throw "
           << STRS[EXCP_IC] << " ();" << endl;

        os << endl
           << "}"
           << "return ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_nil ();" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "void" << endl
           << scope_.name () << "_Context::push_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << " *ev" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (! ::CORBA::is_nil (this->ciao_emits_"
           << e.name () << "_consumer_.in ()))" << endl
           << "{"
           << "this->ciao_emits_" << e.name ()
           << "_consumer_->push_";

        Traversal::EmitterData::belongs (e, simple_belongs_);

        os << " (ev);" << endl
           << "}"
           << "}";

        os << "void" << endl
           << scope_.name () << "_Context::connect_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (c))" << endl
           << "{"
           << "throw ::CORBA::BAD_PARAM ();" << endl
           << "}"
           << "if (! ::CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "throw " << STRS[EXCP_AC] << " ();" << endl
           << "}"
           << "this->ciao_emits_" << e.name ()
           << "_consumer_ = " << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer::_duplicate (c);" << endl
           << "}";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << e.name () << " ()" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if ( ::CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "throw "
           << STRS[EXCP_NC] << " ();" << endl;

        os << endl
           << "}"
           << "return this->ciao_emits_" << e.name ()
           << "_consumer_._retn ();" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      EnclosingTypeNameEmitter enclosing_type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs enclosing_belongs_;
      Traversal::Belongs simple_belongs_;
      SemanticGraph::Component& scope_;
    };

    struct SwappableGetConsumersEmitter : Traversal::PublisherData,
                                          Traversal::EmitterData,
                                          EmitterBase
    {
      SwappableGetConsumersEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "_ciao_size = this->ciao_publishes_" << p.name ()
           << "_map_.current_size ();" << endl
           << "ACE_NEW_THROW_EX ("
           << "tmp," << endl
           << STRS[COMP_CD] << " (_ciao_size)," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << "retval = tmp;"
           << "retval->length (_ciao_size);" << endl
           << "ACE_Active_Map_Manager<" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator end =" << endl
           << "  this->ciao_publishes_" << p.name ()
           << "_map_.end ();" << endl
           << "for (ACE_Active_Map_Manager<" << endl
           << "  ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator iter =" << endl
           << "    this->ciao_publishes_" << p.name ()
           << "_map_.begin ();"
           << "iter != end;"
           << "++iter)" << endl
           << "{"
           << "ACE_Active_Map_Manager<" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::entry &e = *iter;" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var c =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "e.int_id_.in ()" << endl
           << ");"
           << endl
           << "if ( ::CORBA::is_nil (c.in ()))"
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();"
           << "}"
           << "::Components::ConsumerDescription *cd = 0;"
           << "ACE_NEW_THROW_EX ("
           << "cd," << endl
           << "OBV_Components::ConsumerDescription ()," << endl
           << "::CORBA::NO_MEMORY ());" << endl
           << "::Components::ConsumerDescription_var safe = cd;"
           << "safe->name (\"\");"
           << "safe->type_id (\"\");"
           << "safe->consumer (c.in ());" << endl
           << "retval[_ciao_index++] = safe;"
           << "}"
           << "return retval._retn ();"
           << "}";
      }

      virtual void
      traverse (SemanticGraph::Emitter&)
      {
        // Don't know yet if swappable components need anything here.
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << t.name () << "_Context::"
         << t.name () << "_Context (" << endl
         << "::Components::CCMHome_ptr h," << endl
         << "::CIAO::Session_Container *c," << endl
         << t.name () << "_Servant *sv)" << endl
         << "  : ::CIAO::Context_Impl_Base (h, c), " << endl
         << "    ::CIAO::Context_Impl<" << endl
         << "        " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_Context," << endl
         << "        " << t.name () << "_Servant," << endl
         << "        " << t.scoped_name () << endl
         << "      > (h, c, sv)";

      string swap_option = ctx.cl ().get_value ("custom-container", "");
      bool swapping = (swap_option == "upgradeable");

      if (swapping)
        {
          os << "," << endl
             << "    ::CIAO::Upgradeable_Context_Impl<" << endl
             << "        " << t.scoped_name ().scope_name () << "::CCM_"
             << t.name () << "_Context," << endl
             << "        " << t.name () << "_Servant," << endl
             << "        " << t.scoped_name () << endl
             << "      > (h, c, sv)";
        }
      else
        {
          os << endl;
        }

      os << "{"
         << "}";

      os << t.name () << "_Context::~"
         << t.name () << "_Context (void)" << endl
         << "{"
         << "}";

      os << "// Operations for " << t.name () << " receptacles"
         << " and event sources," << endl
         << "// defined in " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_Context."
         << endl << endl;

      // Generate Context class operations for receptacles and event sources.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        ContextPortsEmitter ports_emitter (ctx, t);
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

        os << STRS[COMP_CD] << " *" << endl
           << t.name () << "_Context::get_registered_consumers (" << endl
           << "const char *publisher_name)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IN] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if (publisher_name == 0)" << endl
           << "{"
           << "throw " << STRS[EXCP_IN] << " ();"
           << "}"
           << STRS[COMP_CD] << " *tmp = 0;"
           << STRS[COMP_CD] << "_var retval;"
           << "CORBA::ULong _ciao_index = 0;"
           << "CORBA::ULong _ciao_size = 0;"
           << STRS[ACE_UA] << " (tmp);"
           << STRS[ACE_UA] << " (retval);"
           << STRS[ACE_UA] << " (_ciao_index);"
           << STRS[ACE_UA] << " (_ciao_size);" << endl;

        // Generate IF block for each event sources.
        {
          Traversal::Component component_emitter;

          Traversal::Inherits inherits;
          inherits.node_traverser (component_emitter);

          Traversal::Defines defines;
          component_emitter.edge_traverser (defines);
          component_emitter.edge_traverser (inherits);

          SwappableGetConsumersEmitter get_consumers_emitter (ctx);
          defines.node_traverser (get_consumers_emitter);

          component_emitter.traverse (t);
        }

        os << "throw " << STRS[EXCP_IN] << " ();"
           << "}";
      }

      os << "// CIAO-specific." << endl << endl;

      os << t.name () << "_Context *" << endl
         << t.name () << "_Context::_narrow (" << endl
         << STRS[COMP_SC] << "_ptr p" << endl
         << ")" << endl
         << "{"
         << "return dynamic_cast<" << t.name () << "_Context *> (p);"
         << endl
         << "}";
    }
  };

  struct ServantEmitter : Traversal::Component, EmitterBase
  {
    ServantEmitter (Context& c)
      : EmitterBase (c)
    {}

    // Nested classes used by ServantEmitter.
  private:
    struct EmitsConnectEmitter : Traversal::EmitterData,
                                 EmitterBase

    {
      EmitsConnectEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& t)
      {
        os << "if (ACE_OS::strcmp (emitter_name, \""
           << t.name ().unescaped_str () << "\") == 0)" << endl
           << "{";

        Traversal::EmitterData::belongs (t, belongs_);

        os << "Consumer_var _ciao_consumer =" << endl
           << "  ";

        Traversal::EmitterData::belongs (t, belongs_);

        os << "Consumer::_narrow (" << endl
           << "  consumer" << endl
           << "  " << ");" << endl
           << "if ( ::CORBA::is_nil (_ciao_consumer.in ()))" << endl
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();"
           << endl
           << "}"
           << "this->connect_" << t.name ()
           << " (" << endl
           << "_ciao_consumer.in ()" << endl
           << ");" << endl
           << "return;"
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct EmitsDisconnectEmitter : Traversal::EmitterData,
                                    EmitterBase
    {
      EmitsDisconnectEmitter (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      traverse (Type& t)
      {
        os << "if (ACE_OS::strcmp (source_name, \""
           << t.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "return this->disconnect_" << t.name ()
           << " ();" << endl
           << "}";
      }
    };

    struct NavigationGetFacetExecEmitter : Traversal::ProviderData,
                                           EmitterBase
    {
      NavigationGetFacetExecEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& t)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << t.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "return this->executor_->get_" << t.name ()
           << " ();" << endl
           << "}";
      }
    };

    struct PublishesEmitter : Traversal::PublisherData,
                              EmitterBase
    {
      PublishesEmitter (Context& c,
                        SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << STRS[COMP_CK] << " *" << endl
           << scope_.name ()
           << "_Servant::subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{";

        string swap_option = ctx.cl ().get_value ("custom-container", "");
        bool swapping = (swap_option == "upgradeable");

        if (swapping)
          {
            os << "this->activate_component ();"
               << endl;
          }

        os << "return this->context_->subscribe_" << p.name ()
           << " (" << endl
           << "c" << endl
           << ");" << endl
           << "}";

        os << STRS[COMP_CK] << " *" << endl
           << scope_.name ()
           << "_Servant::subscribe_" << p.name () << "_generic (" << endl
           << STRS[COMP_ECB] << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{";

        if (swapping)
          {
            os << "this->activate_component ();"
               << endl;
          }

        os << "return this->context_->subscribe_" << p.name ()
           << "_generic (" << endl
           << "c" << endl
           << ");" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name ()
           << "_Servant::unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{";

        if (swapping)
        {
          os << "this->activate_component ();"
             << endl;
          }

        os << "return this->context_->unsubscribe_"
           << p.name () << " (" << endl
           << "ck" << endl
           << ");" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct UsesConnectEmitter : Traversal::SingleUserData,
                                Traversal::MultiUserData,
                                EmitterBase
    {
      UsesConnectEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name ().unescaped_str () << "\") == 0)" << endl
           << "{";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_var _ciao_conn =" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "connection" << endl
           << ");"
           << endl
           << "if ( ::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();"
           << endl
           << "}"
           << "// Simplex connect." << endl
           << "this->connect_" << u.name () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << ");"
           << endl
           << "return 0;" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name ().unescaped_str () << "\") == 0)" << endl
           << "{";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var _ciao_conn =" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "connection" << endl
           << ");"
           << endl
           << "if ( ::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();"
           << endl
           << "}"
           << "// Multiplex connect." << endl
           << "return this->connect_" << u.name () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << ");"
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct UsesDisconnectEmitter : Traversal::SingleUserData,
                                   Traversal::MultiUserData,
                                   EmitterBase
    {
      UsesDisconnectEmitter (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "// Simplex disconnect." << endl
           << "return this->disconnect_" << u.name ()
           << " ();" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "// Multiplex disconnect." << endl
           << "if (ck == 0)" << endl
           << "{"
           << "throw "
           << STRS[EXCP_CR] << " ();" << endl
           << "}"
           << "return this->disconnect_" << u.name () << " (" << endl
           << "ck" << endl
           << ");" << endl
           << "}";
      }
    };

    struct UsesEmitter : Traversal::SingleUserData,
                         Traversal::MultiUserData,
                         EmitterBase
    {
      UsesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "void" << endl
           << scope_.name () << "_Servant::connect_"
           << u.name () << " (" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "this->context_->connect_" << u.name () << " ("
           << endl
           << "c" << endl
           << ");" << endl
           << "this->add_receptacle (\"" << u.name ().unescaped_str ()
           << "\", c, 0);" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << u.name () << " ()" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_" << u.name ()
           << " (" << endl
           << ");" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name ()
           << "_Servant::get_connection_" << u.name ()
           << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->context_->get_connection_"
           << u.name () << " (" << endl
           << ");" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << STRS[COMP_CK] << " *" << endl
           << scope_.name () << "_Servant::connect_"
           << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "::Components::Cookie * cookie = "
          "this->context_->connect_" << u.name () << " ("
           << endl
           << "c" << endl
           << ");" << endl
           << "this->add_receptacle (\"" << u.name ().unescaped_str ()
           << "\", c, cookie);" << endl
           << "return cookie;" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << u.name () << " (" << endl
           << STRS[COMP_CK] << " * ck)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_" << u.name ()
           << " (" << endl
           << "ck" << endl
           << ");" << endl
           << "}";

        os << u.scoped_name () << "Connections *" << endl
           << scope_.name ()
           << "_Servant::get_connections_" << u.name ()
           << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->context_->get_connections_"
           << u.name () << " (" << endl
           << ");" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct PublishesSubscribeEmitter : Traversal::PublisherData,
                                       EmitterBase
    {
      PublishesSubscribeEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name ().unescaped_str () << "\") == 0)" << endl
           << "{";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var sub =" << endl
           << "  ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "subscribe" << endl
           << ");"
           << endl;

        os << "if ( ::CORBA::is_nil (sub.in ()))" << endl
           << "{"
           << "::CORBA::Boolean substitutable =" << endl
           << "  subscribe->ciao_is_substitutable (" << endl
           << "  ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "::_tao_obv_static_repository_id ()" << endl
           << "  " << ");"
           << endl;

        os << "if (substitutable)" << endl
           << "{"
           << "return this->subscribe_" << p.name () << "_generic (" << endl
           << "subscribe" << endl
           << ");" << endl
           << "}"
           << "else" << endl
           << "{"
           << "throw " << STRS[EXCP_IC] << " ();" << endl
           << "}"
           << "}"
           << "else" << endl
           << "{"
           << "return this->subscribe_" << p.name () << " (" << endl
           << "sub.in ()" << endl
           << ");" << endl
           << "}"
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PublishesUnsubscribeEmitter : Traversal::PublisherData,
                                         EmitterBase
    {
      PublishesUnsubscribeEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name ().unescaped_str () << "\") == 0)" << endl
           << "{"
           << "return this->unsubscribe_" << p.name ()
           << " (" << endl
           << "ck" << endl
           << ");" << endl
           << "}";
      }
    };

    struct EmitsEmitter : Traversal::EmitterData,
                          EmitterBase
    {
      EmitsEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& e)
      {
        os << "void" << endl
           << scope_.name () << "_Servant::connect_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "this->context_->connect_" << e.name ()
           << " (" << endl
           << "c" << endl
           << ");" << endl
           << "}";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << e.name () << " ()" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_"
           << e.name () << " (" << endl
           << ");" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct ServantTypeNameEmitter : Traversal::Type,
                                    EmitterBase
    {
      ServantTypeNameEmitter (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      traverse (SemanticGraph::Type& t)
      {
        os << t.name () << "_Servant";
      }
    };

    struct FacetProvidesEmitter : Traversal::ProviderData,
                                  EmitterBase
    {
      FacetProvidesEmitter (Context& c,
                            SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          servant_type_name_emitter_ (c),
          enclosing_type_name_emitter_ (c),
          facet_enclosing_type_name_emitter_ (c),
          repo_id_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        servant_belongs_.node_traverser (servant_type_name_emitter_);
        enclosing_belongs_.node_traverser (enclosing_type_name_emitter_);
        facet_enclosing_belongs_.node_traverser (
            facet_enclosing_type_name_emitter_
          );
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        Traversal::ProviderData::belongs (p, belongs_);

        os << "_ptr" << endl
           << scope_.name ()
           << "_Servant::provide_" << p.name () << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{";

        string swap_option = ctx.cl ().get_value ("custom-container", "");
        bool swapping = (swap_option == "upgradeable");

        if (swapping)

        {
          os << "this->activate_component ();"
             << endl;
          }

        os << "if (! ::CORBA::is_nil (this->provide_"
           << p.name () << "_.in ()))" << endl
           << "{"
           << "return ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_duplicate (this->provide_" << p.name () << "_.in ());"
           << "}";

        os << "::CORBA::Object_var obj =" << endl
           << "  this->provide_" << p.name () << "_i ("
           << ");"
           << endl;

        ScopedName scoped (scope_.scoped_name ());
        Name stripped (scoped.begin () + 1, scoped.end ());

        Traversal::ProviderData::belongs (p, belongs_);

        os << "_var fo =" << endl
           << "  ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_narrow ("
           << "obj.in ()" << endl
           << ");"
           << endl
           << "this->provide_" << p.name () << "_ = fo;"
           << "return ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_duplicate (this->provide_"
           << p.name () << "_.in ());" << endl
           << "}";

        // Corresponding private provide_*_i() method.

        os << "::CORBA::Object_ptr" << endl
           << scope_.name ()
           << "_Servant::provide_" << p.name () << "_i (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "::CORBA::Object_ptr ret =" << endl
           << "  this->lookup_facet (\""
           << p.name ().unescaped_str () << "\");" << endl;

        os << "if (! ::CORBA::is_nil (ret))" << endl
           << "{"
           << "return ret;"
           << "}";

        os << "::CIAO::Port_Activator_T<" << endl
           << "    ::" << STRS[FACET_PREFIX];

        Traversal::ProviderData::belongs (p, facet_enclosing_belongs_);

        os << "::";

        Traversal::ProviderData::belongs (p, servant_belongs_);

        os << "," << endl
           << "    ";

        Traversal::ProviderData::belongs (p, enclosing_belongs_);

        os << "::CCM_";

        Traversal::ProviderData::belongs (p, simple_belongs_);

        os << "," << endl
           << "    ::Components::CCMContext," << endl
           << "    " << scope_.name () << "_Servant" << endl
           << "  > *tmp = 0;" << endl
           << "typedef ::CIAO::Port_Activator_T<" << endl
           << "    ::" << STRS[FACET_PREFIX];

        Traversal::ProviderData::belongs (p, facet_enclosing_belongs_);

        os << "::";

        Traversal::ProviderData::belongs (p, servant_belongs_);

        os << "," << endl
           << "    ";

        Traversal::ProviderData::belongs (p, enclosing_belongs_);

        os << "::CCM_";

        Traversal::ProviderData::belongs (p, simple_belongs_);

        os << "," << endl
           << "    ::Components::CCMContext," << endl
           << "    " << scope_.name () << "_Servant" << endl
           << "  >" << endl
           << "MACRO_MADNESS_TYPEDEF;" << endl;

        os << "ACE_CString obj_id (this->ins_name_);"
           << "obj_id += \"_" << p.name ().unescaped_str () << "\";" << endl;

        os << "ACE_NEW_THROW_EX ("
           << "tmp," << endl
           << "MACRO_MADNESS_TYPEDEF (" << endl
           << "obj_id.c_str ()," << endl
           << "\"" << p.name ().unescaped_str () << "\"," << endl
           << "::CIAO::Port_Activator::Facet," << endl
           << "0," << endl
           << "this->context_," << endl
           << "this)," << endl
           << "::CORBA::NO_MEMORY ());" << endl;

        os << "::CIAO::Servant_Activator *sa = " << endl
           << "this->container_->ports_servant_activator ();" << endl
           << "if (!sa->register_port_activator (tmp))" << endl
           << "{"
           << "return ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ();"
           << "}";

        os << "::CORBA::Object_var obj =" << endl
           << "  this->container_->generate_reference (" << endl
           << "  obj_id.c_str ()," << endl
           << "  ";

        Traversal::ProviderData::belongs (p, repo_id_belongs_);

        os << "," << endl
           << "  ::CIAO::Container::Facet_Consumer" << endl
           << "  " << ");"
           << endl
           << "this->add_facet (\""
           << p.name ().unescaped_str () << "\"," << endl
           << "obj.in ()" << endl
           << ");"
           << endl;

        os << "return obj._retn ();" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      ServantTypeNameEmitter servant_type_name_emitter_;
      EnclosingTypeNameEmitter enclosing_type_name_emitter_;
      FacetEnclosingTypeNameEmitter facet_enclosing_type_name_emitter_;
      RepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs servant_belongs_;
      Traversal::Belongs enclosing_belongs_;
      Traversal::Belongs facet_enclosing_belongs_;
      Traversal::Belongs repo_id_belongs_;
      SemanticGraph::Component& scope_;
    };

    struct ConsumesEmitter : Traversal::ConsumerData,
                             EmitterBase
    {
      ConsumesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          repo_id_emitter_ (c),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (Type& c)
      {
        os << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant (" << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name  ()
           << "_ptr executor,"
           << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name  ()
           << "_Context_ptr c)" << endl
           << "  : executor_ ( " << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_" << c.scoped_name ().scope_name ().simple_name  ()
           << "::_duplicate (executor))," << endl
           << "    ctx_ ( " << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_" << c.scoped_name ().scope_name ().simple_name  ()
           << "_Context::_duplicate (c))" << endl
           << "{"
           << "}";

        os << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::~";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant (void)" << endl
           << "{"
           << "}";

        os << "::CORBA::Object_ptr" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::_get_component (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->ctx_->get_CCM_object "
           << "();" << endl
           << "}";

        os << "void" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << " (" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *evt" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "this->executor_->push_" << c.name ()
           << " (" << endl
           << "evt" << endl
           << ");" << endl
           << "}";

        os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "void" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::push_event (" << endl
           << "::Components::EventBase *ev)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_BET] << "))" << endl
           << "{";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *ev_type =" << endl
           << "  ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "::_downcast (ev);" << endl
           << "if (ev_type != 0)" << endl
           << "{"
           << "this->push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << " (" << endl
           << "ev_type" << endl
           << ");" << endl
           << "return;" << endl
           << "}"
           << "throw " << STRS[EXCP_BET] << " ();" << endl
           << "}";

        // GD Added below code
        // Begin

        os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "void" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::ciao_push_event (" << endl
           << "::Components::EventBase *ev," << endl
           << "const char * /* source_id */," << endl
           << "::CORBA::TypeCode_ptr /* tc */)" << endl
           << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_BET] << "))" << endl
           << "{"
           << "this->push_event (ev);" << endl
           << "}";

         // End

        os << "CORBA::Boolean" << endl
           << scope_.name () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::ciao_is_substitutable (" << endl
           << "const char * event_repo_id" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (event_repo_id == 0)" << endl
           << "{"
           << "throw ::CORBA::BAD_PARAM ();" << endl
           << "}"
           << scope_.name () << "_Context *ctx =" << endl
           << "  " << scope_.name () << "_Context::_narrow (" << endl
           << "  this->ctx_.in ()" << endl
           << "  " << ");"
           << endl;

        os << "CORBA::ORB_ptr orb = ctx->_ciao_the_Container ()->the_ORB ();"
           << endl;

        os << "CORBA::ValueFactory f =" << endl
           << "  orb->lookup_value_factory (" << endl
           << "  event_repo_id" << endl
           << "  " << ");"
           << endl;

        os << "if (f == 0)" << endl
           << "{"
           << "return false;" << endl
           << "}"
           << "CORBA::ValueBase_var v =" << endl
           << "  f->create_for_unmarshal ();"
           << endl;

        os << "f->_remove_ref ();" << endl;

        os << "if (v.in () == 0)" << endl
           << "{"
           << "return false;" << endl
           << "}";

        os << "return dynamic_cast< ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *> (v.in ()) != 0;" << endl
           << "}" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name  () << "_Servant::"
           << "get_consumer_" << c.name () << " (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (! ::CORBA::is_nil (this->consumes_" << c.name ()
           << "_.in ()))" << endl
           << "{"
           << "return ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_duplicate (this->consumes_"
           << c.name () << "_.in ());"
           << "}";

        os << "::Components::EventConsumerBase_var obj =" << endl
           << "  this->get_consumer_" << c.name () << "_i (" << endl
           << "  " << ");"
           << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_var eco =" << endl
           << "  ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_narrow (" << endl
           << "  obj.in ()" << endl
           << "  " << ");"
           << endl
           << "this->consumes_" << c.name () << "_ = eco;"
           << "return ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_duplicate (this->consumes_" << c.name ()
           << "_.in ());" << endl
           << "}";

        // Corresponding private get_consumer_*_i() method.

        os << "::Components::EventConsumerBase_ptr" << endl
           << scope_.name  () << "_Servant::"
           << "get_consumer_" << c.name () << "_i (" << endl
           << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "::Components::EventConsumerBase_ptr ret =" << endl
           << "  this->lookup_consumer (\""
           << c.name ().unescaped_str () << "\");" << endl;

        os << "if (! ::CORBA::is_nil (ret))" << endl
           << "{"
           << "return ret;"
           << "}";

        os << "::CIAO::Port_Activator_T<" << endl;

        os << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant," << endl
           << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "," << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context," << endl
           << scope_.name () << "_Servant"
           << " > *tmp = 0;" << endl
           << "typedef  CIAO::Port_Activator_T<" << endl;

        os << "    " << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant," << endl
           << "    " << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "," << endl
           << "    " << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name ()
           << "_Context, " << endl
           << "    " << scope_.name () << "_Servant" << endl
           << "  >" << endl
           << "MACRO_MADNESS_TYPEDEF;" << endl;

        os << "ACE_CString obj_id (this->ins_name_);"
           << "obj_id += \"_" << c.name ().unescaped_str () << "\";" << endl;

        os << "ACE_NEW_THROW_EX (" << endl
           << "tmp," << endl
           << "MACRO_MADNESS_TYPEDEF ("
           << "obj_id.c_str ()," << endl
           << "\"" << c.name ().unescaped_str () << "\"," << endl
           << "::CIAO::Port_Activator::Sink," << endl
           << "this->executor_.in ()," << endl
           << "this->context_," << endl
           << "this)," << endl
           << "::CORBA::NO_MEMORY ());" << endl;

        os << "::CIAO::Servant_Activator *sa =" << endl
           << "  this->container_->ports_servant_activator ();" << endl
           << "if (!sa->register_port_activator (tmp))" << endl
           << "{"
           << "return ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ();"
           << "}";

        os << "::CORBA::Object_var obj =" << endl
           << "  this->container_->generate_reference (" << endl
           << "  obj_id.c_str ()," << endl
           << "  ";

        Traversal::ConsumerData::belongs (c, repo_id_belongs_);

        os << "," << endl
           << "  ::CIAO::Container::Facet_Consumer" << endl
           << "  " << ");"
           << endl;

        os << "::Components::EventConsumerBase_var ecb =" << endl
           << "  ::Components::EventConsumerBase::_narrow (" << endl
           << "  obj.in ()" << endl
           << "  " << ");"
           << endl;

        os << "this->add_consumer (\""
           << c.name ().unescaped_str () << "\"," << endl
           << "ecb.in ()" << endl
           << ");"
           << endl;

        os << "return ecb._retn ();" << endl
           << "}";
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      ConsumerRepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs repo_id_belongs_;
      SemanticGraph::Component& scope_;
    };

    struct OperationExistsEmitter;

    struct RegisterValueFactoryEmitter : Traversal::ConsumerData,
                                         Traversal::PublisherData,
                                         Traversal::EmitterData,
                                         EmitterBase
    {
      RegisterValueFactoryEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          gen_factory_ (true)
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      void factory_gen_off (void)
      {
        gen_factory_ = false;
      }

      bool gen_factory (void) const
      {
        return gen_factory_;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        Traversal::PublisherData publisher;
        RegisterValueFactoryEmitter::traverse_common<
          SemanticGraph::Publisher,
          Traversal::PublisherData> (p, publisher, this, os, belongs_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        Traversal::EmitterData emitter;
        RegisterValueFactoryEmitter::traverse_common<
          SemanticGraph::Emitter,
          Traversal::EmitterData> (e, emitter, this, os, belongs_);
      }

      virtual void
      traverse (SemanticGraph::Consumer& c)
      {
        Traversal::ConsumerData consumer;
        RegisterValueFactoryEmitter::traverse_common<
          SemanticGraph::Consumer,
          Traversal::ConsumerData> (c, consumer, this, os, belongs_);
      }

      template<typename SemanticType, typename TraversalType>
      static void
      traverse_common (SemanticType& st,
                       TraversalType& tt,
                       RegisterValueFactoryEmitter* r,
                       ostream& os,
                       Traversal::Belongs& blongs)
      {
        {
          Traversal::Belongs tt_belongs;
          tt.edge_traverser (tt_belongs);

          // Separate traversers because we want to catch inherited
          // operations, but not base class factories or private
          // members.
          Traversal::ValueType event_type_operations;
          Traversal::ValueType event_type_factories;
          tt_belongs.node_traverser (event_type_operations);
          tt_belongs.node_traverser (event_type_factories);

          Traversal::Inherits inherits;
          inherits.node_traverser (event_type_operations);
          event_type_operations.edge_traverser (inherits);

          Traversal::Defines include_inherit_defines;
          event_type_operations.edge_traverser (include_inherit_defines);
          OperationExistsEmitter op_emitter (r);
          include_inherit_defines.node_traverser (op_emitter);

          Traversal::Supports supports;
          event_type_operations.edge_traverser (supports);
          Traversal::Interface iface;
          supports.node_traverser (iface);
          inherits.node_traverser (iface);
          iface.edge_traverser (inherits);
          iface.edge_traverser (include_inherit_defines);

          Traversal::Defines no_include_inherit_defines;
          event_type_factories.edge_traverser (no_include_inherit_defines);
          PrivateExistsEmitter priv_emitter (r);
          FactoryExistsEmitter factory_emitter (r);
          no_include_inherit_defines.node_traverser (priv_emitter);
          no_include_inherit_defines.node_traverser (factory_emitter);

          tt.traverse (st);
        }

        if (r->gen_factory ())
          {
            os << "CIAO_REGISTER_OBV_FACTORY (" << endl;

            r->TraversalType::belongs (st, blongs);

            os << "_init," << endl;

            r->TraversalType::belongs (st, blongs);

            os << ");" << endl;
          }
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      bool gen_factory_;
    };

    struct OperationExistsEmitter : Traversal::Operation,
                                    Traversal::ValueTypeFactory
    {
      OperationExistsEmitter (RegisterValueFactoryEmitter* r)
        : r_ (r)
      {
      }

      virtual void
      traverse (SemanticGraph::Operation&)
      {
        r_->factory_gen_off ();
      }

      virtual void
      traverse (SemanticGraph::ValueTypeFactory&)
      {
        // This gets called for base eventtypes as well, so
        // we don't want to catch this here, but there's no
        // way to avoid it, so we make it a no-op. This
        // catches both valuetype and eventtype factories.
      }

    private:
      RegisterValueFactoryEmitter* r_;
    };

    // Since we're not interested in ancestors' factories, we can
    // specialize all the way to EventTypeFactory.
    struct FactoryExistsEmitter : Traversal::EventTypeFactory
    {
      FactoryExistsEmitter (RegisterValueFactoryEmitter* r)
        : r_ (r)
      {
      }

      virtual void
      traverse (SemanticGraph::EventTypeFactory&)
      {
        r_->factory_gen_off ();
      }

    private:
      RegisterValueFactoryEmitter* r_;
    };

    struct PrivateExistsEmitter : Traversal::ValueTypePrivateMember
    {
      PrivateExistsEmitter (RegisterValueFactoryEmitter* r)
        : r_ (r)
      {
      }

      virtual void
      traverse (Type&)
      {
        r_->factory_gen_off ();
      }

    private:
      RegisterValueFactoryEmitter* r_;
    };

    struct PortTablePopulator : Traversal::ProviderData,
                                Traversal::UserData,
                                Traversal::PublisherData,
                                Traversal::EmitterData,
                                Traversal::ConsumerData,
                                EmitterBase
    {
      PortTablePopulator (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      traverse (SemanticGraph::Provider& p)
      {
        os << "obj_var =" << endl
           << "  this->provide_" << p.name () << "_i (" << endl
           << "  " << ");" << endl;
      }

      virtual void
      traverse (SemanticGraph::User&)
      {
      }

      virtual void
      traverse (SemanticGraph::Publisher&)
      {
      }

      virtual void
      traverse (SemanticGraph::Emitter&)
      {
      }

      virtual void
      traverse (SemanticGraph::Consumer& p)
      {
        os << "ecb_var =" << endl
           << "  this->get_consumer_" << p.name () << "_i (" << endl
           <<  "  " << ");" << endl;
      }
    };

    struct SetAttributesEmitter : Traversal::ReadWriteAttribute,
                                  EmitterBase
    {
      SetAttributesEmitter (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      pre (SemanticGraph::ReadWriteAttribute& a)
      {
        Traversal::Belongs delegate_belongs;

        SetAttributeDelegationEmitter delegater (ctx, a);
        delegate_belongs.node_traverser (delegater);

        Traversal::ReadWriteAttribute::belongs (a, delegate_belongs);
      }
    };

    struct UsesSizeCalculator : Traversal::SingleUserData,
                                Traversal::MultiUserData,
                                EmitterBase
    {
      UsesSizeCalculator (Context& c, unsigned long& size)
        : EmitterBase (c),
          size_ (size)
      {
      }

      virtual void
      traverse (SemanticGraph::SingleUser&)
      {
        ++size_;
      }

      virtual void
      traverse (SemanticGraph::MultiUser&)
      {
        ++size_;
      }

    private:
      unsigned long& size_;
    };

    struct UsesGetAllEmitter : Traversal::SingleUserData,
                               Traversal::MultiUserData,
                               EmitterBase
    {
      UsesGetAllEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          repo_id_emitter_ (c),
          slot_ (0UL)
      {
        belongs_.node_traverser (type_name_emitter_);
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "::CIAO::Servant_Impl_Base::describe_simplex_receptacle<"
           << endl
           << "    ";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_var" << endl
           << "  > (\"" << u.name ().unescaped_str () << "\"," << endl;

        Traversal::SingleUserData::belongs (u, repo_id_belongs_);

        os << "," << endl
           << "this->context_->ciao_uses_" << u.name () << "_," << endl
           << "safe_retval," << endl
           << slot_ << "UL);" << endl;

        ++slot_;
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "::CIAO::Servant_Impl_Base::describe_multiplex_receptacle<"
           << endl
           << "    ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var" << endl
           << "  > (\"" << u.name ().unescaped_str () << "\"," << endl;

        Traversal::MultiUserData::belongs (u, repo_id_belongs_);

        os << "," << endl
           << "this->context_->ciao_uses_" << u.name () << "_," << endl
           << "safe_retval," << endl
           << slot_ << "UL);" << endl;

        ++slot_;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      RepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs repo_id_belongs_;
      unsigned long slot_;
    };

    struct PublishesSizeCalculator : Traversal::PublisherData,
                                     EmitterBase
    {
      PublishesSizeCalculator (Context& c, unsigned long& size)
        : EmitterBase (c),
          size_ (size)
      {
      }

      virtual void
      traverse (SemanticGraph::Publisher&)
      {
        ++size_;
      }

    private:
      unsigned long& size_;
    };

    struct PublishesGetAllEmitter : Traversal::PublisherData,
                                    EmitterBase
    {
      PublishesGetAllEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          repo_id_emitter_ (c),
          slot_ (0UL)
      {
        belongs_.node_traverser (type_name_emitter_);
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "::CIAO::Servant_Impl_Base::describe_pub_event_source<"
           << endl
           << "    ";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var" << endl
           << "  > (\"" << p.name ().unescaped_str () << "\"," << endl;

        Traversal::PublisherData::belongs (p, repo_id_belongs_);

        os << "," << endl
           << "this->context_->ciao_publishes_" << p.name ()
           << "_map_," << endl
           << "safe_retval," << endl
           << slot_ << "UL);" << endl;

        ++slot_;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      RepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs repo_id_belongs_;
      unsigned long slot_;
    };

    struct EmitsSizeCalculator : Traversal::EmitterData,
                                 EmitterBase
    {
      EmitsSizeCalculator (Context& c, unsigned long& size)
        : EmitterBase (c),
          size_ (size)

      {
      }

      virtual void
      traverse (SemanticGraph::Emitter&)
      {
        ++size_;
      }

    private:
      unsigned long& size_;
    };

    struct EmitsGetAllEmitter : Traversal::EmitterData,
                                EmitterBase
    {
      EmitsGetAllEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c),
          repo_id_emitter_ (c),
          slot_ (0UL)
      {
        belongs_.node_traverser (type_name_emitter_);
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "::CIAO::Servant_Impl_Base::describe_emit_event_source<"
           << endl
           << "    ";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_var" << endl
           << "  > (\"" << e.name ().unescaped_str () << "\"," << endl;

        Traversal::EmitterData::belongs (e, repo_id_belongs_);

        os << "," << endl
           << "this->context_->ciao_emits_" << e.name ()
           << "_consumer_," << endl
           << "safe_retval," << endl
           << slot_ << "UL);" << endl;

        ++slot_;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      RepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs repo_id_belongs_;
      unsigned long slot_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      ScopedName scoped (t.scoped_name ());
      Name stripped (scoped.begin () + 1, scoped.end ());

      // Servant Constructor
      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::Components::CCMHome_ptr h," << endl
         << "const char *ins_name," << endl
         << "::CIAO::Home_Servant_Impl_Base *hs," << endl
         << "::CIAO::Session_Container *c)" << endl
         << "  : ::CIAO::Servant_Impl_Base "
         << "(h, hs, c)," << endl
         << "    ::CIAO::Servant_Impl<" << endl
         << "        ::POA_" << stripped << "," << endl
         << "        " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "," << endl
         << "        " << t.name () << "_Context" << endl
         << "      > (exe, h, hs, c)," << endl
         << "    ins_name_ (ins_name)" << endl
         << "{"
         << "ACE_NEW ("
         << "this->context_," << endl
         << t.name () << "_Context (h, c, this));" << endl;

      os << "// Set the instance id of the component on the context" << endl
         << endl
         << "this->context_->_ciao_instance_id (this->ins_name_);" << endl;


      // Generate the macro to register a value factory for each
      // eventtype consumed.
      if (!ctx.cl ().get_value ("suppress-register-factory", false))
        {
          Traversal::Component component_emitter;

          Traversal::Inherits inherits;
          inherits.node_traverser (component_emitter);

          Traversal::Defines defines;
          component_emitter.edge_traverser (defines);
          component_emitter.edge_traverser (inherits);

          RegisterValueFactoryEmitter factory_emitter (ctx);
          defines.node_traverser (factory_emitter);

          component_emitter.traverse (t);
        }

      os << "try" << endl
         << "{"
         << "::Components::SessionComponent_var scom =" << endl
         << "  ::Components::SessionComponent::_narrow (" << endl
         << "  exe" << endl
         << "  " << ");" << endl
         << "if (! ::CORBA::is_nil (scom.in ()))" << endl
         << "{"
         << "scom->set_session_context (" << endl
         << "this->context_" << endl
         << ");"
         << "}"
         << "this->populate_port_tables (" << endl
         << ");"
         << "}"
         << "catch (const CORBA::Exception&)" << endl
         << "{"
         << "}"
         << endl
         << "}";

      // Servant Destructor
      os << t.name () << "_Servant::~"
         << t.name () << "_Servant (void)" << endl
         << "{"
         << "}";

      // Override pure virtual set_attributes() operation.
      os << "void" << endl
         << t.name () << "_Servant::set_attributes (" << endl
         << "const ::Components::ConfigValues &descr" << endl
         << ")" << endl
         << "{" << endl;

      string swap_option = ctx.cl ().get_value ("custom-container", "");
      bool swapping = (swap_option == "upgradeable");

      if (swapping)
        {
          os << "this->activate_component ();"
             << endl;
        }

      os << "for ( ::CORBA::ULong i = 0; i < descr.length (); ++i)" << endl
         << "{"
         << "const char *descr_name = descr[i]->name ();"
         << "::CORBA::Any &descr_value = descr[i]->value ();" << endl;

      // Generate string compare and set for each attribute.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        SetAttributesEmitter set_attributes_emitter (ctx);
        defines.node_traverser (set_attributes_emitter);

        component_emitter.traverse (t);
      }


      os << STRS[ACE_UA] << " (descr_name);"
         << STRS[ACE_UA] << " (descr_value);" << endl;

      os << "}"
         << "}";

      // Generate provides_<facet> operation.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        FacetProvidesEmitter provides_emitter (ctx, t);
        defines.node_traverser (provides_emitter);

        component_emitter.traverse (t);
      }

      // Generate subscribe_* and unsubscribe_* operations.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        PublishesEmitter publishes_emitter (ctx, t);
        defines.node_traverser (publishes_emitter);

        component_emitter.traverse (t);
      }

      // Generate operations for nested Consumer classes.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        ConsumesEmitter consumes_emitter (ctx, t);
        defines.node_traverser (consumes_emitter);

        component_emitter.traverse (t);
      }

      os << STRS[COMP_CK] << " *" << endl
         << t.name () << "_Servant::connect (" << endl
         << "const char *name," << endl
         << "::CORBA::Object_ptr connection" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{";

      if (swapping)
        {
          os << "this->activate_component ();"
             << endl;
        }

      os << "// If the component has no receptacles, this will be unused."
         << endl
         << STRS[ACE_UA] << " (connection);" << endl
         << "if (name == 0)" << endl
         << "{"
         << "throw " << STRS[EXCP_IN] << " ();" << endl
         << "}";

      // Generate IF block in connect operation for each receptacle.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        UsesConnectEmitter uses_emitter (ctx);
        defines.node_traverser (uses_emitter);

        component_emitter.traverse (t);
      }

      os << "throw " << STRS[EXCP_IN] << " ();" << endl
         << "}";

      os << "::CORBA::Object_ptr" << endl
         << t.name () << "_Servant::disconnect (" << endl
         << "const char *name," << endl
         << STRS[COMP_CK] << " * ck" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_CR] << "," << endl
         << STRS[EXCP_NC] << "))" << endl
         << "{"
         << STRS[ACE_UA] << " (ck);" << endl;

      if (swapping)
      {
        os << "this->activate_component ();"
           << endl;
      }

      os << "if (name == 0)" << endl
         << "{"
         << "throw "
         << STRS[EXCP_BP] << " ();" << endl
         << "}";

      // Generate IF block in disconnect operation for each receptacle.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        UsesDisconnectEmitter uses_emitter (ctx);
        defines.node_traverser (uses_emitter);

        component_emitter.traverse (t);
      }

      os << "throw "
         << STRS[EXCP_IN] << " ();" << endl
         << "}";

      os << STRS[COMP_RD] << " *" << endl
         << t.name () << "_Servant::get_all_receptacles (" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << STRS[COMP_RD] << " *retval = 0;"
         << "ACE_NEW_RETURN (retval," << endl
         << STRS[COMP_RD] << "," << endl
         << "0);"
         << STRS[COMP_RD] << "_var safe_retval = retval;";

      unsigned long size = 0;

      // Calculate the number of uses ports.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        UsesSizeCalculator size_calculator (ctx, size);
        defines.node_traverser (size_calculator);

        component_emitter.traverse (t);
      }

      os << "safe_retval->length (" << size << "UL);" << endl;

      // Generate a sequence element assignment for each uses port.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        UsesGetAllEmitter get_all_emitter (ctx);
        defines.node_traverser (get_all_emitter);

        component_emitter.traverse (t);
      }

      os << "return safe_retval._retn ();"
         << "}";

      // Generate generic operations for receptacles.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        UsesEmitter uses_emitter (ctx, t);
        defines.node_traverser (uses_emitter);

        component_emitter.traverse (t);
      }

      os << "void" << endl
         << t.name () << "_Servant::connect_consumer ("
         << endl
         << "const char * emitter_name," << endl
         << STRS[COMP_ECB] << "_ptr consumer" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{";

      if (swapping)
      {
        os << "this->activate_component ();"
           << endl;
      }

      os << "if (emitter_name == 0)" << endl
         << "{"
         << "throw " << STRS[EXCP_BP] << " ();" << endl
         << "}";

      // Generate an IF block for each 'emits' declaration.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        EmitsConnectEmitter emits_connect_emitter (ctx);
        defines.node_traverser (emits_connect_emitter);

        component_emitter.traverse (t);
      }

      os << STRS[ACE_UA] << " (consumer);"
         << "throw "
         << STRS[EXCP_IN] << " ();" << endl
         << "}";

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::disconnect_consumer (" << endl
         << "const char *source_name" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_NC] << "))" << endl
         << "{";

      if (swapping)
      {
        os << "this->activate_component ();"
           << endl;
      }

      os << "if (source_name == 0)" << endl
         << "{"
         << "throw " << STRS[EXCP_BP] << " ();" << endl
         << "}";

      // Generate an IF block for each 'emits' declaration.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        EmitsDisconnectEmitter emits_disconnect_emitter (ctx);
        defines.node_traverser (emits_disconnect_emitter);

        component_emitter.traverse (t);
      }

      os << "throw "
         << STRS[EXCP_IN] << " ();" << endl
         << "}";

      os << STRS[COMP_PD] << " *" << endl
         << t.name () << "_Servant::get_all_publishers (" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << STRS[COMP_PD] << " *retval = 0;"
         << "ACE_NEW_RETURN (retval," << endl
         << STRS[COMP_PD] << "," << endl
         << "0);"
         << STRS[COMP_PD] << "_var safe_retval = retval;";

      size = 0UL;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        PublishesSizeCalculator publishes_size_calculator (ctx, size);
        defines.node_traverser (publishes_size_calculator);

        component_emitter.traverse (t);
      }

      os << "safe_retval->length (" << size << "UL);" << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        PublishesGetAllEmitter publishes_get_all_emitter (ctx);
        defines.node_traverser (publishes_get_all_emitter);

        component_emitter.traverse (t);
      }

      os << "return safe_retval._retn ();"
         << "}";

      os << STRS[COMP_ED] << " *" << endl
         << t.name () << "_Servant::get_all_emitters (" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << STRS[COMP_ED] << " *retval = 0;"
         << "ACE_NEW_RETURN (retval," << endl
         << STRS[COMP_ED] << "," << endl
         << "0);"
         << STRS[COMP_ED] << "_var safe_retval = retval;";

      size = 0UL;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        EmitsSizeCalculator emits_size_calculator (ctx, size);
        defines.node_traverser (emits_size_calculator);

        component_emitter.traverse (t);
      }

      os << "safe_retval->length (" << size << "UL);" << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        EmitsGetAllEmitter emits_get_all_emitter (ctx);
        defines.node_traverser (emits_get_all_emitter);

        component_emitter.traverse (t);
      }

      os << "return safe_retval._retn ();"
         << "}";

      os << STRS[COMP_CK] << " *" << endl
         << t.name () << "_Servant::subscribe (" << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_ECB] << "_ptr subscribe" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{";

      if (swapping)
        {
          os << "this->activate_component ();"
             << endl;
        }

      os << "// Just in case there are no if blocks" << endl
         << STRS[ACE_UA] << " (subscribe);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "throw " << STRS[EXCP_IN] << " ();"
         << endl
         << "}";

      // Generate an IF block in for each publisher in subscribe().
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        PublishesSubscribeEmitter publishes_emitter (ctx);
        defines.node_traverser (publishes_emitter);

        component_emitter.traverse (t);
      }

      os << "throw " << STRS[EXCP_IN] << " ();" << endl
         << "}";

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::unsubscribe ("
         << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_CK] << " *ck" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{";

      if (swapping)
      {
        os << "this->activate_component ();"
           << endl;
      }

      os << "// Just in case there are no if blocks" << endl
         << STRS[ACE_UA] << " (ck);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "throw "
         << STRS[EXCP_IN] << " ();" << endl
         << "}";

      // Generate an IF block in for each publisher in unsubscribe().
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        PublishesUnsubscribeEmitter publishes_emitter (ctx);
        defines.node_traverser (publishes_emitter);

        component_emitter.traverse (t);
      }

      os << "throw "
         << STRS[EXCP_IN] << " ();" << endl
         << "}";

      // Generate connect() and disconnect() for each emits declaration.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        EmitsEmitter emits_emitter (ctx, t);
        defines.node_traverser (emits_emitter);

        component_emitter.traverse (t);
      }

      os << "::CORBA::Object_ptr" << endl
         << t.name ()
         << "_Servant::get_facet_executor (" << endl
         << "const char *name" << endl
         << ")" << endl
         << STRS[EXCP_START] << " "
         << STRS[EXCP_SYS] << "))" << endl
         << "{";

      if (swapping)
      {
        os << "this->activate_component ();"
           << endl;
      }

      os << "if (name == 0)" << endl
         << "{"
         << "throw " << STRS[EXCP_BP] << " ();" << endl
         << "}";


      // Generate an IF block for each facet inside provide_facet().
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        NavigationGetFacetExecEmitter navigation_facet_exec_emitter (ctx);
        defines.node_traverser (navigation_facet_exec_emitter);

        component_emitter.traverse (t);
      }

      os << "return ::CORBA::Object::_nil ();"
         << "}";

      os << "// Supported operations." << endl << endl;

      // Generate operations for all supported interfaces.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        Traversal::Supports supports_;
        component_emitter.edge_traverser (supports_);
        component_emitter.edge_traverser (component_inherits);

        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits interface_inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (interface_inherits);

        AttributeSourceEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);

        OperationEmitter<SemanticGraph::Component> operation_emitter (ctx, t);
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

      // Generate operations for component attributes.
      os << "// Component attribute operations." << endl << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        component_emitter.edge_traverser (inherits);
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);

        AttributeSourceEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);

        component_emitter.traverse (t);
      }

      os << "// Private method to populate the port tables."
         << endl;

      os << "void" << endl
         << t.name () << "_Servant::populate_port_tables (" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << ";"
         << "::CORBA::Object_var obj_var;"
         << "::Components::EventConsumerBase_var ecb_var;" << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        component_emitter.edge_traverser (inherits);
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);

        PortTablePopulator port_table_populator (ctx);
        defines.node_traverser (port_table_populator);

        component_emitter.traverse (t);
      }

      os << "}";
    }
  };

  struct HomeEmitter : Traversal::Home, EmitterBase
  {
    HomeEmitter (Context& c, CommandLine const& cl)
      : EmitterBase (c),
        cl_ (cl),
        simple_type_name_emitter_ (c),
        repo_id_emitter_ (c),
        flat_name_emitter_ (c)
    {
      simple_manages_.node_traverser (simple_type_name_emitter_);
      repo_id_manages_.node_traverser (repo_id_emitter_);
      flat_name_manages_.node_traverser (flat_name_emitter_);
    }

    // Nested classes used by this emitter.
  private:
    struct HomeOpExecReturnEmitter : Traversal::Type
    {
      HomeOpExecReturnEmitter (std::ostream& os_)
        : os (os_)
      {}

      virtual void
      traverse (SemanticGraph::Type&)
      {
        os << STRS[COMP_EC] << "_var _ciao_ec =" << endl
           << "this->executor_->";
      }

    private:
      std::ostream& os;
    };

    // HomeFactory and HomeFinder are tied to Operation in
    // the front end. Since we want to treat them differently
    // than regular operations in a home (we don't want to
    // generate anything for base class factory operations,
    // example), we use this class for regular home operations
    // that overrides HomeFactory and HomeFinder traversals
    // to do nothing.
    struct HomeOperationEmitter : OperationEmitter<SemanticGraph::Home>,
                                  Traversal::HomeFactory,
                                  Traversal::HomeFinder
    {
      HomeOperationEmitter (Context& c, SemanticGraph::Home& home)
        : OperationEmitter<SemanticGraph::Home> (c, home)
      {}

      // Overriden to do nothing since we don't want to generate
      // component swapping related code for facet operations.
      virtual void
      gen_swap_related (SemanticGraph::Operation&)
      {
      }

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
          return_type_name_emitter_ (c),
          enclosing_type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          type_name_emitter_ (c),
          scope_ (home)
      {
        returns_.node_traverser (return_type_name_emitter_);
        enclosing_manages_.node_traverser (enclosing_type_name_emitter_);
        simple_manages_.node_traverser (simple_type_name_emitter_);
        manages_.node_traverser (type_name_emitter_);
      }

      virtual void
      returns (SemanticGraph::HomeFactory& hf)
      {
        Traversal::HomeFactory::returns (hf, returns_);

        os << endl;
      }

      virtual void
      name (SemanticGraph::HomeFactory& hf)
      {
        os << scope_.name () << "_Servant::" << hf.name ();
      }

      virtual void
      receives_none (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl
           << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFactory&)
      {
        os << endl << ")" << endl;
      }

      virtual void
      raises_none (SemanticGraph::HomeFactory&)
      {
        os << STRS[EXCP_SNGL];
      }

      virtual void
      raises_pre (SemanticGraph::HomeFactory&)
      {
        os << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl;
      }

      virtual void
      raises_post (SemanticGraph::HomeFactory&)
      {
        os << "))";
      }

      virtual void
      post (SemanticGraph::HomeFactory& hf)
      {
        os << "{";

        {
          OperationExecEmitter operation_emitter (ctx);

          Traversal::Receives receives;
          Traversal::Belongs returns;
          operation_emitter.edge_traverser (receives);
          operation_emitter.edge_traverser (returns);

          ParameterExecEmitter<Traversal::InParameter> in_param (os);
          ParameterExecEmitter<Traversal::InOutParameter> inout_param (os);
          ParameterExecEmitter<Traversal::OutParameter> out_param (os);
          receives.node_traverser (in_param);
          receives.node_traverser (inout_param);
          receives.node_traverser (out_param);

          HomeOpExecReturnEmitter return_emitter (os);
          returns.node_traverser (return_emitter);

          operation_emitter.traverse (hf);
        }

        Traversal::Home::manages (scope_, enclosing_manages_);

        os << "::CCM_";

        Traversal::Home::manages (scope_, simple_manages_);

        os << "_var _ciao_comp =" << endl;

        Traversal::Home::manages (scope_, enclosing_manages_);

        os << "::CCM_";

        Traversal::Home::manages (scope_, simple_manages_);

        os << "::_narrow (" << endl
           << "_ciao_ec.in ()" << endl
           << ");" << endl;

        os << "return this->_ciao_activate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << ");" << endl;

        os << "}";
      }

      virtual void
      comma (SemanticGraph::HomeFactory&)
      {
        os << "," << endl;
      }

    private:
      ReturnTypeNameEmitter return_type_name_emitter_;
      EnclosingTypeNameEmitter enclosing_type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      FullTypeNameEmitter type_name_emitter_;
      Traversal::Returns returns_;
      Traversal::Manages enclosing_manages_;
      Traversal::Manages simple_manages_;
      Traversal::Manages manages_;
      SemanticGraph::Home& scope_;
    };

    struct FinderOperationEmitter : Traversal::HomeFinder,
                                    Traversal::Home,
                                    EmitterBase
    {
      FinderOperationEmitter (Context& c, SemanticGraph::Home& home)
        : EmitterBase (c),
          type_name_emitter_ (c),
          return_type_name_emitter_ (c),
          enclosing_type_name_emitter_ (c),
          simple_type_name_emitter_ (c),
          scope_ (home)
      {
        returns_.node_traverser (return_type_name_emitter_);
        simple_returns_.node_traverser (type_name_emitter_);
        enclosing_manages_.node_traverser (enclosing_type_name_emitter_);
        simple_manages_.node_traverser (simple_type_name_emitter_);
      }

      virtual void
      returns (SemanticGraph::HomeFinder& hf)
      {
        Traversal::HomeFinder::returns (hf, returns_);

        os << endl;
      }

      virtual void
      name (SemanticGraph::HomeFinder& hf)
      {
        os << scope_.name () << "_Servant::" << hf.name ();
      }

      virtual void
      receives_none (SemanticGraph::HomeFinder&)
      {
        os << " (" << endl
           << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFinder&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFinder&)
      {
        os << endl << ")" << endl;
      }

      virtual void
      raises_none (SemanticGraph::HomeFinder&)
      {
        os << STRS[EXCP_SNGL];
      }

      virtual void
      raises_pre (SemanticGraph::HomeFinder&)
      {
        os << STRS[EXCP_START] << " "
           << STRS[EXCP_SYS] << "," << endl;
      }

      virtual void
      raises_post (SemanticGraph::HomeFinder&)
      {
        os << "))";
      }

      virtual void
      post (SemanticGraph::HomeFinder&)
      {
        os << "{"
           << "throw ::CORBA::NO_IMPLEMENT ();" << endl
           << "}";
      }

      virtual void
      comma (SemanticGraph::HomeFinder&)
      {
        os << "," << endl;
      }

    private:
      FullTypeNameEmitter type_name_emitter_;
      ReturnTypeNameEmitter return_type_name_emitter_;
      EnclosingTypeNameEmitter enclosing_type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      Traversal::Returns returns_;
      Traversal::Returns simple_returns_;
      Traversal::Manages enclosing_manages_;
      Traversal::Manages simple_manages_;
      SemanticGraph::Home& scope_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "const char *ins_name," << endl
         << "::CIAO::Session_Container *c)" << endl
         << "  : ::CIAO::Home_Servant_Impl_Base (c)," << endl
         << "    ::CIAO::";

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
         << "        > (exe, c, ins_name";

      if (swapping)
        {
          os << ", \"" << ctx.composition_name ();

          Traversal::Home::manages (t, flat_name_manages_);

          os << "\", ";

          Traversal::Home::manages (t, repo_id_manages_);
        }

      os << ")" << endl
         << "{"
         << "}";

      os << t.name () << "_Servant::~"
         << t.name () << "_Servant (void)" << endl
         << "{"
         << "}";

      os << "// Home operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        HomeOperationEmitter home_operation_emitter (ctx, t);
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

      os << "// Home supported interface operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);
        home_emitter.edge_traverser (home_inherits);

        Traversal::Supports supports_;
        home_emitter.edge_traverser (supports_);

        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits interface_inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (interface_inherits);

        AttributeSourceEmitter<SemanticGraph::Home> attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);

        OperationEmitter<SemanticGraph::Home> operation_emitter (ctx, t);
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

        home_emitter.traverse (t);
      }

      os << "// Home factory and finder operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        FactoryOperationEmitter factory_operation_emitter (ctx, t);
        FinderOperationEmitter finder_operation_emitter (ctx, t);
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

      // Generate operations for component attributes.
      os << "// Home attribute operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        AttributeSourceEmitter<SemanticGraph::Home> attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);

        home_emitter.traverse (t);
      }
    }

    virtual void
    post (Type& t)
    {
      // We need to escape C++ keywords before flattening the name.
      //
      string name;
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << t.scoped_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::PortableServer::Servant"
         << endl
         << "create" << name << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "::CIAO::Session_Container *c," << endl
         << "const char *ins_name" << endl
         << ")" << endl
         << "{"
         << "if (p == 0)" << endl
         << "{"
         << "return 0;" << endl
         << "}"
         << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_var x =" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "::_narrow (" << endl
         << "p" << endl
         << ");"
         << endl
         << "if (::CORBA::is_nil (x.in ()))" << endl
         << "{"
         << "return 0;" << endl
         << "}"
         << "return new" << endl;

      os << t.name () << "_Servant (" << endl
         << "x.in ()," << endl
         << "ins_name," << endl
         << "c);" << endl
         << "}";
    }

  private:
    CommandLine const& cl_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    RepoIdEmitter repo_id_emitter_;
    FlatNameEmitter flat_name_emitter_;
    Traversal::Manages simple_manages_;
    Traversal::Manages repo_id_manages_;
    Traversal::Manages flat_name_manages_;
  };
}

ServantSourceEmitter::ServantSourceEmitter (std::ostream& os_,
                                            CommandLine const& cl,
                                            string export_macro,
                                            fs::path const& file)
  : os (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    file_ (file)
{}

void
ServantSourceEmitter::generate (TranslationUnit& u)
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

  //--
  Traversal::ComponentExecutor component_executor;
  Traversal::HomeExecutor home_executor;
  composition_defines.node_traverser (component_executor);
  composition_defines.node_traverser (home_executor);

  module.edge_traverser (defines);

  // Layer 5
  //
  Traversal::Implements implements;
  component_executor.edge_traverser (implements);
  home_executor.edge_traverser (implements);

  //--
  ContextEmitter context_emitter (c);
  ServantEmitter servant_emitter (c);
  HomeEmitter home_emitter (c, cl_);
  implements.node_traverser (context_emitter);
  implements.node_traverser (servant_emitter);
  implements.node_traverser (home_emitter);

  unit.traverse (u);
}

void
ServantSourceEmitter::pre (TranslationUnit&)
{
  os << COPYRIGHT;

  string file_name ("");

  if (! file_.empty ())
    {
      file_name = file_.leaf ();
    }

  string file_suffix = cl_.get_value ("svnt-hdr-file-suffix",
                                      "_svnt.h");

  file_name = regex::perl_s (file_name,
                             "/^(.+?)(\\.(idl|cidl|cdl))?$/$1"
                             + file_suffix
                             + "/");

  string swap_option = cl_.get_value ("custom-container", "");
  bool swapping = (swap_option == "upgradeable");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"Cookies.h\"" << endl
     << "#include \"ciao/Servant_Activator.h\"" << endl
     << (swapping ? "#include \"ciao/Dynamic_Component_Activator.h\"\n" : "")
     << "#include \"ciao/Port_Activator_T.h\"" << endl
     << "#include \"tao/SystemException.h\"" << endl
     << "#include \"ace/SString.h\"" << endl << endl;
}

void
ServantSourceEmitter::generate_facets (TranslationUnit& u, Context& c)
{
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
  Traversal::Module module;
  Traversal::Composition composition;
  defines.node_traverser (module);
  defines.node_traverser (composition);

  // Layer 4
  //
  Traversal::Defines composition_defines;
  composition.edge_traverser (composition_defines);

  //--
  Traversal::ComponentExecutor component_executor;
  Traversal::HomeExecutor home_executor;
  composition_defines.node_traverser (component_executor);
  composition_defines.node_traverser (home_executor);

  module.edge_traverser (defines);

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
