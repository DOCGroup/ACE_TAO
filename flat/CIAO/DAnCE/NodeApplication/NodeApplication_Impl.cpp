// $Id$
#include "NodeApplication_Impl.h"


#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Object.h"
#include "tao/ORB.h" 
#include "ccm/CCM_ObjectC.h"
#include "ciao/Valuetype_Factories/ConfigValue.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_BaseC.h"
#include "Deployment/Deployment_ApplicationC.h"
#include "Deployment/Deployment_common.h"
#include "ComponentAttributesSetter.h"
//#include "ComponentInstallation_Impl.h"

using namespace DAnCE;

namespace
{
  template<class T>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, T &val)
  {
    CORBA::Any any;
    
    if (properties.find (name, any) == 0)
      if (any >>= val)
        return true;

    return false;
  }
  template<>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, bool &val)
  {
    CORBA::Any any;
    
    if (properties.find (name, any) == 0)
      if (any >>= CORBA::Any::to_boolean(val))
        return true;

    return false;
  }
}


NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb, 
                                            PortableServer::POA_ptr poa, 
                                            const Deployment::DeploymentPlan& plan, 
                                            RedirectionService & redirection, 
                                            const ACE_CString& node_name,
                                            PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)), 
    poa_ (PortableServer::POA::_duplicate (poa)), 
    plan_ (plan), 
    redirection_ (redirection), 
    node_name_ (node_name),
    properties_ (properties)
{
  DANCE_TRACE (DLINFO "NodeApplication_Impl::NodeApplication_Impl");
  this->init ();
}

NodeApplication_Impl::~NodeApplication_Impl()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::~NodeApplication_Impl()");

  using namespace Components;
  ConfigValues config_values;
  config_values.length (1L);
  CORBA::Any feature_any;
  for (TContainers::iterator iter = this->containers_.begin();
       iter != this->containers_.end();
       ++iter)
    {
      /* TODO: This is highly suspect.  I believe we should be using get_component_server,
         not calling create_container. */
      try
        {
          ACE_CString name = (*iter).ext_id_;
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - Retrieving ComponentServer associated with container %s\n",
                       name.c_str ()));
          Components::Deployment::Container_var cont = (*iter).int_id_.in();
          Components::Deployment::ComponentServer_var compServer = cont->get_component_server ();

          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - before remove_container\n"));
          compServer->remove_container (cont.in ());
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - before remove_component_server\n"));
          this->activator_->remove_component_server (compServer.in());
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - after remove_component_server\n"));
        }
      catch (...)
        {
          DANCE_DEBUG ((LM_ERROR, DLINFO
                        "~NodeApplication_impl - An error has ocured during of container \"%s\"\n",
                        (*iter).ext_id_.c_str()));
        }
    }
}

void
NodeApplication_Impl::init()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::init()");
  
  /* TODO:  Lets move this stuff to the constructor, shall we?!? */
  /* TODO:  Might be nice to use a component configurator here to load the proper versions
     of the serveractivator.  */

  /* ServerActivator configuration */
  CORBA::ULong spawn;
  const char *cs_path;
  CORBA::Boolean multithread;

  get_property_value ("edu.vanderbilt.dre.CS_Path", this->properties_, cs_path);
  get_property_value ("edu.vanderbilt.dre.SpawnDelay", this->properties_, spawn);
  get_property_value ("edu.vanderbilt.dre.Multithreaded", this->properties_, multithread);
  
  ACE_NEW_THROW_EX (this->activator_,
                    CIAO::Deployment::CIAO_ServerActivator_i (spawn, 
                                                              cs_path,
                                                              multithread,
                                                              this->orb_.in(),
                                                              this->poa_.in()),
                    CORBA::NO_MEMORY ());

  PortableServer::ObjectId_var sa_id =
    this->poa_->activate_object (this->activator_);

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::init - ServerActivator object created\n"));
}

