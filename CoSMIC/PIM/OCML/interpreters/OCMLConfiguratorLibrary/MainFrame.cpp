#include "MainFrame.hpp"

#include <fstream>
#include <algorithm>
#include <iterator>

#include <wx/splitter.h>
#include "MainApp.hpp"

using namespace OCML;

// The following defines wxGetApp()
DECLARE_APP(MainApp)

class CategoryItemData: public wxTreeItemData
{
public:
  CategoryItemData(OptionCategory* category)
    : category_(category) { }
        
  OptionCategory* category()
  {
    return category_;
  }    
        
private:
  OptionCategory* category_;
};    

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_TREE_SEL_CHANGED(ID_TREE_SELECTION_CHANGED, MainFrame::selection_changed)
  EVT_BUTTON(wxID_OK, MainFrame::on_ok)
  EVT_BUTTON(wxID_CANCEL, MainFrame::on_cancel)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos,
                     const wxSize& size, 
                     const char* tree_file,
                     const char* values, size_t values_size)
  : wxFrame((wxFrame *) NULL, -1, title, pos, size),
    description_(NULL),
    configurator_(tree_file)
{
  configurator_.initialize_options(values, values_size);

  // topsizer contains the top double splitted window and buttons panel
  wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

  // a top-down splitter top: tree/options panel split, down: descriptions
  wxSplitterWindow *top_panel =
    new wxSplitterWindow(this, -1, wxPoint(0, 0), wxSize(400, 500),
                         wxSP_3D);

  // a left-right splitter left: options tree, right: options panel
  wxSplitterWindow *v_split =
    new wxSplitterWindow(top_panel, -1, wxPoint(0, 0), wxSize(400, 400),
                         wxSP_3D);

  tree_ = initialize_tree(v_split);

  OptionCategory* root_category = configurator_.root_category();
  options_panel_ = new OptionEditorPanel(v_split, root_category);
  v_split->SplitVertically(tree_, options_panel_);

  description_ = new wxHtmlWindow(top_panel, -1);

  top_panel->SplitHorizontally(v_split, description_);

  topsizer->Add(top_panel, 1, wxEXPAND | wxALL);

  // button panel contains the ok and cancel buttons.
  wxPanel *button_panel =
	  new wxPanel(this, -1, wxDefaultPosition, wxSize(500, 40));
  wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
  button_panel->SetSizer(button_sizer);

  wxButton* ok_button =
	  new wxButton(button_panel, wxID_OK, "OK");
  wxButton* cancel_button =
	  new wxButton(button_panel, wxID_CANCEL, "Cancel");

  button_sizer->Add(ok_button, 0, wxALIGN_CENTER | wxALL, 3);
  button_sizer->Add(cancel_button, 0, wxALIGN_CENTER | wxALL, 3);

  // Determine the appropriate size for the buttons panel
  // according to the contained buttons.
  int new_height;
  int new_width;
  ok_button->GetSize(NULL, &new_height);
  button_panel->GetSize(&new_width, NULL);
  button_panel->SetSize(new_width, new_height + 6);
  topsizer->Add(button_panel, 0, wxEXPAND | wxALL);

  SetSizer(topsizer);

  button_sizer->SetSizeHints( button_panel );
  topsizer->SetSizeHints( this );   // set size hints to honour minimum size

  options_panel_->add_focus_listener((OptionEditorPanelFocusListener*)this);

  // Creates a status bar
  CreateStatusBar(1);
}

void
MainFrame::initialize_tree_node(wxTreeCtrl* tree, wxTreeItemId root_id,
                                OptionCategory* category)
{
  for (OptionCategory::category_iterator iter = category->begin_categories();
       iter != category->end_categories(); ++iter)
    {
      wxTreeItemId node_id =
        tree->AppendItem(root_id, (*iter)->name().c_str(), -1, -1, 
                         new CategoryItemData(*iter));
      initialize_tree_node(tree, node_id, *iter); 
    }
}

wxTreeCtrl*
MainFrame::initialize_tree(wxWindow* root)
{
  OptionCategory* root_category = configurator_.root_category();

  wxTreeCtrl *tree = new wxTreeCtrl(root, ID_TREE_SELECTION_CHANGED);
  wxTreeItemId root_id =
    tree->AddRoot(root_category->name().c_str(), -1, -1,
                  new CategoryItemData(root_category));

  initialize_tree_node(tree, root_id, root_category);
    
  return tree;
}

void
MainFrame::selection_changed(wxTreeEvent& event)
{
  if (description_)
    {
      OptionCategory* category =
        ((CategoryItemData*) tree_->GetItemData(event.GetItem()))->category();
      description_->SetPage(category->description().c_str());
      options_panel_->set_category(category);
    }
}

void
MainFrame::editor_focused(OptionEditor* editor)
{
  if (description_)
    description_->SetPage(editor->option()->description().c_str());
}

void
MainFrame::on_ok(wxCommandEvent& event)
{
  for (std::list<MainFrameValueChangeListener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
    (*iter)->value_change(configurator_.write().c_str());

  Close();
}

void
MainFrame::on_cancel(wxCommandEvent& event)
{
  Close();
}

MainFrame::~MainFrame()
{
  wxGetApp().ExitMainLoop();
}

void
MainFrame::add_value_change_listener(MainFrameValueChangeListener* l)
{
  value_change_listeners_.push_back(l);
}
