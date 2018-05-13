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
    virtual int GetNbRow()=0;
    virtual int GetNbCol()=0;
    virtual void view()=0;
    virtual int read(SdifFileT* file)=0;
    virtual void Resize(int nrows, int ncols)=0;
    virtual int write(SdifFileT* file)=0;

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
	    if (nrows<0|| ncols<0 ){
		m_Nrows=0;
		m_Ncols=0;
		m_Data.resize(0);
	    }
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	};

    ~SDIFMatrixData()
	{
	};
	

    double GetDouble(int i, int j)
	{
	    if (i > m_Nrows || j > m_Ncols)
	    {
 		std::cerr<<"Error in GetDouble : can not i>m_Nrows or j>m_Ncols"<<std::endl;
		return 1;
	    }
	    return static_cast<double>(m_Data[i*m_Ncols+j]);
	}

    float GetFloat(int i, int j)
	{
	    if (i > m_Nrows || j > m_Ncols)
	    {
 		std::cerr<<"Error in GetFloat : can not i>m_Nrows or j>m_Ncols"<<std::endl;
		return 1;
	    }
	    return static_cast<float>(m_Data[i*m_Ncols+j]);
	}

    int GetInt(int i, int j)
	{
	    if (i > m_Nrows || j > m_Ncols)
	    {
 		std::cerr<<"Error in GetInt : can not i>m_Nrows or j>m_Ncols"<<std::endl;
		return 1;
	    }
	    return static_cast<int>(m_Data[i*m_Ncols+j]);
	}

    void Resize(int nrows, int ncols)
	{
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	}

    int read(SdifFileT* file)/////read matrix
	{
	    int bytesread = 0;	    
	    for (int row = 0; row < m_Nrows; row++)
	    {
		bytesread += SdifFReadOneRow(file);
		T* p= static_cast<T*>(SdifFCurrOneRowData(file));
		
		std::copy(p,p+m_Ncols,&(m_Data[row*m_Ncols]));
	    }
	    bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
	    return bytesread; 
	}
    int write(SdifFileT* file)
	{
	    int SizeFrameW = 0;
	    /* Write matrix data */
	    for (int row = 0; row < m_Nrows; row++)
	    {
		for (int col = 1; col <= m_Ncols; col++)
		{
		    T value = static_cast<T>(m_Data[row*m_Ncols+col-1]);
		   
		        SdifFSetCurrOneRowCol (file, col, value);
		   
		   }
		SizeFrameW += SdifFWriteOneRow (file);
	    }

	    /* Write matrix padding */
	    SizeFrameW += SdifFWritePadding(file, SdifFPaddingCalculate (file->Stream, SizeFrameW));

	    return SizeFrameW;
	}

    void view()
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


   inline  int GetNbRow()
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

    Base* mBase;

    int bytesread;

    SdifSignature mSig;

    SdifDataTypeET mType;


public:
    SDIFMatrix(): mBase(0)
	{};

    ~SDIFMatrix(){
	if(mBase) {
	    delete mBase;
	    mBase =0;
	}
    };

    void createMatrixData(SdifSignature sig, 
			 int nrows, int ncols, SdifDataTypeET  type=eFloat4)
	{
	    if(mBase) {
		delete mBase;
		mBase =0;
	    }
	    //update mSig;
	    //update m_Signature;
	    //update mType;

	    switch (type){
	    case eInt4:
		mBase=static_cast<Base*>(new SDIFMatrixData<int>(nrows,ncols));
		break;
	    case eFloat4:
		mBase=static_cast<Base*>(new SDIFMatrixData<float>(nrows,ncols));
		break;	
	    case eFloat8:
		mBase=static_cast<Base*>(new SDIFMatrixData<double>(nrows,ncols));
		break;
	    }
	}

//#if 0
    int Write(SdifFileT* file)
	{
	    
	    int SizeFrameW = 0;
	    int nrows=mBase->GetNbRow();
	    int ncols=mBase->GetNbCol();

	    // SdifDataTypeET  type  = SdifFCurrDataType(file);
	    mBase->Resize(nrows, ncols);
	    SizeFrameW = 0;

	    /*Write matrix header*/
	    SdifSignature msig = SdifStringToSignature(const_cast<char*>(m_Signature.c_str()));
	    SdifFSetCurrMatrixHeader(file, msig, eFloat4, nrows, ncols);
	    SizeFrameW += SdifFWriteMatrixHeader(file);


	    SizeFrameW +=mBase->write(file);

	    return SizeFrameW;
	}
