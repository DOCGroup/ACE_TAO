// $Id$

#include "Static_Assembly.h"
#include "Static_Assembly_Visitors.h"
#include "../XML_Helpers/XML_Utils.h"
#include "ace/OS_NS_stdio.h"
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

const char*
policy_type_to_str(CORBA::PolicyType type)
{
 switch(type)
 {
 case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
	  return "RTCORBA::PRIORITY_MODEL_POLICY_TYPE";
 case RTCORBA::THREADPOOL_POLICY_TYPE:
	  return "RTCORBA::THREADPOOL_POLICY_TYPE";
 case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE:
	  return "RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE";
 }
 return "";
}

const char *
model_type_to_str(RTCORBA::PriorityModel model)
{
 switch(model)
 {
 case RTCORBA::CLIENT_PROPAGATED:
	 return "RTCORBA::CLIENT_PROPAGATED";
 case RTCORBA::SERVER_DECLARED:
	 return "RTCORBA::SERVER_DECLARED";
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
    thread_pool_table_(info.thread_pool_table_),
    lane_table_(info.lane_table_),
    thread_pool_lanes_table_(info.thread_pool_lanes_table_),
    band_table_(info.band_table_),
    priority_band_table_(info.priority_band_table_),
    policy_config_table_(info.policy_config_table_),
    policy_set_table_(info.policy_set_table_),
    containers_table_last_index_ (info.containers_table_last_index_),
    homes_table_last_index_ (info.homes_table_last_index_),
    components_table_last_index_ (info.components_table_last_index_),
    component_registrations_table_last_index_ (info.component_registrations_table_last_index_),
    connections_table_last_index_ (info.connections_table_last_index_),
    resolvers_table_last_index_ (info.resolvers_table_last_index_),
    thread_pool_table_last_index_ (info.thread_pool_table_last_index_),
    lane_table_last_index_(info.lane_table_last_index_),
    thread_pool_lanes_table_last_index_(info.thread_pool_lanes_table_last_index_),
    band_table_last_index_(info.band_table_last_index_),
    priority_band_table_last_index_(info.priority_band_table_last_index_),
    policy_config_table_last_index_(info.policy_config_table_last_index_),
    policy_set_table_last_index_(info.policy_set_table_last_index_),
    installation_file_ (installation_file),
    is_realtime_(false)
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
      CIAO::RTConfiguration::RTORB_Resource_Info resources;
      CIAO::RTConfiguration::Policy_Sets         pset;

	  is_realtime_ = true;

	  if (CIAO::XML_Utils::parse_rtcad_extension (rtcad, resources, pset) == 0)
	  {
	   // Successfully parse the rtcad file.
           build_rt_resource_info(resources);
	   build_rt_policy_set(pset);
	  }
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

if(is_realtime_)
{
  ACE_OS::fprintf(header_file, "//Thread Pool\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::ThreadPoolAttributes thread_pool_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0; i <= thread_pool_table_last_index_;i++)
  {
	  ACE_OS::fprintf(header_file, "\t{\"%s\", %d, %d, %d, %d, %d, %d, %d}",
	          	         thread_pool_table_[i].name_.c_str (),
				 thread_pool_table_[i].stacksize_,
				 thread_pool_table_[i].static_threads_,
				 thread_pool_table_[i].dynamic_threads_,
				 thread_pool_table_[i].default_priority_,
				 thread_pool_table_[i].allow_request_buffering_,
				 thread_pool_table_[i].max_buffered_requests_,
				 thread_pool_table_[i].max_request_buffer_size_);

	  if(i!=thread_pool_table_last_index_)
       ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//Lanes\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::LaneAttributes lane_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0; i <= lane_table_last_index_;i++)
  {
	  ACE_OS::fprintf(header_file, "\t{%d, %d, %d}",
		              lane_table_[i].lane_priority_,
			      lane_table_[i].static_threads_,
			      lane_table_[i].dynamic_threads_);

      if(i!=lane_table_last_index_)
	    ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//ThreadPoolLanes\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::ThreadPoolLanesAttributes thread_pool_lanes_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0; i <= thread_pool_lanes_table_last_index_;i++)
  {
	  ACE_OS::fprintf(header_file, "\t{\"%s\", %d, %d, %d, %d, %d, %d, %d}",
		              thread_pool_lanes_table_[i].name_.c_str (),
			      thread_pool_lanes_table_[i].stacksize_,
			      thread_pool_lanes_table_[i].lane_begin_index_,
			      thread_pool_lanes_table_[i].lane_end_index_,
			      thread_pool_lanes_table_[i].allow_borrowing_,
			      thread_pool_lanes_table_[i].allow_request_buffering_,
			      thread_pool_lanes_table_[i].max_buffered_requests_,
			      thread_pool_lanes_table_[i].max_request_buffer_size_);
	  
	  if(i!=thread_pool_lanes_table_last_index_)
		  ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//Bands\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::BandAttributes band_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0; i <= band_table_last_index_;i++)
  {
	 ACE_OS::fprintf(header_file, "\t{%d, %d}",
		              band_table_[i].low_,
			      band_table_[i].high_);

	 if(i!=band_table_last_index_)
 	  ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//PriorityBands\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::PriorityBandsAttributes priority_band_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");
  
  for(i=0;i <=priority_band_table_last_index_;i++)
  {
	  ACE_OS::fprintf(header_file, "\t{\"%s\", %d, %d}",
		              priority_band_table_[i].name_.c_str (),
			      priority_band_table_[i].band_begin_index_,
			      priority_band_table_[i].band_end_index_);

      if(i!=priority_band_table_last_index_)
	    ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//Policy Configs\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::PolicyConfigAttributes policy_config_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0; i<=policy_config_table_last_index_;i++)
  {
	switch(policy_config_table_[i].type_)
	{
	case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
		ACE_OS::fprintf(header_file, "\t{%s, \"\", %s, %d}", 
			policy_type_to_str(policy_config_table_[i].type_),
 		        model_type_to_str(policy_config_table_[i].model_),
		        policy_config_table_[i].default_priority_);
		break;
	
	case RTCORBA::THREADPOOL_POLICY_TYPE:
		ACE_OS::fprintf(header_file, "\t{%s, \"%s\", /*dummy variable*/ RTCORBA::CLIENT_PROPAGATED, 0}",
			policy_type_to_str(policy_config_table_[i].type_),
			policy_config_table_[i].name_.c_str ());
		break;
		 
	case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE:
		ACE_OS::fprintf(header_file, "\t{%s, \"%s\", /*dummy variable*/ RTCORBA::CLIENT_PROPAGATED, 0}",
			policy_type_to_str(policy_config_table_[i].type_),
			policy_config_table_[i].name_.c_str ());
		break;
	}
	 
	 if(i!=policy_config_table_last_index_)
       ACE_OS::fprintf(header_file, ",\n");
  }
  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");

  ACE_OS::fprintf(header_file, "//Policy Set\n");
  ACE_OS::fprintf(header_file, "CIAO::Static_Config::PolicySetAttributes  policy_set_table_[]= \n");
  ACE_OS::fprintf(header_file, "{\n");

  for(i=0;i<=policy_set_table_last_index_;i++)
  {
	  ACE_OS::fprintf(header_file, "\t{\"%s\", %d, %d}",
		  policy_set_table_[i].name_.c_str (),
		  policy_set_table_[i].config_begin_index_,
		  policy_set_table_[i].config_end_index_);

     if(i!=policy_set_table_last_index_)
	  ACE_OS::fprintf(header_file, ",\n");
  }

  ACE_OS::fprintf(header_file, "\n};\n");
  ACE_OS::fprintf(header_file, "\n\n");
 }
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
    ACE_ERROR_RETURN ((LM_ERROR, "error in installation open\n"), -1);

  ACE_OS::fprintf (mpc_file, "%s",
  "project(Static_CCM_App) : ciao_server, ciao_client, rtcorba, rtportableserver, iortable, acexml {\n"
  "includes += $(ACE_ROOT)/TAO/CIAO/tools/Assembly_Deployer\n"
  "includes += $(ACE_ROOT)/TAO/CIAO/tools/static_configurator\n");

  if(is_realtime_)
   ACE_OS::fprintf(mpc_file, "includes += $(ACE_ROOT)/TAO/CIAO/tools/RTComponentServer\n");
  
  if(is_realtime_)
  {
   ACE_OS::fprintf(mpc_file, "%s\n\n",
         "libs += RTComponent_Server Static_Configurator CIAO_XML_Helpers Segment_Timer\n"
	 "after += RTComponent_Server Static_Configurator CIAO_XML_Helpers Segment_Timer\n");
  }
  else
  {
   ACE_OS::fprintf(mpc_file, "%s\n\n",
	  "libs += Static_Configurator CIAO_XML_Helpers Segment_Timer\n"
          "after += Static_Configurator CIAO_XML_Helpers Segment_Timer\n");
  }

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

  ACE_OS::fprintf (mpc_file, "%s",
  "   Source_Files {\n"
  "       Static_CCM_App.cpp\n");

  ACE_OS::fprintf(mpc_file, "%s", 
  "    }\n\n"
  "    IDL_Files {\n"
  "    }\n"
  "}\n");

  ACE_OS::fclose (mpc_file);
  return rc;
}

void CIAO::Static_Assembly_Builder_Visitor::
generate_static_app_driver (const char* app_driver_file_name)
{
 if(is_realtime_)
 {
  generate_rt_static_app_driver(app_driver_file_name);
  return;
 }

  FILE* app_driver_file =
    ACE_OS::fopen (app_driver_file_name, "w");

  const char *text =
"\n"
"#include \"ComponentServer_Impl.h\"\n"
"#include \"CIAO_ServersC.h\"\n"
"#include \"Server_init.h\"\n"
"#include \"Static_Configurator.h\"\n"
"#include \"ace/SString.h\" \n"
"#include \"ace/Get_Opt.h\"\n"
"#include \"Static_Assembly_Config.h\"\n"
"#include \"tao/RTPortableServer/RTPortableServer.h\"\n"
"\n"
"char *ior_file_name_ = 0; \n"
"\n"
"int\n"
"parse_args (int argc, char *argv[])\n"
"{\n"
"  ACE_Get_Opt get_opts (argc, argv, \"k:o:\");\n"
"  int c;\n"
"\n"
"  while ((c = get_opts ()) != -1)\n"
"    switch (c)\n"
"    {\n"
"    case 'o':  // get the file name to write to\n"
"     ior_file_name_ = get_opts.opt_arg ();\n"
"     break;\n"
"\n"
"      case '?':  // display help for use of the server.\n"
"      default:\n"
"       ACE_ERROR_RETURN ((LM_ERROR,\n"
"                          \"usage:  %s\\n\"\n"
"                          \"-o <ior_output_file>\\n\"\n"
"                          \"\\n\",\n"
"                          argv [0]),\n"
"                         -1);\n"
"     }\n"
"\n"
"  return 0;\n"
"}\n"
"\n"
"\n"
"int\n"
"main (int argc, char *argv[])\n"
"{\n"
"  ACE_TRY_NEW_ENV\n"
"  {\n"
"      // Initialize orb\n"
"      CORBA::ORB_var orb = CORBA::ORB_init (argc,\n"
"                                            argv\n"
"                                            ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      CIAO::Server_init (orb.in ());\n"
"\n"
"      if (parse_args (argc, argv) != 0)\n"
"        return -1;\n"
"\n"
"      // Get reference to Root POA.\n"
"      CORBA::Object_var obj\n"
"        = orb->resolve_initial_references (\"RootPOA\"\n"
"                                           ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      PortableServer::POA_var poa\n"
"        = PortableServer::POA::_narrow (obj.in ()\n"
"                                        ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      // Activate POA manager\n"
"      PortableServer::POAManager_var mgr\n"
"        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      CIAO::ComponentServer_Impl *comserv_servant;\n"
"\n"
"      CIAO::Static_Configurator configurator;\n"
"      int containers_table_size =\n"
"        sizeof (containers_table_)/sizeof(CIAO::Static_Config::ContainerAttributes);\n"
"      int homes_table_size =\n"
"        sizeof (homes_table_)/sizeof(CIAO::Static_Config::HomeAttributes);\n"
"      int components_table_size =\n"
"        sizeof (components_table_)/sizeof(CIAO::Static_Config::ComponentAttributes);\n"
"      int component_registrations_table_size =\n"
"        sizeof (component_registrations_table_)/sizeof(CIAO::Assembly_Placement::componentinstantiation::Register_Info);\n"
"      int connections_table_size =\n"
"        sizeof (connections_table_)/sizeof(CIAO::Static_Config::ConnectionAttributes);\n"
"      int resolvers_table_size =\n"
"        sizeof (resolvers_table_)/sizeof(CIAO::Static_Config::ResolveInfoAttributes);\n"
"\n"
"      CIAO::HOMECREATOR_FUNCPTR_MAP home_creator_fptr_map;\n"
"      CIAO::HOMESERVANTCREATOR_FUNCPTR_MAP homesvnt_creator_fptr_map;\n"
"      CIAO::Static_Config_EntryPoints_Maps maps;\n"
"      maps.home_creator_funcptr_map_ = &home_creator_fptr_map;\n"
"      maps.home_servant_creator_funcptr_map_ = &homesvnt_creator_fptr_map;\n"
"\n"
"      int i=0;\n"
"      for (i=0; i<homes_table_size; ++i)\n"
"        {\n"
"          home_creator_fptr_map.bind (homes_table_[i].executor_entrypt_,\n"
"                                      homes_table_[i].executor_fptr_);\n"
"\n"
"          homesvnt_creator_fptr_map.bind (homes_table_[i].servant_entrypt_,\n"
"                                          homes_table_[i].servant_fptr_);\n"
"        }\n"
"\n"
"      ACE_NEW_RETURN (comserv_servant,\n"
"                      CIAO::ComponentServer_Impl (orb.in (),\n"
"                                                  poa.in (),\n"
"                                                  1,\n"
"                                                  &maps),\n"
"                      -1);\n"
"\n"
"      PortableServer::ServantBase_var safe_servant (comserv_servant);\n"
"\n"
"      Components::ConfigValues configs;\n"
"\n"
"      comserv_servant->init (configs\n"
"                             ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      // Configuring ComponentServer.\n"
"      PortableServer::ObjectId_var cs_oid\n"
"        = poa->activate_object (comserv_servant\n"
"                                ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      obj = poa->id_to_reference (cs_oid.in ()\n"
"                                  ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      Components::Deployment::ComponentServer_var comserv_obj =\n"
"        Components::Deployment::ComponentServer::_narrow (obj.in ()\n"
"                                                          ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      if (CORBA::is_nil (comserv_obj.in ()))\n"
"        ACE_ERROR_RETURN ((LM_ERROR, \"Unable to activate ComponentServer object\\n\"), -1);\n"
"\n"
"\n"
"      Components::Deployment::ServerActivator_var activator;\n"
"      Components::ConfigValues_var config = new Components::ConfigValues;\n"
"\n"
"      comserv_servant->set_objref (activator.in (),\n"
"                                   config,\n"
"                                   comserv_obj.in ()\n"
"                                   ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      configurator.configure (orb.in (),\n"
"                              comserv_obj.in (),\n"
"                              containers_table_,\n"
"                              containers_table_size,\n"
"                              homes_table_,\n"
"                              homes_table_size,\n"
"                              components_table_,\n"
"                              components_table_size,\n"
"                              component_registrations_table_,\n"
"                              component_registrations_table_size,\n"
"                              connections_table_,\n"
"                              connections_table_size,\n"
"                              resolvers_table_,\n"
"                              resolvers_table_size);\n"
"\n"
"      CORBA::String_var str = orb->object_to_string (comserv_obj.in ()\n"
"                                                     ACE_ENV_ARG_PARAMETER);\n"
"\n"
"      CIAO::Utility::write_IOR (ior_file_name_, str.in ());\n"
"      ACE_DEBUG ((LM_INFO, \"ComponentServer IOR: %s\\n\", str.in ()));\n"
"\n"
"      ACE_DEBUG ((LM_DEBUG,\n"
"                  \"Running ComponentServer...\\n\"));\n"
"\n"
"      // Run the main event loop for the ORB.\n"
"      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK\n"
"    }\n"
"  ACE_CATCHANY\n"
"    {\n"
"      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,\n"
"                           \"server::main\t\\n\");\n"
"      return 1;\n"
"    }\n"
"  ACE_ENDTRY;\n"
"\n"
"  return 0;\n"
"}\n";

  ACE_OS::fprintf( app_driver_file, "%s\n", text);

  ACE_OS::fclose (app_driver_file);
}

void CIAO::Static_Assembly_Builder_Visitor::
generate_rt_static_app_driver (const char* app_driver_file_name)
{
  FILE* app_driver_file =
    ACE_OS::fopen (app_driver_file_name, "w");

  const char *text =
"\n"
"#include \"RTServer_Impl.h\"\n"
"#include \"CIAO_ServersC.h\"\n"
"#include \"Server_init.h\"\n"
"#include \"Static_Configurator.h\"\n"
"#include \"ace/SString.h\"\n" 
"#include \"ace/Get_Opt.h\"\n"
"#include \"Static_Assembly_Config.h\"\n"
"#include \"tao/RTPortableServer/RTPortableServer.h\"\n"
"\n"
"char *ior_file_name_ = 0; \n"
"\n"
"int\n"
"parse_args (int argc, char *argv[])\n"
"{\n"
"  ACE_Get_Opt get_opts (argc, argv, \"k:o:\");\n"
"  int c;\n"
"\n"
"  while ((c = get_opts ()) != -1)\n"
"    switch (c)\n"
"    {\n"
"    case 'o':  // get the file name to write to\n"
"     ior_file_name_ = get_opts.opt_arg ();\n"
"     break;\n"
"\n"
"    case '?':  // display help for use of the server.\n"
"    default:\n"
"       ACE_ERROR_RETURN ((LM_ERROR,\n"
"                          \"usage:  %s\\n\"\n"
"                          \"-o <ior_output_file>\\n\"\n"
"                          \"\\n\",\n"
"                          argv [0]),\n"
"                         -1);\n"
"     }\n"
"\n"
"  return 0;\n"
"}\n"
"\n"
"int\n"
"main (int argc, char *argv[])\n"
"{\n"
" ACE_TRY_NEW_ENV\n"
" {\n"
"     // Initialize orb\n"
"     CORBA::ORB_var orb = CORBA::ORB_init (argc,\n"
"                                           argv\n"
"                                           ACE_ENV_ARG_PARAMETER);\n"
"\n"                                           
"     ACE_TRY_CHECK;\n"
"\n"     
"     CIAO::Server_init (orb.in ());\n"
"\n"
"     if (parse_args (argc, argv) != 0)\n"
"        return -1;\n"
"\n"
"      // Get reference to Root POA.\n"
"      CORBA::Object_var object =\n"
"        orb->resolve_initial_references (\"RootPOA\"\n"
"                                         ACE_ENV_ARG_PARAMETER);\n"
"     ACE_TRY_CHECK;\n"
"     PortableServer::POA_var root_poa =\n"
"     PortableServer::POA::_narrow (object.in ()\n"
"	  			    ACE_ENV_ARG_PARAMETER);\n"
"     // Get reference to RTORB.\n"
"      object =\n"
"        orb->resolve_initial_references (\"RTORB\"\n"
"                                         ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      RTCORBA::RTORB_var rt_orb =\n"
"        RTCORBA::RTORB::_narrow (object.in ()\n"
"                                 ACE_ENV_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      // Activate POA manager\n"
"      PortableServer::POAManager_var poa_manager =\n"
"        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"
"      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK;\n"
"\n"      
"      CIAO::RTServer::RTComponentServer_Impl *comserv_servant;\n"
"      CIAO::Static_Configurator configurator;\n"
"\n"      
"      int containers_table_size =\n"
"        sizeof (containers_table_)/sizeof(CIAO::Static_Config::ContainerAttributes);\n"
"      int homes_table_size =\n"
"        sizeof (homes_table_)/sizeof(CIAO::Static_Config::HomeAttributes);\n"
"      int components_table_size =\n"
"        sizeof (components_table_)/sizeof(CIAO::Static_Config::ComponentAttributes);\n"
"      int component_registrations_table_size =\n"
"        sizeof (component_registrations_table_)/sizeof(CIAO::Assembly_Placement::componentinstantiation::Register_Info);\n"
"      int connections_table_size =\n"
"        sizeof (connections_table_)/sizeof(CIAO::Static_Config::ConnectionAttributes);\n"
"      int resolvers_table_size =\n"
"        sizeof (resolvers_table_)/sizeof(CIAO::Static_Config::ResolveInfoAttributes);\n"
"      int thread_pool_table_size=\n"
"        sizeof(thread_pool_table_)/sizeof(CIAO::Static_Config::ThreadPoolAttributes);\n"
"      int thread_pool_lanes_table_size=\n"
"        sizeof(thread_pool_lanes_table_)/sizeof(CIAO::Static_Config::ThreadPoolLanesAttributes);\n"
"      int priority_band_table_size=\n"
"        sizeof(priority_band_table_)/sizeof(CIAO::Static_Config::PriorityBandsAttributes);\n"
"      int policy_set_table_size=\n"
"        sizeof(policy_set_table_)/sizeof(CIAO::Static_Config::PolicySetAttributes);\n"
"\n"        
"      CIAO::HOMECREATOR_FUNCPTR_MAP home_creator_fptr_map;\n"
"      CIAO::HOMESERVANTCREATOR_FUNCPTR_MAP homesvnt_creator_fptr_map;\n"
"      CIAO::Static_Config_EntryPoints_Maps maps;\n"
"      maps.home_creator_funcptr_map_ = &home_creator_fptr_map;\n"
"      maps.home_servant_creator_funcptr_map_ = &homesvnt_creator_fptr_map;\n"
"\n"
"      int i=0;\n"
"      for (i=0; i<homes_table_size; ++i)\n"
"        {\n"
"          home_creator_fptr_map.bind (homes_table_[i].executor_entrypt_,\n"
"                                      homes_table_[i].executor_fptr_);\n"
"\n"
"          homesvnt_creator_fptr_map.bind (homes_table_[i].servant_entrypt_,\n"
"                                          homes_table_[i].servant_fptr_);\n"
"        }\n"
"\n"
"      ACE_NEW_RETURN (comserv_servant,\n"
"                      CIAO::RTServer::RTComponentServer_Impl (orb.in (),\n"
"                                                              rt_orb.in (),\n"
"                                                              root_poa.in (),\n"
"                                                              1, \n"
"                                                              &maps), \n"
"                      -1);\n"
"\n"                      
"      PortableServer::ServantBase_var safe_servant (comserv_servant);   \n"
"\n"      
"      Components::ConfigValues configs;\n"
"\n"      
"      configurator.config_rt_info(configs, \n"
"                                  thread_pool_table_, \n"
"                                  thread_pool_table_size,\n"
"                                  lane_table_,\n"
"                                  thread_pool_lanes_table_,\n"
"                                  thread_pool_lanes_table_size,\n"
"                                  band_table_,\n"
"                                  priority_band_table_,\n"
"                                  priority_band_table_size,\n"
"                                  policy_config_table_,\n"
"                                  policy_set_table_,\n"
"                                  policy_set_table_size);\n"
"\n"     
"     comserv_servant->init (configs\n"
"                            ACE_ENV_ARG_PARAMETER);\n"
"     ACE_TRY_CHECK;\n"
"     // Configuring ComponentServer.\n"
"     PortableServer::ObjectId_var cs_oid\n"
"        = root_poa->activate_object (comserv_servant\n"
"                                     ACE_ENV_ARG_PARAMETER);\n"
"     ACE_TRY_CHECK; \n"
"\n"
"     object = root_poa->id_to_reference (cs_oid.in ()\n"
"                                          ACE_ENV_ARG_PARAMETER);\n"
"     ACE_TRY_CHECK;\n"
"\n"     
"     Components::Deployment::ComponentServer_var comserv_obj =\n"
"        Components::Deployment::ComponentServer::_narrow (object.in ()\n"
"                                                          ACE_ENV_ARG_PARAMETER);\n"
"     ACE_TRY_CHECK;\n"
"\n"
"     if (CORBA::is_nil (comserv_obj.in ()))\n"
"         ACE_ERROR_RETURN ((LM_ERROR,\n"
"                          \"Unable to activate RTComponentServer object\\n\"),\n"
"                         -1);\n"
"\n"     
"      Components::Deployment::ServerActivator_var activator;\n"
"\n"
"      // We are just storing the original configuration here.\n"
"      // Currently, we don't really use this ConfigValues direclty.\n"
"      Components::ConfigValues_var more_config = new Components::ConfigValues;\n"
"\n"      
"      comserv_servant->set_objref (activator.in (),\n"
"                                   more_config.in (),\n"
"                                   comserv_obj.in ()\n"
"                                   ACE_ENV_ARG_PARAMETER);\n"
"\n"      
"      ACE_TRY_CHECK;\n"
"\n"
"      configurator.configure (orb.in (),\n"
"                              comserv_obj.in (),\n"
"                              containers_table_,\n"
"                              containers_table_size,\n"
"                              homes_table_,\n"
"                              homes_table_size,\n"
"                              components_table_,\n"
"                              components_table_size,\n"
"                              component_registrations_table_,\n"
"                              component_registrations_table_size,\n"
"                              connections_table_,\n"
"                              connections_table_size,\n"
"                              resolvers_table_,\n"
"                              resolvers_table_size);\n"
"\n"
"      CORBA::String_var str = orb->object_to_string (comserv_obj.in ()\n"
"                                                     ACE_ENV_ARG_PARAMETER);\n"
"\n"
"      CIAO::Utility::write_IOR (ior_file_name_, str.in ());\n"
"      ACE_DEBUG ((LM_INFO, \"RTComponentServer IOR: %s\\n\", str.in ()));\n"
"\n"
"      ACE_DEBUG ((LM_DEBUG,\n"
"                  \"Running RTComponentServer...\\n\"));\n"
"\n"
"      // Run the main event loop for the ORB.\n"
"      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);\n"
"      ACE_TRY_CHECK\n"
"    }\n"
"   ACE_CATCHANY\n"
"   {\n"
"      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,\n"
"                           \"server::main	\\n\");\n"
"      return 1;\n"
"   }\n"
"  ACE_ENDTRY;\n"
"\n"
"  return 0;\n"
"}\n";

  ACE_OS::fprintf( app_driver_file, "%s\n", text);

  ACE_OS::fclose (app_driver_file);
}


int CIAO::Static_Assembly_Builder_Visitor::
  build_rt_resource_info(CIAO::RTConfiguration::RTORB_Resource_Info &rt_resources)
{
 unsigned int i, j;
 int begin_index, end_index;

 for(i=0; i < rt_resources.tp_configs.length();i++)
 {
        ++thread_pool_table_last_index_;

	thread_pool_table_[thread_pool_table_last_index_].name_=
		               rt_resources.tp_configs[i].name.in();
	thread_pool_table_[thread_pool_table_last_index_].stacksize_ = 
                       rt_resources.tp_configs[i].stacksize;
	thread_pool_table_[thread_pool_table_last_index_].static_threads_=
		               rt_resources.tp_configs[i].static_threads;
	thread_pool_table_[thread_pool_table_last_index_].dynamic_threads_= 
		               rt_resources.tp_configs[i].dynamic_threads;
	thread_pool_table_[thread_pool_table_last_index_].default_priority_= 
		               rt_resources.tp_configs[i].default_priority;
	thread_pool_table_[thread_pool_table_last_index_].allow_request_buffering_= 
		               rt_resources.tp_configs[i].allow_request_buffering;
	thread_pool_table_[thread_pool_table_last_index_].max_buffered_requests_= 
		               rt_resources.tp_configs[i].max_buffered_requests;
	thread_pool_table_[thread_pool_table_last_index_].max_request_buffer_size_=
		               rt_resources.tp_configs[i].max_request_buffer_size;

 }

 for(i=0; i < rt_resources.tpl_configs.length();i++)
 {
        ++thread_pool_lanes_table_last_index_;
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].name_ = 
		           rt_resources.tpl_configs[i].name.in();
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].stacksize_=
		           rt_resources.tpl_configs[i].stacksize;
        thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].allow_borrowing_=	
	                   rt_resources.tpl_configs[i].allow_borrowing;
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].allow_request_buffering_=
                   rt_resources.tpl_configs[i].allow_request_buffering;
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].max_buffered_requests_=
		           rt_resources.tpl_configs[i].max_buffered_requests;
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].max_request_buffer_size_=
		           rt_resources.tpl_configs[i].max_request_buffer_size;

	begin_index = lane_table_last_index_;
	
	for(j=0;j < rt_resources.tpl_configs[i].lanes.length();j++)
	{
	  ++lane_table_last_index_;
	
	  lane_table_[lane_table_last_index_].lane_priority_=
			           rt_resources.tpl_configs[i].lanes[j].lane_priority;
	  lane_table_[lane_table_last_index_].static_threads_=
			           rt_resources.tpl_configs[i].lanes[j].static_threads;
	  lane_table_[lane_table_last_index_].dynamic_threads_=
			           rt_resources.tpl_configs[i].lanes[j].dynamic_threads;
	}

	if(begin_index != lane_table_last_index_)
	{
	  begin_index++;
	  end_index = lane_table_last_index_;
	}
	else
	{
	  begin_index = -1;
	  end_index = -1;
	}
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].lane_begin_index_= begin_index;
	thread_pool_lanes_table_[thread_pool_lanes_table_last_index_].lane_end_index_= end_index;
 }

 //Dumping Priority_Bands_Configurations

 for(i=0;i < rt_resources.pb_configs.length();i++)
 {
         ++priority_band_table_last_index_;
	 priority_band_table_[priority_band_table_last_index_].name_= 
	                      rt_resources.pb_configs[i].name.in();
	 
	 begin_index = band_table_last_index_;
	 for(j=0; j < rt_resources.pb_configs[i].bands.length();j++)
	 {
		 ++band_table_last_index_;
		 band_table_[band_table_last_index_].low_= 
			         rt_resources.pb_configs[i].bands[j].low;
		 band_table_[band_table_last_index_].high_=
			         rt_resources.pb_configs[i].bands[j].high;
	 }

	 if(begin_index != band_table_last_index_)
	 {
	   begin_index++;
	   end_index = band_table_last_index_;
	 }
	 else
	 {
	   begin_index = end_index = -1;
	 }
     priority_band_table_[priority_band_table_last_index_].band_begin_index_=begin_index;
     priority_band_table_[priority_band_table_last_index_].band_end_index_  = end_index; 
 }

 return 0;
}

