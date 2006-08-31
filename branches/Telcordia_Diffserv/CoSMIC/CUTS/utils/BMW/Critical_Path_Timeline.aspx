<%@ Register tagprefix="web" namespace="WebChart" assembly="WebChart" %>
<%@ Page language="c#" codefile="Critical_Path_Timeline.aspx.cs" 
         inherits="CUTS.Critical_Path_Timeline" 
         masterpagefile="~/BMW.master" %>

<asp:content runat="server"
             id="main_content_"
             contentplaceholderid="MainContent">

<web:chartcontrol id="timeline_" runat="server" chartpadding="30" borderstyle="Ridge"
  height="400px" width="800px" ycustomend="0" yvaluesinterval="0" ycustomstart="0" bordercolor="Silver"
  toppadding="60" showtitlesonbackground="false" haschartlegend="true" gridlines="horizontal" showxvalues="false">
  <xtitle stringformat="Center,Far,Character,LineLimit" font="Tahoma, 8pt, style=Bold" text="Collection Timeline"></xtitle>
  <yaxisfont stringformat="Far,Near,Character,LineLimit"></yaxisfont>
  <charttitle forecolor="black" stringformat="Center,Near,Character,LineLimit" font="Tahoma, 10pt, style=Bold"></charttitle>
  <xaxisfont stringformat="Center,Near,Character,LineLimit" font="Microsoft Sans Serif, 8pt"></xaxisfont>
  <legend font="Microsoft Sans Serif, 8pt">
  </legend>
  <background type="Solid" angle="90" endpoint="100, 400" color="white" hatchstyle="Horizontal"></background>
  <ytitle stringformat="Center,Near,Character,DirectionVertical" text="Execution Time (ms)"
    font="Tahoma, 8pt, style=Bold"></ytitle>
  <border dashstyle="dashdot" color="black"></border>
  <plotbackground type="hatch" hatchstyle="dottedgrid" forecolor="lightgray"></plotbackground>
</web:chartcontrol>

<br style="clear:both" />

<asp:label id="exception_label_" forecolor="red" runat="server" />
</asp:content>
