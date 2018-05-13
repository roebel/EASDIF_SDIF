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
 * @file   sdifmatrixdata.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:34:25 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrixdata.hpp,v 1.5 2014-05-23 10:30:25 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2011/06/11 17:08:16  roebel
 * Moved constructor and destructor definition out of class scope to
 * lower probability of inlining.
 * Changed handling of empty matrices to avoid assertion failures
 * in MSVC.
 *
 * Revision 1.3  2008/01/22 00:51:28  roebel
 * Completed support for all sdif integer types.
 *
 * Revision 1.2  2007/11/27 16:41:19  roebel
 * Prevent dereferencing vectors outside the range.
 *
 * Revision 1.1  2005/05/30 21:43:00  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.17  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.16  2005/05/03 16:23:41  roebel
 * Fixed documentation.
 *
 * Revision 1.15  2004/11/18 18:05:29  ellis
 * .natural comparator function for Esdif Frames
 * .bug fix in _SetRow
 *
 * Revision 1.14  2004/09/08 09:17:10  roebel
 * Fixed misleading error message.
 *
 * Revision 1.13  2004/07/29 13:44:44  roebel
 * Fixed SetCol to use correct element offset as well.
 *
 * Revision 1.12  2004/07/29 10:23:36  bogaards
 * GetCol uses col increment instead of row increment
 *
 * Revision 1.11  2004/07/28 14:57:34  roebel
 * Added support for accessing data as unsigned char.
 * Added SetRow and SetCol functions.
 *
 * Revision 1.10  2004/07/27 17:37:54  roebel
 * Changed include directive to use user path and not system path for sdif.h
 * Properly cast return values in read and write method.
 *
 * Revision 1.9  2004/07/26 14:49:16  roebel
 * Fixed compiler warnings due to implicite casts
 *
 * Revision 1.8  2004/07/21 13:27:12  roebel
 * Added new data accessing functions to read complete columns
 * GetCol(double *,int icol), GetCol(float *,int icol), GetCol(int *,int icol)
 * and rows
 * GetRow(double *,int irow), GetRow(float *,int irow), GetRow(int *,int irow)
 *
 * Revision 1.7  2004/07/20 19:32:36  roebel
 * Added support for row and column selection.
 * Matrix reading/writing reorganized to handle complete matrices whenever
 * possible (if no selection on row or column is used).
 * Drastically improves IO performance!
 *
 * Revision 1.6  2003/07/17 18:09:35  roebel
 * Improved Resize method, added Clear method and proper assigment operator
 *
 * Revision 1.5  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.4  2003/05/18 23:14:10  roebel
 * Improved error message should be an exception
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
 * Revision 1.6  2002/11/07 21:09:33  roebel
 * Provide clone method.
 * Fixed bounds checking in Get template.
 * Reorganized CopyData.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.3  2002/07/12 10:27:34  ftissera
 * *** empty log message ***
 *
 * Revision 1.2  2002/06/18 14:47:45  ftissera
 * add comments
 * 
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATA_H_
#define SDIFMATRIXDATA_H_ 1

#include <vector>
#include "sdif.h"
#include "easdif/sdifmatrixdatainterface.hpp"


