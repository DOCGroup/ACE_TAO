#include "ace/OS.h"
// $Id$

#include "Config_Files.h"

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

  while ((result = this->getint (entry.proxy_id_)) != FP::SUCCESS)
    if (result == FP::EOFILE)
      return FP::EOFILE;
    else if (result == FP::EOLINE
             || result == FP::COMMENT)
      line_number++;

  // Get the supplier id.
  result = this->getint (entry.supplier_id_);
  if (result != FP::SUCCESS)
    return result;

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
Proxy_Config_File_Parser::read_entry (Proxy_Config_Info &entry,
					   int &line_number)
{
  char buf[BUFSIZ];
  FP_RETURN_TYPE result;

  // Increment the line count.
  line_number++;

  // Ignore comments, check for EOF and EOLINE if this succeeds, we
  // have our connection id

  while ((result = this->getint (entry.proxy_id_)) != FP::SUCCESS)
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
        entry.proxy_role_ = buf[0];

      if (entry.proxy_role_ == 'C')
        entry.remote_port_ = Options::instance ()->consumer_connector_port ();
      else (entry.proxy_role_ == 'S')
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
        entry.proxy_role_ = buf[0];
    }

  // Get the max retry delay.
  result = this->getint (entry.max_retry_timeout_);
  if (result == FP::DEFAULT)
    entry.max_retry_timeout_ = Options::instance ()->max_timeout ();
  else if (result != FP::SUCCESS)
    return result;

  // Get the local port number.
  result = this->getint (port);
  if (result != FP::SUCCESS)
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
int main (int argc, char *argv[])
{
  if (argc != 4) 
    {
      //    ACE_ERROR_RETURN ((LM_ERROR, "%s filename\n", argv[0]), -1);
      cerr << argv[0] << " CCfilename filename Mapfilename.\n";
      exit (1);
    }
  FP_RETURN_TYPE result;
  Proxy_Config_Info entry;
  Proxy_Config_File_Parser CCfile;

  CCfile.open (argv[1]);

  int line_number = 0;

  printf ("ConnID\tHost\t\tRPort\tDir\tRetry\tLPort\n");

  // Read config file line at a time.
  while ((result = CCfile.read_entry (entry, line_number)) != EOF)
    {
      if (result != FP::SUCCESS)
 //	ACE_DEBUG ((LM_DEBUG, "Error line %d.\n", line_number));
	cerr << "Error at line " << line_number << endl;
      else
	printf ("%d\t%s\t%d\t%c\t%d\t%c\t%d\n",
	       entry.proxy_id_, entry.host_, entry.remote_port_, entry.proxy_role_,
	       entry.max_retry_timeout_, entry.transform_, entry.local_port_);
    }
  CCfile.close();

  Consumer_Config_Info entry;
  Consumer_Config_File_Parser file;

  file.open (argv[2]);

  line_number = 0;

  printf ("\nConnID\tLogic\tPayload\tDestinations\n");

  // Read config file line at a time.
  while ((result = file.read_entry (entry, line_number)) != EOF)
    {
      if (result != FP::SUCCESS)
	cerr << "Error at line " << line_number << endl;
      else
	{
	  printf ("%d\t%d\t%d\t%d\t",
		  entry.proxy_id_, entry.supplier_id_, entry.type_);
	  while (--entry.total_consumers_ >= 0)
	    printf ("%d,", entry.consumers_[entry.total_consumers_]);
	  printf ("\n");
	}
    }
  file.close();

  return 0;
}
#endif /* DEBUGGING */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class File_Parser<Proxy_Config_Info>;
template class File_Parser<Consumer_Config_Info>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate File_Parser<Proxy_Config_Info>
#pragma instantiate File_Parser<Consumer_Config_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

