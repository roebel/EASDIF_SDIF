% function numwritten=Fsdifwrite(file,frames)
%
% write 1-d array of structured sdif frames into file related to
% filehandle file
%
% INPUT :
%
% file   : filehandle opened for writing returned by Fsdifopen 
% frames : 1d array of frames holding complete SDIF frames with non
%          decreasing times
%
% frame format :
%    frames.fsig     = 1x4 double array indicating the frame signature
%    frames.stream   = real scalar of arbitrary type holding the
%                      streamid
%    frames.time     = real double indicating frame time
%    frames.data     = struct with fields named MD_XXXX
%                      where XXXX is representing the 4-char matrix
%                      signature and each field contains exactly one real
%                      valued matrix of any but 64-bit integer types.
%
%  example :
%
% frames(1)
%
%        fsig: [73 71 66 71] == > double('IGBG')
%      stream: 0
%        time: 1.3
%        data: [1x1 struct]
%
%
% frames(1).data
% 
%  MD_IGBG: [1 44100 1 1024 185]
%
% Remark :  frames.msig filed of the directory/reading format is redundant with
%           the last 4 characters of the data.MD_XXXX field, accordingly the 
%           frames.msig field is not used when writing data.
%
% OUTPUT :
%
% number of frames written
%
%
% SEE also : Fsdifopen, Fsdifclose, Fsdifread, and the low level handlers
%     Fsdif_read_handler and Fsdif_write_handler
%
% AUTHOR : Axel Roebel
% DATE   : 21.01.2008
%
% $Revision: 1.3 $    last changed $Date: 2008-07-31 18:10:31 $
%
%                                                         Copyright (c)
%                                                         2008 by  IRCAM 
%
function numwritten=Fsdifwrite(file,frames)
  
  numwritten=Fsdif_write_handler('write',file,frames);
  