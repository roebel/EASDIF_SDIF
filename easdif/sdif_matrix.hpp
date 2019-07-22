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
 * @file   sdif_matrix.hpp
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:45:33 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdif_matrix.hpp,v 1.14 2014-09-26 13:41:09 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.13  2014/09/24 15:20:15  roebel
 * Fixed previous fix that did not use the GetInt function properly for strings stored in rows.
 *
 * Revision 1.12  2014/09/24 14:09:19  roebel
 * Fixed reading text stored i a column for text matrices.
 *
 * Revision 1.11  2014/05/23 10:23:46  roebel
 * Added throw specifications.
 *
 * Revision 1.10  2012/09/02 01:10:27  roebel
 * Fixed confusing argument names.
 *
 * Revision 1.9  2012/08/28 22:00:44  roebel
 * Renamed function arguments to avoid compiler warnings in swig python interface.
 *
 * Revision 1.8  2012/01/03 23:59:25  roebel
 * Changed SetRow(std::vector and SetCol(const std::vector into template member functions and add exception if inout vector size
 * does not fit the row or column size.
 *
 * Revision 1.7  2008/06/20 17:07:02  roebel
 * Changed Print methods to be const.
 *
 * Revision 1.6  2008/02/14 12:18:17  roebel
 * Extended the GetRow/GetCol functions ability to read into  std::vector
 * containing arbitrary types.
 *
 * Revision 1.5  2007/11/26 19:10:23  roebel
 * Fixed to avoid compiler warnings in MSVC.
 * Little problem is the export of std::containers that should be defined as export
 * which is not possible due to the given STL include files.
 * For the moment it seems these warnings are not important, because all these functions
 * are inlined as templates in the STL anyway. Has to be handled with care !!!!
 *
 * Revision 1.4  2007/04/30 11:32:29  roebel
 * Matrix reading routine returns bytecount read for selected matrices only.
 *
 * Revision 1.3  2006/11/26 16:38:06  roebel
 * declared GetRow/GetCol  to be void to improve efficiency of
 * swig interface.
 *
 * Revision 1.2  2006/04/22 11:48:09  roebel
 * Fixed left over problems from last renameing operation.
 *
 * Revision 1.1  2006/04/22 08:57:24  roebel
 * Renamed some files to prevent name clash of object files on macosx
 *
 * Revision 1.1  2005/05/30 21:43:00  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.26  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.25  2004/09/08 09:17:45  roebel
 * Fixed error in GetNbCols which would return the number of rows!
 *
 * Revision 1.24  2004/07/28 15:06:23  roebel
 * Added missing GetUChar.
 *
 * Revision 1.23  2004/07/28 14:57:34  roebel
 * Added support for accessing data as unsigned char.
 * Added SetRow and SetCol functions.
 *
 * Revision 1.22  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.21  2004/07/26 14:49:16  roebel
 * Fixed compiler warnings due to implicite casts
 *
 * Revision 1.20  2004/07/21 13:27:22  roebel
 * Added new data accessing functions to read complete columns
 * GetCol(double *,int icol), GetCol(float *,int icol), GetCol(int *,int icol)
 * and rows
 * GetRow(double *,int irow), GetRow(float *,int irow), GetRow(int *,int irow)
 *
 * Revision 1.19  2004/05/04 14:56:56  roebel
 * Reorganized documentation.
 *
 * Revision 1.18  2004/03/19 22:09:50  roebel
 * Version 0.8.3: Added new methods to set marix signatures.
 *
 * Revision 1.17  2003/11/25 10:55:42  roebel
 * Added missing namespace qualifier for exceptions.
 *
 * Revision 1.16  2003/11/18 18:21:21  roebel
 * Impemented Get(std::string) for text matrices.
 * declared exceptions for Init and Get methods.
 *
 * Revision 1.15  2003/07/18 20:42:22  roebel
 * Moved constructor implementations to .cpp, fixed bug in constructor with allocation
 *
 * Revision 1.14  2003/07/18 19:31:13  roebel
 * Improved documentation.
 * Changed constructor to use  const std::string as argument.
 * Fixed warning due to comparison between int and unsigned int.
 *
 * Revision 1.13  2003/07/17 18:09:35  roebel
 * Improved Resize method, added Clear method and proper assigment operator
 *
 * Revision 1.12  2003/07/10 16:14:03  roebel
 * Documentation of constructors.
 *
 * Revision 1.11  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.10  2003/06/09 21:13:36  schwarz
 * Implemented Set(const std::string& str), redoes
 * Init(mSig, str.length(), 1, eText) and sets data.
 * Better: check if matrix was declared eText?
 *
 * Revision 1.9  2003/05/19 14:00:00  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.8  2003/05/18 20:46:46  roebel
 * Added method to read column names of matrix. For this the current sdiffile is stored in the matrix.
 * Improved documentation.
 * Added support for sdif data types eChar and eInt2.
 *
 * Revision 1.7  2003/05/01 19:02:25  roebel
 * Renamed CreateMatrixData to Init.
 * Reorganized Init functions to use only a single argument to specify the matrix signature.
 * Removed redundant m_Signature from class.
 * Added Resize method.
 *
 * Revision 1.6  2003/04/29 15:54:07  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.5  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.4  2003/04/29 15:19:51  schwarz
 * Constructor with space initialisation, default singleton float matrix.
 * CreateMatrixData accepts SdifSignature or std::string.
 * Sketch string matrix access functions to be implemented,
 * throw exception when used.
 *
 * Revision 1.3  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.7  2003/02/10 14:15:03  roebel
 * Comment changed.
 *
 * Revision 1.6  2002/11/07 21:07:24  roebel
 * Allow value type to be specified during Matrix construction.
 * Moved Get/Set methods to header to have the possibility to use
 * real templates.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.3  2002/07/12 10:25:14  ftissera
 * Add comments
 *
 * Revision 1.2  2002/06/18 14:48:30  ftissera
 * add GetSignature(), GetStringSignature() and GetType()
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIX_H_
#define SDIFMATRIX_H_ 1



#include <string>
#include <set>
#include "sdif.h"
#include "easdif_exports.hpp"
#include "easdif/sdifexception.hpp"
#include "easdif/sdifmatrixdatainterface.hpp"
#include "easdif/sdifmatrixdata.hpp"


namespace Easdif {

  /** 
   * @brief class which can be associated with a matrix
   *
   * SDIFMatrix is composed of different methods which permits to manipulate a
   * matrix.
   */
  class EASDIF_API SDIFMatrix
  {
  private:
    SDIFMatrixDataInterface* mInter;
  
    int bytesread;
    SdifSignature mSig;
    SdifDataTypeET mType;
    // file matrix was read from
    SdifFileT * mFile;

  public:
    /**
     * \defgroup create SDIFMatrix - Construction
     */

    /** 
     * \ingroup create
     * \brief default constructor
     * 
     * Constructs matrix without allocating memory. However, the type
     * for internal data representation is fixed.
     *
     * @param _type data type for internal representation
     *  defaults to: eFloat <br>
     *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
     *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
     *  are not yet
     *  in the sdif standard and if you use them only the IRCAM sdif library will
     *  be able to read your data.
     */
    SDIFMatrix(const SdifDataTypeET _type=eFloat4);


    /** 
     * \ingroup create
     * \brief assignment operator
     *
     * does a deep copy of the data
     * 
     *\param aMatrix matrix to copy
     *
     */
    SDIFMatrix & operator=(const SDIFMatrix& aMatrix);


    /** 
     * \ingroup create
     * \brief copy constrctor
     *
     * does a deep copy of the data
     * 
     *\param aMatrix
     *
     */
    SDIFMatrix(const SDIFMatrix& aMatrix);


    /** 
     * \ingroup create
     * \brief constructor that allocates internal memory
     * 
     * Constructs matrix with given signature
     * to hold given number of rows and columns. The type
     * for internal data representation is fixed.
     *
     * \param sig     Matrix signature
     * \param nrows   number of rows allocated
     * \param ncols   number of cols allocated
     * \param type data type for internal representation
     *  defaults to: eFloat <br>
     *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
     *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
     *  are not yet
     *  in the sdif standard and if you use them only the IRCAM sdif library will
     *  be able to read your data.
     */
    SDIFMatrix(SdifSignature sig, int nrows = 1, int ncols = 1, 
               SdifDataTypeET type = eFloat4);

    /** 
     * \ingroup create
     * \brief constructor that allocates internal memory
     * 
     * Constructs matrix with given signature
     * to hold given number of rows and columns. The type
     * for internal data representation is fixed.
     *
     * \param sig     4 element string specifying Matrix signature
     * \param nrows   number of rows allocated
     * \param ncols   number of cols allocated
     * \param type data type for internal representation
     *  defaults to: eFloat <br>
     *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
     *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
     *  are not yet
     *  in the sdif standard and if you use them only the IRCAM sdif library will
     *  be able to read your data.
     */
    SDIFMatrix(const std::string& sig, int nrows = 1, int ncols = 1, 
               SdifDataTypeET type = eFloat4);

    ~SDIFMatrix(){
      if(mInter) {
        delete mInter;
        mInter =0;
      }
    };



    /*************************************************************************/
    /* To see a SDIFMatrix */
    /**
     * \defgroup print SDIFMatrix - Print
     */
  
    /** 
     * \ingroup print
     * print matrix values
     */
    void Print() const;

    /*************************************************************************/
    /* Read and write a SDIFMatrix */
    /**
     * \defgroup rwmat SDIFMatrix - Read and write and create a SDIFMatrix
     */
  
    /** 
     * \ingroup rwmat
     * write the matrix in the file
     * @return the size in bytes of the matrix writing
     */
    int Write(SdifFileT* file);

    /**
     * \ingroup rwmat 
     * \brief read a matrix
     * filter only matrices that match the set given in hlsel
     * \param file sdiffeil 
     * \param hlsel pointer std::set<SsdifSignature> containing the signatures 
     *          that should be read, this is the signature restriction on Easdif level
     *          that filters the existing sdif selection (for efficient implementation
     *          it is assumed that hlsel contains a subset of the sdif file selection
     *          which is not used if hlsel != 0 && !hlsel->empty() )
     *
     * @return the complete count of bytes for the selected matrices including header
     * 
     */
    int Read(SdifFileT* file,const std::set<SdifSignature> *hlsel=0);

    /** 
     * \ingroup create
     * \brief Re-Initialize matrix to hold data
     * 
     * @param sig    SDIFMatrix Signature
     * @param nrows  Number of rows fo the matrix
     * @param ncols  Number of columns of the matrix
     * @param type   SDIFDataType = type of internal representation of the matrix
     */
    void Init(SdifSignature sig, 
	      int nrows, int ncols, SdifDataTypeET  type);

    /** 
     * \ingroup create
     * \brief Re-Initialize matrix to hold data
     * 
     * @param sig    SDIFMatrix Signature
     * @param nrows  Number of rows fo the matrix
     * @param ncols  Number of columns of the matrix
     * @param type   SDIFDataType = type of internal representation of the matrix
     */
    void Init(const std::string &sig, 
	      int nrows, int ncols, SdifDataTypeET  type);

    /** 
     * \ingroup create
     * \brief Resize matrix to hold rowsxcolumns
     * 
     *  Existing data is preserved in the correct locations
     *  newly initilized data is set to zero
     *
     * @param nrows  Number of rows fo the matrix
     * @param ncols  Number of columns of the matrix
     *
     * \return true if successful/false if matrix has not yet been initialized to a signature/data type
     *       
     */
    bool Resize(int nrows, int ncols);

    /** 
     * \ingroup create
     * \brief clear matrix 
     *
     *  all data entries are set to zero.
     * \return true if successful/false if matrix has not yet been initialized to a signature/data type
     *       
     */
    bool  Clear();

    /*************************************************************************/
    /* Get the members of the matrix */
    /**
     * \defgroup membmat SDIFMatrix - Get members of the SDIFMatrix 
     */

    /** 
     * \ingroup membmat
     * get the size of matrix occupied in SdifFile (including padding)!!
     */
    int GetSize() const;

    /**
     * \ingroup membmat 
     * get the number of rows of the matrix
     */
    int GetNbRows() const {return mInter->GetNbRows();};

    /** 
     * \ingroup membmat
     * get the number of columns of the matrix
     */
    int GetNbCols() const {return mInter->GetNbCols();};

    /**
     * \ingroup membmat 
     * get the matrix SdifSignature
     */
    SdifSignature GetSignature() const;

    /** 
     * \ingroup membmat
     * get the matrix string Signature
     */
    std::string GetStringSignature() const;

    /** 
     * \ingroup membmat
     * get the matrix type of data
     */
    SdifDataTypeET GetType() const;

    /** 
     * \ingroup membmat
     * get name of column or empty string if unknown
     */
    std::string GetColName(int i) const;

    /** 
     * \ingroup membmat
     * get a void pointer to the start of the memory of the internal matrix
     * @return void * to internal data
     */
    void* GetData() {
      return mInter->GetData();
    }

    /** 
     * \ingroup membmat
     * @return element size in bytes of internal representation
     */
    int GetElementSize()	{
      return mInter->GetElementSize();
    }


    /** 
     * \ingroup membmat
     * @brief Set one element of the matrix header : the signature
     */
    void SetSignature(SdifSignature sig) {  mSig = sig;}

    /** 
     * \ingroup membmat
     * @brief Set one element of the matrix header : the signature with a string
     */
    void SetSignature(const std::string& signature){
      mSig = SdifStringToSignature(const_cast<char*>(signature.c_str()));
    }




    /*************************************************************************/
    /* Get the values of the matrix */
    /**
     * \defgroup valmat SDIFMatrix - Get values of the SDIFMatrix 
     */

    /**
     * \ingroup valmat 
     * get a value in : unsigned char
     * @param i row index
     * @param j column index
     * 
     * @return the value
     */
    int GetUChar(int i, int j) const{
      return mInter->GetUChar(i, j);};

    /**
     * \ingroup valmat 
     * get a value in : int
     * @param i row index
     * @param j column index
     * 
     * @return the value
     */
    int GetInt(int i, int j) const {
      return mInter->GetInt(i, j);};


    /**
     * \ingroup valmat  
     * get a value in : float
     * @param i row index
     * @param j column index
     * 
     * @return the value
     */
    float GetFloat(int i, int j)const{ 
      return mInter->GetFloat(i, j);}

    /**
     * \ingroup valmat  
     * get the value in double
     * 
     * @param i row index
     * @param j column index
     * 
     * @return the value
     */
    double GetDouble(int i, int j) const {
      return mInter->GetDouble(i, j);}


    /** 
     * \ingroup valmat 
     * \brief get a value in form of given type
     * 
     * @param i row    index
     * @param j column index
     * @param value reference of variable to store value in 
     * 
     */
    template<typename Tout>
    void Get(int i, int j, Tout& value) const
    {
      value = static_cast<Tout>(mInter->GetDouble(i, j));
    }

    // specialization for float that does not use cast
    void Get(int i, int j, float& value) const 
    {
      value = mInter->GetFloat(i, j);
    }
  
    // specialization for int that does not use cast
    void Get(int i, int j, int& value)  const 
    {
      value = mInter->GetInt(i, j);
    }



    // std::string Get() ??? exception when not string matrix?
    void Get(std::string& value)  const 
    {

      if (mType != eText)
        throw SDIFMatrixDataError(eError,
                                  "Error in  SDIFMatrix::!!! string matrix access to matrix containing binary data !!!",
                                  0,eUnknown,0,0); 

      // garcefully handle errors where text is stored as a row and not as a column.
      if(GetNbCols() == 1) 
        for(int ii=0;ii<GetNbRows();++ii)
          value.append(1,static_cast<char>(GetInt(ii, 0)));
      else if(GetNbRows()  == 1)
        for(int ii=0;ii<GetNbCols();++ii)
          value.append(1,static_cast<char>(GetInt(0, ii)));
    }


    /**
     * \ingroup valmat
     * getting an entire row 
     * 
     * @param out  pointer to memory holding at least GetNbCols() elements
     * @param irow row index
     * 
     */

    template <class TT>
    void GetRow(TT* out,int irow) const  {
      mInter->GetRow(out,irow);
      return;
    }

    /**
     * \ingroup valmat
     * getting an entire row
     * 
     * @param out std::vector will be resized to hold the row
     * @param irow row index
     * 
     */
    template<class VEC_REAL>
    void
    GetRow(std::vector<VEC_REAL> &out,int irow) const  {
      out.resize(GetNbCols());
      if(!out.empty())
        mInter->GetRow(&(out[0]),irow);
    }

    /**
     * \ingroup valmat
     * getting an entire column
     * 
     * @param out  pointer to memory holding at least GetNbRows() elements
     * @param icol row index
     * 
     */
    template <class TT>
    void GetCol(TT* out,int icol) const {
      mInter->GetCol(out,icol);
      return;
    }

    /**
     * \ingroup valmat
     * getting an entire column
     * 
     * @param out std::vector will be resized to hold the column
     * @param icol column index
     * 
     */
    template<class VEC_REAL>
    void
    GetCol(std::vector<VEC_REAL> &out,int icol) const  {
      out.resize(GetNbRows());
      if(!out.empty())
        mInter->GetCol(&(out[0]),icol);
    }

    /*************************************************************************
     * Set the values of the matrix  
     */

    /**
     * \defgroup setmat SDIFMatrix - Set values of the SDIFMatrix 
     */

    /** 
     * \ingroup setmat
     * \brief set a value using arbitrary input type
     * 
     * @param i    row    index (C-notation: first row has index 0)
     * @param j    column index (C-notation: first col has index 0)
     * @param value 
     * 
     */
    template<typename Tin>
    void Set(int i, int j, const Tin& value) 
    {
      mInter->Set(i, j, static_cast<double>(value) );
    }
  
    void Set(int i, int j, const float value)
    {
      mInter->Set(i, j, value);
    }
  
    void Set(int i, int j, const int value)
    {
      mInter->Set(i, j, value);
    }

    void Set(int i, int j, const unsigned char value)
    {
      mInter->Set(i, j, value);
    }


    /** 
     * Set matrix type to eText, change matrix size to num. of bytes in
     * string and set string data
     */
    void Set(const std::string& str)
    {
      int i;

      Init(mSig, static_cast<int>(str.length()), 1, eText);

      for (i = 0; i < static_cast<int>(str.length()); i++)
        mInter->Set(i, 0, static_cast<int>(str[i]));
    }



    /**
     * \ingroup setmat
     * setting an entire row 
     * 
     * @param out  pointer to memory holding at least GetNbCols() elements
     * @param irow row index
     * 
     */

    template <class TT>
    void SetRow(const TT* in_arr,int irow) {
      mInter->SetRow(in_arr,irow);
      return;
    }

    /**
     * \ingroup valmat
     * setting an entire row
     * 
     * @param in std::vector containing row
     * @param irow row index
     * 
     */
    template <class VECVAL_T>
    void
    SetRow(const std::vector<VECVAL_T> &inVec,int irow){
      if(static_cast<int>(inVec.size()) != GetNbCols())
  	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrix::SetRow!!! vector size of input vector does not match number of columns !!!",
				0, eArrayPosition,__FILE__,__LINE__);

      mInter->SetRow(&(inVec[0]),irow);
      return;
    }

    /**
     * \ingroup setmat
     * setting an entire column
     * 
     * @param out  pointer to memory holding at least GetNbRows() elements
     * @param icol row index
     * 
     */
    template <class TT>
    void SetCol(const TT* in_arr,int icol){
      mInter->SetCol(in_arr,icol);
      return;
    }
  
    /**
     * \ingroup valmat
     * setting an entire column
     * 
     * @param in std::vector holding the column
     * @param icol col index
     * 
     */
    template <class VECVAL_T>
    void
    SetCol(const std::vector<VECVAL_T> &inVec, int icol)  {
      if(static_cast<int>(inVec.size()) != GetNbRows()){
  	throw SDIFArrayPosition(eError,
				"Error in  SDIFMatrix::SetCol:: vector size of input vector does not match number of rows !!!",
				0, eArrayPosition,__FILE__,__LINE__);
      }
      mInter->SetCol(&(inVec[0]),icol);
    }
  };
} // end of namespace Easdif

#endif
