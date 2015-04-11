---
title: Scala Patience Diff
sub_title: An implementation of the patience diff algorithm, as found in several modern VCS programs
---
Consider [diffing][diff] the following two files:

<div style="width: 49%; float: left;">
{% capture left-float %}
File1

```c
#include <stdio.h>

// Frobs foo heartily
int frobnitz(int foo)
{
    int i;
    for(i = 0; i < 10; i++)
    {
        printf("Your answer is: ");
        printf("%d\n", foo);
    }
}

int fact(int n)
{
    if(n > 1)
    {
        return fact(n-1) * n;
    }
    return 1;
}

int main(int argc, char **argv)
{
    frobnitz(fact(10));
}
{% endcapture %}
{{ left-float }}
</div>
```

<div style="width: 49%; float: right;">
{% capture right-float %}
File2

```c
#include <stdio.h>

int fib(int n)
{
    if(n > 2)
    {
        return fib(n-1) + fib(n-2);
    }
    return 1;
}

// Frobs foo heartily
int frobnitz(int foo)
{
    int i;
    for(i = 0; i < 10; i++)
    {
        printf("%d\n", foo);
    }
}

int main(int argc, char **argv)
{
    frobnitz(fib(10));
}                        
{% endcapture %}
{{ right-float  }}
</div>
```

<div style="width: 100%; clear: both;"> </div>

It's easy to see that the call to the "fib" function has been added instead of
the "fact" function call. The "fact" function was removed, with the "fib"
function being added a few lines below.  Let's examine how a typical diff tool
presents these changes.

<div style="width: 49%; float: left;">
{% capture left-float2 -%}
First compare the result of diffing file1 and file2 using GNU diff:  

`diff -u file1.txt file2.txt`  
    
```diff
 #include <stdio.h>
 
-// Frobs foo heartily
-int frobnitz(int foo)
+int fib(int n)
 {
-    int i;
-    for(i = 0; i < 10; i++)
+    if(n > 2)
     {
-        printf("Your answer is: ");
-        printf("%d\n", foo);
+        return fib(n-1) + fib(n-2);
     }
+    return 1;
 }
 
-int fact(int n)
+// Frobs foo heartily
+int frobnitz(int foo)
 {
-    if(n > 1)
+    int i;
+    for(i = 0; i < 10; i++)
     {
-        return fact(n-1) * n;
+        printf("%d\n", foo);
     }
-    return 1;
 }
 
 int main(int argc, char **argv)
 {
-    frobnitz(fact(10));
+    frobnitz(fib(10));
 }
{% endcapture %}
{{ left-float2  }}
</div>
```

<div style="width: 49%; float: right;">
{% capture right-float2 -%}
And now, the result using the patience diff algorithm (as described
[here][diff1] and [here][diff2]):  

`scala -classpath <path_to_class_files> Main file1.txt file2.txt`

```diff
 #include <stdio.h>
 
+int fib(int n)
+{
+    if(n > 2)
+    {
+        return fib(n-1) + fib(n-2);
+    }
+    return 1;
+}
+
 // Frobs foo heartily
 int frobnitz(int foo)
 {
     int i;
     for(i = 0; i < 10; i++)
     {
-        printf("Your answer is: ");
         printf("%d\n", foo);
     }
 }
 
-int fact(int n)
-{
-    if(n > 1)
-    {
-        return fact(n-1) * n;
-    }
-    return 1;
-}
-
 int main(int argc, char **argv)
 {
-    frobnitz(fact(10));
+    frobnitz(fib(10));
 }
[diff1]: http://bramcohen.livejournal.com/73318.html
[diff2]: http://alfedenzo.livejournal.com/170301.html
{% endcapture %}
{{ right-float2  }}
</div>
```

<div style="width: 100%; clear: both;"></div>

It's clear for this example that the patience diff has produced a superior diff
output; it represents something closer to how one would describe the changes
verbally.

The reason that the patience diff algorithm (whose name comes from the fact
that a [patience sort][patience] is used to calculate the LCS) produces more
readable diff outputs is because it only considers common lines that are unique
within each file. What this means practically, is that lines that are blank, or
contain a single brace are not considered as forming part of the common
subsequence; the diff algorithm will not focus on trying to match braces (the
GNU `diff` output shown above does this) but instead, focusses on unique lines
(e.g.  function definitions).

My Scala implementation can be found on [Github, here][github_link]. An example
usage (which takes the two filenames to diff on stdin and prints the diff) is
shown below. Patience diff is used by the [Bazaar][bazaar] version control
system, whose implementation I consulted for ideas whilst coding my Scala
version.

```scala
import scala.io.Source._
import OwenDiff._
object Main {
    def main(args : Array[String]) : Unit = {
    	val file1 = fromFile(args(0)).getLines.toList
    	val file2 = fromFile(args(1)).getLines.toList

        val diffList = Diff.diff(file1, file2)

        println(diffList.mkString)
    }
}
```

The main diff method is fairly simple; it recursively creates a list of pairs
of matching line indices between the two files and coalesces them into
contiguous blocks if possible, which are used to calculate the list of line
insertion/deletion/changes required to go from file1->file2, as per `diff`. 

A slight modification of my [patience sort][patience_impl] code, with the
inclusion of a bisect method implementation gives the code that calculates the
longest increasing subsequence (LIS) of its argument. The LIS code is used to
determine the LCS of the line pairs. 

[diff]: http://en.wikipedia.org/wiki/Diff
[patience]: http://en.wikipedia.org/wiki/Patience_sorting
[github_link]: https://github.com/owst/Scala-Patience-Diff
[bazaar]: http://bazaar.canonical.com/en/
[patience_impl]: /programming/2010/11/29/scala-patience-sort.html
