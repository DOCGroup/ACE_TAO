// $Id$

#define ACE_BUILD_SVC_DLL

#include "Config_Files.h"
#include "Options.h"

ACE_RCSID(Gateway, Config_Files, "$Id$")

// This fixes a nasty bug with cfront-based compilers (like
// Centerline).
typedef FP::Return_Type FP_RETURN_TYPE;

FP_RETURN_TYPE
Consumer_Config_File_Parser::read_entry (Consumer_Config_Info &entry,
                                         int &line_number)
{
  FP_RETURN_TYPE result;

  // Increment the line count.
  line_number++;

  // Ignore comments, check for EOF and EOLINE if this succeeds, we
  // have our connection id.

  while ((result = this->getint (entry.connection_id_)) != FP::SUCCESS)
    if (result == FP::EOFILE)
      return FP::EOFILE;
    else if (result == FP::EOLINE
             || result == FP::COMMENT)
      line_number++;

  // Get the payload type.
  result = this->getint (entry.type_);
  if (result != FP::SUCCESS)
    return result;

  // get all the consumers.
  entry.total_consumers_ = 0;

  while ((result = this->getint
          (entry.consumers_[entry.total_consumers_])) == FP::SUCCESS)
    ++entry.total_consumers_; // do nothing (should check against max...)

  if (result == FP::EOLINE || result == FP::EOFILE)
    return FP::SUCCESS;
  else
    return result;
}

FP_RETURN_TYPE
Connection_Config_File_Parser::read_entry (Connection_Config_Info &entry,
                                           int &line_number)
{
  char buf[BUFSIZ];
  FP_RETURN_TYPE result;

  // Increment the line count.
  line_number++;

  // Ignore comments, check for EOF and EOLINE if this succeeds, we
  // have our connection id

  while ((result = this->getint (entry.connection_id_)) != FP::SUCCESS)
    if (result == FP::EOFILE)
      return FP::EOFILE;
    else if (result == FP::EOLINE
             || result == FP::COMMENT)
      line_number++;

  // Get the hostname.
  result = this->getword (entry.host_);
  if (result != FP::SUCCESS)
    return result;

  ACE_INT32 port;

  // Get the port number.
  result = this->getint (port);
  if (result == FP::DEFAULT)
    {
      // Get the proxy role, i.e., 'C' (Consumer) or 'S' (Supplier).
      result = this->getword (buf);
      if (result != FP::SUCCESS)
        return result;
      else
        entry.connection_role_ = buf[0];

      if (entry.connection_role_ == 'C')
        entry.remote_port_ = Options::instance ()->consumer_connector_port ();
      else if (entry.connection_role_ == 'S')
        entry.remote_port_ = Options::instance ()->supplier_connector_port ();
      else
        // Yikes, this is a *weird* error!
        entry.remote_port_ = 0;
    }
  else if (result != FP::SUCCESS)
    return result;
  else
    {
      entry.remote_port_ = u_short (port);

      // Get the proxy role, i.e., 'C' (Consumer) or 'S' (Supplier).
      result = this->getword (buf);
      if (result != FP::SUCCESS)
        return result;
      else
        entry.connection_role_ = buf[0];
    }

  // Get the max retry delay.
  result = this->getint (entry.max_retry_timeout_);
  if (result == FP::DEFAULT)
    entry.max_retry_timeout_ = Options::instance ()->max_timeout ();
  else if (result != FP::SUCCESS)
    return result;

  // Get the local port number.
  result = this->getint (port);
  if (result == FP::DEFAULT)
    entry.local_port_ = 0; // @@ Should make this an option.
  else if (result != FP::SUCCESS)
    return result;
  else
    entry.local_port_ = u_short (port);

  ACE_INT32 priority;

  // Get the priority.
  result = this->getint (priority);
  if (result != FP::SUCCESS)
    return result;
  else
    entry.priority_ = priority;

  return FP::SUCCESS;
}

#if defined (DEBUGGING)
int 
main (int argc, char *argv[])
{
  FP_RETURN_TYPE result;
  int line_number = 0;

  {
    Connection_Config_Info entry;
    Connection_Config_File_Parser connection_config_file;

    connection_config_file.open (argc > 1 ? argv[1] : "connection_config");

    int line_number = 0;

    ACE_DEBUG ((LM_DEBUG,
                "ConnID\tHost\t\tRPort\tRole\tRetry\tLPort\tPriority\n"));

    // Read config file line at a time.
    while ((result = connection_config_file.read_entry (entry, line_number)) != FP::EOFILE)
      if (result == FP::PARSE_ERROR)
        ACE_DEBUG ((LM_DEBUG,
                    "Error line %d.\n",
                    line_number));
      else
        ACE_DEBUG ((LM_DEBUG, 
                    "%d\t%s\t%d\t%c\t%d\t%d\t%d\n",
                    entry.connection_id_,
                    entry.host_,
                    entry.remote_port_,
                    entry.connection_role_,
                    entry.max_retry_timeout_,
                    entry.local_port_,
                    entry.priority_));

    connection_config_file.close ();
  }

  {
    Consumer_Config_Info entry;
    Consumer_Config_File_Parser consumer_config_file;

    consumer_config_file.open (argc > 2 ? argv[2] : "consumer_config");

    line_number = 0;

    ACE_DEBUG ((LM_DEBUG,
                "\nConnID\tLogic\tPayload\tDestinations\n"));

    // Read config file line at a time.
    while ((result = consumer_config_file.read_entry (entry, line_number)) != FP::EOFILE)
      if (result == FP::PARSE_ERROR)
        ACE_DEBUG ((LM_DEBUG,
                    "Error line %d.\n",
                    line_number));
      else
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "%d\t%d\t",
                      entry.connection_id_,
                      entry.type_));

          while (--entry.total_consumers_ >= 0)
            ACE_DEBUG ((LM_DEBUG, 
                        "%d,",
                        entry.consumers_[entry.total_consumers_]));
          ACE_DEBUG ((LM_DEBUG,
                      "\n"));
        }

    consumer_config_file.close ();
  }

  return 0;
}
#endif /* DEBUGGING */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class File_Parser<Connection_Config_Info>;
template class File_Parser<Consumer_Config_Info>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate File_Parser<Connection_Config_Info>
#pragma instantiate File_Parser<Consumer_Config_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
