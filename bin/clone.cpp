#include "ace/OS.h"
// @(#)clone.cpp	1.1	10/18/96


#if 0
#if defined (USG)
#define lstat stat
#else
extern "C" char *getwd (char *);
#define getcwd(str,len) (getwd(str))
#endif
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

#ifndef BLKDEV_IOSIZE
#define BLKDEV_IOSIZE	1024
#endif

extern char *sys_errlist[];
static void clone (char* s_path, char* d_path, int sroot_flag);

static char *pname;
static int errors = 0;

static char* src_path = 0;
static char* dst_path = 0;

static int quiet_flag = 0;
static int verbose_flag = 0;
static int force_flag = 0;
#ifndef USG
static int symlink_flag = 0;
#endif
static int copy_flag = 0;
static int sccs_flag = 0;

static void
usage (void)
{
#ifdef USG
  fprintf (stderr, "%s: usage: '%s [-q][-v][-f][-c] pathname1 pathname2'\n", pname, pname);
#else
  fprintf (stderr, "%s: usage: '%s [-q][-v][-f][-S][-c | -s | -S] pathname1 pathname2'\n", pname, pname);
#endif
  exit (1);
}

/* abspath(): return the absolutized pathname for the given relative
   pathname.  Note that if that pathname is already absolute, it may
   still be returned in a modified form because this routine also
   eliminates redundant slashes and single dots and eliminates double
   dots to get a shortest possible pathname from the given input
   pathname.  The absolutization of relative pathnames is made by
   assuming that the given pathname is to be taken as relative to the
   first argument (cwd) or to the current directory if cwd is null.  */

static char *
abspath (char *cwd, char *rel_pathname)
{
  static char cwd_buffer[MAXPATHLEN];
  char abs_buffer[MAXPATHLEN];
  register char *endp;
  register char *p;
  register char *inp = abs_buffer;
  register char *outp = abs_buffer;

  /* Setup the current working directory as needed.  */

  if (!cwd)
    {
      if (!cwd_buffer[0])
        getcwd (cwd_buffer, MAXPATHLEN);
      cwd = cwd_buffer;
    }
  else if (*cwd != '/')
    abort ();			/* base path must be absolute */

  /* Copy the  pathname (possibly preceeded by the current working
     directory name) into the absolutization buffer.  */

  endp = abs_buffer;
  if (rel_pathname[0] != '/')
    {
      p = cwd;
      while (*endp++ = *p++)
        continue;
      *(endp-1) = '/';			/* overwrite null */
    }
  p = rel_pathname;
  while (*endp++ = *p++)
    continue;
  if (endp[-1] == '/')
    *endp = (char) 0;

  /* Now make a copy of abs_buffer into abs_buffer, shortening the
     pathname (by taking out slashes and dots) as we go.  */

  *outp++ = *inp++;		/* copy first slash */
  for (;;)
    {
      if (!inp[0])
        break;
      else if (inp[0] == '/' && outp[-1] == '/')
	{
          inp++;
          continue;
	}
      else if (inp[0] == '.' && outp[-1] == '/')
	{
          if (!inp[1])
      	    break;
          else if (inp[1] == '/')
	    {
      	      inp += 2;
      	      continue;
	    }
          else if ((inp[1] == '.') && (inp[2] == 0 || inp[2] == '/'))
	    {
      	      inp += (inp[2] == '/') ? 3 : 2;
      	      outp -= 2;
      	      while (outp >= abs_buffer && *outp != '/')
      		outp--;
      	      if (outp < abs_buffer)
		{
		  /* Catch cases like /.. where we try to backup to a
		     point above the absolute root of the logical file
		     system.  */

      		  fprintf (stderr, "%s: fatal: invalid pathname: %s\n",
		    pname, rel_pathname);
      		  exit (1);
      		}
      	      *++outp = (char) 0;
      	      continue;
            }
        }
      *outp++ = *inp++;
    }

  /* On exit, make sure that there is a trailing null, and make sure that
     the last character of the returned string is *not* a slash.  */

  *outp = (char) 0;
  if (outp[-1] == '/')
    *--outp  = (char) 0;

  /* Make a copy (in the heap) of the stuff left in the absolutization
     buffer and return a pointer to the copy.  */

  return strcpy ((char *) malloc (outp - abs_buffer + 1), abs_buffer);
}

