<%@ Control language="C#" codefile="Sender_Metrics.ascx.cs" inherits="CUTS.Sender_Metrics" %>

<br style="clear:both" />
<div><b>Sender:</b> <%= this.instance_name_ %></div>
<asp:datagrid runat="server" id="metrics_" autogeneratecolumns="false"
  width="100%" borderstyle="none" borderwidth="1"
  onpageindexchanged="handle_page_index_changed">

  <headerstyle cssclass="header" />
  <footerstyle cssclass="footer" />
  <alternatingitemstyle cssclass="alternate-row" />
  <pagerstyle mode="NumericPages" />
  <itemstyle horizontalalign="center" />

  <columns>
    <asp:boundcolumn datafield="src" headertext="In Port" />
    <asp:boundcolumn datafield="metric_type" headertext="Type" />
    <asp:boundcolumn datafield="dst" headertext="Out Port" />
    <asp:boundcolumn datafield="metric_count" headertext="Count"
      itemstyle-width="50"/>
    <asp:templatecolumn headertext="Timeline"
      itemstyle-backcolor="darkgray" itemstyle-width="70">
      <itemtemplate>
        <asp:hyperlink runat="server" imageurl="images/graph.gif"
          navigateurl='
          <%#
            "Timeline.aspx?t=" + this.test_ +
            "&c=" + this.owner_ +
            "&s=" + this.instance_id_ +
            "&m=" + DataBinder.Eval (Container.DataItem, "metric_type") +
            "&src=" + DataBinder.Eval (Container.DataItem, "src") +
            "&dst=" + DataBinder.Eval (Container.DataItem, "dst")
          %>' />
      </itemtemplate>
    </asp:templatecolumn>

    <asp:boundcolumn datafield="best_time"
      headertext="Best (ms)" itemstyle-width="75" />
    <asp:boundcolumn datafield="average_time"
      headertext="Average (ms)" itemstyle-width="95" />
    <asp:boundcolumn datafield="worse_time"
      headertext="Worse (ms)" itemstyle-width="95" />
  </columns>
</asp:datagrid>
