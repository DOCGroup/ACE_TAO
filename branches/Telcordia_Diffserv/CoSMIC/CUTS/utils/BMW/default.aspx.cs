// -*- C# -*-

//=============================================================================
/**
 * @file      default.aspx.cs
 * 
 * $Id$
 * 
 * @author    James H. Hill
 */
//=============================================================================

using System;
using System.Collections;
using System.Configuration;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

namespace CUTS
{
  //===========================================================================
  /**
   * @class Test  
   * 
   * Backend source for the default.aspx web page.
   */
  //===========================================================================

  public partial class Test : System.Web.UI.Page
  {
    private CUTS_Database_Utility cuts_database_ =
      new CUTS_Database_Utility(ConfigurationManager.AppSettings["MySQL"]);

    /**
     * Callback method for loading a page.
     * 
     * @param[in]     sender        Sender of the event.
     * @param[in]     e             Arguments for the event.
     */
    private void Page_Load (object sender, 
                            System.EventArgs e)
    {
      if (!this.IsPostBack)
        this.load_test_data();
    }

    /**
     * Callback method for changing the index of a page.
     * 
     * @param[in]       sender        Sender of the event.
     * @param[in]       e             Arguments for the event.
     */
    protected void handle_onpageindexchanged(object sender, 
                                             DataGridPageChangedEventArgs e)
    {
      this.tests_.CurrentPageIndex = e.NewPageIndex;
      this.load_test_data ();
    }

    //
    // status_image_filename
    //
    protected string status_image_filename (string status)
    {
      switch (status)
      {
      case "active":
        return "stoplight-green.gif";

      case "inactive":
        return "stoplight-green.gif";

      default:
        return "stoplight-blank.gif";
      }
    }

    /**
     * Callback method for creating items in a datagrid.
     * 
     * @param[in]     sender        Sender of the event.
     * @param[in]     e             Arguments for the event.
     */
    protected void handle_onitemcreated(object sender, 
                                        DataGridItemEventArgs e)
    {
      ListItemType item_type = e.Item.ItemType;

      switch (item_type)
      {
        case ListItemType.Pager:
          TableCell pager = (TableCell)e.Item.Controls[0];
          int count = pager.Controls.Count;

          for (int i = 0; i < count; i += 2)
          {
            Object obj = pager.Controls[i];

            if (obj is LinkButton)
            {
              LinkButton link = (LinkButton)obj;
              link.Font.Underline = true;
              link.ForeColor = Color.Blue;
            }
            else
            {
              Label label = (Label) obj;
              label.Text = "[ " + label.Text + " ]";
            }
          }

          pager.Controls.AddAt(0, new LiteralControl("Page(s): "));

          break;

        case ListItemType.Header:
          break;

        default:
          /* do nothing */
          break;
      }
    }

    //
    // load_test_data
    //
    private void load_test_data ()
    {
      try
      {
        // Get all the test from the database.
        DataSet ds = new DataSet();
        this.cuts_database_.get_all_test(ref ds);

        // Expose the <DefaultView> of the result.
        this.tests_.DataSource = ds.Tables["tests"];
        this.tests_.DataBind ();
      }
      catch (Exception ex)
      {
        this.message_.Text = ex.Message;
      }
    }

    /**
     * Callback method for selecting all the test on the 
     * current page.
     * 
     * @param[in]       e       Arguments for the event.
     */
    protected void handle_toggle_action (object sender, 
                                         System.EventArgs e)
    {
      // The sender of this event is a <CheckBox>
      CheckBox check = (CheckBox)sender;

      if (check != null)
        this.toggle_action_i(check.Checked);
    }

    /**
     * Callback method for clicking the "Select All" link button.
     * 
     * @param[in]       sender          Sender of the event.
     * @param[in]       e               Arguments for the event.
     */
    protected void handle_select_all(object sender, EventArgs e)
    {
      this.toggle_action_i(true);
    }

    /**
     * Callback method for clicking the "Unselect All" link.
     * 
     * @param[in]       sender        Sender of the event.
     * @param[in]       e             Arguments for the event.
     */
    protected void handle_unselect_all(object sender,
                                       System.EventArgs e)
    {
      this.toggle_action_i(false);
    }

    /**
     * Helper method for toggling all the "action_" checkboxes
     * on the current page.
     * 
     * @param[in]     state         Enable state for the item.
     */
    private void toggle_action_i (bool state)
    {
      foreach (DataGridItem item in this.tests_.Items)
      {
        CheckBox action = (CheckBox)item.FindControl("action_");

        if (action != null)
          action.Checked = state;
      }
    }

    /**
     * Callback method for clicking the "Delete All" link.
     * 
     * @param[in]       sender        Sender of the event.
     */
    protected void handle_delete_all(object sender, 
                                     System.EventArgs e)
    {
      // Create a list for holding the selected numbers.
      ArrayList list = new ArrayList();

      foreach (DataGridItem item in this.tests_.Items)
      {
        // Locate the <action_> control since it's the checkbox
        // that determines the action of the current test.
        CheckBox action = (CheckBox) item.FindControl("action_");

        if (action != null && action.Checked)
        {
          // Add the test number to the list if we are checked.
          System.Int32 testnum = 
            (System.Int32)this.tests_.DataKeys[item.ItemIndex];

          list.Add(testnum);
        }
      }

      if (list.Count > 0)
      {
        // Let's convert the array to an <System.Int32> array
        // and pass control to the database utility.
        System.Int32 [] testlist = 
          (System.Int32 [])list.ToArray(typeof(System.Int32));

        this.cuts_database_.delete_tests(testlist);
        this.load_test_data();
      }
    }
    ///
    #region Web Form Designer generated code
    override protected void OnInit(EventArgs e)
    {
      //
      // CODEGEN: This call is required by the ASP.NET Web Form Designer.
      //
      InitializeComponent();
      base.OnInit(e);
    }

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.Load += new System.EventHandler(this.Page_Load);

    }
    #endregion
  }
}