static char*
path_concat (char* s1, char* s2)
{
  int s1_len;
  char* ret_val = (char *) malloc ((s1_len = strlen (s1)) + strlen (s2) + 2);

  strcpy (ret_val, s1);
  ret_val[s1_len] = '/';
  strcpy (&ret_val[s1_len+1], s2);
  return ret_val;
}

/* Decide if the given path (which may be relative to . or absolute) designa
tes
   a point within the original "src_path" directory, and return non-zero if 
it
   does, or zero otherwise.  */

static int
in_original_tree (char* other_path)
{
  char* abs_src_path = abspath (NULL, src_path);
  char* abs_src_path_slash = path_concat (abs_src_path, "");
  char* abs_other_path = abspath (NULL, other_path);
  int ret_val =  !strncmp (abs_src_path_slash, abs_other_path, strlen (abs_src_path_slash));

  free (abs_src_path);
  free (abs_src_path_slash);
  free (abs_other_path);
  return ret_val;
}

static void
fix_mode (int new_mode, char* d_path)
{
  if (chmod (d_path, new_mode))
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: warning: can't chmod on output entity %s: %s\n",
	  pname, d_path, sys_errlist[errno]);
    }
}

static int
remove_item (char* s_path, char* d_path)
{
  struct stat dst_stat_buf;
  DIR* dirp;
  char containing_dir[MAXPATHLEN];

  if (lstat (d_path, &dst_stat_buf) == -1)
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: error: cannot get status of %s: %s\n",
	  pname, d_path, sys_errlist[errno]);
      return -1;
    }

  /* Before wasting a lot of time sniffing at the thing we are trying to
     delete, first make sure that we have write permission into the
     directory that contains this thing.  Otherwise, it is all a waste
     of time.  */

  if (*d_path == '/')
    strcpy(containing_dir, d_path);
  else
    {
      containing_dir[0] = '.';
      containing_dir[1] = '/';
      strcpy(containing_dir+2, d_path);
    }
  *(strrchr (containing_dir, '/')) = '\0';
  if (containing_dir[0] == '\0')
    {
      containing_dir[0] = '/';
      containing_dir[1] = '\0';
    }
  if (access (containing_dir, W_OK))
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: error: don't have write access to %s: %s\n",
	  pname, containing_dir, sys_errlist[errno]);
      return -1;
    }
  
  switch (dst_stat_buf.st_mode & S_IFMT)
    {
      case  S_IFDIR:
	if (access (d_path, R_OK) != 0)
	  {
	    if (!quiet_flag)
              fprintf (stderr, "%s: error: don't have read permission for directory %s\n",
	        pname, d_path);
            return -1;
	  }
	if (access (d_path, X_OK) != 0)
	  {
	    if (!quiet_flag)
              fprintf (stderr, 
                "%s: error: don't have search permission for directory %s\n",
	        pname, d_path);
            return -1;
	  }
	if (access (d_path, W_OK) != 0)
	  {
	    if (!quiet_flag)
              fprintf (stderr, 
                "%s: error: don't have write permission for directory %s\n",
	        pname, d_path);
            return -1;
	  }
	if ((dirp = opendir (d_path)) == NULL)
	  {
	    if (!quiet_flag)
              fprintf (stderr, 
                "%s: error: can't open directory %s for reading: %s\n",
	        pname, d_path, sys_errlist[errno]);
            return -1;
	  }
	for (;;)
	  {
	    struct dirent* dir_entry_p;
	    char* new_s_path;
	    char* new_d_path;

	    if ((dir_entry_p = readdir (dirp)) == NULL)
	      break;
	    if (!strcmp (dir_entry_p->d_name, "."))
	      continue;
	    if (!strcmp (dir_entry_p->d_name, ".."))
	      continue;
	    new_s_path = path_concat (s_path, dir_entry_p->d_name);
	    new_d_path = path_concat (d_path, dir_entry_p->d_name);
	    if (remove_item (new_s_path, new_d_path))
	      {
		closedir (dirp);
		return -1;
	      }
	    free (new_s_path);
	    free (new_d_path);
	  }
	closedir (dirp);
	if (rmdir (d_path))
	  {
	    if (!quiet_flag)
	      fprintf (stderr, "%s: error: can't delete existing directory %s: %s\n",
	        pname, d_path, sys_errlist[errno]);
	    return -1;
	  }
	if (!quiet_flag)
	  fprintf (stderr, "%s: removed directory %s\n",
	    pname, d_path);
	break;

      /* Note that symbolic links can be treated just like normal files
	 when the time comes for deleting them.  Unlinking a symbolic link
	 just deletes the link and *not* the thing it points to.  */

      default:
	if (unlink (d_path))
	  {
	    if (!quiet_flag)
              fprintf (stderr, "%s: error: can't delete existing file %s: %s\n",
	        pname, d_path, sys_errlist[errno]);
            return -1;
	  }
	if (!quiet_flag)
	  fprintf (stderr, "%s: removed file %s\n",
	    pname, d_path);
	break;
    }
  return 0;
}

