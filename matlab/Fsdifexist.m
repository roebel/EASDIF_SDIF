
% function ok=Fsdifexist(filename)
%
% test whether sdif file filename exists
%
% INPUT 
% filename : string containing filename with selection
%
% OUTPUT 
% ok : true if file exists and is an sdif file

function ok=Fsdifexist(filename)
  
  ok=Fsdif_read_handler('issdif',filename);
  