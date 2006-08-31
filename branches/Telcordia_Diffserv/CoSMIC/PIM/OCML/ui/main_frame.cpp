#include "main_frame.hpp"

#include <fstream>
#include <algorithm>
#include <iterator>

#include <wx/splitter.h>
#include "main_app.hpp"

// The following defines wxGetApp()
DECLARE_APP(MainApp)

class CategoryItemData: public wxTreeItemData
{
public:
  CategoryItemData(OCML::Option_Category* category)
    : category_(category) { }
        
  OCML::Option_Category* category()
  {
    return category_;
  }    
        
private:
  OCML::Option_Category* category_;
};    

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_TREE_SEL_CHANGED(ID_TREE_SELECTION_CHANGED, MainFrame::selection_changed)
  EVT_BUTTON(wxID_OK, MainFrame::on_ok)
  EVT_BUTTON(wxID_CANCEL, MainFrame::on_cancel)
END_EVENT_TABLE()

wxPanel* tmp_panel(wxWindow* parent)
{
  return new wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize);
}
  
MainFrame::MainFrame(const wxString& title, const wxPoint& pos,
                     const wxSize& size, 
                     const char* tree_file,
                     const char* rules, size_t rules_size,
                     const char* values, size_t values_size)
  : wxFrame((wxFrame *) NULL, -1, title, pos, size),
    configurator_(OCML::Configurator::instance(tree_file, rules, rules_size, values, values_size))
{
  description_ = new wxHtmlWindow(this);

  // topsizer contains the top double splitted window and buttons panel
  wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

  // a top-down splitter. top: tree/options panel split, down: descriptions
  wxSplitterWindow *main_panel =
    new wxSplitterWindow(this, -1, wxPoint(0, 0), wxSize(400, 500),
                         wxSP_3D);

  // a left-right splitter left: options tree, right: options panel
  wxSplitterWindow *top_panel =
    new wxSplitterWindow(main_panel, -1, wxPoint(0, 0), wxSize(400, 400),
                         wxSP_3D);

  tree_ = initialize_tree(top_panel);

  OCML::Option_Category* root_category = configurator_->root_category();
  options_panel_ = new OCML::Editor_Panel(top_panel, root_category, configurator_);

  top_panel->SplitVertically(tree_, options_panel_);

  description_ = new wxHtmlWindow(main_panel);

  main_panel->SplitHorizontally(top_panel, description_);

  topsizer->Add(main_panel, 1, wxEXPAND | wxALL);
  topsizer->Add(create_button_panel(), 0, wxEXPAND | wxALL);

  SetSizer(topsizer);

  topsizer->SetSizeHints(this);   // set size hints to honour minimum size

  options_panel_->add_focus_listener(this);

  CreateStatusBar(1);
}

wxPanel*
MainFrame::create_button_panel()
{
  // button panel contains the ok and cancel buttons.
  wxPanel *button_panel =
	  new wxPanel(this, -1, wxDefaultPosition, wxSize(500, 40));
  wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
  button_panel->SetSizer(button_sizer);

  wxButton* ok_button = new wxButton(button_panel, wxID_OK, "OK");
  wxButton* cancel_button = new wxButton(button_panel, wxID_CANCEL, "Cancel");

  button_sizer->Add(ok_button, 0, wxALIGN_CENTER | wxALL, 3);
  button_sizer->Add(cancel_button, 0, wxALIGN_CENTER | wxALL, 3);

  // Determine the appropriate size for the buttons panel
  // according to the contained buttons.
  int new_height, new_width;
  ok_button->GetSize(NULL, &new_height);
  button_panel->GetSize(&new_width, NULL);
  button_panel->SetSize(new_width, new_height + 6);
  button_sizer->SetSizeHints(button_panel);

  return button_panel;
}

void
MainFrame::initialize_tree_node(wxTreeCtrl* tree, wxTreeItemId root_id,
                                OCML::Option_Category* category)
{
  for (OCML::Option_Category::category_iterator iter = category->begin_categories();
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
  OCML::Option_Category* root_category = configurator_->root_category();

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
      OCML::Option_Category* category =
        ((CategoryItemData*) tree_->GetItemData(event.GetItem()))->category();
      description_->SetPage(category->description().c_str());
      options_panel_->set_category(category);
    }
}

void
MainFrame::editor_focused(OCML::Option_Editor* editor)
{
  if (description_)
    description_->SetPage(editor->option()->definition()->description().c_str());
}

void
MainFrame::on_ok(wxCommandEvent& event)
{
  for (std::list<MainFrameValueChangeListener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
  {
    (*iter)->value_change(configurator_->write().c_str());
  }
  Close();
}

void
MainFrame::on_cancel(wxCommandEvent& event)
{
  Close();
}

MainFrame::~MainFrame()
{
  delete configurator_;
  wxGetApp().ExitMainLoop();
}

void
MainFrame::add_value_change_listener(MainFrameValueChangeListener* l)
{
  value_change_listeners_.push_back(l);
}


