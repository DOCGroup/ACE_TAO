using System.Collections;
using System.Collections.Generic;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using Ch.Elca.Iiop;
using Ch.Elca.Iiop.Idl;
using Ch.Elca.Iiop.Services;
using System;
using System.IO;

public class ConsoleApp
{
	public static void Main(string[] args)
	{
		if(args.Length != 1)
		{
			Console.WriteLine("Usage: client.exe server.ior");
			return;
		}

		MappingConfiguration.Instance.UseBoxedInAny = false;

		IDictionary props = new Hashtable();
		IiopChannel channel = new IiopChannel(props);
		ChannelServices.RegisterChannel(channel, false);

		StreamReader sr = new StreamReader(args[0]);
		string ior = sr.ReadLine();

		Test test = (Test)RemotingServices.Connect(typeof(Test), ior);
		object[] p = new object[2];
		for(int i = 0; i < p.Length; ++i)
		{
			Container container;
			Inner inner;
            inner.value1 = 1 + i;
            inner.value2 = 2 + i;
            inner.value3 = 3 + i;
            inner.value4 = 4 + i;
            inner.value5 = 5 + i;
			container.contents = inner;
			p[i] = container;
		}

		try
		{
			p = test.RunTest(p);

			for(int i = 0; i < p.Length; ++i)
			{
				if(p[i] is Container)
				{
					Inner inner = (Inner)((Container)p[i]).contents;
					Console.WriteLine("[{0}]: {1} {2} {3} {4} {5}", i,
						inner.value1, inner.value2, inner.value3, inner.value4, inner.value5);
				}
			}
            test.shutdown ();
		}
		catch(Exception ex)
		{
			Console.WriteLine("Invoke failed: {0}", ex.ToString());
		}
	}
}
