// $Id$

using System;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

namespace CUTS
{
  /// <summary>
  ///    Summary description for Component_Metrics.
  /// </summary>
  public partial class Component_Metrics : System.Web.UI.UserControl
  {
    protected string instance_name_;

    protected long instance_id_;

    protected long test_;

    public string InstanceName
    {
      get { return this.instance_name_; }
      set { this.instance_name_ = value; }
    }

    public long Test
    {
      get { return this.test_; }
      set { this.test_ = value; }
    }

    public long InstanceID
    {
      get { return this.instance_id_; }
      set { this.instance_id_ = value; }
    }

    public System.Web.UI.Control SenderMetrics
    {
      get { return this.sender_metrics_; }
    }

    private void Page_Load(object sender, System.EventArgs e)
    {

    }

    protected void toggle_metrics (object sender, System.EventArgs e)
    {

    }

    #region Web Form Designer generated code
    override protected void OnInit(EventArgs e)
    {
      InitializeComponent();

      base.OnInit(e);
    }

    /// <summary>
    ///    Required method for Designer support - do not modify
    ///    the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.Load += new System.EventHandler(this.Page_Load);

    }
    #endregion

    /// <summary>
    ///
    /// </summary>
    // protected System.Web.UI.WebControls.TableCell sender_metrics_;
 }
}
