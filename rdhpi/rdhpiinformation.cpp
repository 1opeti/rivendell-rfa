// rdhpiinformation.cpp
//
// A Container Class for AudioScience HPI Adapter Info
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: rdhpiinformation.cpp,v 1.1 2007/09/14 14:06:53 fredg Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <rdhpiinformation.h>


RDHPIInformation::RDHPIInformation()
{
  clear();
}


unsigned RDHPIInformation::serialNumber() const
{
  return serial_number;
}


void RDHPIInformation::setSerialNumber(unsigned num)
{
  serial_number=num;
}


unsigned RDHPIInformation::hpiMajorVersion() const
{
  return hpi_major_version;
}


void RDHPIInformation::setHpiMajorVersion(unsigned ver)
{
  hpi_major_version=ver;
}


unsigned RDHPIInformation::hpiMinorVersion() const
{
  return hpi_minor_version;
}


void RDHPIInformation::setHpiMinorVersion(unsigned ver)
{
  hpi_minor_version=ver;
}


unsigned RDHPIInformation::dspMajorVersion() const
{
  return dsp_major_version;
}


void RDHPIInformation::setDspMajorVersion(unsigned ver)
{
  dsp_major_version=ver;
}


unsigned RDHPIInformation::dspMinorVersion() const
{
  return dsp_minor_version;
}


void RDHPIInformation::setDspMinorVersion(unsigned ver)
{
  dsp_minor_version=ver;
}


char RDHPIInformation::pcbVersion() const
{
  return pcb_version;
}


void RDHPIInformation::setPcbVersion(char ver)
{
  pcb_version=ver;
}


unsigned RDHPIInformation::assemblyVersion() const
{
  return assembly_version;
}


void RDHPIInformation::setAssemblyVersion(unsigned ver)
{
  assembly_version=ver;
}


void RDHPIInformation::clear()
{
  serial_number=0;
  dsp_major_version=0;
  dsp_minor_version=0;
  pcb_version='0';
  assembly_version=0;
}
