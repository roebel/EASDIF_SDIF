#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
class Base
{ 
public:
    virtual double GetDouble(int i, int j)=0;
    virtual float GetFloat(int i, int j)=0;
    virtual int GetInt(int i, int j)=0;
    //virtual void Read(SdifFileT* file)=0;
    // virtual void Write(SdifFileT* file)=0;

    virtual ~Base() {};
};

template <class T>
class SDIFMatrixData : public Base
{
private:
    int m_Nrows;
    int m_Ncols;
    std::vector<T> m_Data;

public:
    SDIFMatrixData(int nrows, int ncols)
	{
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	};

    ~SDIFMatrixData()
	{
	};

    T SetValue(int row,int col, const T& value)
	{
	    m_Data[row*m_Ncols+col] = value;
	    return value;
	}

    T GetValue(int row,int col)
	{
	    return m_Data[row*m_Ncols+col];
	}

    double GetDouble(int i, int j)
	{
	    return static_cast<double>(m_Data[i*m_Ncols+j]);
	}

    float GetFloat(int i, int j)
	{
	    return static_cast<float>(m_Data[i*m_Ncols+j]);
	}

    int GetInt(int i, int j)
	{
	    return static_cast<int>(m_Data[i*m_Ncols+j]);
	}

    int Resize(int nrows, int ncols)
	{
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	    
	}

    inline int GetNbRow() 
	{
	    return m_Nrows;
	}
    inline int GetNbCol() 
	{
	    return m_Ncols;
	}

};


class SDIFMatrix
{
private:
    std::string m_Signature;
    SDIFMatrixData<double> m_MatrixData;

    int bytesread;


public:
    SDIFMatrix(): m_MatrixData(1,1)
	{};

    ~SDIFMatrix(){};


#if 0
    void Write(SdifFileT* file)
	{

	    /* Write matrix data */
	    for (row = 0; row < nrows; row++)
	    {
		for (col = 1; col <= ncols; col++)
		{
		    SdifFSetCurrOneRowCol (file, col, value);
		    
		}
		SizeFrameW += SdifFWriteOneRow (file);
	    }
	}
#endif

    int Read(SdifFileT* file)
	{   	    
	    // int bytesread = 0;
	    bytesread += SdifFReadMatrixHeader(file);

	    SdifSignature msig = SdifFCurrMatrixSignature(file);
	    m_Signature = SdifSignatureToString(msig);
	    int nrows = SdifFCurrNbRow(file);
	    int ncols = SdifFCurrNbCol(file);
 SdifDataTypeET  type  = SdifFCurrDataType(file);

 m_MatrixData.Resize(nrows, ncols);

	    //nrows = GetNbRow();
	    //ncols = GetNbCol();
	    
// A METTRE DANS LE READ DU SDIFMatrixData
	    for (int row = 0; row < nrows; row++)
	    {
		bytesread += SdifFReadOneRow(file);
		
		for (int col = 1; col <= ncols; col++)
		{
		    /* Access value by value... */
		    double value = SdifFCurrOneRowCol(file, col);		    
		    m_MatrixData.SetValue(row, col-1,static_cast<double>(value));
		}
	    }
// FIN DE A METTRE

	    return bytesread;
	} 

    int View()
	{
	    int row;
	    int col;
	    std::cout << " Signature : " << m_Signature << std::endl;
	    for (row = 0; row < m_MatrixData.GetNbRow(); row++)
	    {
		for (col = 0; col < m_MatrixData.GetNbCol(); col++)
		{
		    std::cout << m_MatrixData.GetValue(row, col) << " ";
		}
		std::cout << std::endl;
	    }
	}
};



int main()//int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
///////////////////*les variables ajoutes*/
    size_t generalHeader;
    size_t asciiChunks;
/////////////////
    char* filename = NULL;
/////////////////    
    SdifSignature sign_fich;
////////////////
    SdifFileT *file;


    SdifGenInit("");

    
    /* Open file for reading and parse selection. (See Utilities for selection syntax.) */


    //filename = argv[1];
    /* 
       if(filename==NULL)
       {
       fprintf(stderr, "Usage: %s <filename.sdif>\n",
       argv[0]);
       return 1;
       }
    */
    // file = SdifFOpen (filename, eReadFile);
    file = SdifFOpen ("fae.sdif", eReadFile);




    generalHeader = SdifFReadGeneralHeader(file); /* Read file header et retourne la taille avec un size_t */
     
    asciiChunks = SdifFReadAllASCIIChunks(file); /* Read ASCII header info, such as name-value tables */
    

///////////////////
    sign_fich = SdifFCurrSignature (file);
////////////////////


    while (!eof  &&  SdifFLastError(file) == NULL)
    {
	bytesread += SdifFReadFrameHeader(file);


    /* search for a frame we're interested in */
    while (!SdifFCurrFrameIsSelected (file)
           ||  SdifFCurrSignature (file) != sign_fich)
    {
        SdifFSkipFrameData (file);
        if ((eof = SdifFGetSignature(file, &bytesread) == eEof))
            break;                                         /* eof */
        bytesread += SdifFReadFrameHeader(file);
    }


	if (!eof)
	{    /* Access frame header information */
	    SdifFloat8      time     = SdifFCurrTime(file);
	    SdifSignature   sig      = SdifFCurrFrameSignature(file);
	    SdifUInt4       streamid = SdifFCurrID(file);/*index de l'objet du frame courant*/
	    SdifUInt4       nmatrix  = SdifFCurrNbMatrix(file);
	    int             m;
    
	    /*On ecrit les informations relatives a la frame : */
	    // printf("Signature de la frame : %s\n",sig);
	    printf("\nSignature de la frame : %s\n", SdifSignatureToString(sig));
	    printf("sId: %d", streamid);
	    printf(" Tps=%f", time);
	    printf(" nbreMat ds frame : %d\n",nmatrix);

	    /* Read all matrices in this frame matching the selection. */
	   
	    for (m = 0; m < nmatrix; m++)
	    {
		int a;
		SDIFMatrix mat;
///////////////////////
		a=mat.Read(file);
		bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
		std::cout << " Matrix Affichage " << std::endl;

		//mat.View();
///////////////////////


//#if 0
	
//#endif
	    }
	}
    }
    if (SdifFLastError(file))   /* Check for errors */
    {
	exit (1);
    }

    SdifFClose(file);

///////////////////////////////////////

    

    SdifGenKill();

    return 0;
}
     


