
 while (!eof  &&  SdifFLastError(file) == NULL)
    {
	/*******  Creating a frame  *******/
	SDIFFrame frameToWrite;	   

	if (!eof)
	{   	    
	    SDIFFrame frame;
	    /*******  Reading the frame  *******/
	    frame.Read(file);
	    /* print to stdout the content of the frame */
	    frame.View();


	    for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	    {
		/*take the matrix number "i" and put it in tmpMatrix */
		SDIFMatrix tmpMatrix = frame.GetMatrix(i);		
		/*if we want to see the matrix :*/
		/*view the matrix before transformation*/
		tmpMatrix.View();
	       		  
		/*add a matrix in the matrix vector of the frame*/
		frameToWrite.AddMatrix(tmpMatrix);
		/*how to access  the data : an example, if we want to multiply by 2 the last column of a matrix  matrix :*/
		double dou;
		int ncols = tmpMatrix.GetNbCols();
		for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
		{
		    /*in this version, there are different methods to get a data element : 
		     * the method "Get" (specifying the data type by means of the argument only
                     * or the method "GetInt", "GetFloat", "GetDouble" : */

		    /*using "Get" :*/
		    tmpMatrix.Get(i, ncols-1,dou);

		    /*alternatively with "GetDouble" :
		      dou = tmpMatrix.GetDouble(i, ncols-1);*/

		    tmpMatrix.Set(i, ncols-1,2.*dou);
		}
		/* view the matrix after transformation */
		tmpMatrix.View();
		/*end of example*/
	    }
	    /*set the header of the frame*/
	    frameToWrite.SetStreamID(frame.GetStreamID());
	    frameToWrite.SetTime(frame.GetTime());
	    frameToWrite.SetSignature(frame.GetSignature());

	    /*if we want to  set all the informations :
	      frameToWrite.SetInfo(Sig, StreamID, Time);
	    */
	    /*write the header and the data*/
	    frameToWrite.Write(filew);
	    /* Print to stdout the content of the frame */
	    frameToWrite.View();
	    /*clear the data of the frame to reuse :*/
	    frameToWrite.ClearData();
	    /* read next signature */
	    eof = SdifFGetSignature(file, &bytesread) == eEof;
	}
    }




 
