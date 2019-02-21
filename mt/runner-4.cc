/*
 * runner for testing self-implemented client-server
 *
 */

#include "ns3/grail-module.h"
#include "ns3/traffic-control-module.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-star.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/olsr-module.h"
#include "ns3/error-model.h"

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include "ns3/int64x64-128.h"

#include <iostream>
#include <functional>

using namespace ns3;



int main (int argc, char *argv[]){

    std::string server = "/home/qp/Projects/ns-3.29/src/grail/mt/client-server/new-server.o";
    std::string client = "/home/qp/Projects/ns-3.29/src/grail/mt/client-server/new-simple-client.o";


    LogComponentEnable("GrailApplication", LOG_LEVEL_ERROR);
    LogComponentEnable("GrailNetlink", LOG_LEVEL_ERROR);
    // LogComponentEnable("GrailApplication", LOG_LEVEL_LOGIC);
    // LogComponentEnable("GrailNetlink", LOG_LEVEL_LOGIC);

    uint32_t rngRun = 0;
    bool pcap = false;
    bool enablePreloading = true;
    DataRate rate = DataRate("1Mbps");
      
    CommandLine cmd;
    cmd.AddValue("rngRun", "run-# of the PRNG", rngRun);
    cmd.AddValue("rate", "bitrate of link", rate);
    cmd.AddValue("pcap", "enable pcap", pcap);
    cmd.AddValue("enablePreloading", "enable LD-preloading helper technique", enablePreloading);
    cmd.Parse (argc, argv);
      
    RngSeedManager::SetRun(rngRun);

    Time serverStartTime = Seconds(5.0);
    Time clientStartTime = Seconds(5.5);
      
    NodeContainer nodes;
    nodes.Create(3 /* num nodes */);

    Ptr<RateErrorModel> rem = CreateObject<RateErrorModel> ();
    Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
    rem->SetRandomVariable (uv);
    rem->SetRate (1.0/rate.GetBitRate());
      
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", DataRateValue(rate));
    pointToPoint.SetDeviceAttribute ("ReceiveErrorModel", PointerValue(rem));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));
    NetDeviceContainer devices0 = pointToPoint.Install(nodes.Get(0), nodes.Get(1));
    NetDeviceContainer devices1 = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

    InternetStackHelper stack;
    stack.Install (nodes);
    Ipv4AddressHelper address;
    address.SetBase ("10.0.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces0 = address.Assign(devices0);
    address.NewNetwork();
    Ipv4InterfaceContainer interfaces1 = address.Assign(devices1);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    ApplicationContainer serverApps, clientApps; // only used for baseline
    { // server
        Ptr<GrailApplication> app = CreateObject<GrailApplication>();
        app->Setup({server});
        app->SetAttribute("PrintStdout", BooleanValue(true));
        app->SetAttribute("SyscallProcessingTime", TimeValue(NanoSeconds(100)));
        app->SetAttribute("EnablePreloading", BooleanValue(enablePreloading));
        app->SetAttribute("MayQuit", BooleanValue(false));
        app->SetStartTime( serverStartTime );
        nodes.Get (0)->AddApplication(app);
    }

    { // client
        Ptr<GrailApplication> app = CreateObject<GrailApplication>();
        app->Setup({client});
        app->SetAttribute("PrintStdout", BooleanValue(true));
        app->SetAttribute("SyscallProcessingTime", TimeValue(NanoSeconds(100)));
        app->SetAttribute("EnablePreloading", BooleanValue(enablePreloading));
        app->SetAttribute("MayQuit", BooleanValue(true));
        app->SetStartTime( clientStartTime );
        nodes.Get (1)->AddApplication(app);
    }

    if(pcap) {
        pointToPoint.EnablePcapAll ("grail-iperf-example");
    }

    // Packet::EnableChecking();
    // Packet::EnablePrinting();

    
    // activate tracing of client and server. Tracing will be written into corresponding PCAP files
    pointToPoint.EnablePcapAll("Runner");

    Simulator::Stop( Seconds (100) );
    Simulator::Run ();

    Simulator::Destroy ();

    return 0;
}
