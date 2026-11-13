// -*- C++ -*-
// File: SO_Group.cpp

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/Process.h"
#include "ace/Pipe.h"
#include <utility>

#include "Library.h"
#include "SO_Group.h"
#include <cstring>

SO_Group::SO_Group ()
  : undef_wrapper_ ("nothing"),
    undefs_ (undef_wrapper_.imports())
{
}

void
SO_Group::add_executable (char const *path)
{
  ACE_Process proc;
  ACE_Process_Options opts;

  ACE_HANDLE pipe[2] {};
  ACE_Pipe io(pipe);

  opts.set_handles (ACE_STDIN,pipe[1]);

  int const result = opts.command_line ("ldd %s",path);
  // Prevent compiler warning about "unused variable" if ACE_ASSERT is
  // an empty macro.
  ACE_UNUSED_ARG (result);
  ACE_ASSERT (result == 0);

  proc.spawn (opts);
  if (ACE_OS::close(pipe[1]) == -1)
    ACE_DEBUG ((LM_DEBUG, "%p\n", "close"));
  opts.release_handles();

  size_t constexpr max_line_length = 1000;
  char line[max_line_length] {};

  while (true)
  {
    int len = 0;
    int nread = 0;
    bool bogus = false;

    // Skip initial whitespace.
    while ((nread = ACE_OS::read (pipe[0], line,1)) == 1
           && (*line == ' ' || *line == '\t'))
      {
        // No action.
      }

    if (nread != 1)
      {
        break;
      }

    // read the library name
    len = 1;

    while ((nread = ACE_OS::read (pipe[0], line + len, 1)) == 1
           && (line[len] != ' '))
      {
        if (!bogus && ++len == max_line_length)
          {
            bogus = true;
            break;
          }
      }

    if (nread != 1 || bogus)
      {
        break;
      }

    line[len] = '\0';
    char * const dot = std::strchr (line, '.');

    if (dot != nullptr)
      {
        *dot = '\0';
      }

    char * const libname = line + 3; // skip over "lib"

    // check to see if this is a new library
    bool found = false;

    for (size_t i = 0; !found && i < libs_.size (); ++i)
      {
        found = libs_[i]->name() == libname;
      }

    if (!found)
    {
      std::unique_ptr<Library> nlib = std::make_unique<Library> (
        ACE_TEXT_CHAR_TO_TCHAR (libname));
      // Skip over '=> '.
      if (ACE_OS::read (pipe[0], line, 3) != 3)
        {
          break;
        }

      // get library path
      len = 0;

      while ((nread = ACE_OS::read(pipe[0],line + len,1)) == 1 &&
             (line[len] != ' '))
        {
          if (!bogus && ++len == max_line_length)
            {
              bogus = true;
              break;
            }
        }

      if (nread != 1 || bogus)
        {
          break;
        }

      line[len] = '\0';
      nlib->set_path (line);
      libs_.push_back (std::move (nlib));
    }

    // Skip the rest of the line.
    while ((nread = ACE_OS::read (pipe[0], line, 1)) == 1
           && *line != '\n')
      {
        // No action.
      }

    if (nread != 1)
      {
        break;
      }
  }
  proc.wait ();
  ACE_OS::close (pipe[0]);

  undef_wrapper_.add_source (path, true);
  // now do the ldd, iterate over the results to add new libs, etc.
}

void
SO_Group::analize ()
{
  for (int passcount = 0; undefs_.modified (); ++passcount)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "pass %d, undef count = %zu\n",
                  passcount,
                  undefs_.size ()));

      for (size_t i = 0; i < libs_.size (); ++i)
        libs_[i]->resolve (undefs_);
    }
}

void
SO_Group::write_results ()
{
  for (size_t i = 0; i < libs_.size (); ++i)
    libs_[i]->write_export_list (true);
}

void
SO_Group::load_modules ()
{
  for (size_t i = 0; i < libs_.size (); ++i)
    libs_[i]->load_modules ();
}

void
SO_Group::list_libs ()
{
  ACE_DEBUG ((LM_DEBUG, "Libs subject to analysis:\n"));

  for (size_t i = 0; i < libs_.size (); ++i)
  {
    if (libs_[i]->has_modules ())
      {
        ACE_DEBUG ((LM_DEBUG, "  %s\n", libs_[i]->name ().c_str ()));
      }
  }
}
