#include "ace/OS.h"
// $Id$

#include "Config_Files.h"

// This fixes a nasty bug with cfront-based compilers (like
// Centerline).
typedef FP::Return_Type FP_RETURN_TYPE;

FP_RETURN_TYPE
RT_Config_File_Parser::read_entry (RT_Config_File_Entry &entry, 
				   int &line_number) 
{
  FP_RETURN_TYPE read_result;
  // increment the line count
  line_number++;

  // Ignore comments, check for EOF and EOLINE
  // if this succeeds, we have our connection id
  while ((read_result = this->getint (entry.conn_id_)) != FP::SUCCESS) 
    {
      if (read_result == FP::EOFILE) 
	return FP::EOFILE;
      else if (read_result == FP::EOLINE 
	       || read_result == FP::COMMENT)
	{
	  // increment the line count
	  line_number++;
	}
    }

  // Get the logic id.
  if ((read_result = this->getint (entry.logical_id_)) != FP::SUCCESS)
    return read_result;

  // Get the payload type.
  if ((read_result = this->getint (entry.payload_type_)) != FP::SUCCESS)
    return read_result;

  // get all the destinations.
  entry.total_destinations_ = 0;

  while ((read_result = this->getint (entry.destinations_[entry.total_destinations_]))
	 == FP::SUCCESS)
    ++entry.total_destinations_; // do nothing

  if (read_result == FP::EOLINE || read_result == FP::EOFILE)
    return FP::SUCCESS;
  else
    return read_result;
}

FP_RETURN_TYPE
CC_Config_File_Parser::read_entry (CC_Config_File_Entry &entry, 
				   int &line_number) 
{
  char buf[BUFSIZ];
  FP_RETURN_TYPE read_result;
  // increment the line count
  line_number++;

  // Ignore comments, check for EOF and EOLINE
  // if this succeeds, we have our connection id
  while ((read_result = this->getint (entry.conn_id_)) != FP::SUCCESS) 
    {
      if (read_result == FP::EOFILE) 
	return FP::EOFILE;
      else if (read_result == FP::EOLINE || 
	       read_result == FP::COMMENT) 
	{
	  // increment the line count
	  line_number++;
	}
    }

  // get the hostname
  if ((read_result = this->getword (entry.host_)) != FP::SUCCESS)
    return read_result;

  int port;

  // Get the port number.
  if ((read_result = this->getint (port)) != FP::SUCCESS)
    return read_result;
  else
    entry.remote_port_ = (u_short) port;

  // Get the direction.
  if ((read_result = this->getword (buf)) != FP::SUCCESS)
    return read_result;
  else
    entry.direction_ = buf[0];

  // Get the max retry delay.
  if ((read_result = this->getint (entry.max_retry_delay_)) != FP::SUCCESS)
    return read_result;

  // Get the local port number.
  if ((read_result = this->getint (port)) != FP::SUCCESS)
    return read_result;
  else
    entry.local_port_ = (u_short) port;

  return FP::SUCCESS;
}

#if defined (DEBUGGING)
int main (int argc, char *argv[])
{
  if (argc != 4) {
//    ACE_ERROR_RETURN ((LM_ERROR, "%s filename\n", argv[0]), -1);
    cerr << argv[0] << " CCfilename RTfilename Mapfilename.\n";
    exit (1);
  }
  FP_RETURN_TYPE result;
  CC_Config_File_Entry CCentry;
  CC_Config_File_Parser CCfile;
  
  CCfile.open (argv[1]);

  int line_number = 0;

  printf ("ConnID\tHost\t\tRPort\tDir\tRetry\tLPort\n");

  // Read config file line at a time.
  while ((result = CCfile.read_entry (CCentry, line_number)) != EOF)
    {
      if (result != FP::SUCCESS)
 //	ACE_DEBUG ((LM_DEBUG, "Error line %d.\n", line_number));
	cerr << "Error at line " << line_number << endl;
      else 
	printf ("%d\t%s\t%d\t%c\t%d\t%c\t%d\n",
	       CCentry.conn_id_, CCentry.host_, CCentry.remote_port_, CCentry.direction_,
	       CCentry.max_retry_delay_, CCentry.transform_, CCentry.local_port_);
    }
  CCfile.close();

  RT_Config_File_Entry RTentry;
  RT_Config_File_Parser RTfile;

  RTfile.open (argv[2]);

  line_number = 0;

  printf ("\nConnID\tLogic\tPayload\tDestinations\n");

  // Read config file line at a time.
  while ((result = RTfile.read_entry (RTentry, line_number)) != EOF)
    {
      if (result != FP::SUCCESS)
	cerr << "Error at line " << line_number << endl;
      else 
	{
	  printf ("%d\t%d\t%d\t%d\t",
		  RTentry.conn_id_, RTentry.logical_id_, RTentry.payload_type_);
	  while (--RTentry.total_destinations_ >= 0)
	    printf ("%d,", RTentry.destinations_[RTentry.total_destinations_]);
	  printf ("\n");
	}
    }
  RTfile.close();

  return 0;
}
#endif /* DEBUGGING */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class File_Parser<CC_Config_File_Entry>;
template class File_Parser<RT_Config_File_Entry>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
