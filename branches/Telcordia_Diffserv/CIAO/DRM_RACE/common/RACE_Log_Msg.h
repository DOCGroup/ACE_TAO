/**
 * @file RACE_Log_Msg.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * Logging macros used in RACE.
 */

#include "common/RACE_commonC.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Log
    {
      CORBA::Long find_importance (const ::Deployment::PackageConfiguration &package)
      {
	if (package.basePackage.length () == 0)
	  return 0;

        const Deployment::Properties &configProperties =
          package.basePackage[0].implementation[0].referencedImplementation.configProperty;

        for (CORBA::ULong i = 0; i < configProperties.length (); ++i)
          {
            if (ACE_OS::strcmp ("Importance", configProperties[i].name.in ()) == 0)
              {
                CORBA::Long retval (0);
                configProperties[i].value >>= retval;
                return retval;
              }
          }

        return 0;
      }

      struct EventType
      {
        enum EventTypeEnum
          {
            INVALID = 0,
            MANUAL_PLAN = 1,
	    AUTO_PLAN = 2,
            TYPE_MAX=99
          };
      };

      struct SubType
      {
        enum SubTypeEnum
          {
            INVALID = 0,
            DEPLOY_BEGIN = 1,
            DEPLOY_END = 2,
            TEARDOWN_BEGIN = 3,
            TEARDOWN_END = 4,
            PLANNING_BEGIN = 5,
            PLANNING_END = 6,
            SWAPOUT_BEGIN = 7,
            SWAPOUT_END = 8,
            COMPONENT_DEPLOY = 9,
            COMPONENT_TEARDOWN = 10,
            DEPLOYED_PLAN_FAILED = 11,
            SUBTYPE_MAX = 99
          };
      };

      struct Status
      {
        enum StatusEnum
          {
            SUCCESS = 0,
            FAILURE = 1,

            STATUS_MAX = 99
          };
      };

      struct EventMark { 
	  enum EventMarkEnum { 
	     BEGIN_EVENT, END_EVENT,  // Used with actions from planners
	     BEGIN_PLAN, END_PLAN,    // Used by planner mgr for planning
	     COMPONENT_DEPLOY         // Used by planner mgr for details
	  };
      };

      inline bool getEventType( 
	     ::CIAO::RACE::Action action,  
	     EventMark::EventMarkEnum mark,
	     Log::EventType::EventTypeEnum& evt,
	     Log::SubType::SubTypeEnum& svt )
      {
	  // update this switch when changes to action require mapping to EventType
	  switch (action)
	  {
		  case ::CIAO::RACE::TEARDOWN:
		  case ::CIAO::RACE::DEPLOY:
		  case ::CIAO::RACE::AUTO_TEARDOWN:
		          evt = Log::EventType::MANUAL_PLAN;
			  break;
		  case ::CIAO::RACE::REPLAN_DEPLOY:
		  case ::CIAO::RACE::REPLAN_TEARDOWN:
		  case ::CIAO::RACE::REPLAN_AUTO_TEARDOWN:
		          evt = Log::EventType::AUTO_PLAN;
			  break;
	          default:
                      ACE_DEBUG ((LM_DEBUG, "RACE Log: !!Unhandled Action Type!! ... Bailing out!\n"));
		      return false;
	  }

	  // update these switches when changes to action require mapping to SubType
	  switch (mark)
	  {
		  case EventMark::BEGIN_PLAN:
			  svt = SubType::PLANNING_BEGIN;
			  break;
		  case EventMark::END_PLAN:
			  svt = SubType::PLANNING_END;
		          break;
		  case EventMark::COMPONENT_DEPLOY:
		          switch (action)
		          {
		             case ::CIAO::RACE::TEARDOWN: 
			     case ::CIAO::RACE::AUTO_TEARDOWN: 
			     case ::CIAO::RACE::REPLAN_TEARDOWN: 
			     case ::CIAO::RACE::REPLAN_AUTO_TEARDOWN:
			             svt = SubType::COMPONENT_TEARDOWN;
				     break;
			     case ::CIAO::RACE::DEPLOY: 
			     case ::CIAO::RACE::REPLAN_DEPLOY: 
			             svt = SubType::COMPONENT_DEPLOY;
				     break;
	                     default:
                                ACE_DEBUG ((LM_DEBUG, 
	                          "RACE Log: !!Unhandled Action Type!! ... Bailing out!\n"));
		                return false;
			  }
			  break;
		  case EventMark::BEGIN_EVENT:
		  case EventMark::END_EVENT:
	                  switch (action)
	                  {
		             case ::CIAO::RACE::TEARDOWN:
		             case ::CIAO::RACE::REPLAN_TEARDOWN:
			             svt = (mark==EventMark::BEGIN_EVENT)?
			                 Log::SubType::TEARDOWN_BEGIN:Log::SubType::TEARDOWN_END;
			             break;

		             case ::CIAO::RACE::DEPLOY:
		             case ::CIAO::RACE::REPLAN_DEPLOY:
			             svt = (mark==EventMark::BEGIN_EVENT)?
			                 Log::SubType::DEPLOY_BEGIN:Log::SubType::DEPLOY_END;
			             break;

           		     case ::CIAO::RACE::AUTO_TEARDOWN:
	           	     case ::CIAO::RACE::REPLAN_AUTO_TEARDOWN:
		           	     svt = (mark==EventMark::BEGIN_EVENT)?
			                 Log::SubType::SWAPOUT_BEGIN:Log::SubType::SWAPOUT_END;
			             break;
			     default:
                                ACE_DEBUG(( LM_DEBUG, "RACE Log: !!Unhandled Action!!... Bailing out!\n"));
				return false;
	                  }
		          break;
	          default:
                     ACE_DEBUG(( LM_DEBUG, "RACE Log: !!Unhandled EventMark!!... Bailing out!\n"));
		     return false;

	  }

	  return true;
      }
	  

      inline void RACE_DEPLOYMENT_STATUS ( 
           EventType::EventTypeEnum etype,
           SubType::SubTypeEnum subtype,
           const char *UUID,
           CORBA::ULong sequence,
           Status::StatusEnum status,
           const char *arg1,
           CORBA::Long arg2,
           const char *arg3)
      {
        ACE_DEBUG((LM_INFO, "B;%i;%i;RACE;%s;%i;%i;3;%s;%d;%s\n",
                   etype,
                   subtype,
                   UUID,
                   sequence,
                   status,
                   arg1,
                   arg2,
		   arg3  ));
      }

      inline void RACE_DEPLOYMENT_STATUS ( 
	  ::CIAO::RACE::Action action,  
	  EventMark::EventMarkEnum mark,
          const char *UUID,
          CORBA::ULong sequence,
          Status::StatusEnum status,
           const char *arg1,
           CORBA::Long arg2,
           const char *arg3  )
      {
	  EventType::EventTypeEnum evt;
	  SubType::SubTypeEnum svt;

	  if (getEventType( action, mark, evt, svt ))
             RACE_DEPLOYMENT_STATUS( 
	        evt, svt, UUID, sequence, status, arg1, arg2, arg3 );
	  
	  return;
      }


      inline void RACE_DEPLOYMENT_STATUS ( 
           EventType::EventTypeEnum etype,
           SubType::SubTypeEnum subtype,
           const char *UUID,
           CORBA::ULong sequence,
           Status::StatusEnum status,
           CORBA::Long arg1,
           const char *arg2)
      {
        ACE_DEBUG((LM_INFO, "B;%i;%i;RACE;%s;%i;%i;2;%d;%s\n",
                   etype,
                   subtype,
                   UUID,
                   sequence,
                   status,
                   arg1,
                   arg2));
      }

      inline void RACE_DEPLOYMENT_STATUS ( 
	  ::CIAO::RACE::Action action,  
	  EventMark::EventMarkEnum mark,
          const char *UUID,
          CORBA::ULong sequence,
          Status::StatusEnum status,
          CORBA::Long arg1,
          const char *arg2 )
      {
	  EventType::EventTypeEnum evt;
	  SubType::SubTypeEnum svt;

	  if (getEventType( action, mark, evt, svt ))
             RACE_DEPLOYMENT_STATUS( 
	        evt, svt, UUID, sequence, status, arg1, arg2 );
	  
	  return;
      }

      inline void RACE_DEPLOYMENT_STATUS (
           EventType::EventTypeEnum etype,
           SubType::SubTypeEnum subtype,
           const char *UUID,
           CORBA::ULong sequence,
           Status::StatusEnum status,
           const char *message)
      {
        ACE_DEBUG((LM_INFO, "B;%i;%i;RACE;%s;%i;%i;1;%s\n",
                   etype,
                   subtype,
                   UUID,
                   sequence,
                   status,
                   message));
      }

      inline void RACE_DEPLOYMENT_STATUS (
	  ::CIAO::RACE::Action action,  
	  EventMark::EventMarkEnum mark,
          const char *UUID,
          CORBA::ULong sequence,
          Status::StatusEnum status,
          const char *message)
      {
	  EventType::EventTypeEnum evt;
	  SubType::SubTypeEnum svt;

	  if (getEventType( action, mark, evt, svt ))
             RACE_DEPLOYMENT_STATUS ( 
	        evt, svt, UUID, sequence, status, message );
	  
	  return;
      }
    }
  }
}

using CIAO::RACE::Log::RACE_DEPLOYMENT_STATUS;
