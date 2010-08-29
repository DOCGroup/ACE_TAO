// $Id$

#include "SHS_CORBA_Transport.h"
#include "orbsvcs/CosNamingC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "LocalityManager/Scheduler/Plugin_Manager.h"

namespace DAnCE
{
  namespace SHS
  {
    SHS_CORBA_Transport::SHS_CORBA_Transport (void)
    {
    }
    
    SHS_CORBA_Transport::~SHS_CORBA_Transport (void)
    {
    }

    void
    SHS_CORBA_Transport::configure (const Deployment::Properties &props)
    {
      CosNaming::NamingContext_var ctx;

      for (CORBA::ULong i = 0; i < props.length (); ++i)
        {
	  DANCE_DEBUG (10, (LM_TRACE, DLINFO
			    ACE_TEXT ("SHS_CORBA_Transport::configure - ")
			    ACE_TEXT ("Considering property <%C>\n"),
			    props[i].name.in ()));

          if ((ACE_OS::strcmp (props[i].name.in (),
                              DAnCE::LOCALITY_NAMINGCONTEXT) == 0) ||
              (ACE_OS::strcmp (props[i].name.in (),
                               DAnCE::INSTANCE_NC) == 0))
            {
	      DANCE_DEBUG (7, (LM_DEBUG, DLINFO
			       ACE_TEXT ("SHS_CORBA_Transport::configure - ")
			       ACE_TEXT ("Found property <%C>\n"),
			       props[i].name.in ()));
              
              CORBA::Object_var obj;
              
              if (!(props[i].value >>= CORBA::Any::to_object (obj)))
                {
                  const char *val;
                  props[i].value >>= CORBA::Any::to_string (val, 0);
                  
                  CORBA::ORB_var orb = PLUGIN_MANAGER::instance ()->get_orb ();
                  obj = orb->string_to_object (val);
                }
              
              if (CORBA::is_nil (obj))
		{
		  DANCE_ERROR (3, (LM_NOTICE, DLINFO
				   ACE_TEXT ("Unable to extract an object reference from ")
				   ACE_TEXT ("property <%C>\n"),
				   props[i].name.in ()));
		  return;
		}
                
              ctx = CosNaming::NamingContext::_narrow (obj.in ());
              break;
            }
        }
    
      if (CORBA::is_nil (ctx))
        {
          DANCE_ERROR (3, (LM_NOTICE, DLINFO
                           ACE_TEXT ("SHS_CORBA_Transport::configure - ")
                           ACE_TEXT ("Failed to get a naming context\n")));
          return;
        }
    
      try
        {
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = "DAnCE.SystemHealthDaemon";
        
          CORBA::Object_var obj = ctx->resolve (name);
        
          this->shs_daemon_ = DAnCE::SHS::SHS_Daemon::_narrow (obj.in ());
        
          if (CORBA::is_nil (this->shs_daemon_))
            DANCE_ERROR (3, (LM_NOTICE, DLINFO
                             ACE_TEXT ("SHS_CORBA_Transport::configure - ")
                             ACE_TEXT ("Unable to resolve SHS Daemon\n")));
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (3, (LM_NOTICE, DLINFO
                           ACE_TEXT ("SHS_CORBA_Transport::configure - ")
                           ACE_TEXT ("Unable to resolve SHS Daemon: %C\n"),
                           ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (3, (LM_NOTICE, DLINFO
                           ACE_TEXT ("SHS_CORBA_Transport::configure - ")
                           ACE_TEXT ("Unable to resolve SHS Daemon: Unknown C++ exception\n")));
        }
    }
      
    void
    SHS_CORBA_Transport::push_event (const Status_Update &update)
    {
      try
        {
          if (!CORBA::is_nil (this->shs_daemon_))
            {
              this->shs_daemon_->update (update);
            }
          else
            DANCE_DEBUG (9, (LM_TRACE, DLINFO 
                             ACE_TEXT ("SHS_CORBA_Transport::push_event - ")
                             ACE_TEXT ("Unable to publish SHS status update, nil daemon ref\n")));
          
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (3, (LM_ERROR, DLINFO
                           ACE_TEXT ("SHS_CORBA_Transport::push_event - ")
                           ACE_TEXT ("Caught CORBA Exception: <%C>\n"),
                           ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (3, (LM_ERROR, DLINFO
                           ACE_TEXT ("SHS_CORBA_Transport::push_event - ")
                           ACE_TEXT ("Caught C++ Exception\n")));
        }
    }
  }
}
