 /**
  * @file PIP_Connection_Manager.cpp
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  */




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
   for (int i = 0; i < connection_definitions_->size(); ++i)
   {
      if ((*connection_definitions_)[i]->source_site_id ==
          source_site_id)
      {
         ACE_INET_Addr address;
         address.set((*connection_definitions_)[i]->port,
                     (*connection_definitions_)[i]->address.c_str());
         
        if ((*connection_definitions_)[i]->type ==
            Connection_Definition::ACTIVE)
        {
           ACE_PIP_Active_IO_Handler* handler = new ACE_PIP_Active_IO_Handler;
           result = active_connector_.connect(handler, address);
           if (result == -1)
           { 
              return -1;
           }
           else
           {
              handler->init(
                 (*connection_definitions_)[i]->source_site_id,
                 (*connection_definitions_)[i]->destination_site_id,
                 (*connection_definitions_)[i]->priority);
              
              handlers_.push_back(handler);
           }
        }
        else
        {
           ACE_PIP_Reactive_IO_Handler* handler = new ACE_PIP_Reactive_IO_Handler;
           result = reactive_connector_.connect(handler, address);
           if (result == -1)
           {
              std::cerr << "Unable to connect to " 
                        << (*connection_definitions_)[i]->address << " "
                        << (*connection_definitions_)[i]->port 
                        << std::endl;

              return -1;
           }
           else
           {
              handler->init(
                 (*connection_definitions_)[i]->source_site_id,
                 (*connection_definitions_)[i]->destination_site_id,
                 (*connection_definitions_)[i]->priority);
              
              handlers_.push_back(handler);
           }
        }
     }
      
   }
   
   return result;
}

int ACE_PIP_Connection_Manager::process_connection_file(char* file_name)
{
   // Expecting the file to contain one tuple per line 
   // where each is of form (source_id, dest_id, dest_address, dest_port, priority, type)
   std::ifstream* my_stream = new std::ifstream;
   
   my_stream->open(file_name);

   if (my_stream->fail())
   {
      std::cerr << "Failed to open connection file: " << file_name 
                << std::endl;
      
      return -1;
   }
   
   std::string line;
   std::string token;
   int strlen;
   int first_pos;
   int second_pos;
   Connection_Definition* current_definition(0);
   
   std::getline(*my_stream, line);
   int num_entries = atoi(line.c_str());
   
   connection_definitions_ = new ACE_Vector<Connection_Definition*>;
   for (int i = 0; i < num_entries; ++i)
   {
      current_definition = new Connection_Definition;
      std::getline(*my_stream, line);
      strlen = line.length();
      first_pos = line.find("(");
      if (first_pos > strlen)
      {
         delete current_definition;
         return -1;
      }
      
      second_pos = line.find(",", first_pos);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }
      
      // source site ID
      token.assign(line, first_pos + 1, second_pos - first_pos - 1);
      current_definition->source_site_id = atoi(token.c_str());
      
      first_pos = second_pos;
      second_pos = line.find(",", first_pos + 1);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }
      
      // destination site ID
      token.assign(line, first_pos + 1, second_pos - first_pos - 1);
      current_definition->destination_site_id = atoi(token.c_str());
      
      first_pos = second_pos;
      second_pos = line.find(",", first_pos + 1);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }
      
      // IP address
      current_definition->address.assign(line, first_pos + 1, second_pos - first_pos - 1);
      
      first_pos = second_pos;
      second_pos = line.find(",", first_pos + 1);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }

      // IP port
      token.assign(line, first_pos + 1, second_pos - first_pos - 1);
      current_definition->port = atoi(token.c_str());

      first_pos = second_pos;
      second_pos = line.find(",", first_pos + 1);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }

      // Connection priority
      token.assign(line, first_pos + 1, second_pos - first_pos - 1);
      current_definition->priority = atoi(token.c_str());

      first_pos = second_pos;
      second_pos = line.find(")", first_pos + 1);
      if (second_pos > strlen)
      {
         delete current_definition;
         return -1;
      }

      // Connection Type
      token.assign(line, first_pos + 1, second_pos - first_pos - 1);
      if (token == "ACTIVE")
      {
         current_definition->type = Connection_Definition::ACTIVE;
      }
      else
      {
         current_definition->type = Connection_Definition::REACTIVE;
      }

      connection_definitions_->push_back(current_definition);
   }

   return 0;
}

const ACE_Vector<ACE_PIP_Connection_Manager::Connection_Definition*>* ACE_PIP_Connection_Manager::get_connections() const
{
  return connection_definitions_;
}