int  CIAO::Static_Assembly_Builder_Visitor::build_rt_policy_set(CIAO::RTConfiguration::Policy_Sets &pset)
{
 unsigned int i, j;
 int begin_index, end_index;
 const char *name;

 for(i=0;i < pset.length();i++)
 {
  ++policy_set_table_last_index_;
  policy_set_table_[policy_set_table_last_index_].name_ = pset[i].name.in();
 
  begin_index = policy_config_table_last_index_;

  for(j=0; j < pset[i].configs.length();j++)
  { 
        ++policy_config_table_last_index_;

	policy_config_table_[policy_config_table_last_index_].type_ =
	         pset[i].configs[j].type;

	if(pset[i].configs[j].type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
	{
	  CIAO::RTConfiguration::Priority_Model_Config *config;
	  pset[i].configs[j].configuration >>= config;

	  policy_config_table_[policy_config_table_last_index_].model_ = 
		     config->model;

	  policy_config_table_[policy_config_table_last_index_].default_priority_ =
		     config->default_priority;
	}
	else if(pset[i].configs[j].type == RTCORBA::THREADPOOL_POLICY_TYPE ||
		    pset[i].configs[j].type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE
	       )
	{
          pset[i].configs[j].configuration >>= name;
  	  policy_config_table_[policy_config_table_last_index_].name_ = name;
	} 
  }

  if(begin_index != policy_config_table_last_index_)
  {
    begin_index++;
    end_index = policy_config_table_last_index_;
  }
  else
  {
    begin_index = end_index = -1;
  }

  policy_set_table_[policy_set_table_last_index_].config_begin_index_ = begin_index;
  policy_set_table_[policy_set_table_last_index_].config_end_index_ = end_index;
 }
 return 0;
}

