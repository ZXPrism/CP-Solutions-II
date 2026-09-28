# ABC475 C — Walk the Line

- [Problem statement](https://atcoder.jp/contests/abc475/tasks/abc475_c)
- [Solution code](../../solutions/atc/abc475_c.py)
- Platform: AtCoder
- Techniques: interval enumeration, prefix sums, route simplification
- clist rating (AtCoder): not recorded

## Problem

Towns lie on a line, with positive distances between adjacent towns. Starting at town S, maximize the number of distinct towns visited within travel budget L. The starting town counts. N is at most 8,000.

## My discovery process

1. Movement is only between adjacent towns, so the visited towns must form a contiguous interval containing the starting town.
2. I did not see a direct way to find the maximum. Instead, I looked for representative cases to enumerate, such that other walks could not improve on those representatives.
3. I split the routes into four cases:
   - Walk left without turning.
   - Walk right without turning.
   - Walk left, then turn right.
   - Walk right, then turn left.
4. I handled the straight walks with two `while` loops. For the turning routes, I enumerated the left and right endpoints and checked the minimum travel cost using prefix sums.
5. I judged quadratic enumeration sufficient for N ≤ 8,000. I considered binary search as a possible direction for larger constraints, but did not develop that alternative here.

The central reduction was from arbitrary walks to a small family of representative routes. I considered why one turn should suffice, but only at a surface level; the full justification still deserves reflection.

## Cost model used in the code

The code uses zero-based town indices. For an interval `left <= s <= right`, let:

- `a = pre[s] - pre[left]`: distance from the starting town to the left endpoint.
- `b = pre[right] - pre[s]`: distance from the starting town to the right endpoint.

The left-first route costs `2*a + b`; the right-first route costs `a + 2*b`. The code checks the smaller cost:

```text
a + b + min(a, b) <= L
```

Equivalently, this is the interval's full span plus the distance from the start to the nearer endpoint. A feasible interval contributes `right - left + 1` towns.

## Where I actually got stuck

The longest sticking point was indexing: the objects being counted are towns, but the input distances describe roads between towns. Prefix sums made the off-by-one relationship feel more subtle.

A convention clarified during the discussion was:

```text
dist[i] = length of the road from town i to town i + 1
pre[i]  = coordinate of town i, with town 0 at coordinate 0
```

This separates distance from count:

```text
distance from left to right = pre[right] - pre[left]
number of towns             = right - left + 1
```

This attempt's reported bottleneck was translating the mathematical model into indices, rather than discovering the interval structure.

## What the discussion revealed

- My approach was structural: first identify what adjacency forces, then reduce the possibilities before enumerating them.
- The coverage argument needs more attention. Listing plausible route shapes is not yet a complete explanation of why they cover an optimum.
- Revisiting special cases after deriving a general formula is a useful extra exercise. The discussion pointed out that the straight walks are already included when an interval endpoint equals `s`, so the two `while` loops are redundant in the final algorithm. They still reflect my discovery process.
- These are observations from this attempt, not a general assessment of my ability or all my problem-solving habits.

## Reflection still to do

The following points were raised in discussion; I have not yet recorded my own completed arguments.

1. **One-turn sufficiency:** given an arbitrary walk with several turns, construct a walk with at most one turn that visits the same towns and travels no farther.
   - Hint already received: fix the extreme visited towns and consider which extreme the original walk reaches first. What travel distance is unavoidable in each order?
2. **Unifying the cases:** substitute `left == s`, `right == s`, and both equal to `s` into the interval formula. Explain why it includes both straight walks and staying at the starting town.

Preserve the distinction between the original intuition and the later proof: during solving, I believed one turn sufficed; making that argument precise is follow-up work.
