// $Id$

#include "Assembly_Visitors.h"
#include "../XML_Helpers/XML_Utils.h"

#if !defined (__ACE_INLINE__)
# include "Assembly_Visitors.inl"
#endif /* __ACE_INLINE__ */

CIAO::Assembly_Builder_Visitor::~Assembly_Builder_Visitor (void)
{
}

int
CIAO::Assembly_Builder_Visitor::visit_Container
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
CIAO::Assembly_Builder_Visitor::visit_hostcollocation
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
CIAO::Assembly_Builder_Visitor::visit_processcollocation
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
      ssize_t endpos = desti_string.find ('|');

      destination_host =
        desti_string.substring (0, endpos);

      if (endpos != ACE_CString::npos)
        {
          ACE_CString svcconf = desti_string.substring (endpos + 1);
          server_config.length (1);

          Components::ConfigValue *newconfig =
            new OBV_Components::ConfigValue;
          newconfig->name ((const char *) "CIAO-svcconf-id");
          newconfig->value () <<= svcconf.c_str ();
          server_config[0] = newconfig;
        }
    }

  // @@ check for RTCAD file, parse the file, and insert the thing in server_config.

  const char *rtcad = pc->rtcad_filename ();

  if (rtcad != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Using RTCAD file: %s\n",
                  rtcad));
      CORBA::ULong len = server_config.length ();
      server_config.length (len+1);

      Components::ConfigValue *newconfig =
        new OBV_Components::ConfigValue;
      newconfig->name ((const char *) "CIAO-rtcad-filename");
      newconfig->value () <<= rtcad;
      server_config[len] = newconfig;
    }

  Components::Deployment::ServerActivator_var activator =
    this->deployment_config_.get_activator
    (destination_host.length () == 0 ? 0 : destination_host.c_str ());

  if (CORBA::is_nil (activator.in ()))
    ACE_ERROR_RETURN ((LM_ERROR, "Fail to acquire ServerActivator (%s)\n",
                       pc->destination ()),
                      -1);

  this->compserv_ =
    activator->create_component_server (server_config
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_DEBUG ((LM_DEBUG, "Done creating component server\n"));

  this->context_.component_servers_.enqueue_tail (this->compserv_);

  // @@ Do not create a default container here.  We should wait until
  // the children ask for a specific containers
  this->container_ = Components::Deployment::Container::_nil ();
  this->rtpolicy_name_.clear ();

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
CIAO::Assembly_Builder_Visitor::visit_homeplacement
(CIAO::Assembly_Placement::homeplacement *hp
 ACE_ENV_ARG_DECL)
{
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
      //      info.dump ();             // For debug purpose.

      // install home
      Components::ConfigValues home_config;
      // Setting home config value here:
      home_config.length (2);

      Components::ConfigValue *item = new OBV_Components::ConfigValue ();
      item->name ((const char *) "CIAO-servant-UUID");
      item->value () <<= info.servant_UUID_.c_str ();
      home_config[0] = item;

      item = new OBV_Components::ConfigValue ();
      item->name ((const char *) "CIAO-servant-entrypt");
      item->value () <<= info.servant_entrypt_.c_str ();
      home_config[1] = item;

      // @@ How do I get a customized container here, if we named a
      // RTpolicy_Set for this home placement?

      // @@ We actually have to take care of both home_config and
      // container_config here.  Should we use a Container Manager
      // here to make sure that we always use the same container for
      // different homes with similar policies?  Naw, we'll just
      // require putting similar policied home together for now.  They
      // have abandoned this implementation already anyway.

      Components::Deployment::Container_var container
        = this->get_container (hp->rtpolicyset_ref ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (container.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Unable to acquire a reference to ServerActivator\n"),
                          -1);

      Components::CCMHome_var home =
        container->install_home (info.executor_UUID_.c_str (),
                                 info.executor_entrypt_.c_str (),
                                 home_config
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      Components::KeylessCCMHome_var klhome =
        Components::KeylessCCMHome::_narrow (home.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (klhome.in ()))
        return -1;

      // register home with context
      if (this->context_.installed_homes_.bind (hp->id (),
                                                home) != 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Failed to register home\n"),
                          -1);
      // Save the home for component instantiation.
      this->home_ = klhome;

      // @@ Register home according to register spec.
      // @@ Not implemented yet.
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

  // Reset current home
  this->home_ = 0;
  return 0;
}

int
CIAO::Assembly_Builder_Visitor::visit_componentinstantiation
(CIAO::Assembly_Placement::componentinstantiation *ci
 ACE_ENV_ARG_DECL)
{
  // @@ instantiation and register component.
  ACE_DEBUG ((LM_DEBUG, "ComponentInstantiation %s\n", ci->id ()));

  Components::CCMObject_var comp
    = this->home_->create_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->context_.instantiated_components_.bind (ci->id (),
                                                comp);

  // Registering component.
  CIAO::Assembly_Placement::componentinstantiation::REGISTRATION_QUEUE::ITERATOR
    iter (ci->register_info_);

  while (!iter.done ())
    {
      CIAO::Assembly_Placement::componentinstantiation::Register_Info *info;
      iter.next (info);

      this->register_component (info,
                                comp.in ()
                                ACE_ENV_ARG_PARAMETER);

      iter.advance ();
    }
  return 0;
}

Components::Deployment::ComponentServer_ptr
CIAO::Assembly_Builder_Visitor::get_current_componentserver (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->compserv_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Creating new ComponenetServer\n"));

      Components::Deployment::ServerActivator_var activator =
        this->deployment_config_.get_default_activator ();

      if (CORBA::is_nil (activator.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Fail to acquire default ServerActivator\n"),
                          0);

      Components::ConfigValues server_config;
      // @@ Nothing to config yet.

      this->compserv_ =
        activator->create_component_server (server_config
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->context_.component_servers_.enqueue_tail (this->compserv_);
      this->container_ = Components::Deployment::Container::_nil ();
      this->rtpolicy_name_.clear ();
    }
  return Components::Deployment::ComponentServer::_duplicate
    (this->compserv_.in ());
}

Components::Deployment::Container_ptr
CIAO::Assembly_Builder_Visitor::get_container (const char *rtpolicy
                                               ACE_ENV_ARG_DECL)
{
  // If we are not using the same rtpolicy set, or the there's no
  // cached container, then create a new one.
  if (this->rtpolicy_name_ != ACE_CString (rtpolicy) ||
      CORBA::is_nil (this->container_.in ()))
    {
      Components::Deployment::ComponentServer_var server
        = this->get_current_componentserver ();

      Components::ConfigValues container_config;
      // @@ Should we get the config value from Softpkg_Info?
      if (rtpolicy != 0)
        {
          CORBA::ULong len = container_config.length ();
          container_config.length (len + 1);

          Components::ConfigValue *newconfig
            = new OBV_Components::ConfigValue;

          newconfig->name ((const char *) "CIAO-RTPolicySet");
          newconfig->value () <<= rtpolicy;
          container_config[len] = newconfig;
        }

      this->container_ =
        server->create_container (container_config
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->rtpolicy_name_ = rtpolicy;

      if (rtpolicy != 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Creating container with RTPolicySet %s\n",
                    rtpolicy));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Creating container with empty policy set\n"));
    }

  return Components::Deployment::Container::_duplicate
    (this->container_.in ());
}

void
CIAO::Assembly_Builder_Visitor::register_component
(Assembly_Placement::componentinstantiation::Register_Info *i,
 Components::CCMObject_ptr c
 ACE_ENV_ARG_DECL)
{
  CORBA::Object_ptr reg_obj;

  // Extract the right interface to register:
  switch (i->type_)
    {
    case CIAO::Assembly_Placement::componentinstantiation::COMPONENT:
      reg_obj = c;
      break;

    case CIAO::Assembly_Placement::componentinstantiation::PROVIDESID:
      reg_obj = c->provide_facet (i->port_id_.c_str ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    case CIAO::Assembly_Placement::componentinstantiation::CONSUMESID:
      reg_obj = c->get_consumer (i->port_id_.c_str ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    default:
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Now actually register the interface
  switch (i->method_)
    {
    case CIAO::Assembly_Placement::componentinstantiation::NAMINGSERVICE:
      ACE_THROW (CORBA::NO_IMPLEMENT ());

    case CIAO::Assembly_Placement::componentinstantiation::IORFILE:
      {
        CORBA::String_var ior
          = this->orb_->object_to_string (reg_obj
                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        FILE* ior_output_file_ =
          ACE_OS::fopen (i->name_.c_str (), "w");

        if (ior_output_file_)
          {
            ACE_OS::fprintf (ior_output_file_,
                             "%s",
                             ior.in ());
            ACE_OS::fclose (ior_output_file_);
          }
      }
    break;

    default:
      ACE_THROW (CORBA::INTERNAL ());
    }
}
