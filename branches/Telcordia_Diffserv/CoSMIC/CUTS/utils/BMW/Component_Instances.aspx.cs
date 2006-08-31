// -*- C# -*- 

//=============================================================================
/**
 * @file      Component_Instances.aspx.cs
 * 
 * $Id$
 * 
 * @author    James H. Hill
 */
//=============================================================================

using System;
using System.Collections;
using System.ComponentModel;
using System.Configuration;
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
   * @class Component_Instances
   * 
   * Code-behind implemetation for the Component_Instances.aspx 
   * page.
   */
  //===========================================================================

  public partial class Component_Instances : System.Web.UI.Page
  {
    private CUTS_Database_Utility cutsdb_ =
      new CUTS_Database_Utility(ConfigurationManager.AppSettings["MySQL"]);

    /**
     * Callback method for when the page is loading.
     * 
     * @param[in]       sender        Sender of the event.
     * @param[in]       e             Event arguments.
     */
    private void Page_Load(object sender, System.EventArgs e)
    {
      if (!this.IsPostBack)
      {
        try
        {
          DataSet ds = new DataSet();
          this.cutsdb_.get_component_instances(ref ds);

          // Expose the <DefaultView> of the result.
          this.component_instances_.DataSource = ds.Tables["instances"];
          this.component_instances_.DataBind();
        }
        catch (Exception ex)
        {
          this.message_.Text = ex.Message;
        }

        // Initialize the current page of the table.
        this.component_instances_.CurrentPageIndex = 0;
      }
    }

    /**
     * Callback method for when the page index changes.
     * 
     * @param[in]     sender        Sender of the event.
     * @param[in]     e             Event arguments.
     */
    protected void handle_onpageindexchanged (
      Object sender, 
      DataGridPageChangedEventArgs e)
    {
      this.component_instances_.CurrentPageIndex = e.NewPageIndex;
    }

    /**
     * Callback method for creating an item in the datagrid.
     * 
     * @param[in]     sender        Sender of the event.
     * @param[in]     e             Event arguments
     */
    protected void handle_onitemcreated(Object sender, DataGridItemEventArgs e)
    {
      ListItemType item_type = e.Item.ItemType;

      if (item_type == ListItemType.Pager)
      {
        TableCell pager = (TableCell)e.Item.Controls[0];

        if (pager.Controls.Count > 0)
        {
          Label label = (Label)pager.Controls[0];
          label.Text = "Page: " + label.Text;
        }
      }
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
