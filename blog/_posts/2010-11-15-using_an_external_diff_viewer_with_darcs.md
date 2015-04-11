---
title: Using an external diff viewer with Darcs
---
Use the following command to show differences between the working and pristine
copies of a darcs repo, using an external diff viewer.

```bash
darcs diff --diff-command 'kdiff3 %1 %2'
```

This will create two temporary folders containing the two versions of the repo,
and will pass the folder names as parameters to the diff program specified.
Once the diff program has exited, the temporary folders are deleted.