#ifndef USG
static void
mk_symbolic_link (char *s_path, char *d_path)
{
  if (symlink (s_path, d_path))
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: error: can't symlink %s to %s: %s\n",
          pname, s_path, d_path, sys_errlist[errno]);
     }
  else
    {
      if (verbose_flag)
        fprintf (stderr, "%s: created symlink %s -> %s\n",
          pname, d_path, s_path);
    }
}
#endif

static void
mk_hard_link (char *s_path, char *d_path)
{
  if (link (s_path, d_path))
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: error: can't link %s to %s: %s\n",
          pname, s_path, d_path, sys_errlist[errno]);
    }
  else
    {
      if (verbose_flag)
        fprintf (stderr, "%s: created hard link %s = %s\n",
          pname, d_path, s_path);
    }
}

static void
copy_file (char *s_path, char *d_path)
{
  int input, output;
  struct stat src_stat_buf;

  if (lstat (s_path, &src_stat_buf) == -1)
    {
      if (!quiet_flag)
	{
          fprintf (stderr, "%s: error: can't get status of %s: %s\n",
	    pname, s_path, sys_errlist[errno]);
          fprintf (stderr, "%s: input entity %s will be ignored\n",
	    pname, s_path);
	}
      return;
    }

  if ((input = open (s_path, O_RDONLY, 0)) == -1)
    {
      if (!quiet_flag)
        {
          fprintf (stderr, "%s: error: can't open input file %s: %s\n",
	    pname, d_path, sys_errlist[errno]);
	  fprintf (stderr, "%s: input file %s will be ignored\n",
	    pname, s_path);
	}
	return;
    }

  if ((output = open (d_path, O_CREAT | O_WRONLY, src_stat_buf.st_mode & 07777)) == -1)
    {
      if (!quiet_flag)
        {
          fprintf (stderr, "%s: error: can't create output file %s: %s\n",
	    pname, d_path, sys_errlist[errno]);
	  fprintf (stderr, "%s: input file %s will be ignored\n",
	    pname, s_path);
	}
	return;
    }

  for (;;)
    {
      int rlen, wlen;
      char block_buf[BLKDEV_IOSIZE];

      if ((rlen = read (input, block_buf, BLKDEV_IOSIZE)) == -1)
        {
          if (!quiet_flag)
	    {
	      fprintf (stderr, "%s: error: bad read from input file %s: %s\n",
	        pname, s_path, sys_errlist[errno]);
	      fprintf (stderr, "%s: input file %s was not fully copied\n",
	        pname, s_path);
	    }
	  break;
	}

    if (rlen == 0)
      break;

    if ((wlen = write (output, block_buf, rlen)) == -1 || wlen != rlen)
      {
        if (!quiet_flag)
          {
	    fprintf (stderr, "%s: error: bad write to output file %s: %s\n",
	      pname, s_path, sys_errlist[errno]);
	    fprintf (stderr, "%s: input file %s not fully copied\n",
	      pname, s_path);
	  }
	break;
      }
    }

  close (output);
  close (input);

  fix_mode (src_stat_buf.st_mode & 07777, d_path);

  if (verbose_flag)
    fprintf (stderr, "%s: created file copy %s = %s\n",
      pname, d_path, s_path);
}

