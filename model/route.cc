/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "route.h"

#include <algorithm>
#include <string>

namespace ns3 {
  NS_OBJECT_ENSURE_REGISTERED (HgRoutingProtocol);

  TypeId 
  HgRoutingProtocol::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::HgRoutingProtocol")
      .SetParent<Ipv4RoutingProtocol> ()
      .AddConstructor<HgRoutingProtocol> ()
      ;
    return tid;
  }
}

void ns3::HgRoutingProtocol::DelRoute(Ipv4Address dst, uint8_t len, Ipv4Address gw)
{
  ns3::RtEntry entry(dst,len,gw);
  auto it = std::find(m_entries.begin(), m_entries.end(), entry);
  if(it != m_entries.end()) {
    m_entries.erase(it);
  }
}

void ns3::HgRoutingProtocol::AddRoute(Ipv4Address dst, uint8_t len, Ipv4Address gw)
{
  ns3::RtEntry entry(dst,len,gw);
  auto it = std::find(m_entries.begin(), m_entries.end(), entry);
  if(it != m_entries.end()) {
    m_entries.erase(it);
  }
  m_entries.push_back(entry);

  // fprintf(stderr, "routing table of %p:\n", this);
  // for( ns3::RtEntry rt : m_entries ) {
  //   std::stringstream dst_ss;
  //   std::stringstream gw_ss;
  //   rt.dst.Print(dst_ss);
  //   rt.gw.Print(gw_ss);
  //   fprintf(stderr, "ns3-rt: %s/%d via %s\n", dst_ss.str().c_str(), rt.len, gw_ss.str().c_str());
  // }
  // fprintf(stderr, "------------- < end table\n");
}

ns3::Ptr<ns3::Ipv4Route> ns3::HgRoutingProtocol::RouteOutput (ns3::Ptr<ns3::Packet> p,
                                                    const ns3::Ipv4Header &header,
                                                    ns3::Ptr<ns3::NetDevice> oif,
                                                    ns3::Socket::SocketErrno &sockerr)
{
  Ipv4Address dst = header.GetDestination ();
  // Ipv4Address origin = header.GetSource ();
  // Forwarding
  ns3::Ptr<ns3::Ipv4Route> route;

  // {
  //   std::stringstream ss_origin, ss_dst;
  //   origin.Print(ss_origin);
  //   dst.Print(ss_dst);
  //   fprintf(stderr, "route output: %s -> %s\n", ss_origin.str().c_str(), ss_dst.str().c_str());
  // }
  // fprintf(stderr, "routing table of %p:\n", this);
  // for( ns3::RtEntry rt : m_entries ) {
  //   std::stringstream dst_ss;
  //   std::stringstream gw_ss;
  //   rt.dst.Print(dst_ss);
  //   rt.gw.Print(gw_ss);
  //   fprintf(stderr, "ns3-rt: %s/%d via %s\n", dst_ss.str().c_str(), rt.len, gw_ss.str().c_str());
  // }
  // fprintf(stderr, "------------- < end table\n");

  ns3::RtEntry finalRt;
  int bestLen = -1; // greater = better -> more precision, -1 = no route
  for( ns3::RtEntry rt : m_entries ) {
    // check that route matches (according to netmask)
    if(ns3::Ipv4Mask(("/"+std::to_string(rt.len)).c_str()).IsMatch(rt.dst,dst)) {
      if((int)rt.len > bestLen) {
        bestLen = rt.len;
        finalRt = rt;
      }
    }
  }
  if(bestLen >= 0) {
    // find first matching device for ip/netmask
    for(uint32_t if_idx = 0; if_idx < m_ipv4->GetNInterfaces(); ++if_idx) {
      for(uint32_t addr_idx = 0; addr_idx < m_ipv4->GetNAddresses(if_idx); ++addr_idx) {
        auto if_addr = m_ipv4->GetAddress (if_idx, addr_idx);
        if (if_addr.GetMask().IsMatch(if_addr.GetLocal(), finalRt.gw)) {
          route = ns3::Create<Ipv4Route> ();
          route->SetDestination (dst);
          route->SetGateway (finalRt.gw);
          route->SetSource (if_addr.GetLocal ());
          route->SetOutputDevice (m_ipv4->GetNetDevice (if_idx));
          goto __out;
        }
      }
    }
  }
 __out:
  return route;
}

