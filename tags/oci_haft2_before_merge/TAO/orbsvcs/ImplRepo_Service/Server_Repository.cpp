#include "Server_Repository.h"
#include "XML_ContentHandler.h"
#include "Activator_Options.h"

#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"

ACE_RCSID (ImplRepo_Service,
           Server_Repository,
           "$Id$")


int
Server_Repository::init (void)
{
  int rmode = OPTIONS::instance ()->repository_mode ();
  if (rmode != Options::REPO_XML_FILE)
    {
      Repository_Configuration *config = OPTIONS::instance ()->config ();
      ACE_ASSERT(config != 0);

      // iterate through the list of registered servers and register them

      config->open_section (config->root_section (),
                            ACE_LIB_TEXT ("Servers"),
                            1,
                            this->servers_);
      int index = 0;
      ACE_CString name;

      while (config->enumerate_sections (servers_, index, name) == 0)
        {
          ACE_CString logical, startup, working_dir;
          u_int activation_val = 0;
          ImplementationRepository::ActivationMode activation;

          ImplementationRepository::EnvironmentList environment_vars;

          ACE_Configuration_Section_Key server_key;
          int error = 0;

          error += config->open_section (this->servers_,
                                         name.c_str(),
                                         0,
                                         server_key);

          error += config->get_string_value (server_key,
                                             ACE_LIB_TEXT ("LogicalServer"),
                                             logical);

          error += config->get_string_value (server_key,
                                             ACE_LIB_TEXT ("StartupCommand"),
                                             startup);

          error += config->get_string_value (server_key,
                                             ACE_LIB_TEXT ("WorkingDir"),
                                             working_dir);

          error += config->get_integer_value (server_key,
                                              ACE_LIB_TEXT ("Activation"),
                                              activation_val);

          activation =
            ACE_static_cast (ImplementationRepository::ActivationMode,
                             activation_val);

          // Maybe environments variables?? need a straightforward
          // way to store env vars.

          if (error != 0)
            {              
              ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Error reading configuration data for ")
                ACE_TEXT ("service '%s',skipping\n"),
                name.c_str ()));
            }
          else
            {
              this->add (name,
                         logical,
                         startup,
                         environment_vars,
                         working_dir,
                         activation);
            }

          index++;
        }
    }
  else
    {
      /// @@ Not quite sure what I want to do here .. leave for
      /// now. I guess similar stuff like check if that particular
      /// server info exists etc.
    }

  return 0;
}


// Add a new server to the Repository

int
Server_Repository::add (
    const ACE_CString& POA_name,
    const ACE_CString& logical_server_name,
    const ACE_CString& startup_command,
    const ImplementationRepository::EnvironmentList& environment_vars,
    const ACE_CString& working_dir,
    const ImplementationRepository::ActivationMode& activation
  )
{
  int rmode = OPTIONS::instance ()->repository_mode ();
  if (rmode != Options::REPO_XML_FILE)
    {
      Repository_Configuration *config = OPTIONS::instance ()->config ();
      ACE_ASSERT(config != 0);

      // @@ Add this to the persistent configuration; environment_vars??
      ACE_Configuration_Section_Key server;
      config->open_section (this->servers_,
                            POA_name.c_str(),
                            1,
                            server);

      config->set_string_value (server,
                                ACE_LIB_TEXT ("LogicalServer"),
                                logical_server_name);

      config->set_string_value (server,
                                ACE_LIB_TEXT ("StartupCommand"),
                                startup_command);

      config->set_string_value (server,
                                ACE_LIB_TEXT ("WorkingDir"),
                                working_dir);

      config->set_integer_value (server,
                                 ACE_LIB_TEXT ("Activation"),
                                 activation);

      Server_Info *new_server = 0;
      ACE_NEW_RETURN (new_server,
                      Server_Info (POA_name,
                                   logical_server_name,
                                   startup_command,
                                   environment_vars,
                                   working_dir,
                                   activation),
                      -1);

      return this->repository_.bind (POA_name, new_server);
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r");
      
      if (fp != 0) 
        {
          ACE_TCHAR buffer[4096];
          while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
            {
              /// Obviously, we need to/can update only if we find an
              /// entry for it in the xml file.
              ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

              if (found != 0)
                {
                  /// An entry found for the POA_name. So, dont
                  /// add it again.
                  ACE_DEBUG ((LM_DEBUG,
                              "ImR Activator: The %s is already added.\n", POA_name.c_str ()));

                  ACE_OS::fclose (fp);

                  return 0;
                }
            }
        }

      /// If control comes here, it means this server isnt added already.
      fp = ACE_OS::fopen (filename.c_str(), "a");

      if (fp == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Coudnt open the file to append\n"),
                            -1);
        }

      ACE_CString server_info = "<SERVER_INFORMATION>";
      server_info += "<Server>";
      server_info += POA_name.c_str ();
      server_info += "</Server>\n<StartupOptions>\n<Command_Line>";
      server_info += startup_command;
      server_info += "</Command_Line>\n<WorkingDir>";
      server_info += working_dir;
      server_info += "</WorkingDir>\n<Activation>";
      server_info += OPTIONS::instance ()->convert_str (activation);
      server_info += "</Activation>\n</StartupOptions>\n</SERVER_INFORMATION>\n";

      ACE_OS::fprintf(fp, server_info.c_str());
      ACE_OS::fclose(fp);

      return 0;
    }
}

