// $Id$
#ifndef POLLER_H
#define POLLER_H
#include "cuts/CUTSC.h"
#include "TargetManager/TargetManagerExtC.h"
#include "ace/Task.h"
#include "ace/Time_Value.h"
#include "ace/Hash_Map_Manager_T.h"


namespace CIAO
{

  namespace RACE
  {

    struct Component_Info
    {
      /// Component ID.
      CORBA::String_var id_;

      /// Name of the node where the component is deployed.
      CORBA::String_var node_;

      /// Reference to the NodeManager where the component is running.
      ::CIAO::NodeManagerDaemon_var node_mgr_;
    };


    struct App_String
    {
      /// Critical path.
      CUTS::Path_Sequence  critical_path_;

      /// Critical path deadline.
      CORBA::Double deadline_;

      /// ID obtained by registering this string with the BDC.
      CORBA::Long cuts_id_;

      /// Plan ID obtained from the DeploymentPlan IDL.
      CORBA::String_var plan_id_;

      /// Plan label obtained from the DeploymentPlan IDL.
      CORBA::String_var label_;


      /// ACE_Vector containing the Component_Info for all components of this
      /// app_string.
      ACE_Vector <Component_Info> components_;

    };

    class Poller : public ACE_Task_Base
    {
    public:

      Poller (CIAO::TargetManagerExt_ptr TM_ptr,
              CUTS::Path_Measurement_ptr PM_ptr,
              CUTS::BDC_Control_Handle_ptr BDC_ptr,
              size_t interval);



      void init ();

      int start_poller ();

      int stop_poller ();

      void start_controller ();

      void stop_controller ();

      int register_string (App_String & string);

      int unregister_string (const char * UUID);

      int svc ();


    protected:

      void populate_info (App_String & string);

      void perform_control (const App_String & string);


    private:

      CIAO::TargetManagerExt_var TM_;

      CUTS::Path_Measurement_var PM_;

      CUTS::BDC_Control_Handle_var BDC_;

      ACE_Time_Value interval_;

      bool active_;


      //      ACE_Vector <App_String> strings_;
      typedef  ACE_Hash_Map_Manager_Ex<ACE_CString,
                                      App_String,
                                      ACE_Hash<ACE_CString>,
                                      ACE_Equal_To<ACE_CString>,
                                      ACE_Thread_Mutex> App_Strings_Map;

      App_Strings_Map strings_;

      typedef App_Strings_Map::iterator Strings_Iterator;

      CIAO::Host_NodeManager_seq_var node_manager_seq_;

      FILE *controller_log_;

      bool initialized_;

      bool perform_control_;

    };
  }
}

#endif /*POLLER_H */
