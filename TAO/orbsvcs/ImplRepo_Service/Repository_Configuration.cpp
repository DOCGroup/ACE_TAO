// $Id$

#include "Repository_Configuration.h"
#include "NT_Service.h"

#include "ace/Configuration.h"

#if defined (ACE_WIN32)
const HKEY SERVICE_REG_ROOT = HKEY_LOCAL_MACHINE;
const ACE_TCHAR *SERVICE_REG_PATH =
  ACE_TEXT ("SYSTEM\\CurrentControlSet\\Services\\TAOImplRepo\\Parameters");
const ACE_TCHAR *SERVICE_REG_VALUE_NAME = ACE_TEXT ("ORBOptions");
#endif /* ACE_WIN32 */

Repository_Configuration::Repository_Configuration (ACE_TCHAR *repository_mode)
  : config_ (0)
{
  /// Default Constructor
  if (ACE_OS::strcmp (repository_mode, "x") == 0)
    {
      /// XML Mode : Dont do anything special
    }
  else if (ACE_OS::strcmp (repository_mode, "w") == 0)
    {
#if defined (ACE_WIN32)
      /// Win32 Repository.
      HKEY root =
        ACE_Configuration_Win32Registry::resolve_key(HKEY_LOCAL_MACHINE,
                                                     "Software\\TAO\\IR");
      ACE_NEW (this->config_,
               ACE_Configuration_Win32Registry(root));

#endif /* ACE_WIN32 */
    }
 else if (ACE_OS::strcmp (repository_mode, "h") == 0)
    {
      ACE_NEW (this->config_,
               ACE_Configuration_Heap);

      if (this->config_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Initialization of config_ didnt happen correctly\n"));
        }
    }
}

Repository_Configuration::~Repository_Configuration (void)
{
  /// Destructor
  if (this->config_ != 0)
    delete this->config_;
}

const ACE_Configuration_Section_Key &
Repository_Configuration::root_section ()
{
  return this->config_->root_section ();
}

int
Repository_Configuration::open (void)
{
  // This method would have been called only if some kind of
  // persistence is to be used: either the ACE_Configuration_Heap
  // or the XML format of the file.
  if (this->config_ == 0)
    {
      // The persistence file type is to be in XML format. Dont do
      // anything here. Just return.
      return 0;
    }
  else
    {
      // Open the configuration heap.
      ACE_Configuration_Heap *heap =
        ACE_dynamic_cast (ACE_Configuration_Heap *,
                          this->config_);

      return heap->open ();
    }
  return 0;
}


int
Repository_Configuration::open (const ACE_TCHAR *file_name)
{
  if (this->config_ == 0)
    {
      FILE *fp = ACE_OS::fopen (file_name, "w");

      if (fp == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Couldnt open the file %s\n", file_name),
                            -1);
        }

      // @@@ Initial setup: write the DTD here.
      ACE_OS::fprintf (fp,
                       "<?xml version=\"1.0\"?>\n");
      ACE_OS::fprintf (fp,
                       "<!DOCTYPE SERVER_REPOSITORY>\n");
      ACE_OS::fclose (fp);
    }
  else
    {
      ACE_Configuration_Heap *heap =
        ACE_dynamic_cast (ACE_Configuration_Heap *,
                          this->config_);

      return heap->open (file_name);
    }
  return 0;
}

int
Repository_Configuration::open_section (const ACE_Configuration_Section_Key& base,
                                        const ACE_TCHAR* sub_section,
                                        int create,
                                        ACE_Configuration_Section_Key& result)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->open_section (base,
                                          sub_section,
                                          create,
                                          result);
    }
  return 0;

}

int
Repository_Configuration::remove_section (const ACE_Configuration_Section_Key& key,
                                          const ACE_TCHAR* sub_section,
                                          int recursive)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->remove_section (key,
                                          sub_section,
                                          recursive);
    }

  return 0;
}

int
Repository_Configuration::enumerate_sections (const ACE_Configuration_Section_Key& key,
                                              int Index,
                                              ACE_CString& name)
{
  if (this->config_ == 0)
    {
      return 0;
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->enumerate_sections (key,
                                              Index,
                                              name);
    }

  return 0;

}

int
Repository_Configuration::set_string_value (const ACE_Configuration_Section_Key& key,
                        const ACE_TCHAR* name,
                        const ACE_CString& value)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->set_string_value (key,
                                            name,
                                            value);
    }

  return 0;

}

int
Repository_Configuration::set_integer_value (const ACE_Configuration_Section_Key& key,
                                             const ACE_TCHAR* name,
                                             u_int value)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->set_integer_value (key,
                                             name,
                                             value);
    }

  return 0;
}

int
Repository_Configuration::get_string_value (const ACE_Configuration_Section_Key& key,
                                            const ACE_TCHAR* name,
                                            ACE_CString& value)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->get_string_value (key,
                                            name,
                                            value);
    }

  return 0;
}

int
Repository_Configuration::get_integer_value (const ACE_Configuration_Section_Key& key,
                                             const ACE_TCHAR* name,
                                             u_int& value)
{
  if (this->config_ == 0)
    {
      /// @@ May be I need to write the initial DTD etc stuff here.
    }
  else
    {
      return this->config_->get_integer_value (key,
                                               name,
                                               value);
    }

  return 0;

 }