// Update the associated process information.

int
Server_Repository::update (const ACE_CString& POA_name,
                           const ACE_CString& location,
                           const ACE_CString& server_object_ior)
{
  int rmode = OPTIONS::instance ()->repository_mode ();
  if (rmode != Options::REPO_XML_FILE)
    {
      Server_Info *server = 0;
      int retval = this->repository_.find (POA_name, server);

      // Only fill in data if it was found
      if (retval == 0)
        {
          ACE_ASSERT(server != 0);
          server->update_running_info (location, server_object_ior);
        }

      return retval;
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r+");
      ACE_ASSERT(fp != 0);

      ACE_TCHAR buffer[4096];

      while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
        {
          /// Obviously, we need to/can update only if we find an
          /// entry for it in the xml file.
          ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

          if (found != 0)
            {
              /// found an entry. So, need to update the entry
              /// information.
              this->handler_->update_running_information (POA_name,
                                                          location,
                                                          server_object_ior);
              return 0;
            }
        }
      return -1;
    }
}

// Returns information related to startup.

int
Server_Repository::get_startup_info (
    const ACE_CString& POA_name,
    ACE_CString& logical_server_name,
    ACE_CString& startup_command,
    ImplementationRepository::EnvironmentList& environment_vars,
    ACE_CString& working_dir,
    ImplementationRepository::ActivationMode& activation
  )
{
  int rmode = OPTIONS::instance ()->repository_mode ();
  if (rmode != Options::REPO_XML_FILE)
    {
      Server_Info* server = 0;
      int retval = this->repository_.find (POA_name, server);

      // Only fill in data if it was found
      if (retval == 0)
        {
          ACE_ASSERT(server != 0);
          server->get_startup_info (logical_server_name,
                                    startup_command,
                                    environment_vars,
                                    working_dir,
                                    activation);
        }
      return retval;
    }
  else
    {
      ACEXML_FileCharStream *fstm = 0;
      ACE_NEW_RETURN (fstm,
                      ACEXML_FileCharStream (),
                      1);

      const char* fname = OPTIONS::instance()->file_name().c_str();
      if (fstm->open (fname) != 0)
        {
          ACE_ERROR((LM_ERROR, ACE_LIB_TEXT("Fail to open XML file: %s\n"), fname));
          return 1;
        }

      this->handler_ = new XML_ContentHandler (POA_name.c_str ());

      ACEXML_Parser parser;

      ACEXML_InputSource input(fstm);

      parser.setContentHandler (this->handler_);
      parser.setDTDHandler (this->handler_);
      parser.setErrorHandler (this->handler_);
      parser.setEntityResolver (this->handler_);

      ACEXML_TRY_NEW_ENV
        {
          parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
          ACEXML_TRY_CHECK;
        }
      ACEXML_CATCH (ACEXML_Exception, ex)
        {
          ex.print();
          return -1;
        }
      ACEXML_ENDTRY;

      ACE_CString activation_mode;

      this->handler_->get_startup_information (logical_server_name,
                                               startup_command,
                                               working_dir,
                                               activation_mode);

      activation = OPTIONS::instance ()->convert_mode (activation_mode.c_str ());

      return 0;
    }
}


