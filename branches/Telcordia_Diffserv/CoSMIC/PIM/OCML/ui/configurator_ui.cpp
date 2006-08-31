#include "main_app.hpp"
#include "configurator_ui.hpp"
#include <wx/wx.h>

#include <fstream>
#include <algorithm>
#include <iterator>

#include <windows.h>

DECLARE_APP(MainApp)

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    wxEntry(hModule, 0, NULL, 0, false);
    break;

  case DLL_THREAD_ATTACH:
    break;

  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    wxGetApp().OnExit();
    wxApp::CleanUp();
    break;
  }

  return TRUE;
}

class Value_Change_Listener: public MainFrameValueChangeListener
{
public:
  Value_Change_Listener()
    : changed_(false)
  {
  }

  virtual void value_change(const char* new_value)
  {
    changed_ = true;
    value_ = new_value;
  }

  bool changed() { return changed_; }

  std::string value() { return value_; }

private:
  std::string value_;
  bool changed_;
};

std::string
Configurator_Dialog::show(const std::string& tree_file,
                          const std::string& values,
                          const std::string& rule_file)
{
  wxWindow win;
  win.SetHWND((WXHWND) 0);
  win.Enable(false);

//  std::string values;
  std::string rules;

//  if (value_file != "")
//  {
//    std::ifstream f(value_file.c_str());
//    std::copy(std::istream_iterator<char>(f), std::istream_iterator<char>(),
//              std::back_inserter(values));
//  }

  if (rule_file != "")
  {
    std::ifstream f(rule_file.c_str());
    std::copy(std::istream_iterator<char>(f), std::istream_iterator<char>(),
              std::back_inserter(rules));
  }

  const char* value_file_ptr = 0;
  const char* rule_file_ptr = 0;
  int value_size = values.size();
  int rule_size = rules.size();

  if (value_size)
    value_file_ptr = values.c_str();
  if (rule_size)
    rule_file_ptr = rules.c_str();

  Value_Change_Listener l;
  ((MainApp&) wxGetApp()).show_dialog(tree_file.c_str(),
                                      value_file_ptr, value_size,
                                      rule_file_ptr, rule_size, &l);

  win.Enable(true);
  win.SetHWND(0);

  std::string result;
  if (l.changed())
    result = l.value();
  return result;
}
