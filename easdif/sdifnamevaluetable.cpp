/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
 *
 */
/**
 * @file   sdifnamevaluetable.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 19:41:01 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifnamevaluetable.cpp,v 1.7 2005-05-30 21:43:00 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.6  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.5  2003/05/22 21:23:58  roebel
 * SDIFNameValueTable now derived from std::map which makes handling more conform
 * to standard and handling in swig easier.
 *
 * Revision 1.4  2003/05/19 13:59:11  roebel
 * Include new easdif_config.h.
 *
 * Revision 1.3  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.5  2002/11/28 21:16:56  roebel
 * Back to simple map which sorts alphabetically. In any case
 * in SDIF the name value tables are mixed arbitrarily.
 *
 * Revision 1.4  2002/11/27 20:10:39  roebel
 * Fixed NVT to keep ordering as defined by the sequence of additions to the table.
 * Added begin/end iterator.
 * Added global functions to get name and value from SDIFNameValueTable iterators.
 * Removed member that redundantly indicated size of map.
 *
 * Revision 1.3  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.2  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.1  2002/06/18 17:56:40  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#include <iostream>

#include "easdif/easdif_config.hpp"
#include "easdif/sdifnamevaluetable.hpp"

namespace Easdif {

int SDIFNameValueTable::AddNameValue(const std::string& name,
				     const std::string& value) 
{
    NVTMap::operator[](name) = value;
    return GetNbNameValue();
}

int SDIFNameValueTable::GetNbNameValue()
{
    return size();
}

SdifUInt4 SDIFNameValueTable::GetStreamID()
{
    return mStreamID;
}

std::string SDIFNameValueTable::GetValue(const std::string& name) const
{
  const_iterator it;
  if((it= find(name)) != end())
    return it->second;
  else {
    return std::string("UNKNOWN NAME");
  }
}

bool SDIFNameValueTable::GetValue(const std::string& name, std::string& value) const
{
  const_iterator it;
  if((it= find(name)) != end()){
    value = it->second;
    return true;
  }
  else {
    return false;
  }
}

SdifUInt4 SDIFNameValueTable::SetStreamID(const SdifUInt4& streamid)
{
    return mStreamID = streamid;
}


void SDIFNameValueTable::PrintNameValue(const std::string& name) const
{
  std::cout << name << "\t\t"
	    << GetValue(name) << std::endl;
}




void SDIFNameValueTable::PrintNameValueTable() const
{

    for (const_iterator p = begin(); 
	 p != end() ; ++p)
    {
      std::cout << p->first << "\t\t" 
		<< p->second << "\n";
    }
}



} // end of namespace Easdif

