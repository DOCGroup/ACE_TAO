<?xml version="1.0" encoding="utf-8" ?>
<SA-POP:taskMap
  xmlns:SA-POP="http://www.vanderbilt.edu/SA-POP"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.vanderbilt.edu/SA-POP Task_Map.xsd"
  xmlns="http://www.vanderbilt.edu/SA-POP">



  <!-- Available task implementations (component specific types). -->

  <taskImpl>
    <implID>IDL:SPACE/Gizmo1_Low:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Gizmo1_Medium:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Gizmo2_Medium:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Gizmo1_High:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Gizmo2_High:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Filter_Component:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Compression_Component:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Comm_Component:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 
  <taskImpl>
    <implID>IDL:SPACE/Analysis_Component:1.0</implID>
    <param>
      <paramID></paramID>
      <kind></kind>
      <value></value>
    </param>
  </taskImpl> 




  <!-- System resources. -->

  <resource>
    <resourceID>Memory</resourceID>
    <kind>DISCRETE</kind>
    <capacity>512</capacity>
  </resource>
  <resource>
    <resourceID>CPU</resourceID>
    <kind>DISCRETE</kind>
    <capacity />
  </resource>




  <!-- Task to task implementation mapping. -->

  <!-- Sensor tasks. -->
  <taskToImpl>
    <taskID>2101</taskID>
    <implID>IDL:SPACE/Gizmo1_Low:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2201</taskID>
    <implID>IDL:SPACE/Gizmo1_Medium:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2202</taskID>
    <implID>IDL:SPACE/Gizmo2_Medium:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2301</taskID>
    <implID>IDL:SPACE/Gizmo1_High:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2302</taskID>
    <implID>IDL:SPACE/Gizmo2_High:1.0</implID>
    <duration />
  </taskToImpl>

  <!-- Filter tasks. -->
  <taskToImpl>
    <taskID>2111</taskID>
    <implID>IDL:SPACE/Filter_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2211</taskID>
    <implID>IDL:SPACE/Filter_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2311</taskID>
    <implID>IDL:SPACE/Filter_Component:1.0</implID>
    <duration />
  </taskToImpl>

  <!-- Compression tasks. -->
  <taskToImpl>
    <taskID>2121</taskID>
    <implID>IDL:SPACE/Compression_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2221</taskID>
    <implID>IDL:SPACE/Compression_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2321</taskID>
    <implID>IDL:SPACE/Compression_Component:1.0</implID>
    <duration />
  </taskToImpl>

  <!-- Communication tasks. -->
  <taskToImpl>
    <taskID>2131</taskID>
    <implID>IDL:SPACE/Comm_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2231</taskID>
    <implID>IDL:SPACE/Comm_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2331</taskID>
    <implID>IDL:SPACE/Comm_Component:1.0</implID>
    <duration />
  </taskToImpl>

  <!-- Analysis tasks. -->
  <taskToImpl>
    <taskID>2141</taskID>
    <implID>IDL:SPACE/Analysis_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2241</taskID>
    <implID>IDL:SPACE/Analysis_Component:1.0</implID>
    <duration />
  </taskToImpl>
  <taskToImpl>
    <taskID>2341</taskID>
    <implID>IDL:SPACE/Analysis_Component:1.0</implID>
    <duration />
  </taskToImpl>




  <!-- Task implementation to resource consumption mapping. -->

  <implToResource>
    <implID>IDL:SPACE/Gizmo1_Low:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Gizmo1_Medium:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Gizmo2_Medium:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Gizmo1_High:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Gizmo2_High:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Filter_Component:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Compression_Component:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Comm_Component:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 
  <implToResource>
    <implID>IDL:SPACE/Analysis_Component:1.0</implID>
    <resourceID>CPU</resourceID>
    <utilization>5</utilization>
  </implToResource> 

</SA-POP:taskMap>