 /**
  * @file PIP_Connection_Manager.cpp
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  */

#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_string.h>
#include <stdio.h>
#include <ace/INET_Addr.h>
#include <ace/PIP_Connection_Manager.h>


ACE_PIP_Connection_Manager* ACE_PIP_Connection_Manager::connection_manager_ = 0;
ACE_Mutex                   ACE_PIP_Connection_Manager::instance_lock_;
bool                        ACE_PIP_Connection_Manager::delete_manager_ = false;

/// Default Constructor
ACE_PIP_Connection_Manager::ACE_PIP_Connection_Manager()
{
}

/// Destructor
ACE_PIP_Connection_Manager::~ACE_PIP_Connection_Manager()
{

}

ACE_PIP_Connection_Manager* ACE_PIP_Connection_Manager::instance()
{
   if (connection_manager_ == 0)
   {
      instance_lock_.acquire();

      if (ACE_PIP_Connection_Manager::connection_manager_ == 0)
      {
         ACE_NEW_RETURN (ACE_PIP_Connection_Manager::connection_manager_,
                         ACE_PIP_Connection_Manager,
                         0);

         delete_manager_ = true;
      }

      instance_lock_.release();
   }

   return connection_manager_;
}

int ACE_PIP_Connection_Manager::establish_connections(ACE_UINT32 source_site_id)
{
   int result(0);

   //establish connections
   for (unsigned i = 0; i < (this->connection_definitions_)->size(); ++i)
   {
      if ((*(this->connection_definitions_))[i]->source_site_id ==
          source_site_id)
      {
         ACE_INET_Addr address;
         address.set((*(this->connection_definitions_))[i]->port,
                     (*(this->connection_definitions_))[i]->address);

        if ((*(this->connection_definitions_))[i]->type ==
            Connection_Definition::ACTIVE)
        {
           ACE_PIP_Active_IO_Handler* handler(0);
           ACE_NEW_RETURN (handler, ACE_PIP_Active_IO_Handler, 0);

           result = this->active_connector_.connect(handler, address);
           if (result == -1)
           {
              return -1;
           }
           else
           {
              handler->init(
                 (*(this->connection_definitions_))[i]->source_site_id,
                 (*(this->connection_definitions_))[i]->destination_site_id,
                 (*(this->connection_definitions_))[i]->priority);

              this->handlers_.push_back(handler);
           }
        }
        else
        {
           ACE_PIP_Reactive_IO_Handler* handler(0);
           ACE_NEW_RETURN(handler, ACE_PIP_Reactive_IO_Handler, 0);
           result = this->reactive_connector_.connect(handler, address);
           if (result == -1)
           {
              return -1;
           }
           else
           {
              handler->init(
                 (*(this->connection_definitions_))[i]->source_site_id,
                 (*(this->connection_definitions_))[i]->destination_site_id,
                 (*(this->connection_definitions_))[i]->priority);

              this->handlers_.push_back(handler);
           }
        }
     }

   }

   return result;
}

int ACE_PIP_Connection_Manager::process_connection_file(char* file_name)
{
   // Expecting the file to contain one tuple per line 
   // where each is of form (source_id, dest_id, dest_address, 
   // dest_port, priority)
   
   ACE_TCHAR line[100];
   char* token(0);
   FILE* fp = ACE_OS::fopen(file_name, "r");
   Connection_Definition* current_definition(0);
   if (fp)
   {
      int num_entries(0);
      fscanf(fp, "%d", &num_entries);
      if (num_entries > 0)
      {
         ACE_NEW_RETURN(this->connection_definitions_, 
                        ACE_Vector<Connection_Definition*>, 0);
         for (int i = 0; i < num_entries; ++i)
         {
            ACE_NEW_RETURN(current_definition, Connection_Definition, 0);
            fscanf(fp, "%s", line);
            token = strtok(line, " (,)");
            if (!token)
            {
               delete current_definition;
               return -1;
            }
            
            // Store the source site ID
            current_definition->source_site_id = atoi(token);
            
            token = ACE_OS::strtok(0, " (,)");
            if (!token)
            {
               delete current_definition;
               return -1;
            }
            
            // destination site ID
            current_definition->destination_site_id = atoi(token);
            
            token = ACE_OS::strtok(0, " (,)");
            if (!token)
            {
               delete current_definition;
               return -1;
            }
            
            // IP address
            size_t length = ACE_OS::strlen(token);
            ACE_NEW_RETURN(current_definition->address, char[length + 1], 0);
            ACE_OS::strncpy(current_definition->address, token, length);
            current_definition->address[length] = 0;
            
            token = ACE_OS::strtok(0, " (,)");
            if (!token)
            {
               delete current_definition;
               return -1;
            }
            
            // IP port
            current_definition->port = atoi(token);
            
            token = ACE_OS::strtok(0, " (,)");
            if (!token)
            {
               delete current_definition;
               return -1;
            }
            
            // Connection Priority
            current_definition->priority = atoi(token);
            current_definition->type = Connection_Definition::REACTIVE;
            this->connection_definitions_->push_back(current_definition);
         }
      }
   }
   else
   {
      //error
      return -1;
   }

  return 0; 
}

const ACE_Vector<ACE_PIP_Connection_Manager::Connection_Definition*>* 
ACE_PIP_Connection_Manager::get_connections() const
{
  return this->connection_definitions_;
}

