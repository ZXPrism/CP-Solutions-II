import sys, heapq
from bisect import bisect_left, bisect_right
from collections import deque, defaultdict, Counter
from math import log2, log10, gcd

input = sys.stdin.readline


def solve():
    q = int(input())
    s = input().strip()
    t = input().strip()

    ns = len(s)
    nt = len(t)
    next_start = [ns] * ns
    for start in range(ns):
        ok = True
        for i in range(nt):
            if start + i >= ns or s[start + i] != t[i]:
                ok = False
                break
        if ok:
            next_start[start] = start

    for i in range(ns - 2, -1, -1):
        next_start[i] = min(next_start[i], next_start[i + 1])


    for _ in range(q):
        l, r = map(int, input().split())
        l -= 1
        r -= 1

        print("Yes" if next_start[l] + nt - 1 <= r else "No")


t = 1
#t = int(input())
for _ in range(t):
    solve()
