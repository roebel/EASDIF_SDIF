#! /bin/bash
# co directory from  svn://m1320/anasynth_deploy/
# and run this script on all directories that contain externals which in the git repos are marked by
# means of having .revision files
#set -x

if [ $# != 1 ] ; then
    echo usage: $0 dist_path
    exit 1
fi

cd $1
set -x
#stop if an error occurs
set -e
for revfile in $(find . -name .revision) ; do

    revrootdir=$(dirname $revfile) 
    subdirs=""
    gitdir=${revrootdir#.}
    gitdir=${gitdir#/}
    set +e
    read svnrepo svnrepovers subdirs < $revfile
    set -e
    newrev=$(svn info $svnrepo | fgrep Revision: |  cut -d" " -f2)
    if [ $newrev -gt $svnrepovers ]; then
        for sdir in $subdirs; do
            echo do svn diff --git -r ${svnrepovers}:${newrev} $svnrepo/$sdir 
            svn diff --git -r ${svnrepovers}:${newrev} $svnrepo/$sdir > $revrootdir/$sdir/svn_${newrev}.patch
            
            (cd $revrootdir/$sdir; git apply --directory=$gitdir --check  -p 2 ./svn_${newrev}.patch)
            (cd $revrootdir/$sdir; git apply --directory=$gitdir -p 2  ./svn_${newrev}.patch)
        done
        echo $svnrepo $newrev $subdirs 
        echo $svnrepo $newrev $subdirs > $revfile
    else
        echo $revrootdir is up to date
    fi
done
