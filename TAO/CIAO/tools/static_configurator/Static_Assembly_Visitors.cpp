// $Id$

#include "Static_Assembly.h"
#include "Static_Assembly_Visitors.h"
#include "../XML_Helpers/XML_Utils.h"
#include "ace/Configuration_Import_Export.h"

const char* comp_reg_type_to_str (
              CIAO::Assembly_Placement::componentinstantiation::IF_Register_Type type)
{
  switch (type)
    {
    case CIAO::Assembly_Placement::componentinstantiation::COMPONENT:
      return "CIAO::Assembly_Placement::componentinstantiation::COMPONENT";
    case CIAO::Assembly_Placement::componentinstantiation::PROVIDESID:
      return "CIAO::Assembly_Placement::componentinstantiation::PROVIDESID";
    case CIAO::Assembly_Placement::componentinstantiation::CONSUMESID:
      return "CIAO::Assembly_Placement::componentinstantiation::CONSUMESID";
    }
  return "";
}

const char* comp_reg_method_to_str (
               CIAO::Assembly_Placement::componentinstantiation::Register_Method method)
{
  switch (method)
    {
    case CIAO::Assembly_Placement::componentinstantiation::NAMINGSERVICE:
      return "CIAO::Assembly_Placement::componentinstantiation::NAMINGSERVICE";
    case CIAO::Assembly_Placement::componentinstantiation::IORFILE:
      return "CIAO::Assembly_Placement::componentinstantiation::IORFILE";
    }
return "";
}

const char*
connection_type_to_str (CIAO::Assembly_Connection::Connection_Type type)
{
  switch (type)
    {
    case  CIAO::Assembly_Connection::INTERFACE:
      return "CIAO::Assembly_Connection::INTERFACE";
    case  CIAO::Assembly_Connection::EMITTER_CONSUMER:
      return "CIAO::Assembly_Connection::EMITTER_CONSUMER";
    case  CIAO::Assembly_Connection::PUBLISHER_CONSUMER:
      return "CIAO::Assembly_Connection::PUBLISHER_CONSUMER";
    case  CIAO::Assembly_Connection::HOME:
      return "CIAO::Assembly_Connection::HOME";
    case  CIAO::Assembly_Connection::INVALID_CONN:
      return "CIAO::Assembly_Connection::INVALID_CONN";
    }
  return "";
}

const char*
resolution_method_to_str (CIAO::Assembly_Connection::IF_Resolution_Method method)
{
  switch (method)
    {
    case CIAO::Assembly_Connection::PROVIDER:
      return "CIAO::Assembly_Connection::PROVIDER";
    case CIAO::Assembly_Connection::CONSUMER:
      return "CIAO::Assembly_Connection::CONSUMER";
    case CIAO::Assembly_Connection::COMP_IDREF:
      return "CIAO::Assembly_Connection::COMP_IDREF";
    case CIAO::Assembly_Connection::HOME_IDREF:
      return "CIAO::Assembly_Connection::HOME_IDREF";
    case CIAO::Assembly_Connection::NAMINGSERVICE:
      return "CIAO::Assembly_Connection::NAMINGSERVICE";
    case CIAO::Assembly_Connection::STRINGIFIEDOBJECTREF:
      return "CIAO::Assembly_Connection::STRINGIFIEDOBJECTREF";
    case CIAO::Assembly_Connection::HOMEFINDER:
      return "CIAO::Assembly_Connection::HOMEFINDER";
    case CIAO::Assembly_Connection::TRADERQUERY:
      return "CIAO::Assembly_Connection::TRADEQUERY";
    }
  return "";
}

