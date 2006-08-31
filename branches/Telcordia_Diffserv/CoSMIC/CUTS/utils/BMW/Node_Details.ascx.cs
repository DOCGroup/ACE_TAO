// -*- C# -*-

//=============================================================================
/**
 * @file        Node_Details.ascx.cs
 * 
 * $Id$
 * 
 * @author      James H. Hill
 */
//=============================================================================

using System;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;

// from IIOPChannel.dll
using Ch.Elca.Iiop.Idl;
using Ch.Elca.Iiop;
using Ch.Elca.Iiop.Services;
using omg.org.CORBA;

namespace CUTS
{
  //============================================================================
  /**
   * @class Node_Details_Control
   */
  //============================================================================

  public partial class Node_Details_Control : System.Web.UI.UserControl
  {
    /**
     * Event handler for loading the page.
     * 
     * @param[in]       sender      Sender of the event.
     * @param[in]       e           Arguments for the event.
     */
    private void Page_Load(object sender, System.EventArgs e)
    {
      if (!this.IsPostBack)
      {
        try
        {
          IiopChannel channel = new IiopChannel();
          ChannelServices.RegisterChannel(channel, false);
        }
        catch (RemotingException)
        {

        }
      }
    }

    /**
     * Show the details of the node.
     * 
     * @param[in]     ipaddr      IP-address of the port.
     * @param[in]     port        Port number.
     */
    public void show_details (String ipaddr, int port)
    {
      // Save the address of the node daemon for postback.
      String addr = String.Format("{0}:{1}", ipaddr, port);
      this.daemon_.Value = addr;

      // Call the help method to make out life easier.
      this.show_details_i();
    }

    /**
     * Implemenation for showing the details of the node managers.
     */
    private void show_details_i ()
    {
      CUTS.Node_Daemon daemon = this.get_daemon();

      if (daemon != null)
        this.show_details_i(daemon);
    }

    /**
     * Implemenation method showing the details given a node 
     * deamon. This is useful when showing updated information
     * without having to requery for the node daemon.
     */
    private void show_details_i(CUTS.Node_Daemon daemon)
    {
      if (this.local_.Items.Count > 0)
        this.local_.Items.Clear();
      if (this.nonlocal_.Items.Count > 0)
        this.nonlocal_.Items.Clear();

      try
      {
        CUTS.Node_Binding[] bindings = daemon.details();

        foreach (CUTS.Node_Binding binding in bindings)
        {
          ListItem item = new ListItem(binding.port.ToString(),
                                       binding.port.ToString());
          if (binding.localhost)
            this.local_.Items.Add(item);
          else
            this.nonlocal_.Items.Add(item);
        }
      }
      catch (omg.org.CORBA.COMM_FAILURE)
      {
        this.error_message(
          String.Format("there is no daemon active on {0}",
                        this.daemon_.Value));
      }
      catch (omg.org.CORBA.TRANSIENT)
      {
        this.error_message(
          String.Format("failed to communicate with daemon on {0}",
                        this.daemon_.Value));
      }
      catch (Exception ex)
      {
        this.error_message(ex.Message);
      }
    }

    /**
     * Method called when the "Kill Selected" link is triggered
     * 
     * @param[in]     sender          Sender of the event.
     * @param[in]     e               Arguments associated with command.
     */
    protected void kill_nodes(Object sender, CommandEventArgs e)
    {
      LinkButton linkbtn = (LinkButton)sender;
      CUTS.Node_Details_Control parent = (CUTS.Node_Details_Control)linkbtn.Parent;
      int selected = 0;
      
      // Before we can do anything with the data, we have to figure out
      // how many items are selected all togther.
      foreach (ListItem item in parent.local_.Items)
      {
        if (item.Selected)
          selected++;
      }

      foreach (ListItem item in parent.nonlocal_.Items)
      {
        if (item.Selected)
          selected++;
      }

      // Now that we know how many items are selected, we can create
      // an array of the correct size. In the future, we should register
      // a callback method that keeps track of the count.
      if (selected > 0)
      {
        int index = 0;
        CUTS.Node_Binding [] bindings = new CUTS.Node_Binding[selected];

        foreach (ListItem item in parent.local_.Items)
        {
          if (item.Selected)
          {
            bindings[index].localhost = true;
            bindings[index].port = short.Parse(item.Text);

            ++index;
          }
        }

        foreach (ListItem item in parent.nonlocal_.Items)
        {
          if (item.Selected)
          {
            bindings[index].localhost = false;
            bindings[index].port = short.Parse(item.Value);

            ++index;
          }
        }

        CUTS.Node_Daemon daemon = this.get_daemon();

        try
        {
          // Connect to the <CUTS.Node_Daemon> and kill the selected nodes.
          int count = daemon.kill(bindings);

          // Show a message to the user.
          this.message(String.Format("successfully killed {0} node manager(s)",
                                     count));
        }
        catch (Exception ex)
        {
          this.error_message(ex.Message);
        }

        this.show_details_i(daemon);
      }
    }

