// file      : CIDLC/ServantSourceGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantSourceGenerator.hpp"
#include "Literals.hpp"
#include "TypeNameEmitter.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"

#include <ostream>

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

using std::string;

namespace
{
  class Context
  {
  public:
    Context (std::ostream& os,
             string export_macro,
             CommandLine const& cl)
        : os_ (os),
          export_macro_ (export_macro),
          cl_ (cl)
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
  
  CommandLine const&
  cl ()
  {
    return cl_;
  }

  private:
    std::ostream& os_;
    string export_macro_;
    CommandLine const& cl_;
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
      os << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");";
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_ARG] << ");";
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
    OperationEmitter (Context& c, T& scope)
      : EmitterBase (c),
        scope_ (scope)
    {}

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
      os << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_SRC] << ")" << endl;
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
    post (Type& o)
    {
      os << "{";

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

  struct RepoIdEmitter : Traversal::Type
  {
    RepoIdEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << "\"" << t.context ().get<string> (STRS[REPO_ID]) << "\"";
    }

  private:
    std::ostream& os;
  };

  struct ConsumerRepoIdEmitter : Traversal::Type
  {
    ConsumerRepoIdEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      string holder (t.context ().get<string> (STRS[REPO_ID]));
      os << "\"" << holder.substr (0, holder.rfind (':'))
         << "Consumer:1.0\"";
    }

  private:
    std::ostream& os;
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

  // Generates operations associated with attributes.
  // @@@ (JP) Need to support exceptions.
  template <typename T>
  struct AttributeEmitter : Traversal::ReadWriteAttribute,
                            EmitterBase
  {
    AttributeEmitter (Context& c, T& scope)
      : EmitterBase (c),
        scope_ (scope),
        write_type_name_emitter_ (c.os ()),
        read_type_name_emitter_ (c.os ())
    {
      write_belongs_.node_traverser (write_type_name_emitter_);
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void name (SemanticGraph::ReadWriteAttribute& a)
    {
      os << scope_.name () << "_Servant::" << a.name ();
    }

    virtual void traverse (SemanticGraph::ReadWriteAttribute& a)
    {
      // Does nothing here, overridden for facet attributes.
      this->pre (a);

      Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

      os << endl;

      // Overridden for facet attributes.
      this->name (a);

      os << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return this->executor_->" << a.name () << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}";

      // Does nothing here, overridden for facet attributes.
      this->pre (a);

      os << "void" << endl;

      // Overridden for facet attributes.
      this->name (a);

       os << " (" << endl;

      Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

      os << " " << a.name () << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "this->executor_->" << a.name () << " (" << endl
         << a.name () << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}";
    }

  protected:
    T& scope_;

  private:
    INArgTypeNameEmitter write_type_name_emitter_;
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs write_belongs_;
    Traversal::Belongs read_belongs_;
  };

  // Generates operations associated with readonly attributes.
  // @@@ (JP) Need to support exceptions.
  template <typename T>
  struct ReadOnlyAttributeEmitter : Traversal::ReadAttribute,
                                    EmitterBase
  {
    ReadOnlyAttributeEmitter (Context& c, T& scope)
      : EmitterBase (c),
        scope_ (scope),
        read_type_name_emitter_ (c.os ())
    {
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void name (SemanticGraph::ReadAttribute& a)
    {
      os << scope_.name () << "_Servant::" << a.name ();
    }

    virtual void traverse (SemanticGraph::ReadAttribute& a)
    {
      // Does nothing here, overridden for facet attributes.
      this->pre (a);

      Traversal::ReadAttribute::belongs (a, read_belongs_);

      os << endl;

      // Overridden for facet attributes.
      this->name (a);

      os << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return this->executor_->" << a.name () << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}";
    }

  protected:
    T& scope_;

  private:
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs read_belongs_;
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

    struct FacetAttributeEmitter
      : AttributeEmitter<SemanticGraph::Interface>
    {
      FacetAttributeEmitter (Context& c, SemanticGraph::Interface& i)
        : AttributeEmitter<SemanticGraph::Interface> (c, i)
      {}

      virtual void
      pre (SemanticGraph::ReadWriteAttribute&)
      {
        os << "template <typename T>" << endl;
      }

      virtual void
      name (SemanticGraph::ReadWriteAttribute& a)
      {
        os << scope_.name () << "_Servant_T<T>::" << a.name ();
      }
    };

    struct FacetReadOnlyAttributeEmitter
      : ReadOnlyAttributeEmitter<SemanticGraph::Interface>
    {
      FacetReadOnlyAttributeEmitter (Context& c, SemanticGraph::Interface& i)
        : ReadOnlyAttributeEmitter<SemanticGraph::Interface> (c, i)
      {}

      virtual void
      pre (SemanticGraph::ReadAttribute&)
      {
        os << "template <typename T>" << endl;
      }

      virtual void
      name (SemanticGraph::ReadAttribute& a)
      {
        os << scope_.name () << "_Servant_T<T>::" << a.name ();
      }
    };

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      if (i.context ().count ("facet_src_gen")) return;

      // Open a namespace.
      os << STRS[GLUE_NS]
         << regex::perl_s (i.scoped_name ().scope_name ().str (),
                           "/::/_/")
         << "{";

      os << "template <typename T>" << endl
         << i.name () << "_Servant_T<T>::" << i.name ()
         << "_Servant_T (" << endl
         << i.scoped_name ().scope_name () << "::CCM_" << i.name ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr c)" << endl
         << ": executor_ (" << i.scoped_name ().scope_name () << "::CCM_"
         << i.name () << "::_duplicate (executor))," << endl
         << "ctx_ (::Components::CCMContext::_duplicate (c))" << endl
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
        FacetReadOnlyAttributeEmitter read_only_attribute_emitter (ctx, i);
        defines_.node_traverser (attribute_emitter);
        defines_.node_traverser (read_only_attribute_emitter);

        FacetOperationEmitter operation_emitter (ctx, i);
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

      os << "template <typename T>" << endl
         << "CORBA::Object_ptr" << endl
         << i.name () << "_Servant_T<T>::_get_component (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << "ACE_THROW_SPEC ((CORBA::SystemException))" << endl
         << "{"
         << STRS[COMP_SC] << "_var sc =" << endl
         << STRS[COMP_SC] << "::_narrow (" << endl
         << "this->ctx_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "if (! CORBA::is_nil (sc.in ()))" << endl
         << "{"
         << "return sc->get_CCM_object (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}"
         << "::Components::EntityContext_var ec =" << endl
         << "::Components::EntityContext::_narrow (" << endl
         << "this->ctx_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "if (! CORBA::is_nil (ec.in ()))" << endl
         << "{"
         << "return ec->get_CCM_object (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}"
         << "ACE_THROW_RETURN (CORBA::INTERNAL (), 0);" << endl
         << "}" << endl;

      // Close the CIAO_GLUE namespace.
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
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::get_connection_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_SRC_NOTUSED] << ")" << endl
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

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if (!CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_IC] << " ());" << endl
           << "}"
           << "this->ciao_uses_" << u.name () << "_ =" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_duplicate (c);" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_nil ());" << endl
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
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << u.scoped_name () << "Connections *tmp_retv;"
           << "ACE_NEW_RETURN (" << endl
           << "tmp_retv," << endl
           << u.scoped_name () << "Connections (this->ciao_uses_"
           << u.name () << "_.current_size ())," << endl
           << "0);" << endl << endl
           << u.scoped_name () << "Connections_var retv"
           << " = tmp_retv ;" << endl
           << "retv->length (this->ciao_uses_" << u.name ()
           << "_.current_size ());" << endl;

        os << "CORBA::ULong i = 0;" << endl;

        os << "for (ACE_Active_Map_Manager< " << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var>::iterator iter =" << endl
           << "       this->ciao_uses_" << u.name () << "_.begin ();"
           << "     iter != this->ciao_uses_" << u.name () << "_.end ();"
           << "     ++iter)" << endl
           << "{"
           << "ACE_Active_Map_Manager< " << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var>::ENTRY & entry = *iter;" << endl
           << "retv[i].objref = ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "entry.int_id_.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl;

        os << "ACE_NEW_RETURN (" << endl
           << "retv[i++].ck," << endl
           << "CIAO::Map_Key_Cookie (entry.ext_id_)," << endl
           << "0);" << endl
           << "}";

        os << "return retv._retn ();" << endl
           << "}";

        os << "::Components::Cookie *" << endl
           << scope_.name () << "_Context::connect_"
           << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var conn = ";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_duplicate (c);"
           << "ACE_Active_Map_Manager_Key key;" << endl;

        os << "if (this->ciao_uses_" << u.name ()
           << "_.bind (conn.in (), key) == -1)" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);" << endl
           << "}";

        os << "conn._retn ();" << endl;

        os << "Components::Cookie_var retv;"
           << "ACE_NEW_RETURN (" << endl
           << "retv.out ()," << endl
           << "CIAO::Map_Key_Cookie (key)," << endl
           << "0);" << endl;

        os << "return retv._retn ();" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << u.name () << " (" << endl
           << "::Components::Cookie * ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var retv;"
           << "ACE_Active_Map_Manager_Key key;" << endl;

        os << "if (ck == 0 || ! CIAO::Map_Key_Cookie::extract (ck, key))"
           << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_nil ());" << endl
           << "}";

        os << "if (this->ciao_uses_" << u.name ()
           << "_.unbind (key, retv) != 0)" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_nil ());" << endl
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
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "ACE_Active_Map_Manager< " << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator end =" << endl
           << "this->ciao_publishes_" << p.name ()
           << "_map_.end ();" << endl
           << "for (ACE_Active_Map_Manager< " << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator iter =" << endl
           << "this->ciao_publishes_" << p.name ()
           << "_map_.begin ();"
           << "iter != end;"
           << "++iter)" << endl
           << "{"
           << "ACE_Active_Map_Manager< " << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::ENTRY &entry = *iter;" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var c =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "entry.int_id_.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK;" << endl
           << "entry.int_id_->push_";

        Traversal::PublisherData::belongs (p, simple_belongs_);

        os << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK;" << endl
           << "}"
           << "}";

        os << STRS[COMP_CK] << " *" << endl
           << scope_.name () << "_Context::subscribe_"
           << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os<< "Consumer_var sub =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_duplicate (c);" << endl
           << "ACE_Active_Map_Manager_Key key;"
           << "this->ciao_publishes_" << p.name ()
           << "_map_.bind (sub.in (), key);"
           << "sub._retn ();" << endl
           << STRS[COMP_CK] << "_var retv = "
           << "new ::CIAO::Map_Key_Cookie (key);"
           << "return retv._retn ();" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Context::unsubscribe_"
           << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{";

        Traversal::PublisherData::belongs (p, belongs_);

        os<< "Consumer_var retv;"
           << "ACE_Active_Map_Manager_Key key;" << endl
           << "if (ck == 0 || ::CIAO::Map_Key_Cookie::extract (ck, key) == false)"
           << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}"
           << "if (this->ciao_publishes_" << p.name ()
           << "_map_.unbind (key, retv) != 0)" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}"
           << "return retv._retn ();" << endl
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
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "this->ciao_emits_" << e.name ()
           << "_consumer_->push_";

        Traversal::EmitterData::belongs (e, simple_belongs_);

        os << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        os << "void" << endl
           << scope_.name () << "_Context::connect_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW (CORBA::BAD_PARAM ());" << endl
           << "}" << endl
           << "if (! CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}" << endl
           << "this->ciao_emits_" << e.name ()
           << "_consumer_ = " << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer::_duplicate (c);" << endl
           << "}";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Context::disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}" << endl
           << "return this->ciao_emits_" << e.name ()
           << "_consumer_._retn ();" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      SemanticGraph::Component& scope_;
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

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

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

      os << t.name () << "_Context::"
         << t.name () << "_Context (" << endl
         << "::Components::CCMHome_ptr home," << endl
         << "::CIAO::Session_Container *c," << endl
         << t.name () << "_Servant *sv)" << endl
         << "  : Context_Impl_Base (home, c)," << endl
         << "    ctx_svnt_base (home, c, sv)" << endl
         << "{"
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

      os << "// CIAO-specific." << endl << endl;

      os << t.name () << "_Context *" << endl
         << t.name () << "_Context::_narrow (" << endl
         << STRS[COMP_SC] << "_ptr p" << endl
         << STRS[ENV_SRC_NOTUSED] << ")" << endl
         << "{"
         << "return dynamic_cast<" << t.name () << "_Context *> (p);"
         << endl
         << "}";
    }

    virtual void
    post (Type&)
    {
      // Namespace closer.
      os << "}";
    }
  };

  struct ServantEmitter : Traversal::Component, EmitterBase
  {
    ServantEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by ServantEmitter.
  private:
    struct NavigationEmitsEmitter : Traversal::EmitterData,
                                    EmitterBase
    {
      NavigationEmitsEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& t)
      {
        os << "if (ACE_OS::strcmp (emitter_name, \""
           << t.name () << "\") == 0)" << endl
           << "{";

        Traversal::EmitterData::belongs (t, belongs_);

        os << "Consumer_var _ciao_consumer =" << endl;

        Traversal::EmitterData::belongs (t, belongs_);

        os << "Consumer::_narrow (" << endl
           << "consumer" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK;" << endl
           << "if (::CORBA::is_nil (_ciao_consumer.in ()))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_IC] << " ());"
           << endl
           << "}"
           << "this->connect_" << t.name ()
           << " (" << endl
           << "_ciao_consumer.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "return;"
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
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
           << t.name () << "\") == 0)" << endl
           << "{"
           << "return this->executor_->get_" << t.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";
      }
    };

    struct PublishesEmitter : Traversal::PublisherData,
                              EmitterBase
    {
      PublishesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
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

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "return this->context_->subscribe_" << p.name ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name ()
           << "_Servant::unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "return this->context_->unsubscribe_"
           << p.name () << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct UsesConnectEmitter : Traversal::SingleUserData,
                                Traversal::MultiUserData,
                                EmitterBase
    {
      UsesConnectEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::SingleUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name () << "\") == 0)" << endl
           << "{";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_var _ciao_conn =" << endl;

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "connection" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "if (::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}"
           << "// Simplex connect." << endl
           << "this->connect_" << u.name () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "return 0;" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name () << "\") == 0)" << endl
           << "{";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_var _ciao_conn =" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "connection" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "if (::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}"
           << "// Multiplex connect." << endl
           << "return this->connect_" << u.name () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
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
           << u.name () << "\") == 0)" << endl
           << "{"
           << "// Simplex disconnect." << endl
           << "return this->disconnect_" << u.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name () << "\") == 0)" << endl
           << "{"
           << "// Multiplex disconnect." << endl
           << "return this->disconnect_" << u.name () << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";
      }
    };

    struct UsesEmitter : Traversal::SingleUserData,
                         Traversal::MultiUserData,
                         EmitterBase
    {
      UsesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
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

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "this->context_->connect_" << u.name () << " ("
           << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_" << u.name ()
           << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";

        Traversal::SingleUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name ()
           << "_Servant::get_connection_" << u.name ()
           << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->context_->get_connection_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";
      }

      virtual void
      traverse (SemanticGraph::MultiUser& u)
      {
        os << "::Components::Cookie *" << endl
           << scope_.name () << "_Servant::connect_"
           << u.name () << " (" << endl;

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "return this->context_->connect_" << u.name () << " ("
           << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        Traversal::MultiUserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << u.name () << " (" << endl
           << "::Components::Cookie * ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_" << u.name ()
           << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        os << u.scoped_name () << "Connections *" << endl
           << scope_.name ()
           << "_Servant::get_connections_" << u.name ()
           << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->context_->get_connections_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct PublishesSubscribeEmitter : Traversal::PublisherData,
                                       EmitterBase
    {
      PublishesSubscribeEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name () << "\") == 0)" << endl
           << "{";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var _ciao_consumer =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "subscribe" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "if (::CORBA::is_nil (_ciao_consumer.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}"
           << "return this->subscribe_" << p.name ()
           << " (" << endl
           << "_ciao_consumer.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
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
           << p.name () << "\") == 0)" << endl
           << "{"
           << "return this->unsubscribe_" << p.name ()
           << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";
      }
    };

    struct EmitsEmitter : Traversal::EmitterData,
                          EmitterBase
    {
      EmitsEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
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

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "this->context_->connect_" << e.name ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name () << "_Servant::disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      SemanticGraph::Component& scope_;
    };

    struct ServantTypeNameEmitter : Traversal::Type
    {
      ServantTypeNameEmitter (std::ostream& os_)
        : os (os_)
      {}

      virtual void
      traverse (SemanticGraph::Type& t)
      {
        os << "CIAO_GLUE";

        ScopedName scope (t.scoped_name ().scope_name ());

        if (!scope.simple ())
        {
          for (Name::Iterator i (scope.begin () + 1); i != scope.end (); ++i)
          {
            os << "_" << i->str ();
          }
        }

        os << "::" << t.name () << "_Servant";
      }

    private:
      std::ostream& os;
    };

    struct FacetProvidesEmitter : Traversal::ProviderData,
                                  EmitterBase
    {
      FacetProvidesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          servant_type_name_emitter_ (c.os ()),
          enclosing_type_name_emitter_ (c.os ()),
          repo_id_emitter_ (c.os ()),
          scope_ (scope)
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        servant_belongs_.node_traverser (servant_type_name_emitter_);
        enclosing_belongs_.node_traverser (enclosing_type_name_emitter_);
        repo_id_belongs_.node_traverser (repo_id_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        Traversal::ProviderData::belongs (p, belongs_);

        os << "_ptr" << endl
           << scope_.name ()
           << "_Servant::provide_" << p.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (! ::CORBA::is_nil (this->provide_"
           << p.name () << "_.in ()))" << endl
           << "{"
           << "return ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_duplicate (this->provide_" << p.name () << "_.in ());"
           << "}";

        /*
        Traversal::ProviderData::belongs (p, enclosing_belongs_);

        os << "::CCM_";

        Traversal::ProviderData::belongs (p, simple_belongs_);

        os << "_var fexe =" << endl
           << "this->executor_->get_" << p.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());"
           << endl
           << "if (::CORBA::is_nil (fexe.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl;

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());" << endl
           << "}";
        */
        os << "::CORBA::Object_var obj =" << endl
           << "this->provide_" << p.name () << "_i ("
           << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());" << endl;

        ScopedName scoped (scope_.scoped_name ());
        Name stripped (scoped.begin () + 1, scoped.end ());
        string unique_obj_name =
          regex::perl_s (stripped.str (), "/::/_/") + "_" + p.name ().str ();


        Traversal::ProviderData::belongs (p, belongs_);

        os << "_var fo =" << endl;

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());" << endl
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
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "::CORBA::Object_ptr ret =" << endl
           << "this->lookup_facet (\"" << p.name () << "\");" << endl;

        os << "if (! ::CORBA::is_nil (ret))" << endl
           << "{"
           << "return ret;"
           << "}";

        os << "CIAO::Port_Activator_T<" << endl
           << "    ";
        
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
           << "typedef CIAO::Port_Activator_T<" << endl
           << "    ";

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

        os << "ACE_NEW_THROW_EX ( " << endl
           << "tmp," << endl
           << "MACRO_MADNESS_TYPEDEF (" << endl
           << "\"" << unique_obj_name << "\"," << endl
           << "\"" << p.name () << "\"," << endl
           << "CIAO::Port_Activator::Facet," << endl
           << "0," << endl
           << "this->context_," << endl
           << "this)," << endl
           << "CORBA::NO_MEMORY ());" << endl;

        os << "CIAO::Servant_Activator *sa = " << endl
           << "this->container_->ports_servant_activator ();" << endl
           << "if (!sa->register_port_activator (tmp))" << endl
           << "{"
           << "return 0;" << endl
           << "}";

        os << "::CORBA::Object_var obj =" << endl
           << "this->container_->generate_reference (" << endl
           << "\"" << unique_obj_name << "\"," << endl;

        Traversal::ProviderData::belongs (p, repo_id_belongs_);

        os << "," << endl
           << "CIAO::Container::Facet_Consumer" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());" << endl
           << "this->add_facet (" << endl
           << "\"" << p.name () << "\"," << endl
           << "obj.in ());" << endl;

        os << "return obj._retn ();" << endl
           << "}";
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      ServantTypeNameEmitter servant_type_name_emitter_;
      EnclosingTypeNameEmitter enclosing_type_name_emitter_;
      RepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs servant_belongs_;
      Traversal::Belongs enclosing_belongs_;
      Traversal::Belongs repo_id_belongs_;
      SemanticGraph::Component& scope_;
    };

    struct ConsumesEmitter : Traversal::ConsumerData,
                             EmitterBase
    {
      ConsumesEmitter (Context& c, SemanticGraph::Component& scope)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          repo_id_emitter_ (c.os ()),
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
           << ": executor_ (" << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_" << c.scoped_name ().scope_name ().simple_name  ()
           << "::_duplicate (executor))," << endl
           << "ctx_ (" << c.scoped_name ().scope_name ().scope_name ()
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

        os << "CORBA::Object_ptr" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::_get_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->ctx_->get_CCM_object "
           << "(" << STRS[ENV_SNGL_ARG] << ");" << endl
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
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "this->executor_->push_" << c.name ()
           << " (" << endl
           << "evt" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}";

        os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "void" << endl
           << scope_.name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::push_event (" << endl
           << "::Components::EventBase *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_BET] << "))" << endl
           << "{";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "_var ev_type =" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "::_downcast (ev);" << endl
           << "if (ev_type != 0)" << endl
           << "{"
           << "this->push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << " (" << endl
           << "ev_type.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "return;" << endl
           << "}"
           << "ACE_THROW (" << STRS[EXCP_BET] << " ());" << endl
           << "}";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_ptr" << endl
           << scope_.name  () << "_Servant::"
           << "get_consumer_" << c.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
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
           << "this->get_consumer_" << c.name () << "_i (" << endl
           << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ());" << endl;

        ScopedName scoped (scope_.scoped_name ());
        Name stripped (scoped.begin () + 1, scoped.end ());
        string unique_obj_name =
          regex::perl_s (stripped.str (), "/::/_/") + "_" + c.name ().str ();

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_var eco =" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ());" << endl
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
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "::Components::EventConsumerBase_ptr ret =" << endl
           << "this->lookup_consumer (\"" << c.name () << "\");" << endl;

        os << "if (! ::CORBA::is_nil (ret))" << endl
           << "{"
           << "return ret;"
           << "}";

        os << "CIAO::Port_Activator_T<" << endl;

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
           << "_Context, " << endl
           << scope_.name () << "_Servant"
           << " > " << endl
           << " MACRO_MADNESS_TYPEDEF;"
           << endl << endl;

        os << "ACE_NEW_THROW_EX ( " << endl
           << "  tmp," << endl
           << "  MACRO_MADNESS_TYPEDEF (" << endl
           << "\"" << unique_obj_name << "\"," << endl
           << "\"" << c.name () << "\"," << endl
           << "CIAO::Port_Activator::Sink," << endl
           << "this->executor_.in ()," << endl
           << "this->context_," << endl
           << "this)," << endl
           << "CORBA::NO_MEMORY ());" << endl << endl;

        os << "CIAO::Servant_Activator *sa = " << endl
           << "this->container_->ports_servant_activator ();" <<endl
           << "if (!sa->register_port_activator (tmp))" << endl
           << "{"
           << "return 0;" << endl
           << "}";

        os << "::CORBA::Object_var obj =" << endl
           << "this->container_->generate_reference (" << endl
           << "\"" << unique_obj_name << "\"," << endl;

        Traversal::ConsumerData::belongs (c, repo_id_belongs_);

        os << "," << endl
           << "CIAO::Container::Facet_Consumer" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ());" << endl;

        os << "::Components::EventConsumerBase_var ecb =" << endl
           << "::Components::EventConsumerBase::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ());" << endl;

        os << "this->add_consumer (" << endl
           << "\"" << c.name () << "\"," << endl
           << "ecb.in ());" << endl;

        os << "return ecb._retn ();" << endl
           << "}";
     }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      ConsumerRepoIdEmitter repo_id_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs repo_id_belongs_;
      SemanticGraph::Component& scope_;
    };
    
    struct OperationExistsEmitter;
    
    struct RegisterValueFactoryEmitter : Traversal::ConsumerData,
                                         EmitterBase
    {
      RegisterValueFactoryEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          gen_factory_ (true)
      {
        belongs_.node_traverser (type_name_emitter_);
      }
      
      void factory_gen_off (void)
      {
        gen_factory_ = false;
      }

      virtual void
      traverse (Type& c)
      {
        {
          Traversal::ConsumerData consumer;
          
          Traversal::Belongs consumer_belongs;
          consumer.edge_traverser (consumer_belongs);
          
          Traversal::EventType event_type;
          consumer_belongs.node_traverser (event_type);
         
          Traversal::Defines defines;
          event_type.edge_traverser (defines);
        
          OperationExistsEmitter op_emitter (this);
          defines.node_traverser (op_emitter);
        
          consumer.traverse (c);
        }
      
        if (gen_factory_)
        {
          os << "CIAO_REGISTER_OBV_FACTORY (" << endl;

          Traversal::ConsumerData::belongs (c, belongs_);

          os << "_init," << endl;

          Traversal::ConsumerData::belongs (c, belongs_);

          os << ");" << endl;
        }
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
      bool gen_factory_;
    };

    struct OperationExistsEmitter : Traversal::Operation,
                                    Traversal::EventTypeFactory
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
      traverse (SemanticGraph::EventTypeFactory&)
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
           << "this->provide_" << p.name () << "_i (" << endl
           << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK;" << endl;
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
           << "this->get_consumer_" << p.name () << "_i (" << endl
           << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK;" << endl;
      }
    };

    struct SetAttributesEmitter : Traversal::ReadWriteAttribute,
                                  EmitterBase
    {
      SetAttributesEmitter (Context& c)
        : EmitterBase (c),
          extract_emitter_ (c.os ()),
          assign_emitter_ (c.os ())
      {
        extract_belongs_.node_traverser (extract_emitter_);
        assign_belongs_.node_traverser (assign_emitter_);
      }

      virtual void
      pre (SemanticGraph::ReadWriteAttribute& a)
      {
        os << "if (ACE_OS::strcmp (descr_name, \""
           << a.name () << "\") == 0)" << endl
           << "{";

        Traversal::ReadWriteAttribute::belongs (a, extract_belongs_);

        os << "descr_value >>= " << STRS[EXTRACT] << ";"
           << "this->" << a.name () << " (";

        Traversal::ReadWriteAttribute::belongs (a, assign_belongs_);

        os << ");"
           << "continue;"
           << "}";
      }

    private:
      ExtractedTypeDeclEmitter extract_emitter_;
      AssignFromExtractedEmitter assign_emitter_;
      Traversal::Belongs extract_belongs_;
      Traversal::Belongs assign_belongs_;
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

      // Servant Constructor
      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::Components::CCMHome_ptr h," << endl
         << "::CIAO::Session_Container *c)" << endl
         << "  : Servant_Impl_Base (c)," << endl
         << "    comp_svnt_base (exe, c)" << endl
         << "{"
         << "this->context_ = "
         << "new " << t.name () << "_Context (h, c, this);" << endl;

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

      os << "ACE_TRY_NEW_ENV" << endl
         << "{"
         << "::Components::SessionComponent_var scom =" << endl
         << "::Components::SessionComponent::_narrow (" << endl
         << "exe" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_TRY_CHECK;" << endl
         << "if (! ::CORBA::is_nil (scom.in ()))" << endl
         << "{"
         << "scom->set_session_context (" << endl
         << "this->context_" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_TRY_CHECK;"
         << "}"
         << "this->populate_port_tables (" << endl
         << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_TRY_CHECK;"
         << "}"
         << "ACE_CATCHANY" << endl
         << "{"
         << "}"
         << "ACE_ENDTRY;" << endl
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
         << STRS[ENV_SRC_NOTUSED] << ")" << endl
         << "{"
         << "for (CORBA::ULong i = 0; i < descr.length (); ++i)" << endl
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


      os << "ACE_UNUSED_ARG (descr_name);"
         << "ACE_UNUSED_ARG (descr_value);" << endl;

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
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{"
         << "// If the component has no receptacles, this will be unused."
         << endl
         << "ACE_UNUSED_ARG (connection);" << endl
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
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

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}";

      os << "CORBA::Object_ptr" << endl
         << t.name () << "_Servant::disconnect (" << endl
         << "const char *name," << endl
         << STRS[COMP_CK] << " * ck" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_CR] << "," << endl
         << STRS[EXCP_NC] << "))" << endl
         << "{"
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
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

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "ACE_UNUSED_ARG (ck);" << endl
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
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{"
         << "if (emitter_name == 0)" << endl
         << "{"
         << "ACE_THROW (::CORBA::BAD_PARAM ());" << endl
         << "}";

      // Generate an IF block for each 'emits' declaration.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (inherits);

        NavigationEmitsEmitter navigation_emits_emitter (ctx);
        defines.node_traverser (navigation_emits_emitter);

        component_emitter.traverse (t);
      }

      os << "ACE_UNUSED_ARG (consumer);"
         << "ACE_THROW ("
         << STRS[EXCP_IN] << " ());" << endl
         << "}";

      os << STRS[COMP_CK] << " *" << endl
         << t.name () << "_Servant::subscribe (" << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_ECB] << "_ptr subscribe" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{"
         << "// Just in case there are no if blocks" << endl
         << "ACE_UNUSED_ARG (subscribe);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);"
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

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}";

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::unsubscribe ("
         << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{"
         << "// Just in case there are no if blocks" << endl
         << "ACE_UNUSED_ARG (ck);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
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

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
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

      os << "CORBA::Object_ptr" << endl
         << t.name ()
         << "_Servant::get_facet_executor (const char *name" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "))" << endl
         << "{"
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::BAD_PARAM ()," << endl
         << "::CORBA::Object::_nil ());" << endl
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

      os << " return CORBA::Object::_nil ();"
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

        AttributeEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<
            SemanticGraph::Component
          > read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter<SemanticGraph::Component> operation_emitter (ctx, t);
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

      // Generate operations for component attributes.
      os << "// Component attribute operations." << endl << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits inherits;
        component_emitter.edge_traverser (inherits);
        inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);

        AttributeEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<SemanticGraph::Component> read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        component_emitter.traverse (t);
      }

      os << "// Private method to populate the port tables."
         << endl;

      os << "void" << endl
         << t.name () << "_Servant::populate_port_tables (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
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

    virtual void
    post (Type&)
    {
      // Namespace closer.
      os << "}";
    }
  };

  struct HomeEmitter : Traversal::Home, EmitterBase
  {
    HomeEmitter (Context& c)
      : EmitterBase (c)
    {}

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
          return_type_name_emitter_ (c.os ()),
          enclosing_type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          scope_ (home)
      {
        returns_.node_traverser (return_type_name_emitter_);
        enclosing_manages_.node_traverser (enclosing_type_name_emitter_);
        simple_manages_.node_traverser (simple_type_name_emitter_);
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
           << STRS[ENV_SNGL_SRC] << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFactory&)
      {
        os << endl << STRS[ENV_SRC] << ")" << endl;
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

        os << "ACE_CHECK_RETURN (";

        Traversal::HomeFactory::returns (hf, returns_);

        os << "::_nil ());" << endl;

        Traversal::Home::manages (scope_, enclosing_manages_);

        os << "::CCM_";

        Traversal::Home::manages (scope_, simple_manages_);

        os << "_var _ciao_comp =" << endl;

        Traversal::Home::manages (scope_, enclosing_manages_);

        os << "::CCM_";

        Traversal::Home::manages (scope_, simple_manages_);

        os << "::_narrow (" << endl
           << "_ciao_ec.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl;

        os << "ACE_CHECK_RETURN (";

        Traversal::HomeFactory::returns (hf, returns_);

        os << "::_nil ());" << endl;

        os << "return this->_ciao_activate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl;

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
      Traversal::Returns returns_;
      Traversal::Manages enclosing_manages_;
      Traversal::Manages simple_manages_;
      SemanticGraph::Home& scope_;
    };

    struct FinderOperationEmitter : Traversal::HomeFinder,
                                    Traversal::Home,
                                    EmitterBase
    {
      FinderOperationEmitter (Context& c, SemanticGraph::Home& home)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          return_type_name_emitter_ (c.os ()),
          enclosing_type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
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
           << STRS[ENV_SNGL_SRC] << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFinder&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFinder&)
      {
        os << endl << STRS[ENV_SRC] << ")" << endl;
      }

      virtual void
      raises_none (SemanticGraph::HomeFinder&)
      {
        os << STRS[EXCP_SNGL];
      }

      virtual void
      raises_pre (SemanticGraph::HomeFinder&)
      {
        os << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl;
      }

      virtual void
      raises_post (SemanticGraph::HomeFinder&)
      {
        os << "))";
      }

      virtual void
      post (SemanticGraph::HomeFinder& hf)
      {
        os << "{"
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::NO_IMPLEMENT ()," << endl;

        Traversal::HomeFinder::returns (hf, simple_returns_);

        os << "::_nil ());"
           << "}";
      }

      virtual void
      comma (SemanticGraph::HomeFinder&)
      {
        os << "," << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
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
      os << STRS[GLUE_NS]
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "{";

      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::CIAO::Session_Container *c)" << endl
         << "  : CIAO::Home_Servant_Impl_Base (c)," << endl
         << "    " << "home_svnt_base (exe, c)" << endl
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

        AttributeEmitter<SemanticGraph::Home> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<
            SemanticGraph::Home
          > read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter<SemanticGraph::Home> operation_emitter (ctx, t);
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

      // Generate operations for component attributes.
      os << "// Home attribute operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        AttributeEmitter<SemanticGraph::Home> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<SemanticGraph::Home> read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        home_emitter.traverse (t);
      }
    }

    virtual void
    post (Type& t)
    {
      // Namespace closer.
      os << "}";

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::PortableServer::Servant"
         << endl
         << "create" << t.name () << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "CIAO::Session_Container *c" << endl
         << STRS[ENV_SRC] << ")" << endl
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
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "if (::CORBA::is_nil (x.in ()))" << endl
         << "{"
         << "return 0;" << endl
         << "}"
         << "return new" << endl;

      os << "CIAO_GLUE"
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "::" << t.name () << "_Servant (" << endl
         << "x.in ()," << endl
         << "c);" << endl
         << "}";
    }
  };

  //@@ There is exactly the same code in header generator.
  //
  struct CompositionEmitter : Traversal::Composition, EmitterBase
  {
    CompositionEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << "namespace " << t.name () << "{";
    }

    virtual void
    post (Type&)
    {
      os << "}";
    }
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
                             "/(\\.(idl|cidl))?$/"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"Cookies.h\"" << endl
     << "#include \"ciao/Servant_Activator.h\"" << endl
     << "#include \"ciao/Port_Activator_T.h\"" << endl << endl;
}

void
ServantSourceEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_, cl_);

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
  includes.node_traverser (region);
  contains_root.node_traverser (root);

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
  HomeEmitter home_emitter (c);
  implements.node_traverser (context_emitter);
  implements.node_traverser (servant_emitter);
  implements.node_traverser (home_emitter);

  unit.traverse (u);
}

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
