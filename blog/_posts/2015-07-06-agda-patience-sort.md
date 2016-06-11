---
title: Agda Patience Sort
---

Having recently decided to [learn myself an Agda][learn you an agda], I thought
that implementing a [Patience sort][patience sort] would be a neat example to
try and program.  Having [already implemented][scala patience sort] a
[Scala][scala] Patience sort, I was interested to see how I could tighten up
the implementation by using a dependently-typed language such as [Agda][agda].

I'm going to go reasonably fast when describing my implementation, feel free to
take a look at the same file [on Github][github link] if you'd like to explore
it using the excellent [Emacs agda-mode][agda-mode].

First, let's define the module, and import a bunch of stuff from the 
[standard library][standard library].

```agda
module Patience where
  open import Relation.Binary using (DecTotalOrder; module DecTotalOrder;
    IsDecTotalOrder; module IsDecTotalOrder; Rel; module IsTotalOrder;
    module IsPartialOrder; module IsPreorder)
  open import Relation.Binary.Core hiding (refl)
  open import Level hiding (_⊔_ ; suc) renaming (zero to lzero)
  open import Data.Nat using (ℕ; z≤n; s≤s; suc; _+_)
    renaming (decTotalOrder to decTotalOrderℕ; _≤_ to _≤ℕ_)
  open import Data.List using (List; []; _∷_)
  open import Data.Product using (Σ; _,_; _×_)
  open import Function using (_∘_)
  open import Data.Sum using (inj₁; inj₂)
  open import Data.Nat.Properties.Simple using (+-suc; +-assoc; +-comm)
  open import Relation.Binary.PropositionalEquality as PropEq
    using (_≡_; subst; cong; sym)
  open PropEq.≡-Reasoning using (begin_; _≡⟨_⟩_; _∎)
```

Now, using a total-ordered set, X, I create a "lifted" data-type that adds a
maximum element to X. I create an ordering on the lifted type (i.e. that
compares all elements as less than the maximum, and otherwise defers to the
underlying ordering), and prove that it is transitive. I could prove other
properties, but do not require them, so save space by not doing so.

```agda
  -- A module that will contain a type of ordered "piles", analogous
  -- to piles of cards in Patience (though with the stronger
  -- restriction that piles are ordered by the top element).
  module Piles (dto : DecTotalOrder lzero lzero lzero) where
    open DecTotalOrder dto public renaming (Carrier to X)

    -- Append maximum element to X
    data X⊤ : Set where
      ⊤ : X⊤
      ⟦_⟧ : (x : X) → X⊤ -- lift an element of X

    -- Define an ordering relation on the resulting type
    data _⟦≤⟧_ : Rel X⊤ Level.zero where
      x≤⊤ : ∀ {x} → x ⟦≤⟧ ⊤
      ⟦_⟧ : ∀ {x y} → (x ≤ y) → ⟦ x ⟧ ⟦≤⟧ ⟦ y ⟧ -- lift an ordering on X

    -- Ensure the order is transitive
    ⟦≤⟧-trans : Transitive _⟦≤⟧_
    ⟦≤⟧-trans _ x≤⊤ = x≤⊤
    ⟦≤⟧-trans ⟦ p ⟧ ⟦ q ⟧ =  ⟦ trans p q ⟧
```

We will want to use this lifted type to track the minimum element in a set of
Piles, and we will therefore need to find the minimum of two elements of this
type.

```agda
    -- Minimum w.r.t. X⊤
    min : X⊤ → X⊤ → X⊤
    min ⊤ x = x
    min x ⊤ = x
    min ⟦ x ⟧ ⟦ y ⟧ with total x y
    ... | inj₁ x≤y = ⟦ x ⟧
    ... | inj₂ y≤x = ⟦ y ⟧
```

We can now define a type of ordered vectors, which records the number of
elements it contains. Notice how we leverage dependent types, taking a proof
object argument that ensures that elements that are `cons`d onto the vector are
indeed smaller than the existing head of the vector.

```agda
    -- Define a type of bounded, ordered vectors
    data OVec : X⊤ → ℕ → Set where
      ε : OVec ⊤ 0
      cons : ∀ {t n} → (x : X) → (⟦ x ⟧ ⟦≤⟧ t) → OVec t n → OVec ⟦ x ⟧ (suc n)
```

Now, using ordered vectors (individual piles), we can define a type representing
a (ordered by top element) set of piles, again tracking the minimum element and
the number of elements in the Piles.

