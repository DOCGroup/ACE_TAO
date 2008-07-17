 /**
  * @file PIP_Connection_Manager.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  */

#ifndef _PIP_CONNECTION_MANAGER_H_
#define _PIP_CONNECTION_MANAGER_H_

#include <ace/Connector.h>
#include <ace/PIP_Active_IO_Handler.h>
#include <ace/PIP_Reactive_IO_Handler.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Connector.h>
#include <ace/Vector_T.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class ACE_Export ACE_PIP_Connection_Manager
{
  public:

   /// Informationa associated with a connection
   struct Connection_Definition
   {
         enum Handler_Type {ACTIVE, REACTIVE};

         ACE_UINT32   source_site_id;
         ACE_UINT32   destination_site_id;
         std::string  address;
         u_short      port;
         ACE_UINT32   priority;
         Handler_Type type;
   };
   
   /// Default Constructor
   ACE_PIP_Connection_Manager();

   /// Destructor
   virtual ~ACE_PIP_Connection_Manager();

   /// obtain the single instance of the manager
   static ACE_PIP_Connection_Manager* instance();

  /// Extract all connection information from a file
   virtual int process_connection_file(char* filename);

   /// Establish all connection for which source_site_id is the source
   virtual int establish_connections(ACE_UINT32 source_site_id);

   const ACE_Vector<Connection_Definition*>* get_connections() const;
   
  private:

   ACE_Vector<Connection_Definition*>* connection_definitions_;

   // The connector used to actively connect to a remote site
   ACE_Connector<
      ACE_PIP_Active_IO_Handler, 
      ACE_SOCK_Connector> active_connector_;

   ACE_Connector<
      ACE_PIP_Reactive_IO_Handler, 
      ACE_SOCK_Connector> reactive_connector_;
   
   static ACE_PIP_Connection_Manager* connection_manager_;
   static ACE_Mutex instance_lock_;
   static bool      delete_manager_;

   std::vector<ACE_PIP_IO_Handler*> handlers_;
};

#endif
