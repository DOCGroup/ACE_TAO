<!-- -*- ASPX -*- -->

<!-----------------------------------------------------------------------------
  -- @file      Node_Details.ascx
  --
  -- $Id$
  --
  -- @author    James H. Hill
  -----------------------------------------------------------------------------
  -->
  
<%@ Control language="C#" codefile="Node_Details.ascx.cs" 
                          inherits="CUTS.Node_Details_Control" %>

<table width="100%">
<tr>
  <td colspan="2">
    <div>Spawn node manager(s):</div>
    <table>
    <tr>
      <td>Port:</td>
      <td><asp:textbox runat="server" id="start_port_"  maxlength="5" /></td>
    </tr>
    <tr>
      <td>Count:</td>
      <td>
        <asp:textbox runat="server" 
                     id="count_" 
                     maxlength="2" 
                     columns="4" />
                     
        <asp:checkbox runat="server"
                      id="localhost_"
                      text="Localhost?" />
      </td>
    </tr> 
    <tr>
      <td>Arguments:</td>
      <td><asp:textbox runat="server"
                       id="args_"
                       columns="50" /></td>
    </tr>
    <tr>
      <td><asp:button id="spawn_" 
                      runat="server" 
                      text="Spawn"
                      onclick="spawn_onclick" /></td>
      <td><asp:label runat="server" id="message_" forecolor="red" /></td>
    </tr>                      
    </table>
  </td>
</tr>

<tr><td colspan="2"><hr /></td></tr>

<tr valign="top">
  <td>
    <asp:hiddenfield id="daemon_" runat="server" />
    
    Non-local manager port(s):<br />
    <asp:checkboxlist runat="server" 
                      id="nonlocal_"
                      repeatdirection="horizontal" />
      
    Local manager port(s):<br />
    <asp:checkboxlist runat="server" 
                      id="local_"
                      repeatdirection="horizontal" />
      
    <p>
       <asp:linkbutton runat="server"
                       id="select_all_"
                       oncommand="select_all"
                       forecolor="blue"
                       font-underline="true"
                       causesvalidation="false"
                       text="Select All" />
                       
       <asp:linkbutton runat="server"
                       id="unselect_all_"
                       oncommand="unselect_all"
                       forecolor="blue"
                       font-underline="true"
                       causesvalidation="false"
                       text="Unselect All" />

       <asp:linkbutton runat="server" 
                       id="kill_"
                       oncommand="kill_nodes"
                       forecolor="blue"
                       font-underline="true"
                       text="Kill Selected"
                       causesvalidation="false" /> 
    </p>
  </td>
</tr>
</table>
