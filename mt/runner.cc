/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/*
NS-3 script to test Grail for working with a simple multithreaded application

Configuration:
    - 2 nodes with a PPP connection
    - on one node is running a multithreaded application
    - multithreaded application has a single purpose: printing some data to STDOUT (e.g. thread id)

    - it's possible to define application to run in the script via a cmd argument, see --PrintHelp
    - default value is a test application from the scratch dir
*/

#include "ns3/grail.h"
#include "ns3/traffic-control-module.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
const unsigned short int SECOND_NODE {1};
const std::string CMD_ARG_APP_HELP_STD {"binary application to run on the node " + std::to_string(SECOND_NODE) };

NS_LOG_COMPONENT_DEFINE ("Runner");


int main (int argc, char *argv[])
{

  std::string appToRun;

  CommandLine cmd;
  cmd.AddValue("app", CMD_ARG_APP_HELP_STD, appToRun);
  cmd.Parse (argc, argv);
  
  if (appToRun.empty()){
    std::cout << "Error: didn't get a binary to run in the runner." << std::endl;
    std::cout << "Usage : runner --app=<path to binary executable>" << std::endl;
    std::cout << "Example : ./waf --run \"runner --app=src/grail/mt/test-3\"" << std::endl;

    return EXIT_FAILURE;
  }

  Time::SetResolution (Time::NS);

  LogComponentEnable("GrailApplication", LOG_LEVEL_ERROR);
  LogComponentEnable("GrailNetlink", LOG_LEVEL_ERROR);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  Time clientStartTime = Seconds(2.0);

  // define and place application on the node 1
  Ptr<GrailApplication> app = CreateObject<GrailApplication>();
  app->Setup({appToRun});
  app->SetAttribute("PrintStdout", BooleanValue(true));
  app->SetAttribute("SyscallProcessingTime", TimeValue(NanoSeconds(100)));
  app->SetAttribute("MayQuit", BooleanValue(true));
  app->SetStartTime( clientStartTime );
  nodes.Get (SECOND_NODE)->AddApplication(app);

  Simulator::Run ();
  Simulator::Destroy ();

  return EXIT_SUCCESS;
}
