---
title: 'GSoC: Darcs Bridge - Week 3'
published: false
---
So, time for a (somewhat delayed - oops!) week 3 update on darcs-bridge: By the
end of last week, I had coded up a poorly-implemented approach to importing
branches, but had hit a few final problems (mostly arising due to my flawed
approach). Petr and Ganesh both helped me to see the light (and also a better
method for handling branches!) which I've spent the weekend on-and-off hacking
up.

The result is this rather innocuous-looking transcript: 

```console

$ git branch -a
  foo_branch
* master

$ git log --graph --all
* commit 48e3317aad56df72977c80c2b40a34b87349e435
| Author: Owen Stephens <git@owenstephens.co.uk>
| Date:   Mon Jun 13 14:28:53 2011 +0100
| 
|     add b
|    
| * commit e85e1bae42b1647bc08bdac2aaec8e402152bce0
|/  Author: Owen Stephens <git@owenstephens.co.uk>
|   Date:   Mon Jun 13 14:28:53 2011 +0100
|   
|       add c
|  
* commit d8b3bafad054acdd8307fbefdc95287dc715e9a7
  Author: Owen Stephens <git@owenstephens.co.uk>
  Date:   Mon Jun 13 14:28:53 2011 +0100
  
      add a

$ (cd git_repo  && git fast-export --all) | darcs-fastconvert import darcs
[...]

$ darcs cha --repo darcs
Mon Jun 13 14:28:53 BST 2011  Owen Stephens &lt; git@owenstephens.co.uk>
  * add c

Mon Jun 13 14:28:53 BST 2011  Owen Stephens <git@owenstephens.co.uk>
  * add a

$ darcs cha --repo darcs-branch_foo_branch
Mon Jun 13 14:28:53 BST 2011  Owen Stephens <git@owenstephens.co.uk>
  * add b

Mon Jun 13 14:28:53 BST 2011  Owen Stephens <git@owenstephens.co.uk>
  * add a
```

Which shows (somewhat unclearly) a multi-head git repo with a base commit -
"add a" and then two branching commits: "add b" and "add c"; "add b" is made on
a new branch, and should not show up in the log of the master branch. Importing
this git repo creates a base repo directory and any branches are created in
adjacent directories. I've not quite finished off this branch-importing work -
I still need to handle merges, but it should not be too much trouble, and it's
good to see that the code works on simple cases already! 

Other things I've completed over the last few days:

- Correctly handling renames/moves, rather than simply diffing before/after
  (which would give patches that see fully removed/added files).  - I no longer
  shell out to darcs-fastconvert (yuck!), when syncing a bridge, as I've
  reworked the input/output handling that was preventing me from calling the
  import code internally.
- Testing: I've added a basic test-suite, that will catch the most egregious of
  foul-ups due to any changes that I make. I'll add more tests in the future,
  to catch tricky cases, especially with handling branches.

Things coming up: handling merges and exporting multiple darcs "branches" into
a single git repository. Onwards!
