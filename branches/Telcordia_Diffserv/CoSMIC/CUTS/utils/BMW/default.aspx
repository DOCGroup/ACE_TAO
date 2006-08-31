<%@ Page language="c#" codefile="default.aspx.cs" inherits="CUTS.Test" 
         masterpagefile="~/BMW.master" %>

<asp:content 
  runat="server" id="content_"
  contentplaceholderid="MainContent">
<asp:datagrid 
  id="tests_" runat="server" autogeneratecolumns="false" 
  datakeyfield="test_number" 
  borderstyle="solid" cellpadding="2" cellspacing="0"
  allowpaging="true" pagesize="25" width="100%" 
  showheader="true" showfooter="false" 
  enableviewstate="true"
  onpageindexchanged="handle_onpageindexchanged"
  onitemcreated="handle_onitemcreated">

  <headerstyle cssclass="header" />
  <footerstyle cssclass="footer" />
  <alternatingitemstyle cssclass="alternate-row" />
  <pagerstyle mode="NumericPages" />
  <itemstyle horizontalalign="center" />

  <columns>
    <asp:boundcolumn datafield="test_number" headertext="Test #"
      itemstyle-horizontalalign="left" itemstyle-font-bold="true" />
    <asp:boundcolumn datafield="test_name" headertext="Deployment" />
    <asp:boundcolumn datafield="start_time" headertext="Start Time" />
    <asp:boundcolumn datafield="stop_time" headertext="Stop Time" />

    <asp:templatecolumn headertext="Status">
      <itemtemplate>
        <asp:image 
          runat="server"
          imageurl='<%# "images/" + DataBinder.Eval (Container.DataItem, "status") + ".gif" %>'
          alternatetext='<%# DataBinder.Eval (Container.DataItem, "status") %>' />
      </itemtemplate>
    </asp:templatecolumn>

    <asp:hyperlinkcolumn 
      headertext="Results"
      datanavigateurlfield="test_number"
      datanavigateurlformatstring="Execution_Times.aspx?test={0}"
      text='<img src="images/table.gif" border="0" />' />

    <asp:templatecolumn>
      <headertemplate>
        <asp:checkbox runat="server" id="action_" 
                      oncheckedchanged="handle_toggle_action" 
                      causesvalidation="false"
                      autopostback="true" />
      </headertemplate>
      <itemtemplate>
        <asp:checkbox runat="server" id="action_" />
      </itemtemplate>
    </asp:templatecolumn>
  </columns>
</asp:datagrid>

<p>
  <!-- controls at the bottom of page used for batch processing -->
  <asp:linkbutton runat="server" id="delete_all_"
                  onclick="handle_delete_all"
                  text="Delete Selected"
                  causesvalidation="false" />
</p>
                
<asp:label runat="server" id="message_" />
</asp:content>