CIAO::Static_Assembly_Builder_Visitor::Static_Assembly_Builder_Visitor (
                                   CIAO::ID_IMPL_MAP &idmap,
                                   const char* installation_file,
                                   CIAO::Static_Config::Static_Config_Info& info)
  : impl_idref_map_ (idmap),
    static_config_info_ (info),
    containers_table_ (info.containers_table_),
    homes_table_ (info.homes_table_),
    components_table_ (info.components_table_),
    component_registrations_table_ (info.component_registrations_table_),
    connections_table_ (info.connections_table_),
    resolvers_table_ (info.resolvers_table_),
    containers_table_last_index_ (info.containers_table_last_index_),
    homes_table_last_index_ (info.homes_table_last_index_),
    components_table_last_index_ (info.components_table_last_index_),
    component_registrations_table_last_index_ (info.component_registrations_table_last_index_),
    connections_table_last_index_ (info.connections_table_last_index_),
    resolvers_table_last_index_ (info.resolvers_table_last_index_),
    installation_file_ (installation_file)
{
  ACE_Configuration_Heap *tmp = 0;
  tmp = new ACE_Configuration_Heap ();

  auto_ptr<ACE_Configuration_Heap> config (tmp);

  if (config->open () != 0)
    {
      ACE_DEBUG ((LM_ERROR, "Unable to initilize installation datafile\n"));
    }

  ACE_Ini_ImpExp import (*config);

  if (import.import_config (installation_file_.c_str ()) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error encountered in Static Assembly reading installation file %s\n",
                  installation_file_.c_str ()));
    }
  this->installation_ = config.release ();
}

CIAO::Static_Assembly_Builder_Visitor::~Static_Assembly_Builder_Visitor (void)
{
  if (this->installation_ != 0)
    {
      delete this->installation_;
    }
}

int
CIAO::Static_Assembly_Builder_Visitor::visit_Container
(CIAO::Assembly_Placement::Container *c
 ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "partitioning %s\n", c->id ()));

  // This can only happen when we hit a partitioning (root) node.
  CIAO::Assembly_Placement::Container::ITERATOR iter (*c);
  CIAO::Assembly_Placement::Node *node = 0;

  while (iter.next (node))
    {
      int retv = node->accept (*this
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (retv != 0)
        return -1;
      iter.advance ();
    }
  return 0;
}

int
CIAO::Static_Assembly_Builder_Visitor::visit_hostcollocation
(CIAO::Assembly_Placement::hostcollocation *hc
 ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "hostcollocation %s\n", hc->id ()));

  CIAO::Assembly_Placement::Container::ITERATOR iter (*hc);
  CIAO::Assembly_Placement::Node *node = 0;

  while (iter.next (node))
    {
      int retv = node->accept (*this
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (retv != 0)
        return -1;
      iter.advance ();
    }
  return 0;
}

int
CIAO::Static_Assembly_Builder_Visitor::visit_processcollocation
(CIAO::Assembly_Placement::processcollocation *pc
 ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "processcollocation %s\n", pc->id ()));
  Components::ConfigValues server_config;

  // Destination logical host id.
  ACE_CString destination_host;

  if (pc->destination () != 0)
    {
      ACE_CString desti_string (pc->destination ());

      ACE_DEBUG ((LM_DEBUG, "desti_string is %s\n", desti_string.c_str()));
      ssize_t endpos = desti_string.find ('|');

      destination_host =
        desti_string.substring (0, endpos);

      if (endpos != ACE_CString::npos)
        {
          ACE_CString svcconf = desti_string.substring (endpos + 1);
          ACE_DEBUG ((LM_DEBUG, "svcconf is %s\n", svcconf.c_str()));
        }
    }

  // @@ check for RTCAD file, parse the file, and insert the thing in server_config.

  const char *rtcad = pc->rtcad_filename ();

  if (rtcad != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Using RTCAD file: %s\n",
                  rtcad));
    }

  // Now deal with the children nodes.
  CIAO::Assembly_Placement::Container::ITERATOR iter (*pc);
  CIAO::Assembly_Placement::Node *node = 0;

  while (iter.next (node))
    {
      int retv = node->accept (*this
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (retv != 0)
        return -1;
      iter.advance ();
    }
  return 0;
}

