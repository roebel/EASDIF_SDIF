/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2007 by IRCAM-Centre Georges Pompidou, Paris, France.
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

#ifndef EASDIF_EXPORTS_H
#define EASDIF_EXPORTS_H

#if defined( EASDIF_IS_STATIC) || defined(SDIF_IS_STATIC)
#  define EASDIF_API
#else
#  if defined( _WIN32) || defined(WIN32)
#    ifdef DO_EXPORT_SDIF
#      define EASDIF_API __declspec(dllexport)
#    else
#      define EASDIF_API __declspec(dllimport)
#    endif
#  else
#    if defined(__GNUC__) && defined( GCC_HAS_VISIBILITY)
#      define EASDIF_API __attribute__ ((visibility("default")))
#    else
#      define EASDIF_API
#    endif
#  endif
#endif

#endif

