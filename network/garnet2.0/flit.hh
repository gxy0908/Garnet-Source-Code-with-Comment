/*
 * Copyright (c) 2008 Princeton University
 * Copyright (c) 2016 Georgia Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Niket Agarwal
 *          Tushar Krishna
 */


#ifndef __MEM_RUBY_NETWORK_GARNET_FLIT_HH__
#define __MEM_RUBY_NETWORK_GARNET_FLIT_HH__

#include <cassert>
#include <iostream>

#include "base/types.hh"
#include "mem/ruby/network/garnet2.0/CommonTypes.hh"
#include "mem/ruby/slicc_interface/Message.hh"

class flit
{
  public:
    flit() {}
    flit(int id, int vc, int vnet, RouteInfo route, int size,
         MsgPtr msg_ptr, Cycles curTime);


    //++ Get Data.
    int get_outport() {return m_outport; }
    int get_size() { return m_size; }
    Cycles get_enqueue_time() { return m_enqueue_time; }
    Cycles get_dequeue_time() { return m_dequeue_time; }
    int get_id() { return m_id; }
    Cycles get_time() { return m_time; }
    int get_vnet() { return m_vnet; }
    int get_vc() { return m_vc; }
    RouteInfo get_route() { return m_route; }
    MsgPtr& get_msg_ptr() { return m_msg_ptr; }
    flit_type get_type() { return m_type; }
    std::pair<flit_stage, Cycles> get_stage() { return m_stage; }
    Cycles get_src_delay() { return src_delay; }

    //++ Set Data.
    void set_outport(int port) { m_outport = port; }
    void set_time(Cycles time) { m_time = time; }
    void set_vc(int vc) { m_vc = vc; }
    void set_route(RouteInfo route) { m_route = route; }
    void set_src_delay(Cycles delay) { src_delay = delay; }
    void set_dequeue_time(Cycles time) { m_dequeue_time = time; }

    void increment_hops() { m_route.hops_traversed++; }
    void print(std::ostream& out) const;

    //++ whether current stage is the target stage and time is ready for the flit.
    //++ TODO: Where is it used?
    bool
    is_stage(flit_stage stage, Cycles time)
    {
        return (stage == m_stage.first &&
                time >= m_stage.second);
    }
    
    //++ TODO: What is it? Where is it used?
    void
    advance_stage(flit_stage t_stage, Cycles newTime)
    {
        m_stage.first = t_stage;
        m_stage.second = newTime;
    }

    //++ whether n2 is in front of n1.
    //++ TODO: Where is it used?
    static bool
    greater(flit* n1, flit* n2)
    {
        if (n1->get_time() == n2->get_time()) {
            //assert(n1->flit_id != n2->flit_id);
            return (n1->get_id() > n2->get_id());
        } else {
            return (n1->get_time() > n2->get_time());
        }
    }

    bool functionalWrite(Packet *pkt);

  protected:
    int m_id;          //++ Flit ID  TODO: What is flit ID?
    int m_vnet;        //++ Current virtual net
    int m_vc;          //++ Current virtual channel
    RouteInfo m_route; //++ Routing information
    int m_size;        //++ TODO
    //TODO
    Cycles m_enqueue_time, m_dequeue_time, m_time;
    flit_type m_type;  //++ Flit type
    MsgPtr m_msg_ptr;  //++ TODO
    int m_outport;     //++ TODO
    Cycles src_delay;  //++ TODO
    std::pair<flit_stage, Cycles> m_stage; //++ TODO
};

inline std::ostream&
operator<<(std::ostream& out, const flit& obj)
{
    obj.print(out);
    out << std::flush;
    return out;
}

#endif // __MEM_RUBY_NETWORK_GARNET_FLIT_HH__