int
CIAO::Static_Assembly_Builder_Visitor::visit_homeplacement
(CIAO::Assembly_Placement::homeplacement *hp
 ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "#################################\n"));
  ACE_DEBUG ((LM_DEBUG, "homeplacement %s\n", hp->id ()));

  // @@ Create and register home before creating components.
  ACE_CString csd_file;

  if (this->impl_idref_map_.find (hp->componentfileref (),
                                  csd_file) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find implementation reference: %s",
                       hp->componentfileref ()),
                      -1);

  CIAO::Softpkg_Handler::Softpkg_Info info;
  info.csd_path_ = csd_file.c_str ();

  if (CIAO::XML_Utils::parse_softpkg (&info) == 0)
    {
      //info.dump ();             // For debug purpose.

      ACE_CString rtpolicy = "";
      if (hp->rtpolicyset_ref ())
        rtpolicy = hp->rtpolicyset_ref ();

      int containers_table_index = find_container(rtpolicy);
      if (containers_table_index == -1)
        {
          containers_table_index = ++containers_table_last_index_;
          containers_table_[containers_table_last_index_].rtpolicyset_ref_ =
            rtpolicy;
        }

      ++homes_table_last_index_;
      homes_table_[homes_table_last_index_].id_ = hp->id ();
      homes_table_[homes_table_last_index_].executor_UUID_ = info.executor_UUID_;
      homes_table_[homes_table_last_index_].executor_entrypt_ = info.executor_entrypt_;
      homes_table_[homes_table_last_index_].servant_UUID_ = info.servant_UUID_;
      homes_table_[homes_table_last_index_].servant_entrypt_ = info.servant_entrypt_;
      homes_table_[homes_table_last_index_].containers_table_index_ = containers_table_index;
    }

  CIAO::Assembly_Placement::Container::ITERATOR iter (*hp);
  CIAO::Assembly_Placement::Node *node = 0;

  while (iter.next (node))
    {
      int retv = node->accept (*this
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (retv != 0)
        return -1;
      iter.advance ();
    }

  return 0;
}

int
CIAO::Static_Assembly_Builder_Visitor::visit_componentinstantiation
(CIAO::Assembly_Placement::componentinstantiation *ci
 ACE_ENV_ARG_DECL_NOT_USED)
{
  // @@ instantiation and register component.
  ACE_DEBUG ((LM_DEBUG, "ComponentInstantiation %s\n", ci->id ()));

  // Registering component.
  CIAO::Assembly_Placement::componentinstantiation::REGISTRATION_QUEUE::ITERATOR
    iter (ci->register_info_);

  ++components_table_last_index_;
  components_table_[components_table_last_index_].id_ = ci->id ();
  components_table_[components_table_last_index_].homes_table_index_ =
    homes_table_last_index_;

  if (ci->register_info_.size () > 0)
    {
      components_table_[components_table_last_index_].
        component_registration_begin_index_ =
        component_registrations_table_last_index_ + 1;

      components_table_[components_table_last_index_].
        component_registration_end_index_ =
        component_registrations_table_last_index_ + ci->register_info_.size ();
    }
  else
    {
      components_table_[components_table_last_index_].
        component_registration_begin_index_ = -1;
      components_table_[components_table_last_index_].
        component_registration_end_index_ = -1;
    }

  while (!iter.done ())
    {
      CIAO::Assembly_Placement::componentinstantiation::Register_Info *info;
      iter.next (info);

      ++component_registrations_table_last_index_;
      component_registrations_table_[component_registrations_table_last_index_].type_ = info->type_;
      component_registrations_table_[component_registrations_table_last_index_].method_ = info->method_;
      component_registrations_table_[component_registrations_table_last_index_].port_id_ = info->port_id_;
      component_registrations_table_[component_registrations_table_last_index_].name_ = info->name_;
      /*
      ACE_DEBUG ((LM_DEBUG, "register type = %d\n", info->type_));
      ACE_DEBUG ((LM_DEBUG, "register method = %d\n", info->method_));
      ACE_DEBUG ((LM_DEBUG, "portid = %s\n", info->port_id_.c_str()));
      ACE_DEBUG ((LM_DEBUG, "name = %s\n", info->name_.c_str()));
      */
      iter.advance ();
    }
  return 0;
}

int CIAO::Static_Assembly_Builder_Visitor::build_resolver_info (CIAO::Assembly_Connection::IF_Resolver_Info* info)
{
  if (info == 0)
    return -1;

  int rc = ++resolvers_table_last_index_;
  int curr_resolvers_table_index = resolvers_table_last_index_;
  resolvers_table_[curr_resolvers_table_index].resolver_type_ =
    info->resolver_type ();
  resolvers_table_[curr_resolvers_table_index].resolver_info_ =
    info->resolver_info ();

  if (info->nested_resolver () != 0)
    {
      resolvers_table_[curr_resolvers_table_index].nested_resolver_index_ =
        build_resolver_info (info->nested_resolver ());
    }
  else
    {
      resolvers_table_[curr_resolvers_table_index].nested_resolver_index_ = -1;
    }

  return rc;
}

