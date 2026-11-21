/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    HTTP_Config.h
 *
 *  @author James Hu
 */
//=============================================================================

class HTTP_Config_Info;

/**
 * @class HTTP_Config
 *
 * @brief Stores server configuration information.
 * Someday, this will be hip and cool and be able to parse
 * NCSA httpd style config files like Apache does.  For now,
 * I'm just going to hack in environment variable stuff.
 * Designed around Singleton pattern.
 */
class HTTP_Config
{
public:
  /// Access the Singleton.
  static HTTP_Config_Info *instance ();
};

/**
 * @class HTTP_Config_Info
 *
 * @brief This is where the information is really stored.
 */
class HTTP_Config_Info
{
friend class HTTP_Config;
public:
  HTTP_Config_Info ();
  HTTP_Config_Info (HTTP_Config_Info const &) = delete;
  HTTP_Config_Info (HTTP_Config_Info &&) = delete;
  HTTP_Config_Info &operator= (HTTP_Config_Info const &) = delete;
  HTTP_Config_Info &operator= (HTTP_Config_Info &&) = delete;
  ~HTTP_Config_Info () = default;

  // Accessors to the information

  /// Where the root of the document tree begins.  This prevents
  /// clients from being able to examine your entire filesystem.
  char const *document_root () const;

  /// A search path for CGI files.
  char const *cgi_path () const;

  /// The directory which is appended to a home user directory, e.g.,
  /// ".www-docs" or "public_html".
  char const *user_dir () const;

  /// What is the default index file for a directory, e.g.,
  /// "index.html".
  char const *dir_index () const;

  /// Will the server support proxy requests?
  int proxy_flag () const;

private:
  /// The directory root from which documents will be fetched
  const char *document_root_ {};

  /// The directories from which to expect CGI scripts
  const char *cgi_path_ {};

  /// Name of the sub-directory where user Web pages are
  const char *user_dir_ {};

  /// Name of the Web page to present in place of a directory listing
  const char *dir_index_ {};

  /// Should we support proxy requests?  Ignored for now.
  int proxy_flag_ {};
};
