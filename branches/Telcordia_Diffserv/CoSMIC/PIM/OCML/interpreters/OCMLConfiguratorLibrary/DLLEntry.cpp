#include "MainApp.hpp"
#include "DLLEntry.hpp"

#include <string.h>
#include <string>
#include <algorithm>

// The following defines wxGetApp() which is used in DllMain.
DECLARE_APP(MainApp)

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
  switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
      // 
      // Start wxDLLApp
      //
      // Note the 4th entry means we don't enter the main application
      // loop now.

      wxEntry(hModule, 0, NULL, 0, false);
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      //
      // Shutdown wxDLLApp
      //
      wxGetApp().OnExit();
      wxApp::CleanUp();
      break;
    }

  return TRUE;
}

class OCMLConfigurator_API Listener: public MainFrameValueChangeListener
{
public:
  Listener()
    : value_set_(false)
  {
  }

  virtual void value_change(const char* value)
  {
    value_set_ = true;
    value_ = value;
  }

  std::string value_;
  bool value_set_;
};

extern "C" OCMLConfigurator_API char*
DLLFunction(const char* values, size_t values_size,
            const char* tree_file)
{
  Listener listener;

  wxApp& app = wxGetApp();
  ((MainApp*)(&app))->new_instance(values, values_size,
                                   &listener, tree_file);
  wxGetApp().MainLoop();

  char* result = 0;
  if (listener.value_set_)
    result = strdup(listener.value_.c_str());

  return result;
}