```agda
    -- Define a type of bounded, ordered (by head elem) vectors of
    -- (non-empty) OVecs.
    data Piles : X⊤ → ℕ → Set where
      ε : Piles ⊤ 0
      consP : ∀{x t n m} → OVec ⟦ x ⟧ (suc n) → (⟦ x ⟧ ⟦≤⟧ t) → Piles t m
        → Piles ⟦ x ⟧ (suc n + m)
```

Later, we will require a couple of lemmas:

```agda
    -- Given a proof of equivalence between two elements of ℕ, we can transform
    -- Piles with one size to the other.
    cong-Piles : ∀ {l} {n m : ℕ} → (n ≡ m) → Piles l n  → Piles l m
    cong-Piles {l} p fn = subst (Piles l) p fn

    ⟦≤⟧-resp-min : ∀ {x y z} → x ⟦≤⟧ y → x ⟦≤⟧ z → x ⟦≤⟧ (min y z)
    ⟦≤⟧-resp-min x≤⊤ q = q
    ⟦≤⟧-resp-min ⟦ p ⟧ x≤⊤ = ⟦ p ⟧
    ⟦≤⟧-resp-min {_} {⟦ y ⟧} {⟦ z ⟧} p q with total y z
    ... | inj₁ y≤z = p
    ... | inj₂ z≤y = q
```

And now, we can move onto the meat of the implementation, inserting single
elements, and complete piles into existing (possibly empty) `Piles`:

```agda
    -- We can insert a single element into existing Piles...
    insertElemPiles : ∀ {l n} → (x : X) → Piles l n
      → Piles (min ⟦ x ⟧ l) (suc n)
    insertElemPiles x ε = consP (cons x x≤⊤ ε) x≤⊤ ε
    insertElemPiles x (consP {x′} {t} {n} {m} p x′≤t ps) with total x x′
    ... | inj₁ x≤x′ = consP (cons x ⟦x≤x′⟧ p) (⟦≤⟧-trans ⟦x≤x′⟧ x′≤t) ps
      where
        ⟦x≤x′⟧ = ⟦ x≤x′ ⟧
    ... | inj₂ x′≤x = cong-Piles ≡ℕ (consP p (⟦≤⟧-resp-min ⟦ x′≤x ⟧ x′≤t) rec)
      where
        rec = insertElemPiles x ps

        ≡ℕ : suc (n + suc m) ≡ suc (suc (n + m))
        ≡ℕ = cong suc (+-suc n m)

    -- ...and insert a Pile into Piles.
    insertPilePiles : ∀ {x t n m} → OVec ⟦ x ⟧ (suc n)
      → Piles t m → Piles (min ⟦ x ⟧ t) (suc n + m)
    insertPilePiles {t = ⊤} p ε = consP p x≤⊤ ε
    insertPilePiles {x} {⟦ t ⟧} {n₁} p (consP {n = n₂} {m = m₁} q x≤t′ qs)
      with total x t
    ... | inj₁ x≤t = consP p ⟦ x≤t ⟧ (consP q x≤t′ qs)
    ... | inj₂ t≤x = cong-Piles ≡ℕ (consP q (⟦≤⟧-resp-min ⟦ t≤x ⟧ x≤t′) rec)
      where
       rec = insertPilePiles p qs

       -- Since we build the piles in two different ways, we need to
       -- prove that the sizes are equivalent, using this lemma about ℕ.
       permute-suc-xyz : {x y z : ℕ} → x + suc (y + z) ≡ y + suc (x + z)
       permute-suc-xyz {x} {y} {z} =
         begin
          x + suc (y + z)
         ≡⟨ +-suc x (y + z) ⟩
          suc (x + (y + z))
         ≡⟨ cong suc (sym (+-assoc x y z)) ⟩
         suc ((x + y) + z)
         ≡⟨ cong suc (cong (λ x → x + z) (+-comm x y)) ⟩
         suc ((y + x) + z)
         ≡⟨ cong suc (+-assoc y x z) ⟩
         suc (y + (x + z))
         ≡⟨ sym (+-suc y (x + z)) ⟩
         y + suc (x + z)
         ∎

       ≡ℕ = cong suc (permute-suc-xyz {n₂} {n₁} {m₁})
   ```

It'd be nice to use some of [Connor McBride's ideas][connor] to avoid the 
Green Slime[^1] (i.e. the use of `min` in the return types). However, I seem to
require the tight bounds, and didn't manage to effectively get rid of the calls
to `min`. If you have any ideas or suggestions on this, please get in
contact!

