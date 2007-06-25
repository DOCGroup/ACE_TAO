// MonitorController.cpp,v 1.7 2006/02/02 16:32:26 wotte Exp
//----------------------------------------------------------------------------------
/**
 * @file MonitorController.cpp
 *
 * @brief The Monitor Controller implementation.
 *
 * This is the facade class for Monitor
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//----------------------------------------------------------------------------------

#include "orbsvcs/CosNamingC.h"
#include "MonitorController.h"
#include "BaseMonitor.h"
#include "CIAO_common.h"


#include "ace/Log_Msg.h"
#include "ace/DLL.h"
#include "ace/SString.h"

#include "NodeManager_Impl.h"

#include "MonitorCB.h"


namespace CIAO
{
  typedef MonitorBase* (*MonitorFactory) ();

  /// for the CIAO monitor
  const char* monitor_lib_name = "ciaomonlib";

  /// na monitor lib name
  const char* na_monitor_lib_name = "namonlib";


  // The interval after which update will be sent.
  // This value will sent by the EM in the later implementation
  const int interval = 10;

  static const char* factory_func = "createMonitor";
}

CIAO::MonitorController::MonitorController (CORBA::ORB_ptr orb,
                                            const ::Deployment::Domain& domain,
                                            ::CIAO::NodeManager_Impl_Base* node_mgr
                                            )
  : terminate_flag_ (0),
    orb_ (orb),
    initial_domain_ (domain),
    node_mgr_ (node_mgr),
    monitor_cpu_usage_ (0),
    monitor_NA_usage_ (0),
    add_component_pid_ (1)
{
}

int CIAO::MonitorController::init ()
{
  ACE_DEBUG ((LM_DEBUG , "Inside the init function [%s]\n", 
	          initial_domain_.node[0].name.in ()));


  create_the_servant ();

  ACE_DEBUG ((LM_DEBUG , "After the create servant function [%s]\n", 
	          initial_domain_.node[0].name.in ()));
  upload_obj_ref ();

  ACE_DEBUG ((LM_DEBUG , "After the upload obj ref function [%s]\n", 
	          initial_domain_.node[0].name.in ()));
    // Parse the intial domain and setup the options
  parse_initial_domain ();
  ACE_DEBUG ((LM_DEBUG , "After the parse_initial_domain function [%s]\n", 
	          initial_domain_.node[0].name.in ()));
    
    // Populat and startup the monitor list
  populate_monitor_list ();
  ACE_DEBUG ((LM_DEBUG , "After the populate monitor list function [%s]\n", 
	          initial_domain_.node[0].name.in ()));

  return 1;
}

::Deployment::Domain* CIAO::MonitorController::update_data_for_TM ()
{
	ACE_DEBUG ((LM_DEBUG , "The list size is %d\n",
		    monitor_list_.size ()));

  ::Deployment::Domain* domain = 
    new ::Deployment::Domain (this->initial_domain_);

  // Set the new domain resource to zero
  domain->node[0].resource.length (0);


  for (size_t i =0;i < monitor_list_.size ();i++)
  {

    ::Deployment::Domain *new_domain =
      monitor_list_[i]->monitor_->get_current_data ();

    add_resource_to_domain (*domain , *new_domain);
  }

  return domain;
}

int CIAO::MonitorController::svc (void)
{
  ACE_DEBUG ((LM_DEBUG , "Inside the Node for [%s]\n", 
	      initial_domain_.node[0].name.in ()));
  

    // Parse the intial domain and setup the options
  parse_initial_domain ();
    
    // Populat and startup the monitor list
  populate_monitor_list ();

    // Wait for system to stabilize itself
  ACE_OS::sleep (interval);

    // The loop in which UpdateData is called
    while (!terminating ())
      {

	ACE_DEBUG ((LM_DEBUG , "The list size is %d\n",
		    monitor_list_.size ()));


        ::Deployment::Domain domain = this->initial_domain_;

        // Set the new domain resource to zero
        domain.node[0].resource.length (0);


        for (size_t i =0;i < monitor_list_.size ();i++)
          {

            ::Deployment::Domain *new_domain =
              monitor_list_[i]->monitor_->get_current_data ();

             add_resource_to_domain (domain , *new_domain);
          }


        // ****** add component data *******************

        NodeManager_Impl_Base::Component_Ids cids =
          node_mgr_->get_component_detail ();

//          ACE_DEBUG ((LM_DEBUG , "\nThe process id is [%d]\n",
//                      CORBA::Long (cids.process_id_)));


        // Here save the old resource length
        int counter = domain.node[0].resource.length ();

        // if pid is already added , dont add
        if (add_component_pid_)
          {
            // then add more resource element to the
            // domain structure
            // ACE_DEBUG ((LM_DEBUG , "Going to add CID/PID data\n"));
            int new_res_size = domain.node[0].resource.length () +
              cids.cid_seq_.size ();

            domain.node[0].resource.length (new_res_size);

            ACE_Unbounded_Set_Iterator<ACE_CString> iter (cids.cid_seq_);

//             for (iter = cids.cid_seq_.begin ();
//                  iter != cids.cid_seq_.end ();
//                  iter++,counter++)
//               {
//                 domain.node[0].resource[counter].name =
//                   CORBA::string_dup ("Component");
//                 domain.node[0].resource[counter].resourceType.length (0);

//                 // Have one property for now
//                 domain.node[0].resource[counter].property.length (1);
//                 domain.node[0].resource[counter].property[0].name =
//                   CORBA::string_dup ((*iter).c_str ());
//                 domain.node[0].resource[counter].property[0].kind =
//                   ::Deployment::Quantity;
//                 domain.node[0].resource[counter].property[0].dynamic =
//                   0;
//                 domain.node[0].resource[counter].property[0].value <<=
//                   CORBA::Long (cids.process_id_);

//                 //                ACE_DEBUG ((LM_DEBUG , "The process id is [%d]\n",
//                 //                            CORBA::Long (cids.process_id_)));
//               }
            // set the add_component_pid_ to 0
            add_component_pid_ = 0;
          }

        //******add compoennt data


        // data will be updated in intervals of 10 secs.
        // in the latest version of spec , this value will
        // come from Execution Manager
        ACE_OS::sleep (interval);

      }

  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::Terminating Monitor\n"));
    }
 return 0;
}

CIAO::MonitorController::~MonitorController ()
{
  ACE_DEBUG ((LM_DEBUG , "CIAO::MonitorController::Destructor\n"));
  terminate ();
  wait ();
}

void CIAO::MonitorController::terminate ()
{
  // make the terminate flag false
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard,
             lock_
             );
  //Ace_DEBUG ((LM_DEBUG , "WITHIN TERMINATE CALL  ......\n"));
  terminate_flag_=1;
}

bool CIAO::MonitorController::terminating ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    guard,
                    lock_,
                    0
                    );
  return terminate_flag_;
}


void CIAO::MonitorController::parse_initial_domain ()
{
  for (unsigned int i = 0;
       i < initial_domain_.node[0].resource.length ();
       i++)
    {
      // check if cpu usage to be monitored or not
      if (!strcmp (initial_domain_.node[0].resource[i].name, "Processor"))
        monitor_cpu_usage_ = 1;

      ACE_DEBUG ((LM_DEBUG, "Initial Domain,Resource Name %s\n",
                  initial_domain_.node[0].resource[i].name.in () ));

      // check if NA usage to be monitored or not
      if (!strcmp (initial_domain_.node[0].resource[i].name, "NA_Monitor"))
        monitor_NA_usage_ = 1;
    }
}

void CIAO::MonitorController::populate_monitor_list ()
{

  MonitorElement* monitor_elem;
  if (monitor_cpu_usage_)
    {
      // create a MonitorElement
      monitor_elem = new MonitorElement;

      // forming the library name
      monitor_elem->lib_name_ = ACE_DLL_PREFIX;
      monitor_elem->lib_name_  += monitor_lib_name;

      int retval
        = monitor_elem->dll_.open (monitor_elem->lib_name_.c_str ());

      ACE_DEBUG ((LM_DEBUG, "Monitor Lib name %s\n",
                  monitor_elem->lib_name_.c_str () ));

      if (retval != 0)
        {
          ACE_ERROR ((LM_ERROR,"%p","dll.open"));
          return;
        }

      MonitorFactory factory =
        (MonitorFactory) monitor_elem->dll_.symbol (factory_func);

      if (factory == 0)
        {
          ACE_ERROR ((LM_ERROR,"%p","dll.symbol"));
          return;
        }

      // here creating the monitor object
        monitor_elem->monitor_ = (MonitorBase*) factory ();

        ACE_DEBUG ((LM_DEBUG, "Inside the init call\n"));

        // get the resource specific data ..

        ::Deployment::Domain resource_domain_ =
            this->create_monitor_domain ("Processor");

        monitor_elem->monitor_->initialize_params (resource_domain_,
                                                   interval);

        monitor_elem->monitor_->set_context (this);

        monitor_list_.push_back (monitor_elem);

        ACE_DEBUG ((LM_DEBUG , "The list size is %d\n",
              monitor_list_.size ()));

    } // if monitor_cpu_usage
  if (monitor_NA_usage_)
    {
      // create a MonitorElement
      monitor_elem = new MonitorElement;

      // forming the library name
      monitor_elem->lib_name_ = ACE_DLL_PREFIX;
      monitor_elem->lib_name_  += na_monitor_lib_name;

      int retval
        = monitor_elem->dll_.open (monitor_elem->lib_name_.c_str ());

      ACE_DEBUG ((LM_DEBUG, "NA Monitor Lib name %s\n",
                  monitor_elem->lib_name_.c_str () ));

      if (retval != 0)
        {
          ACE_ERROR ((LM_ERROR,"%p","dll.open"));
          return;
        }

      MonitorFactory factory =
        (MonitorFactory) monitor_elem->dll_.symbol (factory_func);

      if (factory == 0)
        {
          ACE_ERROR ((LM_ERROR,"%p","dll.symbol"));
          return;
        }


        // here creating the monitor object
        monitor_elem->monitor_ = (MonitorBase*) factory ();

        ACE_DEBUG ((LM_DEBUG, "Inside the NA init call\n"));

        // get the resource specific data ..

        ::Deployment::Domain resource_domain_ =
            this->create_monitor_domain ("NA_Monitor");


        monitor_elem->monitor_->initialize_params (resource_domain_,
                                                   interval);


        monitor_elem->monitor_->set_context (this);

        monitor_list_.push_back (monitor_elem);

    }
}

CIAO::NodeManager_Impl_Base::Component_Ids
CIAO::MonitorController::get_component_process ()
{
    return node_mgr_->get_component_detail ();
}

::Deployment::Domain CIAO::MonitorController::
create_monitor_domain (const char * resource)
{
  ::Deployment::Domain domain;

  domain.UUID = CORBA::string_dup (initial_domain_.UUID);

  domain.label = CORBA::string_dup (initial_domain_.label);

  domain.sharedResource = initial_domain_.sharedResource;
  domain.interconnect = initial_domain_.interconnect;
  domain.bridge = initial_domain_.bridge;
  domain.infoProperty = initial_domain_.infoProperty;

  // set the node ...
  domain.node.length (1);

  domain.node[0].name =
    CORBA::string_dup (initial_domain_.node[0].name);

  domain.node[0].resource.length (1);


  for (size_t i = 0;
       i < initial_domain_.node[0].resource.length ();
       i++)
    {
      if (!strcmp (initial_domain_.node[0].resource[i].name, resource))
        domain.node[0].resource[0] =
          initial_domain_.node[0].resource[i];
    }

  return domain;
}

void CIAO::MonitorController::
add_resource_to_domain (::Deployment::Domain& new_domain,
                        ::Deployment::Domain monitored_domain)
{
  size_t current_size = new_domain.node[0].resource.length ();

  new_domain.node[0].resource.length (current_size + 1);

  //  ACE_DEBUG ((LM_DEBUG , "Adding the resource \n"));

  new_domain.node[0].resource[current_size] =
    monitored_domain.node[0].resource[0];
}

void CIAO::MonitorController::upload_obj_ref ()
{
  CORBA::Object_var naming_context_object =
    orb_->resolve_initial_references ("NameService");
  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_context_object.in ());
 
 // check for context first
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup ("Node_Monitor");

  try
  {
    naming_context->resolve (name);
  }
  catch (CosNaming::NamingContext::NotFound& ex)
  {
    ex._tao_print_exception ("MonitorController::upload_obj_ref\t\n"); 

    try
    {
      naming_context->bind_new_context (name);
    }
    catch (CORBA::Exception & e)
    {
      ex._tao_print_exception ("MonitorController::upload_obj_ref:bind_new_context\t\n"); 
      return;
    }

  }

  name.length (2);

  name[0].id = CORBA::string_dup ("Node_Monitor");
  name[1].id = CORBA::string_dup (initial_domain_.node[0].name.in ());

  try
  {
    naming_context->bind (name, monitorv_);
  }
  catch (CORBA::Exception& ex)
  {
    ex._tao_print_exception ("MonitorController::upload_obj_ref\t\n"); 
    return;
  }
}

void CIAO::MonitorController::create_the_servant ()
{
  CORBA::Object_var poa_object =
      orb_->resolve_initial_references ("RootPOA");

  poa_ = PortableServer::POA::_narrow (poa_object.in ());
  
  monitor_i_ = new Onl_Monitor_NM_Monitor_i (this, this->node_mgr_);  
  
  monitorv_ = monitor_i_->_this ();
}

auto_ptr<Deployment::Domain>  
CIAO::MonitorController::get_initial_domain ()
{
  auto_ptr<Deployment::Domain> domain (new ::Deployment::Domain (this->initial_domain_));
  return domain; 
}