void CIAO::Static_Assembly_Builder_Visitor::build_connections (CIAO::Assembly_Spec::CONNECTION_QUEUE& connections)
{
  CIAO::Assembly_Spec::CONNECTION_QUEUE::ITERATOR
    conn_iter (connections);

  while (!conn_iter.done ())
    {
      CIAO::Assembly_Connection::Connect_Info *connection;
      conn_iter.next (connection);

      ++connections_table_last_index_;
      connections_table_[connections_table_last_index_].type_ = connection->type_;
      connections_table_[connections_table_last_index_].id_ = connection->id_;
      connections_table_[connections_table_last_index_].name_ = connection->name_;

      if (connection->component_ != 0)
        {
          connections_table_[connections_table_last_index_].component_index_ =
            build_resolver_info (connection->component_);
        }
      else
        {
          connections_table_[connections_table_last_index_].component_index_ = -1;
        }

      if (connection->interface_ != 0)
        {
          connections_table_[connections_table_last_index_].interface_index_ =
            build_resolver_info (connection->interface_);
        }
      else
        {
          connections_table_[connections_table_last_index_].interface_index_ = -1;
        }

      conn_iter.advance ();
    }
}

int CIAO::Static_Assembly_Builder_Visitor::find_container (const ACE_CString& rtpolicy_name)
{
  for (int i=0; i <= containers_table_last_index_; ++i)
    {
      if (containers_table_[i].rtpolicyset_ref_ == rtpolicy_name)
        return i;
    }
  return -1;
}