void
NodeApplication_Impl::start ()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::start ()");
  
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::start - started\n"));
  for (TComponents::iterator iter = this->components_.begin();
       iter != this->components_.end();
       ++iter)
    {
      try
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::start - "
                       "configuration_complete started for %s\n", (*iter).ext_id_.c_str()));
          (*iter).int_id_->configuration_complete();
          DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::start - "
                       "configuration_complete finished\n"));
        }
      catch (::Components::InvalidConfiguration& )
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::start - "
                       "Components::CCMObject_var::configuration_complete() "
                       "returned ::Components::InvalidConfiguration exception.\n"));
          throw ::Deployment::StartError();
        }
    }
}

void
NodeApplication_Impl::create_home (unsigned int index)
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::create_home (unsigned int index)");

  
}

void
NodeApplication_Impl::create_container (unsigned int index)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - started\n"));

  Components::ConfigValues config_values;
  ACE_CString processDest;
  CORBA::Any_var feature_any;

  this->create_config_values (this->plan_.instance[index].configProperty,
                              eCreateComponentServer,
                              config_values);

  ::Components::Deployment::ComponentServer_var compServer;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "creating component server for destination: %s\n", processDest.c_str()));
      compServer = this->activator_->create_component_server (config_values);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "component server created\n"));
    }
  catch (::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::CreateFailure exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::Deployment::InvalidConfiguration exception\n"));
      throw ::Deployment::InvalidProperty();
    }

  // COMPONENT_KIND
  this->create_config_values (this->plan_.implementation[this->plan_.instance[index].implementationRef].execParameter,
                              eCreateContainer,
                              config_values);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "creating container for destination: %s\n", processDest.c_str()));
      this->containers_.rebind (processDest.c_str(), compServer->create_container (config_values));
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - container created\n"));
    }
  catch (::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "::Components::Deployment::ComponentServer_var::create_container() "
                   "returned ::Components::CreateFailure exception\n"));
      throw Deployment::StartError();
    }
  catch (::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "::Components::Deployment::ComponentServer_var::create_container() "
                   "returned ::Components::Deployment::InvalidConfiguration exception\n"));
      throw ::Deployment::InvalidProperty();
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - finished\n"));
}

void
NodeApplication_Impl::create_component (unsigned int index)
{
  DANCE_TRACE (DLINFO "NodeApplication_Impl::create_component");
}

void
NodeApplication_Impl::init_components()
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - started\n"));

  Components::ConfigValues config_values;
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
               "Configuring %u component/home instances\n", 
               this->plan_.instance.length()));

  for (unsigned int i = 0; i < this->plan_.instance.length(); i++)
    {
      try
        {
          switch (this->get_instance_type (this->plan_.instance[i].configProperty))
            {
            case eHome:
              {
                //this->configure_home (i);
                break;
              }
            case eComponent:
              {
                //this->configure_component (i);
                break;
              }
            default:
              {
                DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::init_components - "
                             "get_instance_type function returned incorrect instance type\n"));
                throw ::Deployment::InvalidProperty();
              }
            } // switch
        } catch (...)
        {
          DANCE_ERROR((LM_ERROR, DLINFO "Exception was thrown while creating instance \"%s\".\n", this->plan_.instance[i].name.in()));
          throw;
        }
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - finished\n"));
}

NodeApplication_Impl::EInstanceType
NodeApplication_Impl::get_instance_type (const Deployment::Properties& prop) const
{
  DANCE_TRACE (DLINFO "NodeApplication_Impl::get_instance_type");
  return eHome;
}

void
NodeApplication_Impl::create_config_values (const Deployment::Properties& prop,
                                            const ERequestType request,
                                            Components::ConfigValues& cfg) const
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values - started\n"));

  unsigned int ind = 0;
  CORBA::Any_var feature_any;
  switch (request)
    {
    case eCreateComponentServer:
      {
        break;
      }
    case eCreateContainer:
      {

        break;
      }
    case eInstallHome:
      {
        break;
      }
    default:
      {
        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - request is not a know type: eCreateComponentServer, eCreateContainer, eInstallHome, eCreateComponentWithConfigValues\n"));
        throw ::Deployment::InvalidProperty();
      }
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values - finished\n"));
}

