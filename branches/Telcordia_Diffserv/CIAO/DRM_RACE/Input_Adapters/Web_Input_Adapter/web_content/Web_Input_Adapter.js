// -*- C++ -*-

function submitPlan (submitURL, staticOrDynamic)
{
  var elements = document.deploymentPlanForm.elements;
  for (var i = 0; i < elements.length; ++i)
  {
    if (elements[i].name == "dynamic_allocation")
    {
      if (staticOrDynamic == "dynamic")
        elements[i].value = "true";
      else
        elements[i].value = "false";
    }
  }

  document.deploymentPlanForm.action = "/web_input_adapter.cgi/" + submitURL;
  document.deploymentPlanForm.submit ();
}

function deployPlan (staticOrDynamic, planName)
{
  var elements = document.deploymentPlanForm.elements;
  for (var i = 0; i < elements.length; ++i)
  {
    if (elements[i].name == "dynamic_allocation")
    {
      if (staticOrDynamic == "dynamic")
        elements[i].value = "true";
      else
        elements[i].value = "false";
    }

    if (elements[i].name == "deploy_deployment_plan")
    {
      elements[i].value = planName;
    }

    if (elements[i].name == "teardown_deployment_plan")
    {
      elements[i].value = "?";
    }
  }

  document.deploymentPlanForm.action = "/web_input_adapter.cgi/deployable_cdp_list";
  document.deploymentPlanForm.submit ();
}

function tearDownPlan (planName)
{
  var elements = document.deploymentPlanForm.elements;
  for (var i = 0; i < elements.length; ++i)
  {
    if (elements[i].name == "deploy_deployment_plan")
    {
      elements[i].value = "?";
    }

    if (elements[i].name == "teardown_deployment_plan")
    {
      elements[i].value = planName;
    }
  }

  document.deploymentPlanForm.action = "/web_input_adapter.cgi/deployable_cdp_list";
  document.deploymentPlanForm.submit ();
}
