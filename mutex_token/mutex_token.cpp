/**
 * @file      ac_tlm_mem.cpp
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
 * @brief     Implements a ac_tlm memory.
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
// Standard includes
// SystemC includes
// ArchC includes

#include "mutex_token.h"
#include  "ac_tlm_port.H"
#include  "ac_memport.H"

#define CHANGE_ENDIAN(value) (0x00000000 | (value << 24) | ((value << 8) & 0x00FF0000) | ((value >> 8) & 0x0000FF00) | (value >> 24))

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
using user::mutex_token;

/// Constructor
mutex_token::mutex_token( sc_module_name module_name) :
target_export("iport")
{
  /// Binds target_export to the memory
  target_export( *this );
  token = 0;
}

/// Destructor
mutex_token::~mutex_token() {
}

ac_tlm_rsp_status mutex_token::free_token()
{
  token = 0;
  return SUCCESS;
}

ac_tlm_rsp_status mutex_token::get_token( uint32_t &a )
{
  *((uint32_t *) &a) = CHANGE_ENDIAN(acquire_token());
  return SUCCESS;
}

unsigned int mutex_token::acquire_token()
{
  if (!token)
  {
    token = 1;
    return 0;
  }
  return 1;
}