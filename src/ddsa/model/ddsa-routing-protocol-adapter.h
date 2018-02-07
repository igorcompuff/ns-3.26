/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DDSA_ROUTING_PROTOCOL_H
#define DDSA_ROUTING_PROTOCOL_H

#include "ns3/lq-olsr-routing-protocol.h"
#include "ns3/simulator.h"

namespace ns3 {

  namespace ddsa{

    struct Dap
    {
      Ipv4Address address; //!< Address of the destination node.
      double probability; //!< Address of the next hop.
      float cost;
      bool excluded;
      Time expirationTime;

      Dap () : // default values
	address (), probability (0),
	cost (-1), excluded (false)
      {
      }

      bool operator==(const Dap& rhs){ return (address == rhs.address); }
    };

    class DdsaRoutingProtocolAdapter : public ns3::lqolsr::RoutingProtocol
    {
      public:

	/**
	 * \brief Get the type ID.
	 * \return The object TypeId.
	 */
	static TypeId GetTypeId (void);

	DdsaRoutingProtocolAdapter ();
	virtual ~DdsaRoutingProtocolAdapter ();
	double GetAlpha();
	Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, Ipv4Address dstAddr, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
	int GetNRetransmissions();
	virtual Dap SelectDap();
	int GetTotalCurrentEligibleDaps();

	typedef void (* NewRouteComputedTracedCallback)
		    (std::vector<Dap> daps);

      protected:

	virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
	virtual void ProcessHna (const lqolsr::MessageHeader &msg, const Ipv4Address &senderIface);
	virtual void CalculateProbabilities();
	virtual bool ExcludeDaps();
	void RoutingTableComputation ();

      private:


	double SumUpNotExcludedDapCosts();

	void AssociationTupleTimerExpire (Ipv4Address address);
	void BuildEligibleGateways();
	bool DapExists(const Ipv4Address & dapAddress);
	void UpdateDapCosts();

	std::map<Ipv4Address, Dap> m_gateways;
	double alpha;
	Ptr<UniformRandomVariable> m_rnd;
	int n_retransmissions;
	bool dumb;

	TracedCallback<std::vector<Dap> > m_newRouteComputedTrace;


    };

  }
}

#endif /* DDSA_ROUTING_PROTOCOL_H */

