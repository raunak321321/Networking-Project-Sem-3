/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2018 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
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
 * Authors: Jaume Nin <jaume.nin@cttc.cat>
 *          Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/lte-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/network-module.h"
#include <bits/stdc++.h>
using namespace std;
//#include "ns3/gtk-config-store.h"

using namespace ns3;

/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeBs,
 * attaches one UE per eNodeB starts a flow for each UE to and from a remote host.
 * It also starts another flow between each UE pair.
 */

//    enB1       enB2             pgw --------- remoteHostContainer
//     .           .           (1.0.0.1)             (1.0.0.2)
//     .           .
//    ue1        ue2

NS_LOG_COMPONENT_DEFINE ("SourceFile"); // we can give any name

class MyTag : public Tag
{
public:
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;
  void SetTxTm (Time value);
  Time GetTxTm (void) const;
private:
  Time txTm;
};

TypeId
MyTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t
MyTag::GetSerializedSize (void) const
{
  return sizeof(ns3::Time);
}
void
MyTag::Serialize (TagBuffer i) const
{
  i.WriteDouble(txTm.GetDouble());
}
void
MyTag::Deserialize (TagBuffer i)
{
  txTm = Time::FromDouble(i.ReadDouble(), Time::NS);
}
void
MyTag::Print (std::ostream &os) const
{
  os << "txTm=" << txTm << std::endl;
}
void
MyTag::SetTxTm (Time value)
{
  txTm = value;
}
Time
MyTag::GetTxTm (void) const
{
  return txTm;
}

uint32_t txPktCount=0, txData=0;
Time rxTime, txTime;
uint32_t ue1Data=0, ue2Data=0, ue3Data=0, ue4Data=0, ue5Data=0, ue6Data=0, ue7Data=0, ue8Data=0;
uint32_t ue1PktCount=0, ue2PktCount=0, ue3PktCount=0, ue4PktCount=0, ue5PktCount=0, ue6PktCount=0, ue7PktCount=0, ue8PktCount=0;
Time ue1Delay, ue2Delay, ue3Delay, ue4Delay, ue5Delay, ue6Delay, ue7Delay, ue8Delay;

void RxTrace(std::string context, Ptr<const Packet> pkt, const Address &a, const Address &b)
{
	MyTag tag;

	if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.2"))
	{
		pkt->PeekPacketTag(tag);
		txTime = tag.GetTxTm();
		rxTime = Simulator::Now();
		ue1Data = ue1Data + pkt->GetSize();
		ue1PktCount++;
		ue1Delay += (rxTime - txTime);
	}
	if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.3"))
	{
		pkt->PeekPacketTag(tag);
		txTime = tag.GetTxTm();
		rxTime = Simulator::Now();
		ue2Data = ue2Data + pkt->GetSize();
	              ue2PktCount++;
		ue2Delay += rxTime - txTime;
	}
	if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.4"))
	{
		pkt->PeekPacketTag(tag);
		txTime = tag.GetTxTm();
		rxTime = Simulator::Now();
		ue3Data = ue3Data + pkt->GetSize();
		ue3PktCount++;
		ue3Delay += rxTime - txTime;
	}
	if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.5"))
	{
		pkt->PeekPacketTag(tag);
		txTime = tag.GetTxTm();
		rxTime = Simulator::Now();
		ue4Data = ue4Data + pkt->GetSize();
		ue4PktCount++;
		ue4Delay += rxTime - txTime;
	}
	// if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.6"))
	// {
	// 	pkt->PeekPacketTag(tag);
	// 	txTime = tag.GetTxTm();
	// 	rxTime = Simulator::Now();
	// 	ue5Data = ue5Data + pkt->GetSize();
	// 	ue5PktCount++;
	// 	ue5Delay += rxTime - txTime;
	// }
	// if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.7"))
	// {
	// 	pkt->PeekPacketTag(tag);
	// 	txTime = tag.GetTxTm();
	// 	rxTime = Simulator::Now();
	// 	ue6Data = ue6Data + pkt->GetSize();
	// 	ue6PktCount++;
	// 	ue6Delay += rxTime - txTime;
	// }
	// if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.8"))
	// {
	// 	pkt->PeekPacketTag(tag);
	// 	txTime = tag.GetTxTm();
	// 	rxTime = Simulator::Now();
	// 	ue7Data = ue7Data + pkt->GetSize();
	// 	ue7PktCount++;
	// 	ue7Delay += rxTime - txTime;
	// }
	// if(InetSocketAddress::ConvertFrom(a).GetIpv4() == Ipv4Address("7.0.0.9"))
	// {
	// 	pkt->PeekPacketTag(tag);
	// 	txTime = tag.GetTxTm();
	// 	rxTime = Simulator::Now();
	// 	ue8Data = ue8Data + pkt->GetSize();
	// 	ue8PktCount++;
	// 	ue8Delay += rxTime - txTime;
	// }
}