Deployment::Connections*
NodeApplication_Impl::getAllConnections()
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - started\n"));

  Deployment::Connections_var conn;
  ACE_NEW_THROW_EX (conn,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());
  unsigned int index = 0;
  /*
  this->redirection_.registration_start (this->node_name_, this->plan_.UUID.in());

  CCMObjects registration
  for (TComponents::iterator it = this->components_.begin();
       !it.done();
       ++it)
    {
      this->redirection_.registration (this->node_name_,
                                       this->plan_.UUID.in(),
                                       (*it).ext_id_,
                                       (*it).int_id_.in());

    }*/
  for (unsigned int i = 0; i < this->plan_.connection.length(); i++)
    {
      for (unsigned int j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              index = conn->length ();
              conn->length (index + 1);
              (*conn) [index].name = CORBA::string_dup (this->plan_.connection[i].name.in());
              Components::CCMObject_var obj;
              ACE_CString inst_name = 
                this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in();
              if (0 != this->components_.find (inst_name, obj))
                {
                  DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                               "component instance for current connection cannot be found\n"));
                  throw ::Deployment::InvalidProperty();
                }
              (*conn) [index].endpoint.length (1L);
              //              ACE_CString path = CCMObjectLocator::TreeNode::mergePath (inst_name.c_str()
              //                                                                        , this->plan_.connection[i].internalEndpoint[j].portName.in());
              switch (this->plan_.connection[i].internalEndpoint[j].kind)
                {
                case Deployment::Facet:
                  {
                    try
                      {
                        ACE_CString name = this->plan_.connection[i].internalEndpoint[j].portName.in();
                        if (name.compare ("supports") != 0)
                          {
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %s endpoint %s started\n", 
                                         this->plan_.connection[i].name.in(), 
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                            CORBA::String_var facet_name = CORBA::string_dup (name.c_str());
                            (*conn) [index].endpoint[0] = obj->provide_facet (facet_name.in());
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %s endpoint %s finished\n", 
                                         this->plan_.connection[i].name.in(), 
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                          }
                        else
                          {
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %s endpoint %s started\n", 
                                         this->plan_.connection[i].name.in(), 
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                            (*conn) [index].endpoint[0] = CORBA::Object::_duplicate (obj.in());
                          }
                        /*                        this->redirection_.registration (this->node_name_,
                                                         this->plan_.UUID.in(),
                                                         inst_name,
                                                         name,
                                                         (*conn) [index].endpoint[0].in());*/
                        //app_node.addChild(path.c_str(), obj->provide_facet(facet_name));
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - provide_facet finished\n"));
                      }
                    catch (::Components::InvalidName& )
                      {
                        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                     "Components::CCMObject_var::provide_facet() returned "
                                     "::Components::InvalidName exception\n"));
                        throw ::Deployment::InvalidProperty();
                      }
                    break;
                  }
                case Deployment::EventConsumer:
                  {
                    try
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                     "get_consumer for connection %s endpoint %s started\n", 
                                     this->plan_.connection[i].name.in(), 
                                     this->plan_.connection[i].internalEndpoint[j].portName.in()));
                        (*conn) [index].endpoint[0] = obj->get_consumer (this->plan_.connection[i].internalEndpoint[j].portName.in());
                        /*this->redirection_.registration (this->node_name_,
                                                         this->plan_.UUID.in(),
                                                         inst_name,
                                                         this->plan_.connection[i].internalEndpoint[j].portName.in(),
                                                         (*conn) [index].endpoint[0].in());*/

                        //app_node.addChild(path.c_str(), obj->get_consumer(this->plan_.connection[i].internalEndpoint[j].portName));
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                     "get_consumer finished\n"));
                      }
                    catch (::Components::InvalidName& )
                      {
                        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                     "Components::CCMObject_var::get_consumer() returned "
                                     "::Components::InvalidName exception\n"));
                        throw ::Deployment::InvalidProperty();
                      }
                    break;
                  }
                default:
                  {
                    DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                 "Connection.InternalEndPoint.Kind is not a "
                                 "Deployment::Facet or Deployment::EventConsumer\n"));
                    throw ::Deployment::InvalidProperty();
                  }
                }
              //              index++;
            }
        }
    }
  /*this->redirection_.registration_finish (this->node_name_, this->plan_.UUID.in());*/
  //this->node_.registerObjects(this->plan_.UUID.in(), app_node);
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - finished\n"));
  return conn._retn();
}

