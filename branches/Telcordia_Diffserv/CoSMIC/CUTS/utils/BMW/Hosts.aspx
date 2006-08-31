<%@ Page language="c#" codefile="Hosts.aspx.cs" autoeventwireup="true" 
         inherits="CUTS.Hosts" masterpagefile="~/BMW.master" %>
<%@ Reference control="Node_Details.ascx" %>

<asp:content runat="server"
             id="main_content_"
             contentplaceholderid="MainContent">
             
<!-- form for register host w/ the database -->         
<h2>Registration</h2>
<table>
<tr>
  <td><b>Hostname:</b></td>
  <td><asp:textbox runat="server" 
                   id="hostname_" 
                   width="200" 
                   validationgroup="registration" /></td>
                   
  <td><asp:requiredfieldvalidator runat="server" 
                                  id="hostname_validator_"
                                  controltovalidate="hostname_"
                                  validationgroup="registration"
                                  display="static"
                                  errormessage="hostname is required" /></td>
                                  
  <td rowspan="3" style="vertical-align:top">
   
  </td>
</tr>
<tr>
  <td style="text-align:right" colspan="2">
    <asp:button runat="server" 
                id="register_" 
                text="Register"   
                onclick="register_clicked"
                validationgroup="registration" /></td>
  <td>&nbsp;</td>
</tr>
</table>

<!-- label for displaying messages related to registration -->
<p><asp:label runat="server" id="register_message_" /></p>

<h2>Testing Environment</h2>

<!-- datagrid that displays a list of registered hosts -->
<asp:datagrid runat="server" id="hosts_" cellpadding="4" cellspacing="0"
              autogeneratecolumns="false" 
              width="100%" showheader="true"
              datakeyfield="hostid"
              oneditcommand="handle_oneditcommand"
              onupdatecommand="handle_onupdatecommand"
              oncancelcommand="handle_oncancelcommand"
              onitemcreated="handle_onitemcreated"
              onitemcommand="handle_onitemcommand">
              
  <headerstyle cssclass="header alternate-row" />
  <footerstyle cssclass="footer" />
  <pagerstyle mode="NumericPages" />
  <edititemstyle font-bold="true" />
  
  <columns>
    <asp:buttoncolumn 
      itemstyle-width="10" itemstyle-horizontalalign="center" 
      buttontype="LinkButton"
      text='<img src="images/plus.gif" alt="show" border="0" />'
      commandname="details" />
    
    <asp:boundcolumn datafield="hostname" 
                     headertext="Hostname"
                     readonly="true" />

    <asp:boundcolumn datafield="ipaddr" 
                     headertext="IP Address" 
                     itemstyle-horizontalalign="center" 
                     itemstyle-width="150"
                     readonly="true" />
    
    <asp:boundcolumn datafield="portnum"
                     headertext="Port #"
                     itemstyle-width="75"
                     itemstyle-horizontalalign="center" />

    <asp:editcommandcolumn 
      buttontype="LinkButton"
      edittext='<img src="images/edit.gif" alt="edit" border="0" />'
      updatetext='<img src="images/ok.gif" alt="save" border="0" />'
      canceltext='<img src="images/cancel.gif" alt="cancel" border="0" />'
      itemstyle-width="35"
      itemstyle-horizontalalign="center" />
  </columns>
</asp:datagrid>

<!-- 
  -- link buttons at the bottom of page for showing/collapsing all the
  -- node details
  -->
<p>
<asp:linkbutton id="show_all_"
                runat="server" 
                text="Show All" 
                causesvalidation="false"
                font-underline="true"
                commandname="show-all"
                forecolor="blue"
                oncommand="show_all" />

<asp:linkbutton runat="server" 
                id="collapse_all_"
                text="Collapse All" 
                causesvalidation="false"
                font-underline="true"
                commandname="collapse-all"
                forecolor="blue"
                oncommand="collapse_all" />
</p>

</asp:content>