void TxTrace(std::string context, Ptr<const Packet> pkt, const Address &src, const Address &dest)
{
	MyTag tag;
	tag.SetTxTm(Simulator::Now());
	pkt->AddPacketTag(tag);
}


int
main (int argc, char *argv[])
{
  uint16_t numUe = 4; // numNodePairs
  Time simTime = MilliSeconds (100000);
  double distance = 20.0;
  Time interPacketInterval = MilliSeconds (10); // time delay between two packets
  bool useCa = false;
  bool disableDl = true;
  bool disableUl = false;
  bool disablePl = true;

  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  // using these only we can see the corresponding application output on console.
  LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);

  // Command line arguments
  CommandLine cmd;
  cmd.AddValue ("numUe", "Number of UEs", numUe);
  cmd.AddValue ("simTime", "Total duration of the simulation", simTime);
  cmd.AddValue ("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue ("interPacketInterval", "Inter packet interval", interPacketInterval);
  cmd.AddValue ("useCa", "Whether to use carrier aggregation.", useCa);
  cmd.AddValue ("disableDl", "Disable downlink data flows", disableDl);
  cmd.AddValue ("disableUl", "Disable uplink data flows", disableUl);
  cmd.AddValue ("disablePl", "Disable data flows between peer UEs", disablePl);
  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

  /*if (useCa)
   {
     Config::SetDefault ("ns3::LteHelper::UseCa", BooleanValue (useCa));
     Config::SetDefault ("ns3::LteHelper::NumberOfComponentCarriers", UintegerValue (2));
     Config::SetDefault ("ns3::LteHelper::EnbComponentCarrierManager", StringValue ("ns3::RrComponentCarrierManager"));
   }*/

  // The helper classes are used to configures the parameters of main classes so for this we use helper classes
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  // the communication between eNB's and pgw and in between pgw and remotehostcontainer all these are wired links(point to point links) thats why we are using pointTopoint to helper class
  lteHelper->SetEpcHelper (epcHelper);

  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  // epchelper contains all those things but we are taking only pqw from it becoz we are considering only pdn gateway

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);

  // for data transfer between pdn gateway and remostHost container we are installing an internet stack so there is one more question y v r not installing an internet stack for pdn gateway so the answer is simple like-->so pdn gateway by default is having internet stack(as it is purely based on lte architecture so it has its own stack).

  InternetStackHelper internet;
  internet.Install (remoteHostContainer);
  // As we install internet stack now we have to make connection between pdn gateway and remotehost container

  // Create the Internet
  PointToPointHelper p2ph;
  // below are some attributes which are setting
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1.5)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);

  // now we are installing network cards or ethernet cards

  // now we are setting ip addresses
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  // now we are assigning address
  //Interface 0 is localhost, 1 is the p2p device

  // what is the address of the remoteHost that is we are fetching
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

  // why the routing table is needed becuase node has come to know where the destination is or to which path the packet should be routed
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
      ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (1);
  ueNodes.Create (numUe);

  // Install Mobility Model

  // below are comments which is the original code
  Ptr<ListPositionAllocator> enbPosition = CreateObject<ListPositionAllocator> ();
  Ptr<ListPositionAllocator> uePosition = CreateObject<ListPositionAllocator> ();

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  enbPosition->Add (Vector (10, 10, 0));
  mobility.SetPositionAllocator (enbPosition);
  mobility.Install (enbNodes);

  for (uint16_t i = 0; i < numUe; i++)
    {
      uePosition->Add (Vector (distance * i, 0, 0));
    }
  mobility.SetPositionAllocator (uePosition);
  mobility.Install (ueNodes);

  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  // enbdevice are nothing but are antennas
  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);
  // these are nothing but the mobile phone
  //lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIface;
  ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));
  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting =
          ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      // at which addresss the user equipment node is present.
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  // now both the ue's knew that using which gateway the data has to be send

  // Attach UEs to eNodeB
  for (uint16_t i = 0; i < numUe; i++)
    {
      lteHelper->Attach (ueLteDevs.Get (i), enbLteDevs.Get (0));
      // side effect: the default EPS bearer will be activated
    }

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 1100;
  uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps, cApps;
  ApplicationContainer serverApps, sApps;
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      if (!disableDl)
        {
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          // this is the class using which socket can be created(that ns3::UDPone) and on which address socket to be created
          serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (u)));

          UdpClientHelper dlClient (ueIpIface.GetAddress (u), dlPort);
          dlClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
          dlClient.SetAttribute ("MaxPackets", UintegerValue (100));
          clientApps.Add (dlClient.Install (remoteHost));
        }

      if (!disableUl)
        {
          ++ulPort;
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));

          UdpEchoClientHelper ulClient (remoteHostAddr, ulPort);
          ulClient.SetAttribute ("Interval", TimeValue (interPacketInterval));
          ulClient.SetAttribute ("MaxPackets", UintegerValue (1000));
          ulClient.SetAttribute ("PacketSize", UintegerValue (2048));
          clientApps.Add (ulClient.Install (ueNodes.Get (u)));
        }

      if (!disablePl && numUe > 1)
        {
          ++otherPort;
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory",
                                             InetSocketAddress (Ipv4Address::GetAny (), otherPort));
          serverApps.Add (packetSinkHelper.Install (ueNodes.Get (u)));

          UdpClientHelper client (ueIpIface.GetAddress (u), otherPort);
          client.SetAttribute ("Interval", TimeValue (interPacketInterval));
          client.SetAttribute ("MaxPackets", UintegerValue (10));
          clientApps.Add (client.Install (ueNodes.Get ((u + 1) % numUe)));
        }
    }

  serverApps.Start (MilliSeconds (100));
  clientApps.Start (MilliSeconds (100));
  clientApps.Get (0)->SetStopTime (MilliSeconds (500));
  clientApps.Get (1)->SetStopTime (MilliSeconds (800));

  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::PacketSink/RxWithAddresses", MakeCallback(&RxTrace));
  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/TxWithAddresses", MakeCallback(&TxTrace));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor;
  NodeContainer flowNodes;
  flowNodes.Add (ueNodes);
  flowNodes.Add (remoteHost);
  monitor = flowmon.Install (flowNodes);

  // avgDelay = ue1Delay.GetSeconds()/ue1PktCount + ue2Delay.GetSeconds()/ue2PktCount + ue3Delay.GetSeconds()/ue3PktCount + ue4Delay.GetSeconds()/ue4PktCount + ue5Delay.GetSeconds()/ue5PktCount+ ue6Delay.GetSeconds()/ue6PktCount;
  // avgThroughput = (ue1Data*8)/ue1Delay.GetSeconds()/1000 + (ue2Data*8)/ue2Delay.GetSeconds()/1000 + (ue3Data*8)/ue3Delay.GetSeconds()/1000 + (ue4Data*8)/ue4Delay.GetSeconds()/1000 + (ue5Data*8)/ue5Delay.GetSeconds()/1000 + (ue6Data*8)/ue6Delay.GetSeconds()/1000;
  // avgDelay /= numUe;
  // avgThroughput /= numUe;

  Simulator::Stop (simTime);
  
  Simulator::Run ();

  std::cout<<"\nNo. of prioritized UE: 2\n";
  std::cout<<"------------------------------------------------------------------------\n";
  std::cout<<"UE\tData(Bytes)\tPackets\t\tAvg Delay(sec)\tThroughput(kbps)\n";
  std::cout<<"------------------------------------------------------------------------\n";
  std::cout<<"1\t"<<ue1Data<<"\t\t"<<ue1PktCount<<"\t\t"<<ue1Delay.GetSeconds()/ue1PktCount<<"\t"<<(ue1Data*8)/ue1Delay.GetSeconds()/1000<<std::endl;
  std::cout<<"2\t"<<ue2Data<<"\t\t"<<ue2PktCount<<"\t\t"<<ue2Delay.GetSeconds()/ue2PktCount<<"\t"<<(ue2Data*8)/ue2Delay.GetSeconds()/1000<<std::endl;
  std::cout<<"3\t"<<ue3Data<<"\t\t"<<ue3PktCount<<"\t\t"<<ue3Delay.GetSeconds()/ue3PktCount<<"\t"<<(ue3Data*8)/ue3Delay.GetSeconds()/1000<<std::endl;
  std::cout<<"4\t"<<ue4Data<<"\t\t"<<ue4PktCount<<"\t\t"<<ue4Delay.GetSeconds()/ue4PktCount<<"\t"<<(ue4Data*8)/ue4Delay.GetSeconds()/1000<<std::endl;
  // std::cout<<"5\t"<<ue5Data<<"\t\t"<<ue5PktCount<<"\t\t"<<ue5Delay.GetSeconds()/ue5PktCount<<"\t"<<(ue5Data*8)/ue5Delay.GetSeconds()/1000<<std::endl;
  // std::cout<<"6\t"<<ue6Data<<"\t\t"<<ue6PktCount<<"\t\t"<<ue6Delay.GetSeconds()/ue6PktCount<<"\t"<<(ue6Data*8)/ue6Delay.GetSeconds()/1000<<std::endl;
  // std::cout<<"7\t"<<ue7Data<<"\t\t"<<ue7PktCount<<"\t\t"<<ue7Delay.GetSeconds()/ue7PktCount<<"\t"<<(ue7Data*8)/ue7Delay.GetSeconds()/1000<<std::endl;
  // std::cout<<"8\t"<<ue8Data<<"\t\t"<<ue8PktCount<<"\t\t"<<ue8Delay.GetSeconds()/ue8PktCount<<"\t"<<(ue8Data*8)/ue8Delay.GetSeconds()/1000<<std::endl;
  std::cout<<"------------------------------------------------------------------------\n";

  std::cout<<"Average Delay(sec) is: "<<(ue1Delay.GetSeconds()/ue1PktCount + ue2Delay.GetSeconds()/ue2PktCount + ue3Delay.GetSeconds()/ue3PktCount + ue4Delay.GetSeconds()/ue4PktCount)/numUe<<std::endl;
  std::cout<<"Average Throughput(kbps) is: "<<((ue1Data*8)/ue1Delay.GetSeconds()/1000 + (ue2Data*8)/ue2Delay.GetSeconds()/1000 + (ue3Data*8)/ue3Delay.GetSeconds()/1000 + (ue4Data*8)/ue4Delay.GetSeconds()/1000)/numUe<<std::endl;
  Simulator::Destroy ();
  return 0;
}