//#endif

    int Read(SdifFileT* file)
	{   	    
	    int bytesread = 0;
	    bytesread += SdifFReadMatrixHeader(file);

	    SdifSignature msig = SdifFCurrMatrixSignature(file);
	    m_Signature = SdifSignatureToString(msig);
	    int nrows = SdifFCurrNbRow(file);
	    int ncols = SdifFCurrNbCol(file);
	    SdifDataTypeET  type  = SdifFCurrDataType (file);
	
	    createMatrixData(msig, nrows, ncols, type);
	    mBase->read(file);
	    


	    return bytesread;
	} 


    void View()
	{
            std::cout << " Signature : " << m_Signature << std::endl;
	    mBase->view();

	}

};



int main()//int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
///////////////////*les variables ajoutes*/
    size_t generalHeader;
    size_t asciiChunks;

    size_t generalHeaderw;
    size_t asciiChunksw;
    //  char* filename = NULL;
    SdifSignature sign_fich;
////////////////
    SdifFileT *file;
    SdifFileT *filew;

    SdifGenInit("");

    /*for the reading*/
    file = SdifFOpen ("lic.sdif", eReadFile);
    generalHeader = SdifFReadGeneralHeader(file);
    asciiChunks = SdifFReadAllASCIIChunks(file);
    

    /*for the writing*/
    filew = SdifFOpen ("ecr.sdif", eWriteFile);
    generalHeaderw = SdifFWriteGeneralHeader(filew);
    asciiChunksw = SdifFWriteAllASCIIChunks(filew);
    


///////////////////
    sign_fich = SdifFCurrSignature (file);
////////////////////


    while (!eof  &&  SdifFLastError(file) == NULL)
    {
	bytesread += SdifFReadFrameHeader(file);


  

	if (!eof)
	{    /* Access frame header information */
	    SdifFloat8      time     = SdifFCurrTime(file);
	    SdifSignature   sig      = SdifFCurrFrameSignature(file);
	    SdifUInt4       streamid = SdifFCurrID(file);/*index de l'objet du frame courant*/
	    SdifUInt4       nmatrix  = SdifFCurrNbMatrix(file);
	    SdifUInt4             m;
    
	    /*On ecrit les informations relatives a la frame : */
	    // printf("Signature de la frame : %s\n",sig);
	    printf("\nSignature de la frame : %s\n", SdifSignatureToString(sig));
	    printf("sId: %d", streamid);
	    printf(" Tps=%f", time);
	    printf(" nbreMat ds frame : %d\n",nmatrix);


	    /*for the writing*/
	    SdifUInt4 framesize = SdifSizeOfFrameHeader();
	    //	+7*SdifSizeOfMatrix(eFloat4, nrows, ncols);
	    SdifFSetCurrFrameHeader(filew,sig, framesize, nmatrix, streamid,time);
	    //SdifFSetCurrFrameHeader(file,frameSig, framesize, 2, mystream, mytime);
	    SdifFWriteFrameHeader(filew);



	    /* Read all matrices in this frame matching the selection. */
	   
	    for (m = 0; m < nmatrix; m++)
	    {
		
		SDIFMatrix mat;
		SDIFMatrix matw;


		//SdifSignature msig = SdifFCurrMatrixSignature(file);
///////////////////
		std::cout <<"la valeur de m est : "<< m<< std::endl;
		bytesread+=mat.Read(file);
		mat.View();
		mat.Write(filew);
	
		std::cout << " affichage de la matrice numero :  " << m+1<<std::endl;

		//mat.View();
///////////////////////


	    }	    
/* read next signature */
	    eof = SdifFGetSignature(file, &bytesread) == eEof;
	}


 


    }
    if (SdifFLastError(file))   /* Check for errors */
    {
	exit (1);
    }

    SdifFClose(file);
    SdifFClose(filew);
///////////////////////////////////////

    

    SdifGenKill();

    return 0;
}
     


