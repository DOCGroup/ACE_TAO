/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//   jaws
//
// = FILENAME
//    HTTP_Config.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

// = Forward declaration.
class HTTP_Config_Info;

class HTTP_Config
  // = TITLE
  //     Stores server configuration information.
  //     Someday, this will be hip and cool and be able to parse
  //     NCSA httpd style config files like Apache does.  For now,
  //     I'm just going to hack in environment variable stuff.
  //
  //     Designed around Singleton pattern.
{
public:
  static HTTP_Config_Info *instance (void);
  // Access the Singleton.

private:
  static HTTP_Config_Info *instance_;
  // Store the Singleton.
};

class HTTP_Config_Info
  // = TITLE
  //     This is where the information is really stored.
{
friend class HTTP_Config;
public:
  HTTP_Config_Info (void);
  ~HTTP_Config_Info (void);

  // Accessors to the information

  const char *document_root (void) const;
  // Where the root of the document tree begins.  This prevents
  // clients from being able to examine your entire filesystem.

  const char *cgi_path (void) const;
  // A search path for CGI files.

  const char *user_dir (void) const;
  // The directory which is appended to a home user directory, e.g.,
  // ".www-docs" or "public_html".

  const char *dir_index (void) const;
  // What is the default index file for a directory, e.g.,
  // "index.html".

  int proxy_flag (void) const;
  // Will the server support proxy requests?

private:
  // = Accesors that can set the data

  char *document_root (char *dr_string);
  char *cgi_path (char *cp_string);
  char *user_dir (char *ud_string);
  char *dir_index (char *di_string);

  int proxy_flag (int pf);

private:
  // = Data members

  char *document_root_;
  // The directory root from which documents will be fetched

  char *cgi_path_;
  // The directories from which to expect CGI scripts

  char *user_dir_;
  // Name of the sub-directory where user Web pages are

  char *dir_index_;
  // Name of the Web page to present in place of a directory listing

  int proxy_flag_;
  // Should we support proxy requests?  Ignored for now.
};