// Returns information related to a running copy.

int
Server_Repository::get_running_info (const ACE_CString& POA_name,
                                     ACE_CString& location,
                                     ACE_CString& server_object_ior)
{
  int rmode = OPTIONS::instance ()->repository_mode ();

  if (rmode != Options::REPO_XML_FILE)
    {
      Server_Info *server;
      int retval = this->repository_.find (POA_name, server);

      // Only fill in data if it was found
      if (retval == 0)
        {
          ACE_ASSERT(server != 0);
          server->get_running_info (location, server_object_ior);
        }
      return retval;
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r");
      ACE_ASSERT(fp != 0);

      ACE_TCHAR buffer[4096];

      while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
        {
          ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

          if (found != 0)
            {
              /// Found an entry for the POA_name. So, we can proceed.
              this->handler_->get_running_information (POA_name,
                                                       location,
                                                       server_object_ior);
              return 0;
            }
        }
      /// If control comes here.. implies, there is no entry for the
      /// POA_name.
      return -1;
    }
}


// Checks the starting_up_ variable in the Server_Info and
// returns the previous value or -1 if the POA_name wasn't found

int
Server_Repository::starting_up (const ACE_CString& POA_name,
                                int new_value)
{
  int rmode = OPTIONS::instance ()->repository_mode ();

  if (rmode != Options::REPO_XML_FILE)
    {
      Server_Info *server;
      int retval = this->repository_.find (POA_name, server);

      // Only fill in data if it was found
      if (retval == 0)
        {
          ACE_ASSERT(server != 0);
          retval = server->starting_up_ ? 1 : 0;
          server->starting_up_ = new_value != 0;
        }
      return retval;
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r");
      ACE_ASSERT(fp != 0);

      ACE_TCHAR buffer[4096];

      while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
        {
          ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

          if (found != 0)
            {
              int retval;
              /// Found an entry for the POA_name. So, we can proceed.
              this->handler_->get_startup_value (POA_name, retval);
              this->handler_->set_startup_value (POA_name, new_value);
              return retval;
            }
        }
      /// If control comes here.. implies, there is no entry for the
      /// POA_name.
      return -1;
    }
}

// Same as above but does not alter the value

int
Server_Repository::starting_up (const ACE_CString& POA_name)
{
  int rmode = OPTIONS::instance ()->repository_mode ();

  if (rmode != Options::REPO_XML_FILE)
    {
      Server_Info *server;

      int retval = this->repository_.find (POA_name, server);

      // Only fill in data if it was found
      if (retval == 0)
        {
          ACE_ASSERT(server != 0);
          retval = server->starting_up_ != 0;
        }

      return retval;
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r");
      ACE_ASSERT(fp != 0);

      ACE_TCHAR buffer[4096];
      
      while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
        {
          ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

          if (found != 0)
            {
              int retval;
              /// Found an entry for the POA_name. So, we can proceed.
              this->handler_->get_startup_value (POA_name, retval);
              return retval;
            }
        }
      /// If control comes here.. implies, there is no entry for the
      /// POA_name.
      return -1;
    }
}


// Removes the server from the Repository.

