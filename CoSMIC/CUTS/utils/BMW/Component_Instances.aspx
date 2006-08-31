<%@ Page language="c#" codefile="Component_Instances.aspx.cs" 
         autoeventwireup="true" inherits="CUTS.Component_Instances" 
         masterpagefile="~/BMW.master" %>

<asp:content runat="server" 
             id="main_content" 
             contentplaceholderid="MainContent">
             
<h2>Registered Component Types &amp; Instances</h2>
<asp:label id="message_" 
           runat="server"
           forecolor="red" />
             
<asp:datagrid runat="server" id="component_instances_" 
              autogeneratecolumns="false" borderstyle="solid"
              cellpadding="4" cellspacing="0" allowpaging="true" 
              pagesize="25" width="100%" showheader="true"
              onitemcreated="handle_onitemcreated"
              onpageindexchanged="handle_onpageindexchanged" >
              
  <headerstyle cssclass="header" />
  <footerstyle cssclass="footer"/>
  <alternatingitemstyle cssclass="alternate-row" />
  <pagerstyle mode="NumericPages" />

  <columns>
    <asp:boundcolumn datafield="component_id" 
                     headertext="ID" 
                     itemstyle-horizontalalign="center" 
                     itemstyle-width="50" />
                     
    <asp:boundcolumn datafield="component_name" 
                     headertext="Component Instance Name" />
  </columns>
</asp:datagrid>
</asp:content>