void CIAO::Static_Assembly_Builder_Visitor::dump_static_config_info ()
{
  int i=0;

  ACE_OS::printf("Containers List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= containers_table_last_index_; ++i)
    {
      ACE_OS::printf("%s\n",
                     containers_table_[i].rtpolicyset_ref_.c_str());
    }

  ACE_OS::printf("Homes List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= homes_table_last_index_; ++i)
    {
      ACE_OS::printf("---------------------\n");
      ACE_OS::printf("%s //id\n",
                     homes_table_[i].id_.c_str());
      ACE_OS::printf("%s //executor_UUID_\n",
                     homes_table_[i].executor_UUID_.c_str());
      ACE_OS::printf("%s //executor_entrypt_\n",
                     homes_table_[i].executor_entrypt_.c_str());
      ACE_OS::printf("%s //servant_UUID_\n",
                     homes_table_[i].servant_UUID_.c_str());
      ACE_OS::printf("%s //servant_entrypt_\n",
                     homes_table_[i].servant_entrypt_.c_str());
      ACE_OS::printf("%d //containers_table_index_\n",
                     homes_table_[i].containers_table_index_);
      ACE_OS::printf("---------------------\n");
    }

  ACE_OS::printf("Components List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= components_table_last_index_; ++i)
    {
      ACE_OS::printf("---------------------\n");
      ACE_OS::printf("%s //id_\n",
                     components_table_[i].id_.c_str());
      ACE_OS::printf("%d //component_registration_begin_index_\n",
                     components_table_[i].component_registration_begin_index_);
      ACE_OS::printf("%d //component_registration_end_index_\n",
                     components_table_[i].component_registration_end_index_);
      ACE_OS::printf("---------------------\n");
    }

  ACE_OS::printf("Component registrations List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= component_registrations_table_last_index_; ++i)
    {
      ACE_OS::printf("---------------------\n");
      ACE_OS::printf("%d //type_\n",
                     component_registrations_table_[i].type_);
      ACE_OS::printf("%d //method_\n",
                     component_registrations_table_[i].method_);
      ACE_OS::printf("%s //port_id_\n",
                     component_registrations_table_[i].port_id_.c_str());
      ACE_OS::printf("%s //name_\n",
                     component_registrations_table_[i].name_.c_str());
      ACE_OS::printf("---------------------\n");
    }

  ACE_OS::printf("Connections List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= connections_table_last_index_; ++i)
    {
      //ACE_OS::printf("---------------------\n");
      ACE_OS::printf("%d\t", //type_\n
                     connections_table_[i].type_);
      ACE_OS::printf("%s\t",  //id_\n
                     connections_table_[i].id_.c_str ());
      ACE_OS::printf("%s\t\t",  //name\n
                     connections_table_[i].name_.c_str());
      ACE_OS::printf("%d\t",  //component_index_\n
                     connections_table_[i].component_index_);
      ACE_OS::printf("%d\t\n",  //interface_index_\n
                     connections_table_[i].interface_index_);
      //ACE_OS::printf("---------------------\n");
    }

  ACE_OS::printf("Resolvers List\n");
  ACE_OS::printf("===============\n");
  for (i=0;i <= resolvers_table_last_index_; ++i)
    {
      //ACE_OS::printf("---------------------\n");
      ACE_OS::printf("%d\t", //type_\n
                     resolvers_table_[i].resolver_type_);
      ACE_OS::printf("%s\t\t", //resolver_info_\n
                     resolvers_table_[i].resolver_info_.c_str ());
      ACE_OS::printf("%d\t\n", //nested_resolver_index\n
                     resolvers_table_[i].nested_resolver_index_);
      //ACE_OS::printf("---------------------\n");
    }

}

void CIAO::Static_Assembly_Builder_Visitor::
generate_static_header_file (const char* header_file_name)
{
  FILE* header_file =
    ACE_OS::fopen (header_file_name, "w");

  int i=0;

  ACE_OS::fprintf (header_file, "#include \"Static_Assembly.h\"\n\n");
  ACE_OS::fprintf (header_file, "#include \"CCM_ContainerC.h\"\n");
  ACE_OS::fprintf (header_file, "#include \"Container_Base.h\"\n\n");

  ACE_OS::fprintf (header_file, "//Containers\n");

  ACE_OS::fprintf (header_file, "CIAO::Static_Config::ContainerAttributes containers_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= containers_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{\"%s\", 0}",
                     containers_table_[i].rtpolicyset_ref_.c_str());
      if (i != containers_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }
  ACE_OS::fprintf (header_file, "\n};\n");

  ACE_OS::fprintf (header_file, "\n\n");

  for (i=0;i <= homes_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file,
                       "extern \"C\" ::Components::HomeExecutorBase_ptr %s (void);\n",
                       homes_table_[i].executor_entrypt_.c_str());
      ACE_OS::fprintf (header_file,
                       "extern \"C\" ::PortableServer::Servant %s \n",
                       homes_table_[i].servant_entrypt_.c_str());
      ACE_OS::fprintf (header_file,
                       "\t\t(::Components::HomeExecutorBase_ptr p,\n");
      ACE_OS::fprintf (header_file,
                       "\t\t::CIAO::Session_Container *c\n");
      ACE_OS::fprintf (header_file,
                       "\t\tACE_ENV_ARG_DECL_WITH_DEFAULTS);\n");
    }

  ACE_OS::fprintf (header_file, "//Homes\n");
  ACE_OS::fprintf (header_file, "CIAO::Static_Config::HomeAttributes homes_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= homes_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{\"%s\", \"%s\", \"%s\", %s, \"%s\", \"%s\", %s, %d, 0 }",                      homes_table_[i].id_.c_str(),
                      homes_table_[i].executor_UUID_.c_str(),
                      homes_table_[i].executor_entrypt_.c_str(),
                      homes_table_[i].executor_entrypt_.c_str(),
                      homes_table_[i].servant_UUID_.c_str(),
                      homes_table_[i].servant_entrypt_.c_str(),
                      homes_table_[i].servant_entrypt_.c_str(),
                      homes_table_[i].containers_table_index_ );

      if (i != homes_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }

  ACE_OS::fprintf (header_file, "\n};\n");
  ACE_OS::fprintf (header_file, "\n\n");

  ACE_OS::fprintf (header_file, "//Components\n");
  ACE_OS::fprintf (header_file, "CIAO::Static_Config::ComponentAttributes components_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= components_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{\"%s\", %d, %d, %d }",
                      components_table_[i].id_.c_str(),
                      components_table_[i].component_registration_begin_index_,
                      components_table_[i].component_registration_end_index_,
                      components_table_[i].homes_table_index_ );

      if (i != components_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }
  ACE_OS::fprintf (header_file, "\n};\n");
  ACE_OS::fprintf (header_file, "\n\n");

  ACE_OS::fprintf (header_file, "//Component Registrations\n");
  ACE_OS::fprintf (header_file, "CIAO::Assembly_Placement::componentinstantiation::Register_Info component_registrations_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= component_registrations_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{%s, %s, \"%s\", \"%s\" }",
                      comp_reg_type_to_str (component_registrations_table_[i].type_),
                      comp_reg_method_to_str (component_registrations_table_[i].method_),
                      component_registrations_table_[i].port_id_.c_str(),
                      component_registrations_table_[i].name_.c_str());

      if (i != component_registrations_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }
  ACE_OS::fprintf (header_file, "\n};\n");
  ACE_OS::fprintf (header_file, "\n\n");

  ACE_OS::fprintf (header_file, "//Connections\n");
  ACE_OS::fprintf (header_file, "CIAO::Static_Config::ConnectionAttributes connections_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= connections_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{%s, \"%s\", \"%s\", %d, %d, 0, 0 }",
                      connection_type_to_str (connections_table_[i].type_),
                      connections_table_[i].id_.c_str (),
                      connections_table_[i].name_.c_str (),
                      connections_table_[i].component_index_,
                      connections_table_[i].interface_index_);

      if (i != connections_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }
  ACE_OS::fprintf (header_file, "\n};\n");
  ACE_OS::fprintf (header_file, "\n\n");

  ACE_OS::fprintf (header_file, "//Resolver Infos\n");
  ACE_OS::fprintf (header_file, "CIAO::Static_Config::ResolveInfoAttributes resolvers_table_[]= \n");
  ACE_OS::fprintf (header_file, "{\n");
  for (i=0;i <= resolvers_table_last_index_; ++i)
    {
      ACE_OS::fprintf (header_file, "\t{%s, \"%s\", %d, 0 }",
                      resolution_method_to_str (resolvers_table_[i].resolver_type_),
                      resolvers_table_[i].resolver_info_.c_str (),
                      resolvers_table_[i].nested_resolver_index_);

      if (i != resolvers_table_last_index_)
        ACE_OS::fprintf (header_file, ",\n");
    }
  ACE_OS::fprintf (header_file, "\n};\n");
  ACE_OS::fprintf (header_file, "\n\n");

  ACE_OS::fclose (header_file);
}

int CIAO::Static_Assembly_Builder_Visitor::
generate_static_app_mpc (const char* app_mpc_file_name)
{
  FILE* mpc_file =
    ACE_OS::fopen (app_mpc_file_name, "w");

  const ACE_Configuration_Section_Key &root_section
    = this->installation_->root_section ();

  ACE_Configuration_Section_Key section;
  int rc = this->installation_->open_section (root_section,
                                     "ComponentInstallation",
                                     0,
                                     section);
  if (rc == -1)
    printf ("error in open\n");

  ACE_OS::fprintf (mpc_file, "%s\n\n",
  "project(Static_CCM_App) : ciao_server, ciao_client, rtcorba, iortable { \
  includes += $(ACE_ROOT)/TAO/CIAO/tools/Assembly_Deployer \
  includes += $(ACE_ROOT)/TAO/CIAO/tools/static_configurator \
  libs += CIAO_XML_Helpers Static_Configurator \
  after += CIAO_XML_Helpers Static_Configurator");

  for (int i=0; i<=homes_table_last_index_; ++i)
    {
      ACE_TString exec_uuid = homes_table_[i].executor_UUID_;
      ACE_TString servant_uuid = homes_table_[i].servant_UUID_;
      ACE_CString exec_lib, servant_lib;

      // Check if uuids have already been installed.
      if (this->installation_->get_string_value (section,
                                                 exec_uuid.c_str (),
                                                 exec_lib) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "installation info not available for impl %s: %s\n",
                      homes_table_[i].id_.c_str (),  exec_uuid.c_str ()));
        }

      ACE_OS::fprintf (mpc_file, "libs += %s\n", exec_lib.c_str ());
      ACE_OS::fprintf (mpc_file, "after += %s\n", exec_lib.c_str ());

      if (this->installation_->get_string_value (section,
                                                 servant_uuid.c_str (),
                                                 servant_lib) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "installation info not available for impl %s: %s\n",
                      homes_table_[i].id_.c_str (),  servant_uuid.c_str ()));
        }

      ACE_OS::fprintf (mpc_file, "libs += %s\n", servant_lib.c_str ());
      ACE_OS::fprintf (mpc_file, "after += %s\n", servant_lib.c_str ());
    }

  ACE_OS::fprintf (mpc_file, "%s\n",
  "   Source_Files { \
         Static_CCM_App.cpp \
      } \
      IDL_Files { \
      } \
   }");

  ACE_OS::fclose (mpc_file);

  return 0;
}