int
Server_Repository::remove (const ACE_CString& POA_name)
{
  int rmode = OPTIONS::instance ()->repository_mode ();

  if (rmode != Options::REPO_XML_FILE)
    {
      Repository_Configuration *config = OPTIONS::instance ()->config ();
      ACE_ASSERT(config != 0);
      // Remove the persistent configuration information
      config->remove_section (this->servers_,
                              POA_name.c_str(),
                              1);
      return this->repository_.unbind (POA_name);
    }
  else
    {
      ACE_CString filename = OPTIONS::instance ()->file_name ();

      FILE *fp = ACE_OS::fopen (filename.c_str(), "r");
      ACE_ASSERT(fp != 0);

      /// Have a temporary file
      CORBA::String_var temp_file = "temporary_file";

      FILE *fp_temp = ACE_OS::fopen (temp_file.in (), "w");
      ACE_ASSERT(fp_temp != 0);

      ACE_TCHAR buffer[4096];

      bool remove_section = false;
      // int dtd_section = 0;

      while (ACE_OS::fgets (buffer, sizeof (buffer), fp))
        {
          if (! remove_section)
            {
              ACE_TCHAR* found = ACE_OS::strstr (buffer, POA_name.c_str ());

              if (found == 0)
                {
                  ACE_OS::fprintf (fp_temp, buffer);
                }
              else
                {
                  remove_section = true;
                }
            }
          else
            {
              ACE_TCHAR* found = ACE_OS::strstr (buffer, "</SERVER_INFORMATION>");

              if (found != 0)
                remove_section = false;
            }
        }

      ACE_OS::fclose (fp_temp);
      ACE_OS::fclose (fp);

      // Now copy the temporary file to the original file.
      fp_temp = ACE_OS::fopen (temp_file.in (), "r");
      ACE_ASSERT(fp_temp != 0);

      fp = ACE_OS::fopen (filename.c_str(), "w");
      ACE_ASSERT(fp != 0);

      while (ACE_OS::fgets (buffer, sizeof (buffer), fp_temp))
        {
          ACE_OS::fprintf (fp, buffer);
        }

      ACE_OS::fclose (fp);
      ACE_OS::fclose (fp_temp);
      ACE_OS::unlink (temp_file.in ());
      return 0;
    }
  /*
            /// There is no support for DTD in the XML parser as of
            /// now. Will uncomment this part when the support is
            /// added.

            if (dtd_section == 0)
            {
            found = ACE_OS::strstr (buffer, "]>");

            if (found != 0)
            {
            dtd_section = 1;
            remove_section = 0;
            }

            ACE_OS::fprintf (fp_temp,
            buffer);
            }
            else
            {
            if (remove_section == 0)
            {
            found = ACE_OS::strstr (buffer, POA_name.c_str ());

            if (found == 0)
            {
    ACE_OS::fprintf (fp_temp,
            buffer);
            }
            else
            {
    remove_section = 1;
            }
            }
            else
            {
    found = ACE_OS::strstr (buffer, "</SERVER_INFORMATION>");

            if (found != 0)
            remove_section = 0;
            }
            }

            }
            ACE_OS::fclose (fp_temp);
            ACE_OS::fclose (fp);

            // Now copy the temporary file to the original file.
            fp_temp = ACE_OS::fopen (temp_file, "r");

            fp = ACE_OS::fopen (filename, "w");

            while (ACE_OS::fgets (buffer, 4096, fp_temp))
            {
            ACE_OS::fprintf (fp,
            buffer);
            }

            ACE_OS::fclose (fp);
            ACE_OS::fclose (fp_temp);
            ACE_OS::unlink (temp_file);
            }
          */
}

int
Server_Repository::write_to_xml (
    const ACE_CString&,
    const ACE_CString&,
    const ACE_CString&,
    const ImplementationRepository::EnvironmentList&,
    const ACE_CString&,
    const ImplementationRepository::ActivationMode&)
{
  /*
    ACE_TCHAR *filename = "trial.xml";

    ACE_Configuration_Heap *heap = 0;
    ACE_NEW_RETURN (heap, ACE_Configuration_Heap, -1);

    if (heap->open (filename) != 0)
    {
    ACE_ERROR ((LM_ERROR,
    ACE_TEXT ("Error: Opening persistent heap file '%s'\n"),
    filename));
    }

    heap->open_section (heap->root_section (),
    "Trail_Servers",
    1,

  */
  return 0;
}


// Returns a new iterator that travels over the repository.

Server_Repository::HASH_IMR_MAP::ITERATOR *
Server_Repository::new_iterator (void)
{
  HASH_IMR_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_RETURN (hash_iter,
    Server_Repository::HASH_IMR_MAP::ITERATOR (this->repository_),
                  0);

  return hash_iter;
}


// Returns the number of entries in the repository.

size_t
Server_Repository::get_repository_size (void)
{
  return this->repository_.current_size ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, Server_Info *>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
template class ACE_Equal_To<ACE_CString>;
#endif /* __SUNPRO_CC */
// Instantiate for ACE_WString because ACE_CString can be either
// ACE_CString or ACE_WString.
template class ACE_Equal_To<ACE_WString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, Server_Info *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
#pragma instantiate ACE_Equal_To<ACE_CString>
#endif /* __SUNPRO_CC */
// Instantiate for ACE_WString because ACE_CString can be either
// ACE_CString or ACE_WString.
#pragma instantiate ACE_Equal_To<ACE_WString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
