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

#ifndef SDIFINIT_H_
#define SDIFINIT_H_ 1

#include "sdif.h"
#include "easdif_exports.hpp"
#include "easdif/sdifexception.hpp"

namespace Easdif {

/*
  Initialise the SDIF library, providing a name for an optional additional
  file with type definitions or "". And install exceptions.
  <b>This function has to be called once and only once per process 
  before any other call to the SDIF library.</b> 
*/
  /**
   * \defgroup initialization Init/Deinit of Easdif library
   */

  /** 
   * \ingroup initialization
   * \brief initialize Easdif and sdif library
   *
   * @param PredefinedType 
   * 
   * @return true if sdif has been initialized /false if SDIF was already initialized
   *    such that types could not be changed.
   */
  EASDIF_API
  bool EasdifInit(const std::string& PredefinedType = "");

  /** 
   * \ingroup initialization
   * \brief initialize Easdif and sdif library
   * 
   * @param PredefinedType 
   *
   * @return true if sdif has been initialized /false if SDIF was already initialized
   *    such that types could not be changed.
   */
  EASDIF_API
  bool EasdifInit(const char *PredefinedType);

  /** 
   * \ingroup initialization
   * \brief deinitialize Easdif and sdif library
   * 
   */
  EASDIF_API
  void EasdifEnd();

} // end of namespace Easdif

#endif
