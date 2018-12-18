/*
 *  template for creating tests with multi-threaded apps on nodes,
 *  which are connected through a P2P network
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

// via this defined name u can control logging of ns3-script
NS_LOG_COMPONENT_DEFINE ("NETWORKING");

int
main (int argc, char *argv[])
{

  std::cout << "--------------> running simulation:" << std::endl;

  // parse comannd line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  // set semulation time type to nano-seconds (default-value)
  Time::SetResolution (Time::NS);

  // set log level for echo server and client
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  // define 2 network nodes 
  NodeContainer nodes;
  nodes.Create (2);

  // configure device and channel for a P2P connection 
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  // finish creating P2P with creating and attaching 2 devices to a P2P channel 
  // device settings should be configured via PointToPointHelper Class
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);


  // install internet protocol stack on all defined nodes
  InternetStackHelper stack;
  stack.Install (nodes);

  // set IP base address ==> first addr : 10.1.1.1 and second : 10.1.1.2
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");


  // assign defined addresses to given devices
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // set port number for the echo server
  UdpEchoServerHelper echoServer (9);

  // create echo server application and attach it to the given node
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));

  // schedule events for server
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // set IP and port for echo client
  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);


  // client sends only 1 packet  
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  // interval between sending packets is 1 nanosec
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  // create echo client application and attach it to the given node
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));

  // schedule events for client
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));


  // AsciiTraceHelper ascii;
  // pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("first.tr"));

  // activate tracing of client and server. Tracing will be written into corresponding PCAP files
  pointToPoint.EnablePcapAll("networking");

  // run simulation on the scheduled events
  Simulator::Stop(Seconds(11.0));
  Simulator::Run ();
  Simulator::Destroy ();
  
  std::cout << "--------------> finishing simulation:" << std::endl;
  return 0;
}
