// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "orbsvcs/CosNamingC.h"
#include "Web_ServerC.h"

// Prototype for function that maps filename to viewer.
int external_viewer (const char *content_type,
                     char *viewer,
                     size_t length);

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: client filename\n"),
                          -1);

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Ossama's Mighty ORB",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get a reference to the Name Service.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("NameService",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Narrow to a Naming Context
      CosNaming::NamingContext_var nc;
      nc = CosNaming::NamingContext::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;      

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil reference to Name Service\n"),
                            -1);
        }

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("Iterator_Factory");
      name[0].kind = CORBA::string_dup ("");

      obj = nc->resolve (name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now narrow to an Iterator_Factory reference.
      Web_Server::Iterator_Factory_var factory =
        Web_Server::Iterator_Factory::_narrow (obj.in ());
      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object pointed to by:\n  %s\n"
                             "is not an Iterator_Factory object.\n",
                             argv[1]),
                            -1);
        }

      // Get a Content_Iterator

      // --------------------------------
      {
        // @@ To get around an obscure bug in TAO, make sure the octet
        //    sequence is destroyed prior the ORB::destroy() call by
        //    placing it inside its own scope.

        // Get the file.
        const char *pathname = argv[1];
        Web_Server::Content_Iterator_var contents;
        Web_Server::Metadata_Type_var metadata;
        factory->get_iterator (pathname,
                               contents,
                               metadata,
                               ACE_TRY_ENV);
        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_INFO,
                    "File <%s> has the following characteristics:\n"
                    "  Modification Date: %s\n"
                    "  Content Type: %s\n",
                    argv[1],
                    metadata->modification_date.in (),
                    metadata->content_type.in ()));

        // Write the contents of the octet sequence to a file with the
        // same name in the request.
        char userid[17];
        ACE_OS::cuserid (userid, 17);

        ACE_Auto_String_Free tempname (ACE_OS::tempnam (0,
                                                        userid));
        ACE_HANDLE handle = ACE_OS::open (tempname.get (),
                                          O_CREAT | O_TRUNC | O_WRONLY,
                                          S_IRUSR | S_IWUSR);

        if (handle == ACE_INVALID_HANDLE)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open file %s%p\n",
                               tempname.get (),
                               ""),
                              -1);
          }

        Web_Server::Chunk_Type_var chunk;
        CORBA::ULong offset = 0;
        while (contents->next_chunk (offset, chunk, ACE_TRY_ENV))
          {
            // Write the received data to a file.
            if (ACE_OS::write (handle,
                               chunk->get_buffer (),
                               chunk->length ()) == -1)
              {
                ACE_OS::close (handle);
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "%p\n",
                                   "Unable to write retrieved data to file"),
                                  -1);
              }
            else
              offset += chunk->length ();
          }
        ACE_TRY_CHECK;

        // Done with the Content_Iterator, so destroy it.
        contents->destroy (ACE_TRY_ENV);
        ACE_TRY_CHECK;

        // No longer need the data file to be open.
        ACE_OS::close (handle);

        ACE_DEBUG ((LM_INFO,
                    "Wrote retrieved data to file <%s>\n",
                    tempname.get ()));

        char viewer[80];  // It is highly unlikey, a mime type will
                          // ever be larger than 80 bytes.

        if (external_viewer (metadata->content_type.in (),
                             viewer,
                             sizeof (viewer)) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Problem determining which external "
                             "viewer to use.\n"),
                            -1);

        // Spawn an external viewer based on the content type returned
        // in the metadata.
        char *const viewer_argv[] =
        { viewer, ACE_const_cast (char *const, tempname.get ()), 0 };


        switch (ACE_OS::fork ())
          {
          case 0:
            // Child
            if (ACE_OS::execvp (viewer, viewer_argv) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%p\n",
                                 "Error during execv() call"),
                                -1);
            else
              return 0;  // NOT REACHED
          case -1:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "Error during fork"),
                              -1);
          default:
            // Parent
            ACE_DEBUG ((LM_INFO,
                        "Spawned viewer <%s>.\n",
                        viewer));
            break;
          }
      }
      // --------------------------------

      orb->shutdown (0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (Web_Server::Error_Result, exc)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught Web Server exception with status %d\n",
                         exc.status),
                        -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught unexpected exception:");

      return -1;
    }
  ACE_ENDTRY;


  return 0;
}

int external_viewer (const char *content_type,
                     char *viewer,
                     size_t length)
{
  if (content_type == 0)
    return -1;

  if (ACE_OS::strcasecmp (content_type, "text/html") == 0)
    {
      const char lynx[] = "lynx";
      if (length <= sizeof (lynx))
        return -1;
      else
        ACE_OS::strcpy (viewer, lynx);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "text/plain") == 0)
    {
      const char more[] = "more";
      if (length <= sizeof (more))
        return -1;
      else
        ACE_OS::strcpy (viewer, more);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "application/postscript") == 0)
    {
      const char ghostview[] = "ghostview";
      if (length <= sizeof (ghostview))
        return -1;
      else
        ACE_OS::strcpy (viewer, ghostview);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "application/pdf") == 0)
    {
      const char acroread[] = "acroread";
      if (length <= sizeof (acroread))
        return -1;
      else
        ACE_OS::strcpy (viewer, acroread);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "image/jpeg") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/gif") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/tiff") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/png") == 0)
    {
      const char xv[] = "xv";
      if (length <= sizeof (xv))
        return -1;
      else
        ACE_OS::strcpy (viewer, xv);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unsupported MIME type: <%s>\n",
                       content_type),
                      -1);

  return 0;
}
