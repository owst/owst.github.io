---
title: "Research"
weight: 3
---

My research topics to date have been functional programming languages (in particular, methods of
implementing IO) and model checking of Petri Nets (in particular, *compositional* reachability
checking).

**Compositional model checking of Petri Nets**:

My PhD research focussed on compositionally checking reachability in systems modeled by Petri nets.

- *Compositional Reachability in Petri Nets*, [Reachability Problems '14][rp]

    with Julian Rathke and Paweł Sobociński, [paper (PDF)][rppaper]
    - A divide-and-conquer algorithm for a modified version of the reachability/coverability
      problem in 1-bounded Petri nets that relies on the compositional algebra of nets with
      boundaries. We consider the algebraic decomposition of the net of interest as part of the
      input, allowing our technique to exploit modern techniques for NFA minimisation and
      equivalence checking, giving impressive scalable performance to systems with many (repeated)
      components. We formally prove our technique correct and demonstrate the scalable performance
      of our implementation by contrasting with existing state-of-the-art tools that exploit
      partial order reduction techniques on the global net.

- *A Programming Language for Spatial Distribution of Net Systems*, [Petri Nets '14][pn]

    with Paweł Sobociński, [paper (PDF)][pnpaper]
    - Petri nets famously expose concurrency directly in their statespace. Building on the work on
      the compositional algebra of nets with boundaries, we show how an algebraic decomposition
      allows one to expose both concurrency and spatial distribution in the statespace. We
      introduce a simple domain specific language (DSL), PNBml, for the construction of nets in
      terms of their components, using PNBml to express several well-known parametric examples.

- *Penrose: Putting Compositionality to Work for Petri Net Reachability*, [CALCO'13][calco]

    with Paweł Sobociński, [paper (PDF)][calcopaper]
    - We outline a novel technique for reachability checking in Petri Nets, exploiting
      compositionality to increase performance for some well-known examples. We introduce a tool
      that uses this technique, Penrose, discuss some design details in its implementation.

**IO in functional programming languages**:

My masters research project investigated methods of implementing input/output (I/O) in functional
programming languages: [report (PDF)][report].


[calco]: http://link.springer.com/chapter/10.1007/978-3-642-40206-7_29
[calcopaper]: {{site.static_dir}}/research/calco13.pdf
[pn]: http://link.springer.com/chapter/10.1007%2F978-3-319-07734-5_9
[pnpaper]: {{site.static_dir}}/research/pn14.pdf
[rp]: http://link.springer.com/chapter/10.1007%2F978-3-319-11439-2_18
[rppaper]: {{site.static_dir}}/research/rp14.pdf
[report]: {{site.static_dir}}/research/masters_report.pdf