void CIAO::Static_Assembly_Builder_Visitor::
generate_static_app_driver (const char* app_driver_file_name)
{
  FILE* app_driver_file =
    ACE_OS::fopen (app_driver_file_name, "w");

  const char *text =
"#include \"ComponentServer_Impl.h\" 
#include \"CIAO_ServersC.h\" 
#include \"Server_init.h\" 
#include \"Static_Configurator.h\" 
#include \"ace/SString.h\" 
#include \"ace/Get_Opt.h\"
#include \"Static_Assembly_Config.h\"

char *ior_file_name_ = 0; \

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, \"k:o:\");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           \"usage:  %s\\n\"
                           \"-o <ior_output_file>\\n\"
                           \"\\n\",
                           argv [0]),
                          -1);
      }

  return 0;
}


int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Server_init (orb.in ());

      if (parse_args (argc, argv) != 0)
        return -1;

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references (\"RootPOA\"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::ComponentServer_Impl *comserv_servant;

      CIAO::Static_Configurator configurator;
      int containers_table_size =
        sizeof (containers_table_)/sizeof(CIAO::Static_Config::ContainerAttributes);
      int homes_table_size =
        sizeof (homes_table_)/sizeof(CIAO::Static_Config::HomeAttributes);
      int components_table_size =
        sizeof (components_table_)/sizeof(CIAO::Static_Config::ComponentAttributes);
      int component_registrations_table_size =
        sizeof (component_registrations_table_)/sizeof(CIAO::Assembly_Placement::componentinstantiation::Register_Info);
      int connections_table_size =
        sizeof (connections_table_)/sizeof(CIAO::Static_Config::ConnectionAttributes);
      int resolvers_table_size =
        sizeof (resolvers_table_)/sizeof(CIAO::Static_Config::ResolveInfoAttributes);

      CIAO::HOMECREATOR_FUNCPTR_MAP home_creator_fptr_map;
      CIAO::HOMESERVANTCREATOR_FUNCPTR_MAP homesvnt_creator_fptr_map;
      CIAO::Static_Config_EntryPoints_Maps maps;
      maps.home_creator_funcptr_map_ = &home_creator_fptr_map;
      maps.home_servant_creator_funcptr_map_ = &homesvnt_creator_fptr_map;

      int i=0;
      for (i=0; i<homes_table_size; ++i)
        {
          home_creator_fptr_map.bind (homes_table_[i].executor_entrypt_,
                                      homes_table_[i].executor_fptr_);

          homesvnt_creator_fptr_map.bind (homes_table_[i].servant_entrypt_,
                                          homes_table_[i].servant_fptr_);
        }

      ACE_NEW_RETURN (comserv_servant,
                      CIAO::ComponentServer_Impl (orb.in (),
                                                  poa.in (),
                                                  1,
                                                  &maps),
                      -1);

      PortableServer::ServantBase_var safe_servant (comserv_servant);

      Components::ConfigValues configs;

      comserv_servant->init (configs
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Configuring ComponentServer.
      PortableServer::ObjectId_var cs_oid
        = poa->activate_object (comserv_servant
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (cs_oid.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Components::Deployment::ComponentServer_var comserv_obj =
        Components::Deployment::ComponentServer::_narrow (obj.in ()
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (comserv_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, \"Unable to activate ComponentServer object\\n\"), -1);


      Components::Deployment::ServerActivator_var activator;
      Components::ConfigValues_var config = new Components::ConfigValues;

      comserv_servant->set_objref (activator.in (),
                                   config,
                                   comserv_obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      configurator.configure (orb.in (),
                              comserv_obj.in (),
                              containers_table_,
                              containers_table_size,
                              homes_table_,
                              homes_table_size,
                              components_table_,
                              components_table_size,
                              component_registrations_table_,
                              component_registrations_table_size,
                              connections_table_,
                              connections_table_size,
                              resolvers_table_,
                              resolvers_table_size);

      CORBA::String_var str = orb->object_to_string (comserv_obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CIAO::Utility::write_IOR (ior_file_name_, str.in ());
      ACE_DEBUG ((LM_INFO, \"ComponentServer IOR: %s\\n\", str.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  \"Running ComponentServer...\\n\"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           \"server::main\t\\n\");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}";

  ACE_OS::fprintf( app_driver_file, "%s\n", text);

  ACE_OS::fclose (app_driver_file);
}
