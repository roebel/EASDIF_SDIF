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
 * @file   sdif_matrix.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:36:39 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdif_matrix.cpp,v 1.4 2008-06-20 17:07:02 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2007/04/30 11:32:18  roebel
 * Matrix reading routine returns bytecount read for selected matrices only.
 *
 * Revision 1.2  2006/04/22 11:48:09  roebel
 * Fixed left over problems from last renameing operation.
 *
 * Revision 1.1  2006/04/22 08:57:24  roebel
 * Renamed some files to prevent name clash of object files on macosx
 *
 * Revision 1.25  2005/05/30 21:43:00  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.24  2005/05/24 13:13:25  roebel
 * Added mising return statements and fixed minor compiler warnings.
 *
 * Revision 1.23  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.22  2004/09/08 09:16:26  roebel
 * White spaces only.
 *
 * Revision 1.21  2004/07/21 13:27:07  roebel
 * Added new data accessing functions to read complete columns
 * GetCol(double *,int icol), GetCol(float *,int icol), GetCol(int *,int icol)
 * and rows
 * GetRow(double *,int irow), GetRow(float *,int irow), GetRow(int *,int irow)
 *
 * Revision 1.20  2004/07/20 19:32:36  roebel
 * Added support for row and column selection.
 * Matrix reading/writing reorganized to handle complete matrices whenever
 * possible (if no selection on row or column is used).
 * Drastically improves IO performance!
 *
 * Revision 1.19  2004/07/13 15:02:00  roebel
 * Use member bytesread instead of local variable.
 *
 * Revision 1.18  2003/12/05 13:36:58  roebel
 * Added proper initialization for bytesread variable
 *
 * Revision 1.17  2003/11/25 10:55:42  roebel
 * Added missing namespace qualifier for exceptions.
 *
 * Revision 1.16  2003/11/18 18:22:17  roebel
 * Declared exceptions that are used.
 *
 * Revision 1.15  2003/11/18 01:38:22  roebel
 * Fixed internal matrix handling bug that would occure when matrix
 * would have different data types within a Frame.
 *
 * Revision 1.14  2003/07/18 20:44:15  roebel
 * removed remaining default arguments in constructor call.
 *
 * Revision 1.13  2003/07/18 20:42:22  roebel
 * Moved constructor implementations to .cpp, fixed bug in constructor with allocation
 *
 * Revision 1.12  2003/07/17 20:00:49  roebel
 * Added missing Clear after having changed Resize to not clear the data
 *
 * Revision 1.11  2003/07/17 18:09:35  roebel
 * Improved Resize method, added Clear method and proper assigment operator
 *
 * Revision 1.10  2003/07/09 21:06:55  roebel
 * Added support for eUInt4.
 *
 * Revision 1.9  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.8  2003/05/22 17:58:12  roebel
 * Improved checks in GetColName
 *
 * Revision 1.7  2003/05/19 14:00:20  roebel
 * Include new easdif_config.h.
 *
 * Revision 1.6  2003/05/18 20:46:46  roebel
 * Added method to read column names of matrix. For this the current sdiffile is stored in the matrix.
 * Improved documentation.
 * Added support for sdif data types eChar and eInt2.
 *
 * Revision 1.5  2003/05/01 19:01:39  roebel
 * Renamed CreateMatrixData to Init.
 * Reorganized Init functions to use only a single argument to specify the matrix signature.
 * Removed redundant m_Signature from class.
 * Added Resize method.
 *
 * Revision 1.4  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.3  2003/04/29 15:18:04  schwarz
 * CreateMatrixData accepts SdifSignature or std::string,
 * throws exception when unimplemented data type requested.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.8  2003/02/10 14:14:49  roebel
 * Comment changed.
 *
 * Revision 1.7  2002/11/07 21:06:52  roebel
 * Allow value type to be specified during Matrix construction.
 * Do not resize before writing because size will be already correct.
 * Replaced Copy Constructor to use clone method.
 * Moved Get/Set methods to header to have the possibility to use
 * real templates.
 *
 * Revision 1.6  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.5  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.4  2002/07/12 10:20:04  ftissera
 * Read has changed for selection
 *
 * Revision 1.3  2002/06/18 14:48:53  ftissera
 * add GetSignature(), GetStringSignature() and GetType()
 *
 * Revision 1.2  2002/04/11 16:41:55  ftissera
 * add comment for sdifmatrix.cpp
 * 
 * 
 */


#include <iostream>
#include <set>
#include "easdif/easdif_config.hpp"
#include "easdif/sdif_matrix.hpp"

