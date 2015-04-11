---
title: Bash Script to Get and Apply a Remote Darcs Patch
---
Here's a short shell script that I use when reviewing patches from the Darcs
bugtracker, [roundup][1]. 

I often review patches by following simple steps:

1. Downloading the submitted dpatch file.
1. Grabbing a copy of the "screened" repo - containing all "quick sanity
   checked" submitted patches - and removing patches not in the dpatch's
   context.
1. Applying the dpatch. 

I can then check the implementation of the patch, and screen away!

Of course, I noticed myself redoing these three steps several times, before
*finally* submitting and hacking some bash:

```bash
#! /bin/bash

# Exit on any failures.
set -e

PATCH_NAME=downloaded_patch.dpatch
REPO_NAME=darcs.net_with_downloaded_patch

# I often do this in /tmp, so the target may already exist...
rm -rf $REPO_NAME $PATCH_NAME

# Grab the patch.
wget $1 --output-document $PATCH_NAME

# No need to copy the patch contents of the screened repo.
darcs get --lazy --context $PATCH_NAME http://www.darcs.net $REPO_NAME

cd $REPO_NAME

# Apply the submitted dpatch.
darcs apply ../$PATCH_NAME
```

    # echo the directory, so we can pipe the output into cd, for example.
    echo $REPO_NAME

[1]: http://bugs.darcs.net