    /**
     * Get the CUTS.Node_Daemon using the current ip-address and port
     * number.
     * 
     * @return      Interface to the daemon.
     */
    private CUTS.Node_Daemon get_daemon()
    {
      return this.get_daemon(this.daemon_.Value);
    }

    /**
     * Helper method for getting a reference to the CUTS/Node_Daemon
     * 
     * @param[in]       ipaddr        Ip-address of the node daemon.
     * @param[in]       port          Port number of the node daemon.
     * @return          Reference to the node daemon.
     */
    private CUTS.Node_Daemon get_daemon(String addr)
    {
      CUTS.Node_Daemon daemon = null;
      String url = String.Format("corbaloc:iiop:{0}/CUTS/NodeDaemon",
                                 addr);

      try
      {
        daemon =
          (CUTS.Node_Daemon)RemotingServices.Connect(typeof(CUTS.Node_Daemon),
                                                     url);
      }
      catch (omg.org.CORBA.COMM_FAILURE)
      {
        this.error_message(
          String.Format("there is no daemon active on {0}",
                        addr));
      }
      catch (omg.org.CORBA.TRANSIENT)
      {
        this.error_message(
          String.Format("failed to communicate with daemon on {0}",
                        addr));
      }
      catch (Exception ex)
      {
        this.error_message(ex.Message);
      }

      return daemon;
    }

    /**
     * Event handler for click the \a Spawn button.
     */
    protected void spawn_onclick(Object sender, EventArgs e)
    {
      // We have to manually validate the controls until I figure out
      // how to get the page to validate the controls. The problem is
      // the multiple controls have the same ID and "ValidationGroup"
      // apparently doesn't work in my case, e.g., controls w/ the
      // same ID in different groups.
      short port;

      try
      {
        port = short.Parse(this.start_port_.Text);
      }
      catch (Exception)
      {
        this.start_port_.Focus();
        this.message_.ForeColor = Color.Red;
        this.message_.Text = "invalid port number";
        return;
      }

      int count;

      try
      {
        count = int.Parse(this.count_.Text);
      }
      catch (Exception)
      {
        this.count_.Focus();
        this.message_.ForeColor = Color.Red;
        this.message_.Text = "invalid count";
        return;
      }

      // Create the <Spawn_Detail> for the request.
      CUTS.Spawn_Detail detail = new CUTS.Spawn_Detail();
      detail.@params = this.args_.Text;
      detail.bindings = new CUTS.Node_Binding[count];

      for (int i = 0; i < count; i++)
      {
        detail.bindings[i].port = port ++;
        detail.bindings[i].localhost = this.localhost_.Checked;
      }

      try
      {
        // Get the <daemon> and spawn the nodes. Then we need to 
        // update the view with the new nodes.
        CUTS.Node_Daemon daemon = this.get_daemon();

        count = daemon.spawn(detail);
        this.message(String.Format("Successfully spawned {0} node manager(s)",
                                   count));

        // Update the details for this node.
        this.show_details_i(daemon);
      }
      catch (omg.org.CORBA.COMM_FAILURE)
      {
        this.error_message (String.Format("there is no daemon running on {0}",
                                          this.daemon_.Value));
      }
      catch (Exception ex)
      {
        this.error_message(ex.Message);
      }
    }

    /**
     * Event handler for clicking the \a select_all link.
     * 
     * @param[in]       sender        Sender of the event.
     * @param[in]       e             Arguments for the event.
     */
    protected void select_all(Object sender, CommandEventArgs e)
    {
      foreach (ListItem item in this.local_.Items)
        item.Selected = true;
      foreach (ListItem item in this.nonlocal_.Items)
        item.Selected = true;
    }

    /**
     * Event handler for clicking the \a unselect_all link.
     * 
     * @param[in]       sender        Sender of the event.
     * @param[in]       e             Arguments for the event.
     */
    protected void unselect_all(Object sender, CommandEventArgs e)
    {
      foreach (ListItem item in this.local_.Items)
        item.Selected = false;
      foreach (ListItem item in this.nonlocal_.Items)
        item.Selected = false;
    }

    /**
     * Helper method for displaying an error message in the
     * correct format.
     * 
     * @param[in]     message       Error message.
     */
    private void error_message(string message)
    {
      this.message_.Text = message;
      this.message_.ForeColor = Color.Red;
    }

    /**
     * Helper method for displaying a regular message in the
     * correct format.
     */
    private void message(string message)
    {
      this.message_.Text = message;
      this.message_.ForeColor = Color.Black;
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
    ///    Required method for Designer support - do not modify
    ///    the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.Load += new System.EventHandler(this.Page_Load);
    }
    #endregion
  }
}
