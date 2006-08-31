using System;
using System.Threading;

namespace CUTS
{
    class BDCClient
    {
        static CUTSPath_Element[] createPathId(String pathId)
        {
            char[] pathSeparator = { ';' };
            char[] pathElementSeparator = { ',' };
            string[] pathElements = pathId.Split(pathSeparator);
            //foreach (string nodes in pathElements)
            //{
            //    Console.WriteLine(nodes);
            //}
            CUTSPath_Element[] pathSeq = new CUTSPath_Element[pathElements.Length];
            int counter = 0;
            foreach (string path in pathElements)
            {
                string[] components = path.Split(pathElementSeparator);
                //foreach (string component in components)
                //{
                //    Console.WriteLine(component);
                //}
                CUTSPath_Element pathElement = new CUTSPath_Element();
                pathElement.node = components[0];
                pathElement.src = components[1];
                pathElement.dst = components[2];
                pathSeq[counter++] = pathElement;
            }
            return pathSeq;
        }

        static void Main()
        {
            
            string pathId = "SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerOne,CommandEvent,CommandEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.SensorMain,CommandEvent,TrackEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerOne,TrackEvent,SituationEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerTwo,SituationEvent,AssessmentEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.ConfigOp,AssessmentEvent,CommandEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.EffectorMain,CommandEvent,CommandEvent";
            //Console.WriteLine("pathId = {0}", pathId);
            CUTSPath_Element[] pathSeq = BDCClient.createPathId(pathId);
            while (true)
            {
                try
                {
                    CUTSBenchmark_Data_Collector_SE_online_measurements
                            bdc_measurements = new CUTSBenchmark_Data_Collector_SE_online_measurements();
                    int cutsId = bdc_measurements.bind_to_path(pathSeq, 3);
                    CUTSBenchmark_Data_Collector_SE_controls
                        bdc_controller = new CUTSBenchmark_Data_Collector_SE_controls();
                    while (true)
                    {
                        // bdc_controller.collect_performance_data();
                        try
                        {
                            CUTSEvent_Time_Info eventInfo;
                            CUTSTime_Info time;
                            eventInfo = bdc_measurements.execution_time(cutsId);
                            time = eventInfo.time;
                            Console.Error.WriteLine("ID {0}: Execution time (min/avg/max): ({1} ms/{2} ms/{3} ms)\t No. of Events sampled: ({4})",
                                cutsId,
                                time.min,
                                time.total,
                                time.max,
                                eventInfo.max_events);
                            Thread.Sleep(5000);
                        }
                        catch (System.Net.WebException)
                        {
                            CUTSTime_Info time = new CUTSTime_Info();
                            CUTSEvent_Time_Info eventInfo = new CUTSEvent_Time_Info();
                            Console.Error.WriteLine("ID {0}: Execution time (min/avg/max): ({1} ms/{2} ms/{3} ms)\t No. of Events sampled: ({4})",
                                cutsId,
                                time.min,
                                time.total,
                                time.max,
                                eventInfo.max_events);
                        }
                    }
                }
                catch (System.Web.Services.Protocols.SoapException ex)
                {
                    Console.WriteLine(ex.Message);
                }
                catch (System.Net.WebException)
                {
                    CUTSTime_Info time = new CUTSTime_Info();
                    CUTSEvent_Time_Info eventInfo = new CUTSEvent_Time_Info();
                    Console.Error.WriteLine("ID {0}: Execution time (min/avg/max): ({1} ms/{2} ms/{3} ms)\t No. of Events sampled: ({4})",
                                0,
                                time.min,
                                time.total,
                                time.max,
                                eventInfo.max_events);
                }
            }
        }
    }
}
