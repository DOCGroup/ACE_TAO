// $Id$

using System;
using System.Collections;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Data.Odbc;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

using WebChart;

namespace CUTS
{
  /// <summary>
  /// Summary description for Timeline.
  /// </summary>
  public partial class Timeline : System.Web.UI.Page
  {
    protected string component_name_;

    protected string sender_name_;

    private void Page_Load(object sender, System.EventArgs e)
    {
      OdbcConnection conn =
        new OdbcConnection (ConfigurationManager.AppSettings["ConnectionString"]);

      try
      {
        // Get the appropriate values from the query string.
        int test_number = System.Int32.Parse (Request.QueryString["t"]);
        int component = System.Int32.Parse (Request.QueryString["c"]);
        int sender_component = System.Int32.Parse (Request.QueryString["s"]);
        string metric = Request.QueryString["m"];
        string src = Request.QueryString["src"];
        string dst = Request.QueryString["dst"];

        this.return_link_.NavigateUrl = "~/Execution_Times.aspx?test=" + test_number;

        conn.Open ();

        // Create the command to get the name of the sender component.
        string command_str =
          "SELECT component_name FROM component_instances WHERE (component_id = ?)";
        OdbcCommand select_component_name = new OdbcCommand (command_str, conn);

        // Initialize the paramater.
        OdbcParameter p1 = new OdbcParameter ("component_id", OdbcType.Int);
        p1.Value = sender_component;
        select_component_name.Parameters.Add (p1);

        // Get the name of the sender component.
        this.sender_name_ = (string) select_component_name.ExecuteScalar ();

        // Get the name of the component w/ the metrics.
        select_component_name.Parameters["component_id"].Value = component;
        this.component_name_ = (string) select_component_name.ExecuteScalar ();

        this.timeline_.ChartTitle.Text =
          this.component_name_ + "\nfrom " + this.sender_name_ +
          "\n[metric = '" + metric + "' AND input = '" + src + "'";

        if (dst != String.Empty)
        {
          this.timeline_.ChartTitle.Text += " AND output = '" + dst + "'";
        }

        this.timeline_.ChartTitle.Text += "]";

        // Create the command to get the desired execution times
        System.Text.StringBuilder builder = new System.Text.StringBuilder ();
        builder.Append ("SELECT collection_time, best_time, average_time, worse_time ");
        builder.Append ("FROM execution_time WHERE (test_number = ? AND component = ? ");
        builder.Append ("AND sender = ? AND metric_type = ? AND src = ?");
        if (dst != "")
        {
          builder.Append (" AND dst = ?");
        }

        builder.Append (") ORDER BY collection_time");

        // Create the selection command.
        OdbcCommand select_command = new OdbcCommand (builder.ToString (), conn);

        // Create all the parameters for the command.
        p1 = new OdbcParameter ("t", OdbcType.Int);
        p1.Value = test_number;
        select_command.Parameters.Add (p1);

        OdbcParameter p2 = new OdbcParameter ("c", OdbcType.Int);
        p2.Value = component;
        select_command.Parameters.Add (p2);

        OdbcParameter p3 = new OdbcParameter ("s", OdbcType.Int);
        p3.Value = sender_component;
        select_command.Parameters.Add (p3);

        OdbcParameter p4 = new OdbcParameter ("m", OdbcType.VarChar);
        p4.Value = metric;
        select_command.Parameters.Add (p4);

        OdbcParameter p5 = new OdbcParameter ("src", OdbcType.VarChar);
        p5.Value = src;
        select_command.Parameters.Add (p5);

        if (dst != "")
        {
          OdbcParameter p6 = new OdbcParameter ("dst", OdbcType.VarChar);
          p6.Value = dst;
          select_command.Parameters.Add (p6);
        }

        // Add the parameters to the command.

        // Create a new adapter with using the <select_command>.
        OdbcDataAdapter adapter = new OdbcDataAdapter (select_command);

        // Fill the <dataset> using the <adapter>.
        DataSet ds = new DataSet ();
        adapter.Fill (ds, "execution_time");

        // Get the max time for the worse execution time and update
        // the chart so that the Y-axis is 10 msec more that the
        // max value.
        this.timeline_.YCustomEnd =
          get_max_execution_time(conn, test_number, component) + 10;

        // Close the connection to the database.
        conn.Close ();

        // Create the execution time charts.
        DataTable execution_time = ds.Tables["execution_time"];
        create_execution_time_charts (execution_time);

      }
      catch (OdbcException ex)
      {
        this.exception_label_.Text =
          "ODBC exception has occured; " + ex.Message;
      }
      catch (Exception ex)
      {
        this.exception_label_.Text =
          "Unknown exeception has occurred; " + ex.Message;
      }
      finally
      {
        // Close the connection if it is still open.
        if (conn.State == ConnectionState.Open)
        {
          conn.Close ();
        }

        // Force the redrawing of the chart.
        this.timeline_.RedrawChart ();
      }
    }

    /// <summary>
    /// Creates the execution time charts given the data table. The table
    /// must contain the following fields: collection_time, best_time,
    /// average_time and worse_time.
    /// </summary>
    /// <param name="table"></param>
    private void create_execution_time_charts (DataTable table)
    {
      // Create the <best_time> execution time chart.
      LineChart best_time = new LineChart ();
      best_time.Legend = "Best Time";
      best_time.DataSource = table.DefaultView;
      best_time.DataXValueField = "collection_time";
      best_time.DataYValueField = "best_time";
      best_time.Line.Color = Color.Green;
      best_time.Fill.Color = Color.Green;
      best_time.DataBind ();
      this.timeline_.Charts.Add (best_time);

      // Create the <average_time> execution time chart.
      LineChart average_time = new LineChart ();
      average_time.Legend = "Average Time";
      average_time.DataSource = table.DefaultView;
      average_time.DataXValueField = "collection_time";
      average_time.DataYValueField = "average_time";
      average_time.Line.Color = Color.Blue;
      average_time.Fill.Color = Color.Blue;
      average_time.DataBind ();
      this.timeline_.Charts.Add (average_time);

      // Create the <worse_time> execution time chart.
      LineChart worse_time = new LineChart ();
      worse_time.Legend = "Worst Time";
      worse_time.DataSource = table.DefaultView;
      worse_time.DataXValueField = "collection_time";
      worse_time.DataYValueField = "worse_time";
      worse_time.Line.Color = Color.Red;
      worse_time.Fill.Color = Color.Red;
      worse_time.DataBind ();
      this.timeline_.Charts.Add (worse_time);
    }

    private System.Int32 get_max_execution_time (
      OdbcConnection conn,
      int test, 
      int component)
    {
      OdbcCommand command = conn.CreateCommand ();

      // Create the <test_number> parameter.
      OdbcParameter p1 = new OdbcParameter ("test_number", OdbcType.Int);
      p1.Direction = ParameterDirection.Input;
      p1.Value = test;

      // Create the <component> parameter.
      OdbcParameter p2 = new OdbcParameter("component", OdbcType.Int);
      p2.Direction = ParameterDirection.Input;
      p2.Value = component;

      // Create the command and initialize the parameters.
      System.Text.StringBuilder string_builder = new System.Text.StringBuilder();
      string_builder.Append("SELECT MAX(worse_time) FROM execution_time ");
      string_builder.Append("WHERE (test_number = ? AND component = ?)");

      command.CommandText = string_builder.ToString();
      command.Parameters.Add(p1);
      command.Parameters.Add(p2);

      // Execute the parameters.
      return (System.Int32)command.ExecuteScalar();
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
