// $Id$
#include "global.h"
#include "Options.h"
#include "Rwho_DB_Manager.h"

/* Change to the RWHO directory to speed up and simplify later
   processing.  This requires opening the directory
   for reading with the directory iterator abstraction and then
   skipping the first two files in the directory, which are assumed to
   be "." and ".." (this function needs to be changed if this
   assumption does not hold!) */

RWho_DB_Manager::RWho_DB_Manager (void)
     : number_of_users (0), current_user (0), rwho_dir_name (RWHODIR),
       WHOD_HEADER_SIZE (sizeof host_data - sizeof host_data.wd_we)
{
  if (getcwd (this->original_pathname, MAXPATHLEN + 1) == 0)
    perror (Options::program_name), exit (1);

  if (chdir (this->rwho_dir_name) < 0)
    perror (this->rwho_dir_name), exit (1);

  this->rwho_dir.opendir (this->rwho_dir_name);

  /* Skip "." and ".." */
  this->rwho_dir.readdir ();
  this->rwho_dir.readdir ();
}

/* The destructor cleans up the RWHOD_DIR handle. */

RWho_DB_Manager::~RWho_DB_Manager (void)
{
  if (chdir (this->original_pathname) < 0)
    perror (Options::program_name), exit (1);

  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "disposing the RWho-Manager\n");
}

/* This procedure looks through the rwhod directory until it finds the next 
   valid user file.  
   
   The requirements for user files are:
     1) The file is at least MIN_HOST_DATA_SIZE bytes long
     2) It was received within the last MAX_HOST_TIMEOUT seconds

  Return:
    Are there any more hosts? */

int 
RWho_DB_Manager::get_next_host (void)
{
  dirent *dir_ptr;
  long	 current_time;

  time (&current_time);

  /* Go through each file in the directory looking for valid entries */
  
  while ((dir_ptr = this->rwho_dir.readdir ()) != 0)
    {
      int	user_file;
      int 	host_data_length;
	  
      if ((user_file = open (dir_ptr->d_name, O_RDONLY)) < 0)
	return -1;
	  
      host_data_length = read (user_file, (char *) &this->host_data, sizeof this->host_data);
	  
      if (host_data_length > WHOD_HEADER_SIZE && current_time - this->host_data.wd_recvtime < MAX_HOST_TIMEOUT)
	{
	  this->current_user 	= 0;
	  this->number_of_users = (host_data_length - WHOD_HEADER_SIZE) / sizeof *this->host_data.wd_we;
	  close (user_file);
	  return 1;		/* We found a good host, so return it */
	}
      else
	close (user_file);
    }

  return 0; /* There are no more hosts, so return False */
}

/* Returns the next user's information.  Note that for efficiency
   only pointers are copied, i.e., this info must be used before we
   call this function again. */

int 
RWho_DB_Manager::get_next_user (Protocol_Record &protocol_record)
{
  /* Get the next host file if necessary */
  if (this->current_user >= this->number_of_users && this->get_next_host () == 0)
    return 0;

  Drwho_Node *current_node = protocol_record.get_drwho_list ();

  protocol_record.set_login (this->host_data.wd_we[current_user].we_utmp.out_name);
  current_node->set_host_name (this->host_data.wd_hostname);
  current_node->set_idle_time (this->host_data.wd_we[current_user].we_idle);
  this->current_user++;

  return 1;
}