static void
symlink_SCCS (char* s_path, char* d_path)
{
  struct stat dst_stat_buf;
  char symlink_buf[MAXPATHLEN];
  int count;

  if (access (d_path, F_OK))			/* Does d_path exit? */
    {
      if (errno != ENOENT)
	{
	  if (!quiet_flag)
	    {
	      fprintf (stderr, "%s: error: can't check accessability of %s: %s\n",
	        pname, d_path, sys_errlist[errno]);
	      fprintf (stderr, "%s: input %s will be ignored\n",
	        pname, s_path);
	    }
	  return;
	}
    }
  else						/* d_path exists.  What is it? */
    {
      if (lstat (d_path, &dst_stat_buf) == -1)
	{
	  if (!quiet_flag)
	    {
	      fprintf (stderr, "%s: error: unable to get status of %s: %s\n",
	        pname, d_path, sys_errlist[errno]);
              fprintf (stderr, "%s: input %s will be ignored\n",
	        pname, s_path);
	    }
	  return;
	}

      if (S_ISLNK(dst_stat_buf.st_mode))	/* d_path is a symbolic link */
	{
          if ((count = readlink (d_path, symlink_buf, MAXPATHLEN)) == -1)
            {
              fprintf (stderr, "%s: error: can't read symlink %s: %s\n",
                pname, d_path, sys_errlist[errno]);
              fprintf (stderr, "%s: input file %s will be ignored\n",
                pname, s_path);
              return;
            }
          symlink_buf[count] = '\0';

          if (!strcmp(s_path, symlink_buf))	/* symlink = s_path. Done */
            {
              return;
            }
          else					/* symlink != s_path */
            {
	      if (force_flag)
                {
                  if (remove_item (s_path, d_path) != 0)
	            return;
                }
              else
	        {
	          if (!quiet_flag)
		    {
	              fprintf (stderr, "%s: error: Symbolic link %s already exists \
			but does not point to %s\n",
	                pname, d_path, s_path);
	              fprintf (stderr, "%s: input s %s will be ignored\n",
	                pname, s_path);
		    }
                  return;
                }
	    }
        }
      else					/* d_path is NOT a symbolic link */
	{
          if (force_flag)
	    {
              if (remove_item (s_path, d_path))
	        return;
	    }
          else
	    {
	      if (!quiet_flag)
		{
	          fprintf (stderr, "%s: error: output already exists: %s\n",
	            pname, d_path);
	          fprintf (stderr, "%s: input %s will be ignored\n",
	            pname, s_path);
		}
              return;
	    }
	}
    }
   
  if (symlink (s_path, d_path))
    {
      if (!quiet_flag)
        fprintf (stderr, "%s: error: can't symlink %s to %s: %s\n",
          pname, s_path, d_path, sys_errlist[errno]);
    }
  else
    {
      if (verbose_flag)
        fprintf (stderr, "%s: created symlink %s -> %s\n",
        pname, d_path, s_path);
    }
}

static void
clone_dir (char* s_path, char* d_path)
{
  DIR* dirp;

  if (access (s_path, R_OK) != 0)
    {
      if (!quiet_flag)
        {
          fprintf (stderr, 
            "%s: error: don't have read permission for input directory %s\n"
,
	    pname, s_path);
          fprintf (stderr, "%s: input directory %s will be ignored\n",
	    pname, s_path);
	 }
        return;
    }

  if (access (s_path, X_OK) != 0)
    {
      if (!quiet_flag)
        {
           fprintf (stderr, 
             "%s: error: don't have search permission for input directory %s\n",
	     pname, s_path);
           fprintf (stderr, "%s: input directory %s will be ignored\n",
	     pname, s_path);
	 }
       return;
    }

  if ((dirp = opendir (s_path)) == NULL)
    {
      if (!quiet_flag)
        {
          fprintf (stderr, "%s: error: can't open directory %s for reading: %s\n",
            pname, s_path, sys_errlist[errno]);
          fprintf (stderr, "%s: input directory %s will be ignored\n",
            pname, s_path);
        }
      return;
    }

  for (;;)
    {
      struct dirent* dir_entry_p;
      char* new_s_path;
      char* new_d_path;
      char  symlink_buf[MAXPATHLEN];
      int   len;

      if ((dir_entry_p = readdir (dirp)) == NULL)
        break;
      if (!strcmp (dir_entry_p->d_name, "."))
        continue;
      if (!strcmp (dir_entry_p->d_name, ".."))
        continue;

      new_s_path = path_concat (s_path, dir_entry_p->d_name);
      new_d_path = path_concat (d_path, dir_entry_p->d_name);

      if (sccs_flag && !strcmp (dir_entry_p->d_name, "SCCS"))
        symlink_SCCS(new_s_path, new_d_path);
      else
        clone (new_s_path, new_d_path, 0);

      free (new_s_path);
      free (new_d_path);
    }

  closedir (dirp);
}

