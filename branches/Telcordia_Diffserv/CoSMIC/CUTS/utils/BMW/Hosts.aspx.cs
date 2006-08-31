// -*- C# -*-

//=============================================================================
/**
 * @file      Hosts.aspx.cs
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
   * @class Hosts
   * 
   * Code-behind class for the Hosts.aspx page.
   */
  //===========================================================================

  public partial class Hosts : System.Web.UI.Page
  {
    private CUTS_Database_Utility cuts_database_ =
      new CUTS_Database_Utility(ConfigurationManager.AppSettings["MySQL"]);

    private DataGridItem prev_item_ = null;

    /**
     * Method invoked when the page is loaded.
     * 
     * @param[in]       sender      Sender of the event.
     * @param[in]       e           Arguments associated with the event.
     */
    private void Page_Load(object sender, System.EventArgs e)
    {
      if (!this.IsPostBack)
        this.load_hosts_from_database();
    }

    /**
     * Load the host information from the database.
     */
    private void load_hosts_from_database()
    {
      try
      {
        DataSet dataset = new DataSet();
        this.cuts_database_.get_testenv(ref dataset);

        this.hosts_.DataSource = dataset.Tables["testenv"];
        this.hosts_.DataBind();
      }
      catch (Exception ex)
      {
        this.register_message_.ForeColor = Color.Red;
        this.register_message_.Text = ex.Message;
      }
    }

    /**
     * Method called when a datagrid item is created.
     * 
     * @param[in]     sender        Sender of the event.
     * @param[in]     e             Arguments associated with event.
     */
    protected void handle_onitemcreated(Object sender,
                                        DataGridItemEventArgs e)
    {
      ListItemType itemtype = e.Item.ItemType;
      DataGrid datagrid = (DataGrid)sender;

      switch (itemtype)
      {
        case ListItemType.Item:
        case ListItemType.AlternatingItem:
          TableCell cell = new TableCell();
          cell.ColumnSpan = datagrid.Columns.Count - 1;

          CUTS.Node_Details_Control details =
            (CUTS.Node_Details_Control)this.LoadControl("Node_Details.ascx");
          cell.Controls.Add(details);

          int index = (2 * e.Item.ItemIndex) + 2;
          DataGridItem item = new DataGridItem(index, 0, ListItemType.Item);
          item.Visible = false;

          item.Cells.Add(new TableCell());
          item.Cells.Add(cell);

          if (this.prev_item_ != null)
          {
            Table table = (Table)datagrid.Controls[0];

            // We are making sure we can actaully add the new row without
            // causing any range exceptions.
            if (table.Rows.Count == this.prev_item_.ItemIndex)
              table.Rows.AddAt(this.prev_item_.ItemIndex, this.prev_item_);
          }

          this.prev_item_ = item;
          break;

        case ListItemType.Footer:
          if (this.prev_item_ != null)
          {
            Table table = (Table)datagrid.Controls[0];

            // We are making sure we can actaully add the new row without
            // causing any range exceptions.
            if (table.Rows.Count == this.prev_item_.ItemIndex)
            {
              table.Rows.AddAt(this.prev_item_.ItemIndex, this.prev_item_);
              this.prev_item_ = null;
            }
          }
          break;
      }
    }

    /**
     * Callback method for clicking the \a register_ button.
     * 
     * @param[in]         sender        Sender of the event.
     * @param[in]         e             Argument associated with event.
     */
    protected void register_clicked (Object sender, EventArgs e)
    {
      if (this.verify_edit_mode(false))
      {
        try
        {
          this.cuts_database_.register_host(this.hostname_.Text);
          this.load_hosts_from_database();
        }
        catch (Exception ex)
        {
          this.register_message_.Text = "error: " + ex.Message;
          this.register_message_.ForeColor = Color.Red;
        }
      }
      else
        this.error_message("cannot register host while in edit mode");
    }

    /**
     * Event send when a item command is triggered.
     * 
     * @param[in]       sender      Sender of the event.
     * @param[in]       e           Event arguments.
     */
    protected void handle_onitemcommand(Object sender,
                                        DataGridCommandEventArgs e)
    {
      if (e.CommandName == "details")
      {
        if (this.verify_edit_mode(false))
        {
          TableCell cell = (TableCell)e.Item.Controls[0];
          LinkButton linkbtn = (LinkButton)cell.Controls[0];

          // Get the "details" row for the selected item.
          int index = (2 * e.Item.ItemIndex) + 2;
          Table table = (Table)this.hosts_.Controls[0];
          TableRow row = table.Rows[index];

          this.toggle_details(table.Rows[index - 1], table.Rows[index]);
        }
        else
          this.error_message("cannot view details while in edit mode");
      }
    }

    /**
     * Handle for edit command for the datagrid.
     * 
     * @param[in]         sender          Sender of the event.
     * @param[in]         e               Arguments of the event.
     */
    protected void handle_oneditcommand(Object sender,
                                        DataGridCommandEventArgs e)
    {
      this.hosts_.EditItemIndex = e.Item.ItemIndex;
      this.load_hosts_from_database();
    }

    /**
     * Handle for update command for the datagrid in edit mode.
     * 
     * @param[in]         sender          Sender of the event.
     * @param[in]         e               Arguments of the event.
     */
    protected void handle_onupdatecommand(Object sender,
                                          DataGridCommandEventArgs e)
    {
      TextBox textbox = (TextBox)e.Item.Cells[3].Controls[0];

      try
      {
        // Update the <testenv> with the new port number.
        System.Int32 port = System.Int32.Parse(textbox.Text);
        System.Int32 hostid = (System.Int32)this.hosts_.DataKeys[e.Item.ItemIndex];
        this.cuts_database_.update_testenv(hostid, port);

        // Update the view.
        this.hosts_.EditItemIndex = -1;
        this.load_hosts_from_database();
      }
      catch (Exception ex)
      {
        this.register_message_.ForeColor = Color.Red;
        this.register_message_.Text = ex.Message;
      }
    }
    
    /**
     * Handle for cancel command for the datagrid in edit mode.
     * 
     * @param[in]         sender          Sender of the event.
     * @param[in]         e               Arguments of the event.
     */
    protected void handle_oncancelcommand(Object sender,
                                          DataGridCommandEventArgs e)
    {
      this.hosts_.EditItemIndex = -1;
      this.load_hosts_from_database();
    }

    /**
     * Shows all the details of the visible nodes.
     * 
     * @param[in]         sender          Sender of the command.
     * @param[in]         e               Command arguments.
     */
    protected void show_all(object sender, 
                            CommandEventArgs e)
    {
      Table table = (Table)this.hosts_.Controls[0];

      for (int i = 2; i < table.Rows.Count; i += 2)
        this.show_details_i(table.Rows[i - 1], table.Rows[i], true);
    }

    /**
     * Hides all the details of the visible nodes.
     * 
     * @param[in]       sender        Sender of the command.
     * @param[in]       e             Command arguments.
     */
    protected void collapse_all(object sender, 
                                CommandEventArgs e)
    {
      Table table = (Table)this.hosts_.Controls[0];

      for (int i = 2; i < table.Rows.Count; i += 2)
        this.show_details_i(table.Rows[i - 1], table.Rows[i], false);
    }

    /**
     * Verify the edit mode of the page.
     * 
     * @param[in]       mode        Mode in question.
     * @retval          true        Page is in specified mode.
     * @retval          false       Page is not in specified mode.
     */
    private bool verify_edit_mode(bool mode)
    {
      return mode ? this.hosts_.EditItemIndex != -1 :
                    this.hosts_.EditItemIndex == -1;
    }

    /**
     * Helper method to toggle the details view.
     * 
     * @param[in]       header      Header row for the details.
     * @param[in]       row         Row containing the details.
     */
    private void toggle_details(TableRow header, TableRow row)
    {
      this.show_details_i(header, row, !row.Visible);
    }

    /**
     * Implemetation method for showing/hiding the details.
     * 
     * @param[in]       header      Header row for the details.
     * @param[in]       row         Row containing the details.
     * @param[in]       show        Show/hide the details.
     */
    private void show_details_i(TableRow header, TableRow row, bool show)
    {
      LinkButton linkbtn = (LinkButton)header.Cells[0].Controls[0];

      if (show)
      {
        if (row.Cells[1].Controls.Count == 1)
        {
          CUTS.Node_Details_Control details = 
            (CUTS.Node_Details_Control)row.Cells[1].Controls[0];

          String ipaddr = header.Cells[1].Text;
          int port = 2809;      /* default IIOP port number */

          try
          {
            port = int.Parse(header.Cells[3].Text);
          }
          catch (Exception)
          {

          }

          details.show_details (ipaddr, port);
        }

        linkbtn.Text = "<img src=\"images/minus.gif\" alt=\"hide\" border=\"0\" />";
      }
      else
      {
        linkbtn.Text = "<img src=\"images/plus.gif\" alt=\"show\" border=\"0\" />";
      }

      row.Visible = show;
    }

    /**
     * Helper method for displaying an error message.
     * 
     * @param[in]       message       Error message.
     */
    private void error_message(string message)
    {
      this.register_message_.Text = message;
      this.register_message_.ForeColor = Color.Red;
    }

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
