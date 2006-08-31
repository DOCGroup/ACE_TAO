<%@ Page language="c#" 
         codefile="Critical_Path.aspx.cs" 
         autoeventwireup="true" 
         inherits="CUTS.Critical_Path"
         masterpagefile="~/BMW.master" %>

<asp:content runat="server" 
             id="main_content_"
             contentplaceholderid="MainContent">
             
<h2>Critical Paths / Paths of Interest</h2>

<table width="100%" border="0">
<tr valign="top">
  <!-- table form for creating critical paths -->
  <td style="width:45%">
  <table>
  <tr>
    <td class="title">Path Name:</td>
    <td>
      <asp:textbox runat="server" id="path_name_" />
      <asp:requiredfieldvalidator id="path_name_validator_" runat="server"
                                  controltovalidate="path_name_"
                                  errormessage="path name is required"
                                  forecolor="red"
                                  validationgroup="createpath" />
    </td>
  </tr>  
  <tr>
    <td class="title">Deadline (ms):</td>
    <td>
      <asp:textbox runat="server" id="deadline_" />
      <asp:requiredfieldvalidator id="deadline_validator_"
                                  runat="server"
                                  controltovalidate="deadline_"
                                  errormessage="deadline is required"
                                  forecolor="red"
                                  validationgroup="createpath"
                                  display="dynamic" />
      <asp:rangevalidator id="deadline_value_validator_"
                          runat="server"
                          minimumvalue="1"
                          maximumvalue="4294967296"
                          controltovalidate="deadline_"
                          validationgroup="createpath"
                          errormessage="invalid deadline"
                          display="dynamic" />
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <asp:button runat="server" 
                  text="Create" 
                  id="create_path_" 
                  onclick="button_create_path"
                  validationgroup="createpath" />
      </td>
  </tr>    
  </table>
  </td>
  
  <!-- portion of page for displaying current paths -->  
  <td>
    <asp:datagrid runat="server" id="critical_paths_"
                  borderstyle="solid" cellpadding="4" cellspacing="0"
                  allowpaging="true" pagesize="10" width="100%"
                  autogeneratecolumns="false"
                  showheader="true" showfooter="false">

      <headerstyle cssclass="header" />
      <footerstyle cssclass="footer" />
      <alternatingitemstyle cssclass="alternate-row" />
      <pagerstyle mode="NumericPages" />

      <columns>
        <asp:boundcolumn datafield="path_name" 
                         headertext="Current Paths" 
                         itemstyle-horizontalalign="Left" />
                         
        <asp:boundcolumn datafield="deadline" 
                         headertext="Deadline (ms)" 
                         itemstyle-horizontalalign="Center"
                         itemstyle-width="100" />
                         
        <asp:hyperlinkcolumn datanavigateurlfield="path_id" 
                             datanavigateurlformatstring="Critical_Path.aspx?p={0}"
                             text='<img src="images/edit.gif" border="0" alt="edit path elements" />'
                             itemstyle-horizontalalign="center" />
      </columns>
    </asp:datagrid>
    <div>&nbsp;</div>
    <asp:button runat="server" id="save_paths_" 
                text="Save" 
                onclick="save_paths_onclick"
                causesvalidation="false" />
  </td>
</tr>
</table>
<asp:label runat="server" id="message_" />

<asp:panel runat="server" id="element_panel_">
  <hr />
  
  <h2>Critical Path Elements</h2>
  <asp:datagrid id="path_elements_" runat="server" 
                width="100%" pagesize="15" allowpaging="true"
                autogeneratecolumns="false" showfooter="false"
                borderstyle="solid" cellpadding="4" cellspacing="0"                
                onitemcreated="handle_item_created_path_grid">
                
    <headerstyle cssclass="header" />
    <footerstyle cssclass="footer" />
    <alternatingitemstyle cssclass="alternate-row" />
    <pagerstyle mode="NumericPages" />

    <columns>
      <asp:boundcolumn datafield="path_order" 
                       headertext="Index" />

      <asp:boundcolumn datafield="component_name" 
                       headertext="Instance Name" />

      <asp:boundcolumn datafield="src" 
                       headertext="Source Port" />

      <asp:boundcolumn datafield="dst" 
                       headertext="Destination Port" />
    </columns>
  </asp:datagrid>
  
  <div>&nbsp;</div>
  
  <div>
    <span class="title">Next Index:</span>
    <asp:textbox id="order_" runat="server" width="75" maxlength="11"></asp:textbox>
    <span class="title">Instance:</span>
    <asp:dropdownlist id="instance_" runat="server" datatextfield="component_name" datavaluefield="component_id"></asp:dropdownlist>
  </div>

  <div>
    <span class="title">Source Port: </span>
    <asp:textbox id="src_" runat="server"></asp:textbox>
    <span class="title">Destination Port: </span>
    <asp:textbox id="dst_" runat="server"></asp:textbox>
  </div>

  <div>
    <asp:button id="Button1" onclick="insert_path_element" 
                runat="server" text="Insert" />
    <asp:button id="Button2" onclick="commit_path_element" 
                runat="server" text="Save" />
  </div>
</asp:panel>
</asp:content>
