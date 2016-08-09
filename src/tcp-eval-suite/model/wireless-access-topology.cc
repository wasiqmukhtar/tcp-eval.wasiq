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
 * Author: Wasiq Mukhtar <wasiqmukhtar@gmail.com>
 */

// Implement an object to create dumbbell topology in tcp-eval.

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "wireless-acess-topology.h"
#include "eval-stats.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "wifi-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "wireless-dumbbell-helper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WirelessAccessTopology");

NS_OBJECT_ENSURE_REGISTERED (WirelessAccessTopology);

TypeId
DumbbellTopology::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WirelessAccessTopology")
    .SetParent<ConfigureTopology> ()
    .SetGroupName ("TcpEvaluationSuite")
  ;
  return tid;
}

WirelessAccessTopology::WirelessAccessTopology (void)
{
}

WirelessAccessTopology::~WirelessAccessTopology (void)
{
}

void
WirelessAccessTopology::CreateWirelessAcessTopology (Ptr<TrafficParameters> traffic, std::string fileName)
{
  uint32_t nBottlenecks = 1;

  // Set default parameters for topology
  SetTopologyParameters (traffic, nBottlenecks);

  uint32_t nFwdFtpFlow = traffic->GetNumOfFwdFtpFlows ();
  uint32_t nRevFtpFlow = traffic->GetNumOfRevFtpFlows ();
  uint32_t nVoiceFlow = traffic->GetNumOfVoiceFlows ();
  uint32_t nFwdStreamingFlow = traffic->GetNumOfFwdStreamingFlows ();
  uint32_t nRevStreamingFlow = traffic->GetNumOfRevStreamingFlows ();

  // Calculate total leaf nodes at each side
  uint32_t nLeftLeaf = nFwdFtpFlow + nRevFtpFlow + nVoiceFlow + nFwdStreamingFlow + nRevStreamingFlow;
  uint32_t nRightLeaf = nLeftLeaf;

  WirelessDumbbellHelper wirelessdumbbell (np2pLeaf,nWifiLeaf);

  uint32_t offset = 0;
  Ptr<CreateTraffic> createTraffic = CreateObject<CreateTraffic> ();
  if (nFwdFtpFlow > 0)
    {
      // Create forward FTP traffic
      createTraffic->CreateFwdFtpTraffic (wirelessdumbbell, nFwdFtpFlow, offset, traffic);
      offset += nFwdFtpFlow;
    }

  if (nRevFtpFlow > 0)
    {
      // Create reverse FTP traffic
      createTraffic->CreateRevFtpTraffic (wirelessdumbbell, nRevFtpFlow, offset, traffic);
      offset += nRevFtpFlow;
    }
  if (nVoiceFlow > 0)
    {
      // Create voice traffic
      createTraffic->CreateVoiceTraffic (wirelessdumbbell, nVoiceFlow, offset, traffic);
      offset += nVoiceFlow;
    }
  if (nFwdStreamingFlow > 0)
    {
      // Create forward streaming traffic
      createTraffic->CreateFwdStreamingTraffic (wirelessdumbbell, nFwdStreamingFlow, offset, traffic);
      offset += nFwdStreamingFlow;
    }
  if (nRevStreamingFlow > 0)
    {
      // Create reverse streaming traffic
      createTraffic->CreateRevStreamingTraffic (wirelessdumbbell, nRevStreamingFlow, offset, traffic);
      offset += nRevStreamingFlow;
    }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Push the stats of left most router to a file
  Ptr<Node> left = wirelessdumbbell.GetLeft ();
  Ptr<EvalStats> evalStats = CreateObject<EvalStats> (m_bottleneckBandwidth, m_rttp , fileName);
  evalStats->Install (left, traffic);

  Simulator::Stop (Time::FromDouble (((traffic->GetSimulationTime ()).ToDouble (Time::S) + 5), Time::S));
  Simulator::Run ();
  Simulator::Destroy ();
}

template <typename T>
std::string WirelessAccessTopology::to_string (const T& data)
{
  std::ostringstream conv;
  conv << data;
  return conv.str ();
}

}