static void
clone_symbolic_link (char* s_path,char* d_path)
{
  char symlink_buf[MAXPATHLEN];
  int count;

  if ((count = readlink (s_path, symlink_buf, MAXPATHLEN)) == -1)
    {
      fprintf (stderr, "%s: error: can't read symlink %s: %s\n",
        pname, s_path, sys_errlist[errno]);
      fprintf (stderr, "%s: input file %s will be ignored\n",
        pname, s_path);
      return;
    }
  symlink_buf[count] = '\0';

  if (symlink_buf[0] == '/')		/* symlink is absolute */
    {
      if (in_original_tree (symlink_buf))
        {
           if (!quiet_flag)
	     fprintf (stderr, 
	       "%s: warning: absolute symlink points into source tree %s -> %s\n",
	       pname, s_path, symlink_buf);
	}
    }
  else					/* symlink is relative */
    {
      char* src_root_relative = path_concat (s_path, symlink_buf);
      int in_orig = in_original_tree (src_root_relative);

      free (src_root_relative);
      if (!in_orig)
        {
          if (!quiet_flag)
	    fprintf (stderr, 
              "%s: warning: relative symlink points out of source tree %s -> %s\n",
	      pname, s_path, symlink_buf);
	}
    }

  mk_symbolic_link(symlink_buf, d_path); /* Make an identical symlink.  */
}


/* clone: clone the item designated by s_path as the new item d_path.  */

#define IS_DIR(STAT_BUF) (((STAT_BUF).st_mode & S_IFMT) == S_IFDIR)

static void
clone (char* s_path, char* d_path, int sroot_flag)
{
  struct stat src_stat_buf;
  struct stat dst_stat_buf;
  int dir_already_exists = 0;
  char* intype = "file";

  if (lstat (s_path, &src_stat_buf) == -1)
    {
      if (!quiet_flag)
	{
          fprintf (stderr, "%s: error: can't get status of %s: %s\n",
	    pname, s_path, sys_errlist[errno]);
          fprintf (stderr, "%s: input entity %s will be ignored\n",
	    pname, s_path);
	}
      return;
    }
  if (sccs_flag && sroot_flag && S_ISLNK (src_stat_buf.st_mode))
    {

      /* If root of the source path is a symbolic link and
	 SCCS cloning is enabled, clone the target of the link */

      if (stat(s_path, &src_stat_buf) == -1)
	{
          if (!quiet_flag)
	    {
              fprintf (stderr, "%s: error: can't get status of %s: %s\n",
	        pname, s_path, sys_errlist[errno]);
              fprintf (stderr, "%s: input entity %s will be ignored\n",
	        pname, s_path);
            }	
          return;
	}
    }
  if (IS_DIR (src_stat_buf))
    intype = "directory";
  if (access (d_path, 0))
    {
      if (errno != ENOENT)
	{
	  if (!quiet_flag)
	    {
	      fprintf (stderr, "%s: error: can't check accessability of %s: %s\n",
	        pname, d_path, sys_errlist[errno]);
	      fprintf (stderr, "%s: input %s %s will be ignored\n",
	        pname, intype, s_path);
	    }
	  return;
	}
    }
  else
    {
      char* outtype = "file";

      if (lstat (d_path, &dst_stat_buf) == -1)
	{
	  if (!quiet_flag)
	    {
	      fprintf (stderr, "%s: error: unable to get status of %s: %s\n"
,
	        pname, d_path, sys_errlist[errno]);
              fprintf (stderr, "%s: input %s %s will be ignored\n",
	        pname, intype, s_path);
	    }
	  return;
	}
      if (IS_DIR (dst_stat_buf))
	outtype = "directory";
      if (IS_DIR (src_stat_buf) && IS_DIR (dst_stat_buf))
	{
	  dir_already_exists = -1;

	  /* Have to make sure that we have full access to the output
	     directory (at least temporarily).  */

	  chmod (d_path, (dst_stat_buf.st_mode & 07777) | 0700);
	  if (access (d_path, R_OK | W_OK | X_OK) != 0)
	    {
	      if (!quiet_flag)
		{
	          fprintf (stderr, 
                    "%s: error: too few permissions for existing directory %s\n",
	            pname, d_path);
		  fprintf (stderr, "%s: input directory %s will be ignored\n",
	            pname, s_path);
		}
              return;
	    }
	}
      else
	{
          if (force_flag)
	    {
              if (remove_item (s_path, d_path))
	        return;
	    }
          else
	    {
	      if (!quiet_flag)
		{
	          fprintf (stderr, "%s: error: output %s already exists: %s\n",
	            pname, outtype, d_path);
	          fprintf (stderr, "%s: input %s %s will be ignored\n",
	            pname, intype, s_path);
		}
              return;
	    }
	}
    }

  switch (src_stat_buf.st_mode & S_IFMT)
    {
      case  S_IFDIR:				/* Clone a directory */

        if (!dir_already_exists)
          {
            /* Don't let others sneak in. 
               Only we can write the new directory (for now).  */
      
            if (mkdir (d_path, 0700))
              {
                if (!quiet_flag)
      	          {
                    fprintf (stderr, "%s: error: can't create output directory %s: %s\n",
	              pname, d_path, sys_errlist[errno]);
                    fprintf (stderr, "%s: input directory %s will be ignored\n",
	              pname, s_path);
	          }
                return;
	      }
            if (verbose_flag)
              fprintf (stderr, "%s: created new output directory: %s\n",
                pname, d_path);
          }

	clone_dir(s_path, d_path);

	/* By default, output directories which existed before this
	   program was executed are reset back to their original
	   permissions (when we are done adding things to them).  For
	   output directories which are actually created by this program
	   however, these have their permissions set so that they are
	   essentially the same as the permissions for their corresponding
	   input directories, except that the owner is given full
	   permissions.  */

	if (dir_already_exists)
	  fix_mode (dst_stat_buf.st_mode & 07777, d_path);
	else
	  fix_mode ((src_stat_buf.st_mode & 07777) | 0700, d_path);
	break;

#ifndef USG
      case S_IFLNK:				/* Clone a symbolic link */

	if (!sccs_flag)
          clone_symbolic_link (s_path, d_path);
	break;
#endif

      default:					/* Clone a normal file */

	if (sccs_flag)
	  break;

#ifndef USG
	if (symlink_flag)
	  mk_symbolic_link(s_path, d_path);
	else
#endif
	if (copy_flag)
	  copy_file(s_path, d_path);
	else
	  mk_hard_link(s_path, d_path);

	break;
    } /* switch */
}

