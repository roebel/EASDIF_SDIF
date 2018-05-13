/* -*-c++-*- */
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
 * @file   sdifmatrixdatainterface.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:46:08 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrixdatainterface.hpp,v 1.3 2014-05-23 10:30:26 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2008/01/22 00:51:36  roebel
 * Completed support for all sdif integer types.
 *
 * Revision 1.1  2005/05/30 21:43:00  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.7  2004/07/28 14:57:34  roebel
 * Added support for accessing data as unsigned char.
 * Added SetRow and SetCol functions.
 *
 * Revision 1.6  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.5  2004/07/21 13:27:17  roebel
 * Added new data accessing functions to read complete columns
 * GetCol(double *,int icol), GetCol(float *,int icol), GetCol(int *,int icol)
 * and rows
 * GetRow(double *,int irow), GetRow(float *,int irow), GetRow(int *,int irow)
 *
 * Revision 1.4  2003/07/17 18:09:35  roebel
 * Improved Resize method, added Clear method and proper assigment operator
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
 * Revision 1.5  2002/11/07 21:04:28  roebel
 * Changed Set return value to void.
 * Added method to clone data.
 *
 * Revision 1.4  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.3  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.2  2002/07/12 10:29:54  ftissera
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATAINTERFACE_H_
#define SDIFMATRIXDATAINTERFACE_H_ 1

#include "sdif.h"

namespace Easdif {

//class SdifMatrixDataInterface

/** 
 * abstract class which permits the storage in natural type in SDIFMatrixData :
 * you can get the value in different type
 * @see SDIFMatrixData
 */
class SDIFMatrixDataInterface
{ 
public:
//  virtual <class T> Get(int i, int j)=0;
  virtual double GetDouble(int i, int j) const=0;
  virtual float  GetFloat(int i, int j)const=0;
  virtual int    GetInt(int i, int j)const=0;
  virtual unsigned char    GetUChar(int i, int j)const=0;

  virtual void   GetRow(double *,int i)const=0;
  virtual void   GetRow(float *,int i)const=0;
  virtual void   GetRow(SdifInt4 *,int i)const=0;
  virtual void   GetRow(SdifInt2 *,int i)const=0;
  virtual void   GetRow(SdifInt1 *,int i)const=0;
  virtual void   GetRow(SdifUInt4 *,int i)const=0;
  virtual void   GetRow(SdifUInt2 *,int i)const=0;
  virtual void   GetRow(SdifUInt1 *,int i)const=0;
  virtual void   GetCol(double *,int i)const=0;
  virtual void   GetCol(float *,int i)const=0;
  virtual void   GetCol(SdifInt4 *,int i)const=0;
  virtual void   GetCol(SdifInt2*,int i)const=0;
  virtual void   GetCol(SdifInt1 *,int i)const=0;
  virtual void   GetCol(SdifUInt4 *,int i)const=0;
  virtual void   GetCol(SdifUInt2*,int i)const=0;
  virtual void   GetCol(SdifUInt1 *,int i)const=0;

  virtual int    GetNbRows()const=0;
  virtual int    GetNbCols()const=0;
  virtual void   print()const=0;
  virtual int    read(SdifFileT* file)=0;
  virtual void   Resize(int nrows, int ncols)=0;
  virtual void   Clear()=0;
  virtual int    write(SdifFileT* file)=0;
  
  virtual void   Set(int row, int col, unsigned char value)=0;
  virtual void   Set(int row, int col, int    value)=0;
  virtual void   Set(int row, int col, float  value)=0;
  virtual void   Set(int row, int col, double  value)=0;

  virtual void   SetRow(const double *,int i)=0;
  virtual void   SetRow(const float *,int i)=0;
  virtual void   SetRow(const SdifInt4 *,int i)=0;
  virtual void   SetRow(const SdifInt2 *,int i)=0;
  virtual void   SetRow(const SdifInt1 *,int i)=0;
  virtual void   SetRow(const SdifUInt4 *,int i)=0;
  virtual void   SetRow(const SdifUInt2 *,int i)=0;
  virtual void   SetRow(const SdifUInt1 *,int i)=0;
  virtual void   SetCol(const double *,int i)=0;
  virtual void   SetCol(const float *,int i)=0;
  virtual void   SetCol(const SdifInt4 *,int i)=0;
  virtual void   SetCol(const SdifInt2 *,int i)=0;
  virtual void   SetCol(const SdifInt1 *,int i)=0;
  virtual void   SetCol(const SdifUInt4 *,int i)=0;
  virtual void   SetCol(const SdifUInt2 *,int i)=0;
  virtual void   SetCol(const SdifUInt1 *,int i)=0;

  virtual void * GetData() = 0;
  virtual int    GetElementSize() = 0;
  virtual SDIFMatrixDataInterface * clone()=0;


    virtual ~SDIFMatrixDataInterface() {};
};

} // end of namespace Easdif

#endif
