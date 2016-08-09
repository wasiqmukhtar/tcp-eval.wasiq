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

// Define an object to create a wireless dumbbell topology.

#ifndef WIRELESS_DUMBBELL_HELPER_H
#define WIRELESS_DUMBBELL_HELPER_H

#include <string>

#include "point-to-point-helper.h"
#include "ipv4-address-helper.h"
#include "ipv6-address-helper.h"
#include "internet-stack-helper.h"
#include "ipv4-interface-container.h"
#include "ipv6-interface-container.h"
#include "wifi-module.h"

namespace ns3 {

/**
 * \brief A helper to make it easier to create a wireless dumbbell topology
 * with p2p links and wifi links
 */
class WirelessDumbbellHelper
{
public:
  /**
   * Create a WirelessDumbbellHelper in order to easily create
   * dumbbell topologies using p2p links and wifi links
   *
   * \param np2pLeaf number of left side leaf nodes in the dumbbell using
      point to point links
   *
   * \param nRightLeaf number of right side leaf nodes in the dumbbell
   *  using wifi links

   */
  WirelessDumbbellHelper (np2pLeaf,nWifiLeaf);

  ~WirelessDumbbellHelper ();

public:
  /**
   * \returns pointer to the node of the left side bottleneck
   *          router
   */
  Ptr<Node> GetLeft () const;


private:
  NodeContainer p2pNodes;
  PointToPointHelper pointToPoint;
  NetDeviceContainer p2pDevices;
  NodeContainer p2pLeafNodes;
  NetDeviceContainer p2pDevices;
  NodeContainer wifiLeafNodes;
  NodeContainer wifiApNode;
  NetDeviceContainer staDevices;
  NetDeviceContainer apDevices;
  InternetStackHelper internet;
  Ipv4AddressHelper ipv4;
  MobilityHelper mobility;
};

} // namespace ns3

#endif /* WIRELESS_DUMBBELL_HELPER_H */
