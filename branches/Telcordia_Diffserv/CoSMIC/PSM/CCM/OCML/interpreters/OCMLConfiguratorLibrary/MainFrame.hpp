#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/html/htmlwin.h>

#include "OptionEditors/OptionEditorPanel.hpp"
#include "Options/OptionCategory.hpp"
#include "extern_declarations.hpp"
#include "Configurator.hpp"

class OCMLConfigurator_API MainFrameValueChangeListener
{
public:
  virtual void value_change(const char* new_value) = 0;
};

class OCMLConfigurator_API MainFrame: public wxFrame,
                                      public OCML::OptionEditorPanelFocusListener
{
public:
  MainFrame(const wxString& title, const wxPoint& pos,
            const wxSize& size,
            const char* tree_file,
            const char* values, size_t values_size);
  virtual ~MainFrame();

  void add_value_change_listener(MainFrameValueChangeListener* l);
      
private:
  void initialize_tree_node(wxTreeCtrl* tree, wxTreeItemId root_id,
                            OCML::OptionCategory* category);

  wxTreeCtrl* initialize_tree(wxWindow* root);

  void MainFrame::selection_changed(wxTreeEvent& event);

  virtual void editor_focused(OCML::OptionEditor* editor);

  void OnQuit(wxCommandEvent &event);

  void on_ok(wxCommandEvent& event);
  void on_cancel(wxCommandEvent& event);

private:
  wxHtmlWindow* description_;
  wxTreeCtrl* tree_;
  OCML::OptionEditorPanel* options_panel_;
  OCML::Configurator configurator_;

  std::list<MainFrameValueChangeListener*> value_change_listeners_;

  DECLARE_EVENT_TABLE()
};

enum
{
  ID_MAINWIN_QUIT = wxID_HIGHEST+1,
  ID_TREE_SELECTION_CHANGED
};

#endif // MAIN_FRAME_HPP
