# ABC477 C — Range Search Query

- [Problem statement](https://atcoder.jp/contests/abc477/tasks/abc477_c)
- [Solution code](../../solutions/atc/abc477_c.py)
- [Alternative solution: constant-time queries](../../solutions/atc/abc477_c_alt.py)
- Platform: AtCoder
- Techniques: occurrence preprocessing, binary search, suffix minimum / next-occurrence table
- clist rating (AtCoder): not recorded

## Problem context

Given fixed strings S and T, answer queries asking whether T occurs entirely inside a specified interval of S. T has at most 10 characters.

The refined account below is based on my code and original notes, with clarifications from the assistant discussion. My original wording is preserved separately at the end.

## Reasoning progression

1. Seeing a string problem initially brought techniques such as KMP and tries to mind.
2. Rereading the task drew attention to the short length of T. This prompted reconsideration of what machinery was actually needed.
3. I considered searching inside each query interval. While stuck, the idea of preprocessing suddenly came to mind as something that might work; I did not identify a deliberate chain that produced it.
4. There are only `max(0, n - m + 1)` possible full-match starting positions, where `n = len(S)` and `m = len(T)`.
5. The crucial clarification was what to preprocess: not the answers to varying queries, but the positions where T occurs in S. These matches are independent of query bounds.
6. Once occurrences are represented by sorted starting positions, each query becomes a question about whether any one of those positions describes a match contained within the interval.
7. Only the earliest occurrence starting at or after the left bound needs to be checked. Binary search finds it.

Later clarification: the switch to preprocessing was intuitive. The idea simply popped up while I was stuck: “this may work.” Repeated work across queries explains why preprocessing is useful in retrospect, but I did not report it as the conscious trigger. The logical work afterward was identifying what to preprocess and proving how the stored positions answer each query.

## Why one candidate is sufficient

Use zero-based inclusive query bounds `[l, r]`. An occurrence starting at `p` occupies `[p, p + m - 1]`, so it fits exactly when:

```text
l <= p <= r - m + 1
```

`bisect_left(ok_pos, l)` finds the first occurrence starting **at or after** `l`, including equality.

- If no such occurrence exists, the answer is No.
- If its end is at most `r`, it is a witness that the answer is Yes.
- Otherwise, all later occurrences end even later, because every occurrence has the same length m. None can fit, so the answer is No.

The original notes already contain this rejection argument. The assistant discussion made its dependency on equal occurrence lengths explicit.

## Complexity correction

Preprocessing is not automatically linear merely because it happens before queries. The nested loops in this solution test up to m characters at each of n starting positions, taking `O(n*m)` time in the worst case.

The constraint `m <= 10` therefore matters: it makes direct matching inexpensive and bounds preprocessing by a small constant times n. The original claim that this constraint is useless is corrected here, while preserved below as part of the original reasoning.

Let k be the number of occurrences. The total time is `O(n*m + q*log(k + 1))`, with `O(k)` auxiliary storage for occurrence positions, excluding the input strings. Overlapping occurrences are retained because each starting position is checked independently.

## Implementation details worth retaining

- `ok_pos` is already sorted because candidate starts are scanned in increasing order.
- Query endpoints are converted from one-based to zero-based exactly once.
- An occurrence's inclusive end is `start + m - 1`.
- The code checks that the binary-search result exists before indexing it.
- With `sys.stdin.readline`, remove the line ending from string input. The current `.strip()` also removes leading and trailing whitespace; use it when that whitespace cannot be meaningful input, rather than treating it as a universal rule for string problems.

## Transferable reasoning situation

**Situation:** Many queries ask about different windows over fixed data.

**Useful question:** What query-independent facts can I compute once, and what information must I retain to answer the varying bounds?

**Move in this problem:** Retain occurrence positions, then answer containment queries using their order.

**Proof pattern:** Among candidates satisfying one bound, find the one whose other endpoint is best. If that candidate fails the second bound, prove that every remaining candidate fails too. Here, equal lengths make later starts imply later ends.

These descriptions preserve the reasoning context beyond the technique labels “string matching” and “binary search.” They are retrospective summaries from the discussion, not claims that I explicitly formulated all of them while solving.

## Follow-up: precompute the lookup itself

**Origin:** After solving, I asked whether queries could be faster, having already considered KMP for faster matching preprocessing. The assistant suggested a next-occurrence table. I then implemented it in the alternative solution linked above. This was an assisted extension, not part of my original discovery.

The binary search returns the earliest occurrence starting at or after `l`. That result depends only on `l` and the fixed strings; `r` is used afterward to check containment. There are only n possible left endpoints, so we can store the lookup result for each one without precomputing all quadratic-many query intervals.

Define `next_start[i]` as the earliest occurrence start at or after i, or n if no such occurrence exists.

My alternative implementation first initializes every entry to n and sets `next_start[p] = p` wherever a match starts. It then scans backward:

```python
for i in range(ns - 2, -1, -1):
    next_start[i] = min(next_start[i], next_start[i + 1])
```

Why this works: an occurrence at or after i either starts at i or starts at or after i + 1. The next entry already holds the earliest start in the latter group. Taking the minimum gives the earliest start for i. The last entry is already correct after matching initialization, which supplies the base case for the backward scan.

Each query now needs only:

```python
next_start[l] + nt - 1 <= r
```

The original earliest-occurrence proof still applies. Only the way its candidate is obtained changes. With a nonempty pattern, the sentinel n cannot pass the check because every valid right endpoint is less than n.

### Cost and tradeoff

| Implementation | Preprocessing | Per query | Auxiliary space |
| --- | --- | --- | --- |
| Original: direct matching + binary search | O(n*m) | O(log(k + 1)) | O(k) |
| Implemented alternative: direct matching + next-start table | O(n*m + n) | O(1) | O(n) |
| Possible extension: KMP + next-start table | O(n + m) | O(1) | O(n + m) |

The alternative takes `O(n*m + n + q)` total time. With m at most 10, this is linear in n + q under the problem's bound. KMP is a possible further generalization, not implemented here. The next-start table uses more storage when occurrences are sparse; its advantage is constant-time queries, not a guarantee of lower wall-clock time for every input.

### What to carry forward

**Situation:** A static query repeatedly performs the same kind of search, whose result depends on only one part of the query.

**Useful question:** Can I precompute that search result for every possible value of its input? Can neighboring inputs share work when building the table?

**Concrete connection:** In this problem, the left endpoint selects the earliest candidate, while the right endpoint only validates it. Adjacent left endpoints share almost the entire suffix of candidate positions, enabling a backward scan.

The new step is from “precompute where matches occur” to “precompute the result of looking up the next match.” This explanation is a retrospective derivation of the assistant's suggestion; I did not independently discover it during the original attempt.

## Original solving notes (verbatim)

The following notes are preserved unchanged, including the complexity claim corrected above.

---

if using python, then for string based problems, we almost always need to strip, or some invisible characters will be annoying

quickly surfed the statement, at first sight i immediately think, this looks like some string matching problem, then some techniques popping out, like KMP, trie and more.

then I re-read the problem, finding T is at most 10 characters. hmm, this should be the key point, but how to make use of this?

in each query, given a substring of S, we need to check if it contains T. normal search would take O(n), where n is the length of S.

hmm, actually T has at most n - n_t + 1 positions inside S.

oh! so we can just preprocess the answers. the 10 characters limit is useless here. since preprocess always take O(n).

but there is a caveat. we have not specified what to preprocess. is it really the answer? no. we can only preprocess the positions where t appears as a substring of s.

but real queries have bounds. and the answer is true, if the bounds covers at least one occurence of t inside s. how to check the latter?

greedily, we'd find first starting position right after l. if that substring goes after r, then the answer must be no, since subsequent substrings must also go after r. opposite, if we can find it, then the answer is simply yes.

so we can do a binary search on the starting positions, with the condition on l.

solved.
