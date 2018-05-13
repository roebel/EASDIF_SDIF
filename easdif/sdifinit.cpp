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

#include "easdif/easdif_config.hpp"
#include "easdif/sdifinit.hpp"

namespace Easdif {

//void EasdifInit(char* PredefinedType)
bool EasdifInit(const std::string& PredefinedType)
{
    return EasdifInit(PredefinedType.c_str());
}

//void EasdifInit(char* PredefinedType)
bool EasdifInit(const char *PredefinedType)
{
  if(gSdifInitialised){
    SdifSetErrorFunc(ExceptionThrower);
    return false;
  }
  else{
    SdifGenInit(PredefinedType);
    SdifSetErrorFunc(ExceptionThrower);
    return true;
  }
}

void EasdifEnd()
{
 SdifGenKill();
}

} // end of namespace Easdif
