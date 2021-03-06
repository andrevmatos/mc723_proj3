/**
 * @file      ac_tlm_mem.h
 * @author    Bruno de Carvalho Albertini
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 *
 * @brief     Defines a ac_tlm memory.
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

//////////////////////////////////////////////////////////////////////////////
#ifndef BAR_MEM_H_
#define BAR_MEM_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
// SystemC includes
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include  "ac_tlm_port.H"
#include  "ac_memport.H"

#define HARDWARE_BW_ADDR_HEXACOLOR 5242884U
#define HARDWARE_BW_ADDR_RESULT 5242892U
#define MUTEX_TOKEN_ADDR 5242896U

//////////////////////////////////////////////////////////////////////////////

// using statements
using tlm::tlm_transport_if;

//////////////////////////////////////////////////////////////////////////////

//#define DEBUG

/// Namespace to isolate memory from ArchC
namespace user
{

/// A TLM memory
class bar_mem :
  public sc_module,
  public ac_tlm_transport_if // Using ArchC TLM protocol
{
private:
  volatile bool write_lock;
public:
  /// Exposed port with ArchC interface
  sc_export< ac_tlm_transport_if > target_export1;
  sc_export< ac_tlm_transport_if > target_export2;
  sc_export< ac_tlm_transport_if > target_export3;
  sc_export< ac_tlm_transport_if > target_export4;
  sc_export< ac_tlm_transport_if > target_export5;
  sc_export< ac_tlm_transport_if > target_export6;
  sc_export< ac_tlm_transport_if > target_export7;
  sc_export< ac_tlm_transport_if > target_export8;
  ac_tlm_port DM_port;
  ac_tlm_port BW_port;
  ac_tlm_port MUTEX_port;

  /**
   * Implementation of TLM transport method that
   * handle packets of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * @param request is a received request packet
   * @return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request ) {
    if (request.addr >= HARDWARE_BW_ADDR_HEXACOLOR && request.addr <= HARDWARE_BW_ADDR_RESULT )
      return BW_port->transport(request);
    else if (request.addr == MUTEX_TOKEN_ADDR )
      return MUTEX_port->transport(request);
    else
      return DM_port->transport(request);
  }


  /**
   * Default constructor.
   *
   * @param k Memory size in kilowords.
   *
   */
  bar_mem( sc_module_name module_name);

  /**
   * Default destructor.
   */
  ~bar_mem();

};

};

#endif //AC_TLM_MEM_H_