void
NodeApplication_Impl::finishLaunch (const Deployment::Connections & providedReference,
                                    ::CORBA::Boolean start)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - started for connection \n"));
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
               "started for connections sequence with length: %d\n", providedReference.length()));

  for (unsigned int j = 0; j < this->plan_.connection.length(); ++j)
    {
      Components::CCMObject_var obj;
      if (0 != this->components_.find (this->plan_.instance[this->plan_.connection[j].internalEndpoint[0].instanceRef].name.in(), obj))
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                       "component instance for the connection \"%s\" cannot be found.\n", 
                       this->plan_.connection[j].name.in()));
          throw ::Deployment::StartError();
        }

      for (unsigned int i = 0; i < providedReference.length(); ++i)
        {
          //DANCE_DEBUG((LM_DEBUG, "[%M] NodeApplication_impl::finishLaunch - loop on all connections iteration %d for connection %s\n", i, providedReference[i].name.in()));
          ACE_CString name = this->plan_.connection[j].name.in();
          if (name.compare (providedReference[i].name.in()) == 0)
            {
              switch (this->plan_.connection[j].internalEndpoint[0].kind)
                {
                case Deployment::Facet:
                  {
                    DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                                 "set for facet %s \n", name.c_str ()));
                    Components::CCMObject_var ext_inst;
                    try
                      {
                        if (0 == this->plan_.connection[j].externalReference.length())
                          {
                            break;
                          }
                        CORBA::Object_var tmp = 
                          this->orb_->string_to_object (this->plan_.connection[j].externalReference[0].location.in());
                        ext_inst = Components::CCMObject::_narrow (tmp);
                        if (CORBA::is_nil (ext_inst.in()))
                          {
                            DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                                         "facet for %s can't be narrowed \n", name.c_str ()));
                            break;
                          }
                        this->connect_receptacle_ext (ext_inst,
                                                      this->plan_.connection[j].externalReference[0].portName.in(),
                                                      providedReference[i].endpoint[0].in());
                      }
                    catch (CORBA::OBJECT_NOT_EXIST&)
                      {
                        // @@TODO: Shouldn't this be an error?!?
                        break;
                      }
                    catch (CORBA::TRANSIENT&)
                      {
                        // @@TODO: Shouldn't this be an error?!?
                        break;
                      }

                    break;
                  }
                case Deployment::EventConsumer:
                  {
                    DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for consumer \n"));
                    Components::CCMObject_var ext_inst;
                    try
                      {
                        if (0 == this->plan_.connection[j].externalReference.length())
                          {
                            break;
                          }
                        CORBA::Object_var tmp = 
                          this->orb_->string_to_object (this->plan_.connection[j].externalReference[0].location.in());
                        ext_inst = Components::CCMObject::_narrow (tmp);
                        if (CORBA::is_nil (ext_inst.in()))
                          {
                            DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                                         "facet for %s can't be narrowed \n", name.c_str ()));
                            break;
                          }
                        try
                          {
                            // Check is connection kind is consumer to emitter?
                            this->connect_emitter_ext (ext_inst,
                                                       this->plan_.connection[j].externalReference[0].portName.in(),
                                                       providedReference[i].endpoint[0].in());
                          }
                        catch (::Components::InvalidName&)
                          {
                            // No this is consumer to publisher
                            this->connect_publisher (ext_inst,
                                                     this->plan_.connection[j].externalReference[0].portName.in(),
                                                     providedReference[i].endpoint[0].in());
                          }
                      }
                    catch (CORBA::OBJECT_NOT_EXIST&)
                      {
                        break;
                      }
                    catch (CORBA::TRANSIENT&)
                      {
                        break;
                      }
                    break;
                  }
                case Deployment::MultiplexReceptacle:
                case Deployment::SimplexReceptacle:
                  {
                    // What we should do with Cookie, returned from connect call???
                    DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for receptacle \n"));
                    this->connect_receptacle (obj.in(),
                                              this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                              providedReference[i].endpoint[0].in());
                    break;
                  }
                case Deployment::EventEmitter:
                  {
                    DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for emitter \n"));
                    this->connect_emitter (obj.in(),
                                           this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                           providedReference[i].endpoint[0].in());
                    break;
                  }
                case Deployment::EventPublisher:
                  {
                    DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for publisher \n"));
                    this->connect_publisher (obj.in(),
                                             this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                             providedReference[i].endpoint[0].in());
                    break;
                  }
                default:
                  {
                    DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - currect Connection.InternalEndPoint.Kind "
                                 "is not a Deployment::SimplexReceptacle, Deployment::EventEmitter, Deployment::EventPublisher "
                                 "(Connection:%s Kind:%i PortName:%s)\n", 
                                 this->plan_.connection[j].name.in(), 
                                 this->plan_.connection[j].internalEndpoint[0].kind, 
                                 this->plan_.connection[j].internalEndpoint[0].portName.in()
                                 ));
                    throw ::Deployment::InvalidConnection();
                  }//default
                }//switch
            }//if(name.compare(providedReference[i].name.in()) == 0)
        }//for ( unsigned int i = 0; i < providedReference.length(); ++i )
    }//for ( unsigned int j = 0; j < this->plan_.connection.length(); ++j )


  if (start)
    {
      this->start();
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - finished\n"));
}

