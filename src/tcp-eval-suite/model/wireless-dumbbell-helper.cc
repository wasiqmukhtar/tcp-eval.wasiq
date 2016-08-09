/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Wasiq Mukhtar <wasiqmukhtar@gmail.com>
 */

// Implement an object to create Wireless access topology in tcp-eval.

#include<iostream>
#include<cstdlib>
#include<sstream>
#include<string>

#include"Wireless-acess.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "wifi-module.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WirelessDumbbellHelper")

NS_OBJECT_ENSURE_REGISTERED (WirelessDumbbellHelper);

TypeId
WirelessDumbbellHelper::GetTypeId (void)
{
  static TypeId tid = TypeId("ns3::WirelessDumbbellHelper")
    .SetParent<ConfigureTopology> ()
    .SetGroupName ("TcpEvaluationSuite")
  ;
  return tid;
}

WirelessDumbbellHelper::WirelessDumbbellHelper (void)
{
}

WirelessDumbbellHelper::~WirelessDumbbellHelper (void)
{
}

WirelessDumbbellHelper::CreateWirelessDumbbellHelper (np2pLeaf,nWifiLeaf)
{
  p2pNodes.Create (2);

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  p2pDevices = pointToPoint.Install (p2pNodes);

  p2pLeafNodes.Add (p2pNodes.Get (1));
  p2pLeafNodes.Create (np2pLeaf);

  p2pDevices = pointToPoint.Install (p2pNodes);

  wifiStaNodes.Create (nWifiLeaf);
  wifiApNode = p2pNodes.Get (0);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  apDevices = wifi.Install (phy, mac, wifiApNode);


  internet.Install (p2pNodes);
  internet.Install (p2pLeafNodes);
  internet.Install (wifiLeafNodes);

  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);



  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiLeafNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

}

Ptr<Node> WirelessDumbbellHelper::GetLeft () const
{ // Get the left side bottleneck router
  return p2pNodes.Get (0);
}

}
