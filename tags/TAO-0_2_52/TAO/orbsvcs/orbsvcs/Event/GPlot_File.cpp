// $Id$
//
// ============================================================================
//
// = FILENAME
//    GPlot_File.cpp
//
// = AUTHOR
//    Tim Harrison
//
// ============================================================================

#include "GPlot_File.h"

#if !defined (__ACE_INLINE__)
#include "GPlot_File.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, GPlot_File, "$Id$")

int
ACE_GPlot_File::open (const char *filename)
{
  ACE_OS::strcpy (filename_, filename);

  FILE *read_file = ACE_OS::fopen (filename_, "r");

  long entry;
  char *value;
  ACE_NEW_RETURN (value, char [32], -1);

  if (read_file > 0)
    {
      int result;
      do
	{
	  result = fscanf (read_file,
			   "%ld\t%s\n",
			   &entry,
			   value);

	  //if ((result == -1) && (ACE_OS::last_error () != 0))
	  //ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "ACE_GPlot_File::open"), -1);

	  if (result > 0)
	    // Success.
	    map_.bind (entry, value);
	  else if (result != EOF)
	    // Error.
	    {
	      ACE_OS::fclose (read_file);
	      ACE_ERROR_RETURN
		((LM_ERROR, "Error reading GPlot file %s.\n", filename_), -1);
	    }
	} while (result != EOF);

      fclose (read_file);
    }

  write_file_ = ACE_OS::fopen (filename_, "w");
  if (write_file_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p: can't open\n", filename_), -1);

  closed_ = 0;

  return 0;
}


void
ACE_GPlot_File::close (void)
{
  if (closed_ == 0)
    {
      closed_ = 1;
      GPLOT_ITERATOR iterator ((GPLOT_MAP &) map_);
      for (GPLOT_ENTRY *entry; iterator.next (entry); iterator.advance ())
        {
	  ACE_OS::fprintf (write_file_, "%ld\t%s\n",
			   entry->ext_id_, entry->int_id_);
          delete [] entry->int_id_;
        }

      ACE_OS::fclose (write_file_);
    }
}


void
ACE_GPlot_File::dump (void)
{
  GPLOT_ITERATOR iterator ((GPLOT_MAP &) map_);
  for (GPLOT_ENTRY *entry; iterator.next (entry); iterator.advance ())
    {
      if (entry->int_id_ != 0)
	ACE_DEBUG ((LM_DEBUG, "%d\t%s\n", entry->ext_id_, entry->int_id_));
      else
	{
	  ACE_ERROR ((LM_ERROR, "Value for entry %d is null.\n",
		      entry->ext_id_));
	  return;
	}
    }
}


int
ACE_GPlot_File::get (long entry, long &value)
{
  char *val;

  if (map_.find (entry, val) == -1)
    return -1;
  else
    {
      ::sscanf (val, "%ld", &value);
      return 0;
    }
}


int
ACE_GPlot_File::get (long entry, float &value)
{
  char *val;

  if (map_.find (entry, val) == -1)
    return -1;
  else
    {
      ::sscanf (val, "%f", &value);
      return 0;
    }
}


void
ACE_GPlot_File::set (long entry, long value)
{
  long old_entry;
  char *val;
  char *old_value;

  ACE_NEW (val, char [32]);

  ::sprintf (val, "%ld", value);
  map_.rebind (entry, val, old_entry, old_value);

  delete [] old_value;
}


void
ACE_GPlot_File::set (long entry, float value)
{
  long old_entry;
  char *val;
  char *old_value;

  ACE_NEW (val, char [32]);

  ::sprintf (val, "%f", value);
  map_.rebind (entry, val, old_entry, old_value);

  delete [] old_value;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Entry<long, char *>;
template class ACE_Map_Manager<long, char *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<long, char *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<long, char *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<long, char *, ACE_Null_Mutex>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Entry<long, char *>
#pragma instantiate ACE_Map_Manager<long, char *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<long, char *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<long, char *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<long, char *, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