bool ns3::HgRoutingProtocol::RouteInput (ns3::Ptr<const ns3::Packet> p, const ns3::Ipv4Header &header,
                                         ns3::Ptr<const ns3::NetDevice> idev,
                                         UnicastForwardCallback ucb,
                                         MulticastForwardCallback mcb,
                                         LocalDeliverCallback lcb,
                                         ErrorCallback ecb)
{
  Ipv4Address dst = header.GetDestination ();
  // Ipv4Address origin = header.GetSource ();

  // for (int iff=0; iff<m_ipv4.GetNInterfaces(); iff++) {
  //   for (int addr=0; addr<m_ipv4.GetNAddresses(iff); addr++) {
  //     if(m_ipv4->GetAddress(iff,addr).GetLocal.Equals(origin)) {
  //       if(!lcb.IsNull) {
  //         fprintf(stderr,"... local!\n");
  //         lcb(p,header,0);
  //         return true;
  //       }
  //     }
  //   }
  // }
  
  // Local delivery
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);
  if (m_ipv4->IsDestinationAddress (dst, iif))
    {
      if (!lcb.IsNull ())
        {
          // NS_LOG_LOGIC ("Local delivery to " << dst);
          lcb (p, header, iif);
          return true;
        }
      else
        {
          // The local delivery callback is null.  This may be a multicast
          // or broadcast packet, so return false so that another 
          // multicast routing protocol can handle it.  It should be possible
          // to extend this to explicitly check whether it is a unicast
          // packet, and invoke the error callback if so
          return false;
        }
    }

  // {
  //   std::stringstream ss_origin, ss_dst;
  //   origin.Print(ss_origin);
  //   dst.Print(ss_dst);
  //   fprintf(stderr, "route input: %s -> %s: ", ss_origin.str().c_str(), ss_dst.str().c_str());
  // }
  
  // Forwarding
  ns3::Ptr<ns3::Ipv4Route> route;

  ns3::RtEntry finalRt;
  int bestLen = -1; // greater = better -> more precision, -1 = no route
  for( ns3::RtEntry rt : m_entries ) {
    // check that route matches (according to netmask)
    if(ns3::Ipv4Mask(("/"+std::to_string(rt.len)).c_str()).IsMatch(rt.dst,dst)) {
      if((int)rt.len > bestLen) {
        bestLen = rt.len;
        finalRt = rt;
      }
    }
  }
  if(bestLen >= 0) {
    // find first matching device for ip/netmask
    for(uint32_t if_idx = 0; if_idx < m_ipv4->GetNInterfaces(); ++if_idx) {
      for(uint32_t addr_idx = 0; addr_idx < m_ipv4->GetNAddresses(if_idx); ++addr_idx) {
        auto if_addr = m_ipv4->GetAddress (if_idx, addr_idx);
        if (if_addr.GetMask().IsMatch(if_addr.GetLocal(), finalRt.gw)) {
          route = ns3::Create<Ipv4Route> ();
          route->SetDestination (dst);
          route->SetGateway (finalRt.gw);
          route->SetSource (if_addr.GetLocal ());
          route->SetOutputDevice (m_ipv4->GetNetDevice (if_idx));
          ucb(route, p, header);
          // fprintf(stderr, "found route: %x -> %x via %x\n", origin.Get(), dst.Get(), finalRt.gw.Get());
          return true;
        }
      }
    }
    NS_ASSERT(false && "inconsistent RtEntry, cannot find interface for gw");
  } else {
    // fprintf(stderr, "...failure\n");
    return false;
  }
}

void ns3::HgRoutingProtocol::PrintRoutingTable (Ptr< OutputStreamWrapper > stream) const {
  NS_ASSERT(false && "UNIMPLEMENTED");
}

void 	ns3::HgRoutingProtocol::SetIpv4 (ns3::Ptr< ns3::Ipv4 > ipv4) {
  m_ipv4 = ipv4;
}
