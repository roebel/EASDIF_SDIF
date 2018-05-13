#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
//#include "sdifmatrixdata.h"
//#include "sdifmatrix.h"
#include "easdif/easdif.h"


int main(int argc, char** argv)
{
/*********************************************/
    SDIFEntity readentity;
    SDIFEntity writeentity;

    EasdifInit();

    if (argv[1]==NULL)    
	std::cerr << "Usage : " << argv[0] << " filename.sdif" <<std::endl;    

    try
    {
	//throw SDIFFileError();
	//throw SDIFReDefined();//essai

      /*for the reading*/
      readentity.OpenRead(argv[1]);
      
      /*for the writing*/
      writeentity.OpenWrite("ecr.sdif");
      

      /* END */	
      while (1)
	{
	  SDIFFrame frame;
	  /*******  Reading the frame  *******/
	  readentity.ReadNextFrame(frame);
	  writeentity.ReadNextFrame(frame);
	}

    }
    catch(SDIFAllocFail& e)
    {
	std::cerr << " Catch AllocFail " << std::endl;
  	std::cerr << " View error message " << std::endl;
  	e.ErrorMessage();
	std::cerr << " end of catch " << std::endl;
    }

    catch(SDIFArrayPosition& e)
    {
	std::cerr << " Catch ArrayPosition " << std::endl;
  	std::cerr << " View error message " << std::endl;
  	e.ErrorMessage();
	std::cerr << " end of catch " << std::endl;
    }

    catch(SDIFEof& e)
    {
	std::cerr << " Catch EOF " << std::endl;
  	std::cerr << " View error message " << std::endl;
  	e.ErrorMessage();
	std::cerr << " end of catch " << std::endl;
    }

    catch(SDIFReDefined& e)
    {
	std::cerr << " Catch SDIFRedefined " << std::endl;
  	std::cerr << " View error message " << std::endl;
  	e.ErrorMessage();
	std::cerr << " end of catch " << std::endl;
    }

    catch(SDIFException& e)/* catch all*/
    {
	std::cerr << " catch exception " << std::endl;
  	std::cerr << " View error message " << std::endl;
  	//e.ErrorMessage();
	std::cerr << " end of catch " << std::endl;	
    }
/*
    catch(...)
    {
	std::cerr << " default catch " << std::endl;
	
    }
*/


    EasdifEnd();

    return 0;
}
     



