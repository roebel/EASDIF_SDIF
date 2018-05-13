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
 * @file   easdif.h
 * @author Axel Roebel
 * @date   Wed Aug 28 16:45:00 2002
 * 
 * @brief  user level include file for the use of Easdif library
 * 
 * 
 * 
 * $Id: easdif.h,v 1.10 2007-11-26 19:11:15 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.9  2005/05/30 21:43:01  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.8  2004/09/10 14:44:32  roebel
 * Explicte cast to bool.
 *
 * Revision 1.7  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.6  2003/11/19 11:59:20  roebel
 * Put SdifCheckFileFormat which is a variant of
 * a standard sdif function back into global namespace.
 *
 * Revision 1.5  2003/11/18 12:43:30  roebel
 * Moved SdifCheckFileFormat into Easdif namespace.
 *
 * Revision 1.4  2003/08/06 18:29:11  roebel
 * Added missing inline for sdif helper function
 *
 * Revision 1.3  2003/08/06 17:59:58  roebel
 * Added helper function for testing sdiffiles with std::string filename argument
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.2  2002/10/10 16:32:39  roebel
 * Include version include file.
 *
 * Revision 1.1  2002/08/28 16:45:41  roebel
 * Initial Sdif Version based on the work Fabien Tisseran
 * with some name changes.
 * 
 * 
 */

#ifndef EASDIF_H_
#define EASDIF_H_ 1

#include <string>
#include <vector>
#include "sdif.h"

/**
 * \defgroup  sdifsupport Sdif Support Functions
 */

/** 
 * \ingroup sdifsupport
 * \brief test file for sdif format
 * 
 * Attention SdifCheckFileFormat being a variant of
 * a standard sdif function is located in global namespace
 *
 * 
 * @param name filename to test
 * 
 * @return true if filename contains sdif data 
 */
  inline 
  bool SdifCheckFileFormat (const std::string &name) {
    return SdifCheckFileFormat(name.c_str())!=0;
  }



#include "easdif/easdif_version.hpp"
#include "easdif/sdifentity.hpp"


#endif


