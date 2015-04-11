---
title: Add accessed date to IEEEtran BibTeX style
---
[IEEE referencing style](http://wwwlib.murdoch.edu.au/find/citation/ieee.html)
requires showing the date of access for online documents. I've borrowed the
changes made to the [Harvard BibTex style by Michael
Tyson](http://michael.tyson.id.au/2006/11/02/adding-url-accessed-date-field-to-harvard-bibtex-style/)
and applied them to the IEEE style.

To add the changes, you need to edit the IEEEtran.bst file; the location of
which depends on your installation, but for me, the file was located at
bibtex/bst/IEEEtran/IEEEtran.bst, in my installation folder.

The changes necessary are:

Find the section that starts (~line 292):

```text
%%%%%%%%%%%%%%%%%%
%% ENTRY FIELDS %%
%%%%%%%%%%%%%%%%%%

ENTRY
  { address
    assignee
    author
```

Look for url and add a new attribute, 'urldate' beneath it.

Then find the function that looks like this:

```text
FUNCTION {format.url}
{ url empty$
    { "" }
    { this.to.prev.status
      this.status.std
      cap.yes 'status.cap :=
      name.url.prefix " " *
      "\url{" * url * "}" *
      punct.no 'this.status.punct :=
      punct.period 'prev.status.punct :=
      space.normal 'this.status.space :=
      space.normal 'prev.status.space :=
      quote.no 'this.status.quote :=
    }
  if$
}
```

And change it, so that it looks like this:

```text
FUNCTION {format.url}
{ url empty$
    { "" }
    { this.to.prev.status
      this.status.std
      cap.yes 'status.cap :=
      name.url.prefix " " *
      "\url{" * url * "}" *
      punct.no 'this.status.punct :=
      punct.period 'prev.status.punct :=
      space.normal 'this.status.space :=
      space.normal 'prev.status.space :=
      quote.no 'this.status.quote :=
      urldate empty$
       { "there is url but no urldate in " cite$ * warning$ }
       { " [Accessed: " * urldate * "]" * }
      if$
    }
  if$
}
```

that is, add these lines, at the appropriate offset:

```text
urldate empty$
 { "there is url but no urldate in " cite$ * warning$ }
 { " [Accessed: " * urldate * "]" * }
if$
```

The changes output a warning, if you've added a url, but no urldate. Otherwise
it appends [Accessed: "your-date"] to your reference entry - sweet!

Save the file, and you're done!

We can knock up a quick example .tex file to test whether things are working:

```tex
\documentclass{article}
\begin{document}
Hello world! \cite{wadler1992comprehending}
\bibliographystyle{IEEEtran}
\bibliography{example}
\end{document}
```

and the corresponding example.bib:

```tex
@article{wadler1992comprehending,
  title={Comprehending monads},
  author={Wadler, P.},
  journal={Mathematical Structures in Computer Science},
  volume={2},
  number={04},
  pages={461--493},
  year={1992},
  publisher={Cambridge Univ Press},
  url={http://dx.doi.org/10.1017/S0960129500001560},
  urldate={04/12/2012}
}
Delete the above urldate line to see the new warning.
```

Then, compile the .pdf with something like:

```bash
pdflatex example
pdflatex example
bibtex example
pdflatex example