size_t SdiffReadChar   (SdifChar   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadInt1   (SdifInt1   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadInt2   (SdifInt2   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadUInt1  (SdifUInt1  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadUInt2  (SdifUInt2  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadInt4   (SdifInt4   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadUInt4  (SdifUInt4  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadFloat4 (SdifFloat4 *ptr, size_t nobj, SdifFileT *file);
size_t SdiffReadFloat8 (SdifFloat8 *ptr, size_t nobj, SdifFileT *file);

/* Write, return the number of objects */
size_t SdiffWriteChar   (SdifChar   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteInt1   (SdifInt1   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteInt2   (SdifInt2   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteUInt1  (SdifUInt1  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteUInt2  (SdifUInt2  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteInt4   (SdifInt4   *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteUInt4  (SdifUInt4  *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteFloat4 (SdifFloat4 *ptr, size_t nobj, SdifFileT *file);
size_t SdiffWriteFloat8 (SdifFloat8 *ptr, size_t nobj, SdifFileT *file);



namespace {

  // collection of functions that allow to read  a whole matrix in a single pass
  inline size_t SdiffReadMatrix(double *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadFloat8(outbuf,nobjs,stream)*sizeof(double);
  }
  inline size_t SdiffReadMatrix(float *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadFloat4(outbuf,nobjs,stream)*sizeof(float);
  }
  inline size_t SdiffReadMatrix(SdifUInt1 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadUInt1(outbuf,nobjs,stream)*sizeof(unsigned char);
  }
  inline size_t SdiffReadMatrix(SdifUInt2 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadUInt2(outbuf,nobjs,stream)*sizeof(unsigned short int);
  }
  inline size_t SdiffReadMatrix(SdifUInt4 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadUInt4(outbuf,nobjs,stream)*sizeof(unsigned int);
  }
  inline size_t SdiffReadMatrix(char *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadInt1(outbuf,nobjs,stream)*sizeof(char);
  }
  inline size_t SdiffReadMatrix(SdifInt2 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadInt2(outbuf,nobjs,stream)*sizeof(short int);
  }
  inline size_t SdiffReadMatrix(SdifInt4 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffReadInt4(outbuf,nobjs,stream)*sizeof(int);
  }

  // collection of functions that allow to  write a whole matrix in a single pass
  inline size_t SdiffWriteMatrix(double *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteFloat8(outbuf,nobjs,stream)*sizeof(double);
  }
  inline size_t SdiffWriteMatrix(float *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteFloat4(outbuf,nobjs,stream)*sizeof(float);
  }
  inline size_t SdiffWriteMatrix(SdifUInt1 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteUInt1(outbuf,nobjs,stream)*sizeof(unsigned char);
  }
  inline size_t SdiffWriteMatrix(SdifUInt2 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteUInt2(outbuf,nobjs,stream)*sizeof(unsigned short int);
  }
  inline size_t SdiffWriteMatrix(SdifUInt4 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteUInt4(outbuf,nobjs,stream)*sizeof(unsigned int);
  }
  inline size_t SdiffWriteMatrix(SdifInt1 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteInt1(outbuf,nobjs,stream)*sizeof(char);
  }
  inline size_t SdiffWriteMatrix(SdifInt2 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteInt2(outbuf,nobjs,stream)*sizeof(short int);
  }
  inline size_t SdiffWriteMatrix(SdifInt4 *outbuf,size_t nobjs, SdifFileT * stream) {
    return     SdiffWriteInt4(outbuf,nobjs,stream)*sizeof(int);
  }
 

}
namespace Easdif {

/** 
 * @brief class which can store the value in template type
 *  class for the data storage and the operation on data
 */
template <class T>
class SDIFMatrixData : public SDIFMatrixDataInterface//SdifMatrixDataInterface
{

private:

    int m_Nrows;
    int m_Ncols;

/** 
 * vector which store the data in natural type
 */
    std::vector<T> m_Data;


public:
  SDIFMatrixData(int nrows, int ncols);
  ~SDIFMatrixData();


/*************************************************************************/
/* Get data */
/**
* \defgroup getdata SDIFMatrixData - Get data 
*/


  /** 
   *  template method for getting data: only for internal use
   */
  template <class TT>
  TT Get(int i, int j) const
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrixData::Get!!! requested position is out of range !!!",
				0,eArrayPosition,__FILE__,__LINE__);
      }
    return static_cast<TT>(m_Data[i*m_Ncols+j]);
  }


  /**
   * \ingroup getdata
   * getting an entire row 
   * 
   * @param out  pointer to memory holding at least GetNbCols() elements
   * @param irow row index [0,m_Nrows[
   * 
   */

  template <class TT>
  void _GetRow(TT* out,int irow) const throw (SDIFArrayPosition) 
  {
    if (irow<0 || irow >= m_Nrows || m_Data.empty())      {
      throw SDIFArrayPosition(eError,
			      "Error in  SDIFMatrixData::GetRow!!! requested row is out of range !!!",
			  0,eArrayPosition,__FILE__,__LINE__);

    }
    
    const T* start = &m_Data[0]+irow*m_Ncols;
    const T* end   = &m_Data[0]+(irow+1)*m_Ncols;
    while(start !=end)    *out++ = static_cast<TT>(*start++);

    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  template <class TT>
  void _GetCol(TT* out,int icol) const throw (SDIFArrayPosition) 
  {
    if (icol<0 || icol >= m_Ncols || m_Data.empty())      {
      throw SDIFArrayPosition(eError,
			      "Error in  SDIFMatrixData::GetCol!!! requested column is out of range !!!",
			  0,eArrayPosition,__FILE__,__LINE__);
    }

    const T* start = &m_Data[icol];
    const T* end   = start+m_Nrows*m_Ncols;
    for(;start !=end;start += m_Ncols)    *out++ = static_cast<TT>(*start);

    return;
  }


  /**
   * \ingroup getdata
   * getting an entire column as double
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(double* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column as float
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(float* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column as 32-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifInt4* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column as 32-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifUInt4* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column as 16-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifInt2* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }
  /**
   * \ingroup getdata
   * getting an entire column as 16-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifUInt2* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire column as char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifInt1* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }
  /**
   * \ingroup getdata
   * getting an entire column as unsigned char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void GetCol(SdifUInt1* out,int icol) const throw (SDIFArrayPosition){
    _GetCol(out,icol);
    return;
  }


  /**
   * \ingroup getdata
   * getting an entire row as double
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(double* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as float
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(float* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as 32-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifInt4 * out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as 32-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifUInt4* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as 16-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifInt2* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }
  /**
   * \ingroup getdata
   * getting an entire row as 16-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifUInt2* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as 8-bit int (char)
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifInt1* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }

  /**
   * \ingroup getdata
   * getting an entire row as 8-bit unsigned int (unsigned char)
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void GetRow(SdifUInt1* out,int irow) const throw (SDIFArrayPosition){
    _GetRow(out,irow);
    return;
  }



/**
 * \ingroup getdata
 * get the value as double
 * 
 * @param i row index [0,m_Nrows[
 * @param j column index [0,m_Ncols[
 * 
 * @return the value
 */
    double GetDouble(int i, int j) const
	{
	    return Get<double>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the value as float
 * 
 * @param i row index [0,m_Nrows[
 * @param j column index [0,m_Ncols[
 * 
 * @return the value
 */
    float GetFloat(int i, int j) const
	{
	    return Get<float>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the value as int
 * 
 * @param i row index [0,m_Nrows[
 * @param j column index [0,m_Ncols[
 * 
 * @return the value
 */
    int GetInt(int i, int j) const
	{	    
	    return Get<int>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the value as unsigned char
 * 
 * @param i row index [0,m_Nrows[
 * @param j column index [0,m_Ncols[
 * 
 * @return the value
 */
    unsigned char GetUChar(int i, int j) const
	{	    
	    return Get<unsigned char>(i, j);   
	}


/** 
 * \ingroup getdata
 * get a void pointer to the start of the memory of the internal matrix
 * @return void * to internal storage
 */
     void* GetData()
	{
          if (m_Data.size() )
	    return &m_Data[0];
          return 0;
	}
     int GetElementSize()
	{
          return sizeof(T);
	}


/*************************************************************************/
/* Get members*/
/**
* \defgroup getmemb SDIFMatrixData - Get members
*/
/** 
 * \ingroup getmemb
 * @brief get the row number of values
 */
   int GetNbRows() const
	{
          return m_Nrows;
	}

/** 
 * \ingroup getmemb
 * @brief get the column number of values
 */
    int GetNbCols() const
	{
	    return m_Ncols;
	}

/*************************************************************************/
/* Read and Write data */
/**
* \defgroup rnwdata SDIFMatrixData - Read and Write data 
*/

/** 
 * \ingroup rnwdata
 * read the data in the file and store in a vector of data in template type
 * 
 * @param file 
 */
  int read(SdifFileT* file)
  {
    int _rows = SdifFCurrNbRow(file);
    int _cols = SdifFCurrNbCol(file);
    size_t bytesread = 0;
    /*Read matrix data*/

    if(  m_Nrows == _rows && m_Ncols == _cols && !m_Data.empty() ) {
      bytesread += SdiffReadMatrix(&m_Data[0],_rows*_cols,file);
    }
    else{
      for (int row = 0,outrow=0; row < _rows; row++)
	{ 
	  if (SdifFRowIsSelected(file, row + 1)){
	    bytesread += SdifFReadOneRow(file);
	    T* p= static_cast<T*>(SdifFCurrOneRowData(file));
	    for (int col = 0, outcol = 0; col < _cols; col++)
	      if (SdifFColumnIsSelected(file, col + 1))
		{
		  m_Data[outrow*m_Ncols+outcol] =*(p+col);
		  outcol++;
		}
	    outrow++;
	  }		  
	  else{
	    bytesread +=SdifFSkipOneRow(file);
	  }
	}
    }
    /*Read matrix padding*/
    bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
    return static_cast<int>(bytesread); 
  }

/**
 * \ingroup rnwdata 
 * take the SDIFMatrixData values in the vector and write them in the
 * file
 * 
 * @param file 
 */
    int write(SdifFileT* file)
	{
	    size_t SizeFrameW = 0;
	    /* Write matrix data */
            if (!m_Data.empty())
              SizeFrameW += SdiffWriteMatrix(&m_Data[0],m_Nrows*m_Ncols,file);

	    /* Write matrix padding */
	    SizeFrameW += SdifFWritePadding(file, SdifFPaddingCalculate (file->Stream, SizeFrameW));

	    return static_cast<int>(SizeFrameW);
	}


/*************************************************************************/
/* Other */
/**
* \defgroup otherdata SDIFMatrixData - Other
*/


/** 
 * \ingroup otherdata
 * used for printing the data 
 */
  void print() const
	{
	    int row;
            int col;
	    for (row = 0; row < m_Nrows; row++)
	    {
	        for (col = 0; col < m_Ncols; col++)
		{		    
		    std::cout <<m_Data[row*m_Ncols+col] << " ";
		}
		std::cout << std::endl;
	    }

	}


  /**
   * \brief clone matrix data
   *
   */
  SDIFMatrixData *clone() {
    SDIFMatrixData<T> *tmp = 
      new SDIFMatrixData<T>(this->m_Nrows,this->m_Ncols);
    tmp->CopyData(this->m_Data);    
    return tmp;
  }


  void CopyData(std::vector<T>& data)
  {
    m_Data.resize(data.size());
    std::copy(data.begin(), data.end(), m_Data.begin());
  }

  /** 
   * \ingroup otherdata
   * \brief clear matrix 
   *
   *  all data entries are set to zero.
   */
  void Clear() {
    std::fill(m_Data.begin(),m_Data.end(),T(0));
  }

  /** 
   * \ingroup otherdata
   * \brief resize the matrix 
   *
   *  Existing data is preserved in the correct locations
   *  newly initilized data is set to zero
   * 
   * @param nrows 
   * @param ncols 
   */
  void Resize(int nrows, int ncols)
  {

    if(nrows != m_Nrows || ncols != m_Ncols){
      int i,j;
      
      // Resize sufficiently
      if(static_cast<size_t>(nrows*ncols) > m_Data.size()) {
	m_Data.resize(nrows*ncols);
      }
      
      // reorganize data
      if (m_Ncols < ncols) {
	// start from the end
	// fill all new fields with zero
	for (i = nrows-1; 
	     i >= m_Nrows ; --i){
	  for (j = ncols-1; j >= 0; --j)
	    m_Data[i * ncols + j] = T(0);
	}
	
	for (; i >= 0; --i){
	  for (j = ncols-1; j >=m_Ncols ; --j)
	    m_Data[i * ncols + j] = T(0);
	  
	  for (; j >= 0; --j)
	    m_Data[i * ncols + j] = m_Data[i * m_Ncols + j];
	}
      }
      else{
	// Start from the beginning
	// The first row will need no change
	int prows = m_Nrows;
	if (m_Nrows > nrows) prows = nrows;
	
	for (i = 1; i < prows; ++i){
	  for (j=0; j <ncols; ++j)
	    m_Data[i * ncols + j] = m_Data[i * m_Ncols + j];
	}
	for (; i < nrows; ++i){
	  for (j=0; j <ncols; ++j)
	    m_Data[i * ncols + j] = T(0);	    
	}
      }
      
      // If size has been shrinked --> adjust properly
      if(static_cast<size_t>(ncols*nrows) < m_Data.size()) {
	m_Data.resize(ncols*nrows);
      }
      
      
      m_Ncols   = ncols;
      m_Nrows   = nrows;
    }

    return;
  }



/*************************************************************************/
/* Set the data */
/**
* \defgroup setdata SDIFMatrixData - Set data 
*/

/** 
 * \ingroup setdata
 * set a value in the matrix using double input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, double value)
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrixData::Set!!! requested pos is out of range !!!",
				0,eArrayPosition,__FILE__,__LINE__);
      }
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }

/** 
 * \ingroup setdata
 * set a value in the matrix using float input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, float value)
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrixData::Set!!! requested pos is out of range !!!",
				0,eArrayPosition,__FILE__,__LINE__);
      }
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }

/** 
 * \ingroup setdata
 * set a value in the matrix using int input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, int value)
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrixData::Set!!! requested pos is out of range !!!",
				0,eArrayPosition,__FILE__,__LINE__);
      }
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }


/** 
 * \ingroup setdata
 * set a value in the matrix using unsigned char input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, unsigned char value)
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrixData::Set!!! requested pos is out of range !!!",
				0,eArrayPosition,__FILE__,__LINE__);
      }
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }


  /**
   * \ingroup setdata
   * \brief setting an entire row 
   * 
   * @param in  pointer to memory holding at least  GetNbCols() elements
   * @param irow row index [0,m_Nrows[
   * 
   */

  template <class TT>
  void _SetRow(const TT* in,int irow)  throw (SDIFArrayPosition) 
  {
    if (irow<0 || irow >= m_Nrows || m_Data.empty())      {
      throw SDIFArrayPosition(eError,
			      "Error in  SDIFMatrixData::SetRow!!! requested row is out of range !!!",
			  0,eArrayPosition,__FILE__,__LINE__);

    }
    
    T* start = &m_Data[0]+irow*m_Ncols;
    T* end   = &m_Data[0]+(irow+1)*m_Ncols;
    while(start !=end)    *start++ =  static_cast<T>(*in++) ;

    return;
  }

  /**
   * \ingroup setdata
   * \brief setting an entire column
   * 
   * @param in  pointer to memory holding at least  GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  template <class TT>
  void _SetCol(const TT* in,int icol)  throw (SDIFArrayPosition) 
  {
    if (icol<0 || icol >= m_Ncols || m_Data.empty())      {
      throw SDIFArrayPosition(eError,
			      "Error in  SDIFMatrixData::SetCol!!! requested column is out of range !!!",
			  0,eArrayPosition,__FILE__,__LINE__);
    }

    T* start = &m_Data[icol];
    T* end   = &m_Data[icol]+m_Nrows*m_Ncols;
    for(;start !=end;start += m_Ncols)   *start =  static_cast<T>(*in++) ;

    return;
  }


  /**
   * \ingroup setdata
   * \brief setting an entire column as double
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const double* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire column as float
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const float* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire column as 32-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifInt4* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire column as 32-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifUInt4* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire column as 16-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifInt2* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire column as 16-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifUInt2* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire column as  char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifInt1* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire column as unsigned char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param icol column index [0,m_Ncols[
   * 
   */
  void SetCol(const SdifUInt1* out,int icol)  throw (SDIFArrayPosition){
    _SetCol(out,icol);
    return;
  }


  /**
   * \ingroup setdata
   * setting an entire row as double
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const double* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire row as float
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const float* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire row as 32-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifInt4* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire row as 32-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifUInt4* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire row as 16-bit int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifInt2* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire row as 16-bit unsigned int
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifUInt2* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }

  /**
   * \ingroup setdata
   * setting an entire row as unsigned char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifInt1* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }
  /**
   * \ingroup setdata
   * setting an entire row as unsigned char
   * 
   * @param out  pointer to memory holding at least GetNbRows() elements
   * @param irow row index [0,m_Nrows[
   * 
   */
  void SetRow(const SdifUInt1* out,int irow)  throw (SDIFArrayPosition){
    _SetRow(out,irow);
    return;
  }

};


  template <class T>
  SDIFMatrixData<T>::SDIFMatrixData(int nrows, int ncols)	{
    if (nrows<0|| ncols<0 )      {
      m_Nrows=0;
      m_Ncols=0;
      m_Data.resize(0);
    }
    m_Nrows = nrows;
    m_Ncols = ncols;
    m_Data.resize(m_Nrows*m_Ncols);
  };
  
  template <class T>
  SDIFMatrixData<T>::~SDIFMatrixData(){};

} // end of namespace Easdif

#endif
