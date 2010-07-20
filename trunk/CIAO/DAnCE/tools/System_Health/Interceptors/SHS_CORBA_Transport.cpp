// $Id$

#include "SHS_CORBA_Transport.h"
#include "orbsvcs/CoSNamingC.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"

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
          if ((ACE_OS::strcmp (props[i].name.in (),
                              DAnCE::LOCALITY_NAMINGCONTEXT) == 0) ||
              (ACE_OS::strcmp (props[i].name.in (),
                               DAnCE::INSTANCE_NC) == 0))
            {
              CORBA::Object_var obj;
              props[i].value >>= CORBA::Any::to_object (obj);
              
              CORBA::is_nil (obj);
                
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
