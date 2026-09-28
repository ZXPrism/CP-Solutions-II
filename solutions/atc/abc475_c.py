import sys, heapq
from bisect import bisect_left, bisect_right
from collections import deque, defaultdict, Counter
from math import log2, log10, gcd

input = sys.stdin.readline


def solve():
    n, s, l = map(int, input().split())
    s -= 1
    dist = list(map(int, input().split()))

    pre = [0] * n
    for i in range(n - 1):
        pre[i + 1] = pre[i] + dist[i]

    ans = 0

    curr = s
    total = 0
    while curr > 0:
        total += dist[curr - 1]
        if total > l:
            break
        curr -= 1
    ans = max(ans, s - curr + 1)

    curr = s
    total = 0
    while curr < n - 1:
        total += dist[curr]
        if total > l:
            break
        curr += 1
    ans = max(ans, curr - s + 1)

    for right in range(s, n):
        for left in range(s + 1):
            if pre[right] - pre[left] + min(pre[right] - pre[s], pre[s] - pre[left]) <= l:
                ans = max(ans, right - left + 1)

    print(ans)


t = 1
#t = int(input())
for _ in range(t):
    solve()
