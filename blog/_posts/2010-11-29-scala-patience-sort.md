---
title: Scala implementation of Patience sort
---
Here's a simple Scala implementation of [Patience Sort][patience_sort], that I
wrote as a precursor to implementing the patience diff algorithm.

```scala
def sort[A <% Ordered[A]](source : Traversable[A]) : Traversable[A] = {
    val piles = ListBuffer.empty[Stack[A]]
    var count = 0;

    // Create ordered piles of elements
    for (elem <- source) {
        count += 1

        // Find leftmost "possible" pile
        val toAddPile = piles.find(p => p.head >= elem)

        // If there isn't a pile available, add a new one.
        toAddPile match {
            case Some(p) => p.push(elem)
            case _       => piles += Stack(elem)
        }
    }

    def PileOrdering = new Ordering[Stack[A]] {
        // Order stacks in reverse: from smallest head to largest head.
        def compare(a : Stack[A], b : Stack[A]) = b.head.compare(a.head)
    }

    // Use a priority queue, to simplify extracting minimum elements.
    val pq = new PriorityQueue[Stack[A]]()(PileOrdering)

    pq ++= piles

    // Return a new list, by taking the smallest stack head
    // until all stacks are empty.
    return (0 until count) map (_ => {
        val smallestList = pq.dequeue
        val smallestVal = smallestList.pop

        if (smallestList.length > 0) {
            pq.enqueue(smallestList)
        }

        smallestVal
    })
}
```

[patience_sort]: https://en.wikipedia.org/wiki/Patience_sorting
