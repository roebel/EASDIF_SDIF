% function Fsdifrewind(fh)
%
% rewind sdif file to the first frame
%
% AUTHOR : Axel Roebel
% DATE   : 31.10.2009
%
% $Revision: 1.2 $    last changed $Date: 2009-10-31 17:26:56 $
%
%                                                       Copyright (c) 2009 by  IRCAM 

function Fsdifrewind(fh)

  Fsdif_read_handler('rewind',fh);
