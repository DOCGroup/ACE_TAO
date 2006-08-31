// $Id$

#include "Utils/Utils.h"
#include <atlcomcli.h>

namespace Utils
{

  // This method prompts a dialog to allow the user to specify a folder
  Utils_Export bool getPath (const std::string& description, std::string& path)
  {
    // Initalize the com library
    HRESULT hr = ::CoInitialize (NULL);
    if (FAILED(hr))
      return false;

    // Dialog instruction
    char display_buffer[MAX_PATH];
    BROWSEINFO folder_browsinfo;
    memset (&folder_browsinfo, 0, sizeof (folder_browsinfo));

    // Set GME as the owner of the dialog
    folder_browsinfo.hwndOwner = GetActiveWindow ();
    // Start the browse from desktop
    folder_browsinfo.pidlRoot = NULL;
    // Pointer to the folder name display buffer
    folder_browsinfo.pszDisplayName = &display_buffer[0];
    // Dialog instruction string
    folder_browsinfo.lpszTitle = description.c_str();
    // Use new GUI style and allow edit plus file view
    folder_browsinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    // No callback function
    folder_browsinfo.lpfn = NULL;
    // No parameter passing into the dialog
    folder_browsinfo.lParam = 0;

    // Show the Browse... folder and save the result.
    LPITEMIDLIST folder_pidl = SHBrowseForFolder(&folder_browsinfo);
    bool result = folder_pidl != NULL;

    if (result)
    {
      TCHAR FolderNameBuffer[MAX_PATH];

      // Convert the selection into a path
      if (SHGetPathFromIDList (folder_pidl, FolderNameBuffer))
        path = FolderNameBuffer;

      // Free the ItemIDList object returned from the call to
      // SHBrowseForFolder using Gawp utility function
      ::CoTaskMemFree (folder_pidl);
    }

    ::CoUninitialize ();
    return result;
  }

  std::string CreateUuid (void)
  {
    std::string idstr ("");

    // Create a new UUID in the temporary buffer.
    UUID uuid;
    HRESULT hr = UuidCreate (&uuid);

    if (hr == RPC_S_OK)
    {
      // Convert the UUID to a string.
      unsigned char * temp = 0;
      hr = UuidToString (&uuid, &temp);

      if (hr == RPC_S_OK)
      {
        // Save the string and convert it to uppercase.
        idstr.assign ((const char *)temp);
        std::transform (idstr.begin(), idstr.end(), idstr.begin(), std::toupper);

        // Free the allocated string for the UUID.
        RpcStringFree (&temp);
      }
    }
    return idstr;
  }

  bool ValidUuid (const std::string & uuid)
  {
    UUID uuid_placeholder;

    HRESULT result =
      UuidFromString ((unsigned char *)uuid.c_str (), &uuid_placeholder);

    return result == RPC_S_OK;
  }
} // namespace Utils
