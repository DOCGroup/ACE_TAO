<%@ Control language="C#" 
            codefile="Component_Metrics.ascx.cs" 
            inherits="CUTS.Component_Metrics"
            autoeventwireup="true" %>

<asp:table runat="server" cellpadding="0" cellspacing="0" 
           borderstyle="Double" borderwidth="4"
           width="100%" bordercolor="black" id="table_">
  <asp:tablerow runat="server" horizontalalign="Left" verticalalign="Middle">
    <asp:tableheadercell runat="server">
      <%= this.instance_name_.ToString () %> Execution Times
    </asp:tableheadercell>
  </asp:tablerow>
  <asp:tablerow runat="server">
    <asp:tablecell runat="server" id="sender_metrics_" />
  </asp:tablerow>
</asp:table>
<br style="clear:both" />