Components::Cookie*
NodeApplication_Impl::connect_receptacle (Components::CCMObject_ptr inst,
                                          const ACE_CString& port_name,
                                          CORBA::Object_ptr facet)
{
  Components::Cookie* res = 0;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect SimplexReceptacle for %s started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  return res;
}

Components::Cookie*
NodeApplication_Impl::connect_receptacle_ext (Components::CCMObject_ptr inst,
                                              const ACE_CString& port_name,
                                              CORBA::Object_ptr facet)
{
  Components::Cookie* res = 0;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect SimplexReceptacle for %s started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      //throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  return res;
}

void
NodeApplication_Impl::connect_emitter (Components::CCMObject_ptr inst,
                                       const ACE_CString& port_name,
                                       CORBA::Object_ptr consumer)
{
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer for %s started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
}

void
NodeApplication_Impl::connect_emitter_ext (Components::CCMObject_ptr inst,
                                           const ACE_CString& port_name,
                                           CORBA::Object_ptr consumer)
{
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer for %s started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect_consumer finished\n"));
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_DEBUG ( (LM_WARNING, "NodeApplication_impl::finishLaunch - "
                     "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      //throw ::Deployment::InvalidConnection();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
}

Components::Cookie*
NodeApplication_Impl::connect_publisher (Components::CCMObject_ptr inst,
                                         const ACE_CString& port_name,
                                         CORBA::Object_ptr consumer)
{
  Components::Cookie* res = 0;
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - subscribe for %s started\n", port_name.c_str()));
      res = inst->subscribe (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - subscribe finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::ExceededCOnnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  return res;
}

void NodeApplication_Impl::create_config_values(const Deployment::Properties& prop, 
                                                Components::ConfigValues& cfg) const
{
  ACE_CString cdmw_name = "cdmw.config.";
  CORBA::ULong len = prop.length();
  unsigned int ind = 0;
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_CString s = prop[i].name.in();
      if (0 == s.find(cdmw_name))
        {
          cfg.length(ind+1);
          cfg[ind++] = new CIAO::ConfigValue_impl (s.substring(cdmw_name.length()).c_str(), prop[i].value);
        }
    }
}

