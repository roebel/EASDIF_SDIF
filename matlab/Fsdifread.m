% function frames=Fsdifread(file,select, class_str)
%
% read 1-d array of structured sdif frames from file related to
% filehandle file. Note that data types in the returned matrix
% will reflect the data types used to store the data in the file.
% If you want double format you need to explcitely transform the 
% fields of the frames struct.
%
% INPUT :
%
% file      : filehandle opened for reading returned by Fsdifopen 
% select    : selection controling the read
% class_str : string representing a matlab classid used to specify a desried data type
%
% Frame selection :
% ------------------------------------
% If select is a scalar or not present
%
%   frames=Fsdifread(file) 
%   frames=Fsdifread(file, N) 
%   frames=Fsdifread(file, N, class_str) 
%
% read the next single frame or next N frames from the current read postion
% empty if less then requested frames are returned then
% the eof has been reached 
%
% if class_str is given and a valid matlab data class id then the
% returned data sections will converted from the files matrix dependend value type
% into the specified type.
%
% -----------------------------------
% If select is a struct :
%
% frames=Fsdifread(file, select);
% frames=Fsdifread(file, select, class_str);
% 
% Selects a subset of frames and matrices according to the selection struct "select".
% This local selection takes into account the selection specified in the filename.
% The select argument can be an array of structs holding the fields of the frame
% directory (see Fsdifopen). Each field can hold a set of values and the
% selection will be formed by combining all values from all structs of
% the array. The selection will work on the set of frames and matrices
% that passes the selection mechanism that is part of the filename and
% will pass only those values that are explicitely mentioned in the set
% of selected values. The selection mechanism uses only the non empty fields
% specified in the select struct.
%
% As an alternative to the time and stream fields the selection 
% can use timeRange and streamRange fields with exactly 2 values. These
% will select the range of values between the two boundaries.
% If range fields are present only a scalar selection struct can be used.
%
% All fields of the selection that are present are used to filter the elements of the
% matrix to be read. Accordingly, a empty selection matrix does not
% filter anything and accordingly reads the whole file.
%
% If the third element is given than it should be string represeting a
% matrix class id. The data returned indicates the maximum number of
% frames that will be returned. 
%
% NOTE : the read pointer position used for reading frames
% without selection is not used nor modified by the calls to Fsdifread with a
% selection set.
%
% OUTPUT :
%
% 1d array of data frames read
%
% frame format :
%    frames.fsig     = 1x4 double array indicating the matrix signature
%    frames.stream   = real scalar of arbitrary type holding the
%                      streamid
%    frames.time     = real double indicating frame time
%    frames.data     = struct with fields named MD_XXXX
%                      where XXXX is representing the 4-char matrix
%                      signature and each field contains exactly one real
%                      valued matrix of any but 64-bit integer types.
%
% EXAMPLES :
%
% no selection -> read a single frame
%
% [filehandle] = Fsdifopen('file.sdif');
% frames = Fsdifread(filehandle)
% frames(1)
%
%        fsig: [73 71 66 71] ==  double('IGBG')
%      stream: 0
%        time: 1.3
%        msig: [73 71 66 71] ==  double('IGBG')
%        data: [1x1 struct]
%
% frames(1).data
% 
%  MD_IGBG: [1 44100 1 1024 185]
%
% using selections :
%
% [file,head,dir] = Fsdifopen('file.sdif');
%
% read all frames in time range 1s - 2s.
% sel.timeRange = [1,2]
% frames = Fsdifread(file,sel);
%
% read only 1TRC  frames in time range 1s - 2s.
% sel.sif = double('1TRC')
% frames = Fsdifread(filehandle,sel);
%
% read the whole file 
% dir holds the directory so it selects all matrices and frames in the file
% frames = Fsdifread(filehandle,dir);
%
% select stuct is empty, so the whole file is read as well.
% frames = Fsdifread(filehandle,[]);
% 
%
%
% SEE also : Fsdifopen, Fsdifclose, Fsdifread, Fsdifrewind, Fsdifclearall and the low level handlers
%     Fsdif_read_handler and Fsdif_write_handler
%
%
% AUTHOR : Axel Roebel
% DATE   : 21.01.2008
%
% $Revision: 1.6 $    last changed $Date: 2011-09-27 11:59:34 $
%
%                                                       Copyright (c) 2008 by  IRCAM 

function frames=Fsdifread(file, select, class_str)

  if nargin == 1
    frames = Fsdif_read_handler('read',file);
  else
    % either scalar or struct selection
    if(nargin == 2)
      frames = Fsdif_read_handler('read',file, select);
    else 
      frames = Fsdif_read_handler('read',file, select, class_str);
    end
  end