namespace Easdif {

SDIFMatrix::SDIFMatrix(const SdifDataTypeET _type):
  mInter(0)
{

    /* default signature */
    //mSig = SdifSignatureConst('1','T','R','C');
    // signifies uninitialized matrix
    mSig = 0;
    mFile = 0;
    Init(mSig, 1, 1, _type);
}

SDIFMatrix::SDIFMatrix(const SDIFMatrix& aMatrix):mInter(0)
{
    // Check     
    mSig = aMatrix.mSig;
    mType = aMatrix.mType;
    bytesread = aMatrix.bytesread;
    mFile   = aMatrix.mFile;

    mInter =   aMatrix.mInter->clone(); 
}

SDIFMatrix::SDIFMatrix(const std::string& sig, int nrows, int ncols, 
		       SdifDataTypeET type):mInter(0)
{
  Init(sig, nrows, ncols, type);
}

SDIFMatrix::SDIFMatrix(SdifSignature sig, int nrows, int ncols, 
		       SdifDataTypeET type):mInter(0)
{
  Init(sig, nrows, ncols, type);
}


SDIFMatrix & SDIFMatrix::operator=(const SDIFMatrix& aMatrix) {
    
  delete mInter;
  
  mSig = aMatrix.mSig;
  mType = aMatrix.mType;
  bytesread = aMatrix.bytesread;
  mFile   = aMatrix.mFile;
  mInter =   aMatrix.mInter->clone(); 

  return *this;
}


void SDIFMatrix::Init(SdifSignature sig, int nrows, int ncols, SdifDataTypeET type)
{
    if(mInter) {
      if(mType == type) {
	mInter->Resize(nrows,ncols);
	mInter->Clear();
      }
      else{
	delete mInter;
	mInter =0;
      }
    }

    bytesread = 0;
    if (!mInter) {
      switch (type){
      case eInt1:
      case eChar:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<char>(nrows,ncols));
	break;
      case eUInt1:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned char>(nrows,ncols));
	break;
      case eUInt2:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned short int>(nrows,ncols));
	break;
      case eInt2:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<short int>(nrows,ncols));
	break;
      case eInt4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<int>(nrows,ncols));
	break;
      case eUInt4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned int>(nrows,ncols));
	break;
      case eFloat4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<float>(nrows,ncols));
	break;	
      case eFloat8:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<double>(nrows,ncols));
	break;
	
      default:
	throw SDIFMatrixDataError(eError,
			  "Error in  SDIFMatrix::Init!!! unimplemented matrix type used !!!",
			  0,eTypeDataNotSupported,0,0);
      }
      mType = type;
    }
    mSig  = sig;
}


void SDIFMatrix::Init(const std::string &sig, int nrows, int ncols, SdifDataTypeET type){
    Init(SdifStringToSignature(sig.c_str()), nrows, ncols, type);
}



bool SDIFMatrix::Resize(int _rows,int _ncols) {
  if(mInter){
    mInter->Resize(_rows,_ncols);
    return true;
  }
  return false;
}

bool SDIFMatrix::Clear() {
  if(mInter){
    mInter->Clear();
    return true;
  }
  return false;
}

int SDIFMatrix::Write(SdifFileT* file)
{
    int SizeFrameW = 0;
    int nrows=mInter->GetNbRows();
    int ncols=mInter->GetNbCols();

    // SdifDataTypeET  type  = SdifFCurrDataType(file);
   
    // we do not ned to resize
    //    mInter->Resize(nrows, ncols);
    SizeFrameW = 0;

    /* Set with the current matrix header */
    SdifFSetCurrMatrixHeader(file, mSig, mType, nrows, ncols);
    //SdifFSetCurrMatrixHeader(file, mSig, eFloat4, nrows, ncols);
    SizeFrameW += SdifFWriteMatrixHeader(file);
    SizeFrameW += mInter->write(file);

    return SizeFrameW;
}



/* when reading a matrix, this return the count of bytes and create a matrix which keep the values  */
int SDIFMatrix::Read(SdifFileT* file,const std::set<SdifSignature> * hlselection)
{
  // remember file that we read from
  mFile = file;

  int bytesreadheader = SdifFReadMatrixHeader(file);
  bytesread = 0;
  /* for selection */
  
  bool usehl = (hlselection && ! hlselection->empty());
  if ((!usehl && !SdifFCurrMatrixIsSelected (file))
      || (usehl && hlselection->end() == hlselection->find(SdifFCurrMatrixSignature(file))))
    {
      //bytesread += SdifFSkipMatrixData(file);
      SdifFSkipMatrixData(file);
      
      /*
	bytesread += SdifFReadPadding(file,
	SdifFPaddingCalculate
	(file->Stream, bytesread));
      */
      //return bytesread;
      return 0;
    }
  
    mSig = SdifFCurrMatrixSignature(file);
    int	 nrows = SdifFNumRowsSelected(file);
    int	 ncols = SdifFNumColumnsSelected(file);
	
    Init(mSig, nrows, ncols, SdifFCurrDataType (file));
    /* add bytesread */
    bytesread = mInter->read(file)+bytesreadheader;
    return bytesread;
}


/* to see the matrix */
void SDIFMatrix::Print() const
{
   std::cout << " Signature : " << SdifSignatureToString(mSig) << std::endl;
   mInter->print();
}


/* to get the size of matrix in sdif file including padding */
int SDIFMatrix::GetSize() const
{
    return SdifSizeOfMatrix(mType, mInter->GetNbCols(), mInter->GetNbRows());
}

/* to get the SdifDataType of the matrix */
SdifDataTypeET SDIFMatrix::GetType() const
{
    return mType;
}

/* to get the SdifSignature of the matrix */
SdifSignature SDIFMatrix::GetSignature() const
{
    return mSig;
}

/* to get the string signature of the matrix*/
std::string SDIFMatrix::GetStringSignature() const
{
    return std::string(SdifSignatureToString(mSig));
}

/* Get name of column of current matrix */
std::string SDIFMatrix::GetColName(int col) const {

  if(mFile && mFile->MatrixTypesTable) {
   SdifMatrixTypeT *tt= SdifGetMatrixType(mFile->MatrixTypesTable, 
					 mSig);
   if(tt) {
     SdifColumnDefT*tt2 =  SdifMatrixTypeGetNthColumnDef (tt, col);
     if(tt2 && tt2->Name) {
       return std::string(tt2->Name);
     }
   }
  }
  return std::string("");
}

} // end of namespace Easdif