int
main (int argc, char *argv[])
{
  char **argn;

  pname = (pname = strrchr (argv[0], '/')) ? pname+1 : argv[0];
  for (argn = argv+1; *argn; argn++)
    {
      if (**argn != '-')
	{
	  if (!src_path)
	    src_path = *argn;
	  else if (!dst_path)
	    dst_path = *argn;
	  else
	    usage ();
	}
      else
	{
          switch (* ((*argn)+1))
	    {
              case 0:
          	fprintf (stderr, "%s: invalid option: -\n", pname);
          	errors = -1;
          	break;

	      case 'q':
		quiet_flag = -1;
		break;

	      case 'v':
		verbose_flag = -1;
		break;

	      case 'f':
		force_flag = -1;
		break;

#ifndef USG
	      case 'S':
		sccs_flag = -1;

		if (copy_flag)
		  errors++;
		break;
#endif

#ifndef USG
	      case 's':
		symlink_flag = -1;
		if (copy_flag)
		  errors++;
		break;
#endif

	      case 'c':
		copy_flag = -1;
#ifndef USG
		if (symlink_flag)
		  errors++;

		if (sccs_flag) 
		  errors++;
#endif
		break;

              default:
          	fprintf (stderr, "%s: invalid option: -%c\n",
          		pname, *((*argn)+1));
          	errors = -1;
            }
        }
    }
  if (errors || src_path == 0 || dst_path == 0)
    usage ();
#if 0 // ndef USG
  if (symlink_flag && *src_path != '/')
    {
      fprintf (stderr, "%s: error: source root pathname must be absolute when using -s\n",
	pname);
      exit (1);
    }
#endif
  if (access (src_path, 0) == -1)
    {
      fprintf (stderr, "%s: error: accessing source root entity %s: %s\n",
	pname, src_path, sys_errlist[errno]);
      exit (1);
    }
  umask (0);			/* disable all masking */
  clone (src_path, dst_path, 1);
  return 0;
}