Once we have inserted elements into `Piles`, we can remove elements one-by-one,
to obtain an ordered `List` of elements. When removing a single element (to
obtain the minimum element and a new `Piles`), we have to use a [dependent-sum
type][dependent sum] to encode the fact that the lower bound of the resulting
`Piles` is the removed element. We maintain the ordering by pushing any
non-empty vectors back into the `Piles` to be consumed in-order.

```agda
    -- From any non-empty Piles, we can remove the smallest element, to obtain
    -- a pair of that element, and a one-smaller Piles.
    removeOne : ∀ {l n} → Piles l (suc n) → X × (Σ X⊤ (λ l' → Piles l' n))
    removeOne (consP (cons x _ xs) _ ps) with xs
    ... | ε = x , _ , ps
    ... | cons y q ys = x , _ , insertPilePiles (cons y q ys) ps

    -- By repeatedly removing a single element, we can convert Piles in a List.
    pilesToList : ∀ {l n} → Piles l n → List X
    pilesToList ε = []
    pilesToList {n = suc m} p with removeOne p
    ... | x , _ , ps = x ∷ pilesToList ps
 ```

 When converting an arbitrary list of elements into `Piles`, we cannot up-front
 know the lower bound or size of the `Piles`, thus we can use an existential
 type to "hide" these values. To create a existential Piles, we repeatedly
 insert the elements of the list, one-by-one:

```agda
    -- We can existentially hide the bound and size of Piles...
    data ∃Piles : Set where
      ∃<_> : ∀ {l n} → Piles l n → ∃Piles

    -- ...and can convert an arbitrary List into a Piles with unknown bound and
    -- size, by repeatedly inserting one element at a time.
    listToPiles : List X → ∃Piles
    listToPiles [] = ∃< ε >
    listToPiles (x ∷ xs) with listToPiles xs
    ... | ∃< ps > = ∃< insertElemPiles x ps >
```

Now, Patience sorting is simply defined:

```agda
    -- Finally, patience sort is simply creating Piles from an arbitrary List,
    -- before converting back to an (ordered) List.
    patienceSort : List X → List X
    patienceSort ls with listToPiles ls
    ... | ∃< ps > = pilesToList ps
```

And all that remains is to demonstrate some example sortings of natural
numbers. Recall that Piles are parameterised by the (totally-ordered) elements
they contain.

```agda
  -- Let's create Piles of ℕ
  module Pilesℕ = Piles decTotalOrderℕ

  -- and some test lists.
  xs = Pilesℕ.patienceSort (5 ∷ 4 ∷ 3 ∷ 2 ∷ 1 ∷ [])
  ys = Pilesℕ.patienceSort []
  zs = Pilesℕ.patienceSort (4 ∷ 4 ∷ 1 ∷ 5 ∷ 3 ∷ [])

  sortedXs sortedYs sortedZs : List ℕ
  sortedXs = 1 ∷ 2 ∷ 3 ∷ 4 ∷ 5 ∷ []
  sortedYs = []
  sortedZs = 1 ∷ 3 ∷ 4 ∷ 4 ∷ 5 ∷ []

  -- And check that sorting via patience does the right thing.
  xsOk : xs ≡ sortedXs
  xsOk = PropEq.refl

  ysOk : ys ≡ sortedYs
  ysOk = PropEq.refl

  zsOk : zs ≡ sortedZs
  zsOk = PropEq.refl
```

[learn you an agda]: http://learnyouanagda.liamoc.net/
[patience sort]: https://en.wikipedia.org/wiki/Patience_sorting
[scala]: http://www.scala-lang.org/
[agda]: http://wiki.portal.chalmers.se/agda/pmwiki.php
[scala patience sort]: {% post_url 2010-11-29-scala-patience-sort%}
[github link]: https://github.com/owst/agda-patience-sort/blob/master/Patience.agda
[agda-mode]: http://wiki.portal.chalmers.se/agda/pmwiki.php?n=Main.QuickGuideToEditingTypeCheckingAndCompilingAgdaCode
[standard library]: https://github.com/agda/agda-stdlib/
[connor]: https://personal.cis.strath.ac.uk/conor.mcbride/Pivotal.pdf
[^1]: Functions are highlighted in Green in Agda. Such functions are "opaque" to Agda when performing unification, and are discouraged from being used in data type indices such as here, hence the term "Green Slime".
[dependent sum]: http://ncatlab.org/nlab/show/dependent+sum+type
