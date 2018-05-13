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


#include "easdif/easdif.h"

using namespace Easdif;

int main(int argc, char** argv)
{  

    /* initialise the SDIF library and install exceptions*/
    EasdifInit();

    SDIFEntity readentity;
    SDIFEntity entity;
   
    SDIFFrame frame;
    SDIFFrame frameTowrite;


    if(argc <3){
      std::cerr << "usage: easdifRmNVT filin fileout \n";
      std::cerr << "copy content but remove all NVTs \n";
      exit(1);
    }

    /*for adding the Name Value Tables of the EntityRead, the file of
     *EntityToRead must be Open*/
    try {
      if (!readentity.OpenRead(argv[1]) ) {
	std::cerr << "Could not open input file :"<<argv[1]<<std::endl;
	exit(1);
      }
    }
    catch(SDIFBadHeader& e)
      {
	e.ErrorMessage();
	exit(1);
      }

    std::string type ( readentity.GetTypeString());
    entity.SetTypeString(type);
    /* to open a file for writing */
    
    if(!entity.OpenWrite(argv[2])) {
	std::cerr << "Could not open output file :"<<argv[2]<<std::endl;
	exit(1);
    }


    /******** READ AND WRITE  ********/
    /* to show the exception : we break the loop with an exception */
    try{
      while (1)	{

	/* reading the next frame of the EntityRead, return the number of
	 * bytes read, return 0 if the frame is not selected and -1 if this is 
	 * the last frame of the file */
	if(!readentity.ReadNextFrame(frame))	    
	  continue;
	
	/* for adding new matrix in EntityToWrite */
	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	  {
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	    
	    /*add a matrix in the matrix vector of the frame*/
	    frameTowrite.AddMatrix(tmpMatrix);
	    /*an other method to add just the frame selected in file:
	     * frameTowrite.AddMatrixSelected(readentity.GetFile(),
	     * tmpMatrix);*/
	    
	    /* if you want to access to the data : an example, if we want 
	     * to multiply with 2 the last column of a matrix : */
	    double dou;
	    int ncols = tmpMatrix.GetNbCols();
	    for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
	      {
		//dou = tmpMatrix.GetDouble(i, ncols-1);
		/* to get the value in the double dou*/
		tmpMatrix.Get(i, ncols-1,dou);
		/* an other method can be :
		   dou = tmpMatrix.GetDouble(i, ncols-1);
		*/
		tmpMatrix.Set(i, ncols-1,2.*dou);
	      }
	  }
	
	/* to set the header of the new frame with the one of a frame :*/
	frameTowrite.SetHeader(frame.GetSignature(),
			       frame.GetStreamID(),
			       frame.GetTime() );
	
	/* an other method can be :
	   frameTowrite.SetStreamID(frame.GetStreamID());
	   frameTowrite.SetTime(frame.GetTime());
	   frameTowrite.SetSignature(frame.GetSignature());
	*/
	
	/* to write the EntityToWrite */
	frameTowrite.Write(entity);
	
	/* for reading a frame of the EntityToWrite, this make an 
	 * exception because the file is at end*/
	//frame.Read(entity);
	
	/* clean the frames */
	frameTowrite.ClearData();
	frame.ClearData();
      }
    }
    
    /* to catch an exception */
    catch(SDIFEof& e)
      {
	/* if we want an access to the file */
	const SdifFileT *sf = e.sdifFile();

	std::cerr << " Catch EOF for file " <<sf->Name << " -- ending program " << std::endl;
	/* to have the error message */
      }
    
    catch(SDIFUnDefined& e)
      {
	std::cerr << " Catch Undefined " << std::endl;	
	e.ErrorMessage();
      }
    
    catch(Easdif::SDIFException&e)
      {
	std::cerr << " Catch other SDIFException " << std::endl;
	e.ErrorMessage();
      }
    catch(std::exception &e)
      {
	std::cerr << " Catch other Exception: " << e.what() <<std::endl;

      }



    /* to open a file for writing */
    entity.Close();
    readentity.Close();
    
    /* deinitialise the SDIF library */
    EasdifEnd();
    return 0;
}

