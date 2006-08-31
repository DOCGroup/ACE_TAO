using System;
using System.Collections;
using System.Configuration;
using System.ComponentModel;
using System.Data;
using System.Data.Odbc;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

using WebChart;
using MySql.Data;
using MySql.Data.MySqlClient;

namespace CUTS
{
  struct Path_Element
  {
    public Path_Element (long component, string src, string dst)
    {
      this.component_ = component;
      this.src_ = src;
      this.dst_ = dst;
    }

    public long component_;
    public string src_;
    public string dst_;
  }

  public partial class Critical_Path_Timeline : System.Web.UI.Page
  {
    private long test_;

    private long path_;

    private ArrayList critical_path_ = new ArrayList ();

    private void Page_Load(object sender, System.EventArgs e)
    {
      // Get the query string information.
      if (Request.QueryString["t"] != null &&
          Request.QueryString["p"] != null)
      {
        this.test_ = System.Int32.Parse(Request.QueryString["t"]);
        this.path_ = System.Int32.Parse (Request.QueryString["p"]);

        MySqlConnection conn =
          new MySqlConnection (ConfigurationManager.AppSettings["MySQL"]);

        try
        {
          // Open the connection to the database.
          conn.Open ();

          // Get the critical path deadline
          Int32 deadline = get_deadline(conn);

          // Get the elements in the critical path and place them into a
          // reusable queue.
          MySqlDataReader reader = create_path_element_reader (conn);
          extract_path_elements (reader);
          reader.Close ();

          // Get the metrics of the critical path components and create the
          // performance graphs of the metrics.
          reader = create_path_reader (conn);
          create_performance_graphs (reader, deadline);

          // Close the reader and the connection to the database.
          reader.Close ();
          conn.Close ();
        }
        catch (OdbcException ex)
        {
          this.exception_label_.ForeColor = Color.Red;
          this.exception_label_.Text = "ODBC Exception: " + ex.Message;
        }
        catch (Exception ex)
        {
          this.exception_label_.ForeColor = Color.Red;
          this.exception_label_.Text = "Unknown Exception: " + ex.Message;
        }
        finally
        {
          if (conn.State == ConnectionState.Open)
          {
            conn.Close ();
          }
        }
      }
    }

    private void create_performance_graphs (MySqlDataReader metrics,
                                            Int32 deadline)
    {
      // Bypass all the empty metrics in the collection and store
      // the first DateTime value as the <collection_time>.
      while (metrics.Read () && metrics.GetValue (0) == DBNull.Value);
      long best_time = 0, average_time = 0, worse_time = 0;
      bool done = false;

      // Create the collection of points for the best, average and
      // worse execution time charts.
      ChartPointCollection best_points = new ChartPointCollection ();
      ChartPointCollection average_points = new ChartPointCollection ();
      ChartPointCollection worse_points = new ChartPointCollection ();
      ChartPointCollection deadline_points = new ChartPointCollection();

      do
      {
        bool create_point = false;

        // Get the collection_date, component, src and dst port and create
        // a path element out of it.
        DateTime collection_time = (DateTime) metrics.GetDateTime (0);

        Path_Element element = new Path_Element (metrics.GetUInt32 (1),
                                                 metrics.GetString (3),
                                                 metrics.GetString (4));

        // Locate the following metrics in the collection.
        try
        {
          bool valid = true;
          int index = this.critical_path_.IndexOf (element);

          if (index != 0)
          {
            // Check if the sender of this metric is indeed the previous
            // instance in the critical path for this element.
            Path_Element prev_element =
              (Path_Element) this.critical_path_[index - 1];

            if (prev_element.component_ != metrics.GetInt32 (2))
            {
              valid = false;
            }
          }

          // If this is a valid metric then we need to add it's information
          // to the current execution times for this collection time.
          if (valid)
          {
            best_time += metrics.GetInt32 (7);
            average_time += metrics.GetInt32 (8);
            worse_time += metrics.GetInt32 (9);
          }
        }
        catch (Exception)
        {

        }

        // Determine if this is the last record in the listing. If it
        // is not then we need to see if the next record is part of this
        // collection period. If this is the last record then we have
        // to create a new point regardless.
        if (metrics.Read ())
        {
          if (collection_time != metrics.GetDateTime (0))
          {
            create_point = true;
          }
        }
        else
        {
          create_point = true;
          done = true;
        }

        if (create_point)
        {
          // Create a new point for each of the charts using the
          // summations of the metrics collected.
          best_points.Add (new ChartPoint (
                           collection_time.ToString (), best_time));
          average_points.Add (new ChartPoint (
                              collection_time.ToString (), average_time));
          worse_points.Add (new ChartPoint (
                            collection_time.ToString (), worse_time));
          deadline_points.Add(new ChartPoint(
                              collection_time.ToString(), deadline));

          // Reset the time values.
          best_time = average_time = worse_time = 0;
        }
      } while (!done);

      // Create the chart that will display the execution times for
      // the best execution time for the path.
      WebChart.LineChart best_chart =
        new WebChart.LineChart (best_points, Color.Green);
      best_chart.Legend = "Best Time";
      best_chart.Fill.Color = Color.Green;
      this.timeline_.Charts.Add (best_chart);

      // Create the chart that will display the execution times for
      // the average execution time for the path.
      WebChart.LineChart average_chart = 
        new WebChart.LineChart (average_points, Color.Blue);
      average_chart.Legend = "Average Time";
      average_chart.Fill.Color = Color.Blue;
      this.timeline_.Charts.Add (average_chart);

      // Create the chart that will display the execution times for
      // the worse execution time for the path.
      WebChart.LineChart worse_chart = 
        new WebChart.LineChart (worse_points, Color.Red);
      worse_chart.Legend = "Worst Time";
      worse_chart.Fill.Color = Color.Red;
      this.timeline_.Charts.Add (worse_chart);

      // Create the deadline chart and display it as a orange line.
      WebChart.LineChart deadline_chart =
        new WebChart.LineChart(deadline_points, Color.Orange);
      deadline_chart.ShowLineMarkers = true;
      deadline_chart.ShowLegend = false;
      this.timeline_.Charts.Add(deadline_chart);

      // Redraw the charts.
      this.timeline_.RedrawChart ();
    }
 
    private void extract_path_elements (MySqlDataReader reader)
    {
      while (reader.Read ())
      {
        // Create a new element for the path and insert it into the
        // <critical_path_>.
        Path_Element element = new Path_Element (reader.GetInt32 (1),
                                                 reader.GetString (2),
                                                 reader.GetString (3));

        this.critical_path_.Add (element);
      }
    }

    private int get_deadline(MySqlConnection conn)
    {
      MySqlCommand command = conn.CreateCommand();
      command.CommandText =
        "SELECT deadline FROM critical_path WHERE path_id = ?path";
      command.Parameters.Add("?path", this.path_);

      return (Int32) command.ExecuteScalar();
    }

    private MySqlDataReader create_path_element_reader (MySqlConnection conn)
    {
      // Create the MySQL command.
      MySqlCommand command = conn.CreateCommand ();
      command.CommandText = "CALL select_path (?path)";

      // Initailize the parameters
      command.Parameters.Add ("?path", this.path_);

      return command.ExecuteReader ();
    }

    private MySqlDataReader create_path_reader (MySqlConnection conn)
    {
      // Create the MySQL command.
      MySqlCommand command = conn.CreateCommand ();
      command.CommandText = "CALL select_path_execution_times (?test, ?path)";

      // Initailize the parameters
      command.Parameters.Add ("?test", this.test_);
      command.Parameters.Add ("?path", this.path_);

      return command.ExecuteReader ();
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
