% NAME
%     Fsdifloadfile  -  Load a complete SDIF file
%
% USAGE
%     [ data, header, frame, matrix, dims ] = Fsdifloadfile (name, classid)
%
%     where data{i} is the matrix at time header(i, 1) in stream header(i, 2)
%     with signatures char(frame(i)), char(matrix(i)) and of size dims{i}.
%    
% INPUT:
%     name   : filename to read
%     classid: optional matlab class string. If not specified
%              sdiftypes that are used in the file will be used for the output
%              matrices. if specified output types will be forced to be of the
%              specified type.
%
% SEE ALSO
%
%     Fsdifopen, Fsdifclose, Fsdifread, Fsdif_read_handler, Fsdifexist
%
% AUTHOR
%     Axel Roebel (roebel(at)ircam.fr) 21. January 2008
% 
% CVS REVISION
%     $Id: Fsdifloadfile.m,v 1.3 2011-07-12 10:46:03 roebel Exp $
%
%                                         Copyright (c) 2008 by IRCAM
%
function [ data, header, frame, matrix, dims ] = Fsdifloadfile (name, force_type) 

  if( ~Fsdifexist(name))
    error(sprintf('%s is no sdif file \n',name))
  end

  file =  Fsdif_read_handler('open',name);
  switch nargout
   case {1}
     if(nargin == 2)
       data = Fsdif_read_handler('loadfile',file, force_type);
     else
       data = Fsdif_read_handler('loadfile',file);
     end
   case {2}
     if(nargin == 2)
       [data header] = Fsdif_read_handler('loadfile',file, force_type);
     else
       [data header] = Fsdif_read_handler('loadfile',file);
     end
   case {3}
     if(nargin == 2)
       [data header frame] = Fsdif_read_handler('loadfile',file, force_type);
     else
       [data header frame] = Fsdif_read_handler('loadfile',file);
     end
   case {4}
     if(nargin == 2)
       [data header frame matrix] = Fsdif_read_handler('loadfile',file, force_type);
     else
       [data header frame matrix] = Fsdif_read_handler('loadfile',file);
     end
   case {5}
     if(nargin == 2)
       [data header frame matrix dims] = Fsdif_read_handler('loadfile',file, force_type);
     else
       [data header frame matrix dims] = Fsdif_read_handler('loadfile',file);
     end
  end
  Fsdif_read_handler('close',file);

return

