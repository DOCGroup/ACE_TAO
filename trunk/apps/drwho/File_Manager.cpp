// $Id$
#include "global.h"
#include "ace/Mem_Map.h"
#include "File_Manager.h"

/* Initialize statics... */

char *File_Manager::current_ptr = 0;
int  File_Manager::number_of_friends = 0;
int  File_Manager::max_key_length = 0;
char *File_Manager::buffer_ptr = 0;
int  File_Manager::buffer_size = 0;

/* Either opens the friends file (if FILE_NAME is not a NULL pointer)
   or opens up the password file.  In either case, the number of 
   entries in the file are returned, i.e., number of friends... */

int	
File_Manager::open_file (char *file_name)
{
  return file_name == '\0' 
    ? File_Manager::open_passwd_file ()
    : File_Manager::open_friends_file (file_name);
}

/* Returns the next LOGIN_NAME and REAL_NAME from the file. */

int	
File_Manager::get_login_and_real_name (char *&login_name, char *&real_name)
{
  char *buf_ptr = File_Manager::current_ptr;

  login_name = buf_ptr;

  /* Skip to the end of the login name. */
  
  while (isalnum (*buf_ptr))
    buf_ptr++;
  
  *buf_ptr++ = '\0';
  
  /* Now skip over white space to *start* of real name! */
  
  while (isspace (*buf_ptr) || *buf_ptr == '\0')
    buf_ptr++;
  
  real_name = buf_ptr;
  
  while (*buf_ptr++ != '\n')
    ;
  
  /* Clear the trailing blanks and junk. */
  
  for (char *tmp_ptr = buf_ptr - 1; isspace (*tmp_ptr); tmp_ptr--)
    *tmp_ptr = '\0';
  
  /* Skip over consecutive blank lines. */

  while (*buf_ptr == '\n')
    buf_ptr++;

  File_Manager::current_ptr = buf_ptr;
  return 1;
}

/* Open up the yp passwd file and slurp all the users in! */

int
File_Manager::open_passwd_file (void)
{
  FILE 		*fp;
  char 		*file_name;
  struct passwd *pwent;
  
  file_name = tmpnam ((char *) 0);
  
  if ((fp = fopen (file_name, "w")) == 0)
    return -1;
  
  for (setpwent (); (pwent = getpwent ()) != 0; )
    if (*pwent->pw_gecos != '\0')
      {
	char *cp = strchr (pwent->pw_gecos, ',');

	if (cp != 0)
	  *cp = '\0';

	fprintf (fp, "%-8.8s %s\n", pwent->pw_name, pwent->pw_gecos);
	File_Manager::number_of_friends++;
      }
  
  endpwent ();
  
  fclose (fp);
  
  ACE_Mem_Map mmap (file_name);

  File_Manager::buffer_ptr = mmap.addr ();

  if (File_Manager::buffer_ptr >= 0)
    {
      File_Manager::buffer_size = mmap.size ();
      File_Manager::current_ptr = File_Manager::buffer_ptr;
      return File_Manager::number_of_friends;
    }
  return -1;
}

/* This function opens up FILE_NAME and memory maps it in our address space. */

int
File_Manager::open_friends_file (char *file_name)
{
  char	      *fnp = file_name;
  int	      fd;
  
  /* Split file_name into directory/file_name. */
  
  if ((fnp = strrchr (file_name, '/')) != 0)
    fd = open (file_name, O_RDONLY);
  else
    {
      char directory[MAXPATHLEN];
      
      strcpy (directory, getenv ("HOME")); 
      strcat (directory, "/");
      strcat (directory, file_name);
      fd = open (directory, O_RDONLY);
    }
  
  /* Do the mmaping if we got the file opened correctly... */
  
  if (fd < 0)
    return -1;

  ACE_Mem_Map mmap (fd);

  File_Manager::buffer_ptr = mmap.addr ();

  if (File_Manager::buffer_ptr >= 0)
    {
      File_Manager::buffer_size = mmap.size ();
      File_Manager::current_ptr = File_Manager::buffer_ptr;

      /* Determine how many friends there are by counting the newlines. */
  
      for (char *cp = File_Manager::buffer_ptr + File_Manager::buffer_size; 
	   cp > File_Manager::buffer_ptr
	   ; )
	if (*--cp == '\n')
	  {
	    File_Manager::number_of_friends++;

	    /* Skip consecutive newlines. */
	    while (cp[-1] == '\n')
	      --cp;
	  }
      return File_Manager::number_of_friends;
    }
  return -1;
}

