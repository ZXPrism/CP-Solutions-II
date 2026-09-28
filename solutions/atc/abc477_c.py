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
    ok_pos = []
    for start in range(ns):
        ok = True
        for i in range(nt):
            if start + i >= ns or s[start + i] != t[i]:
                ok = False
                break
        if ok:
            ok_pos.append(start)


    for _ in range(q):
        l, r = map(int, input().split())
        l -= 1
        r -= 1
        first = bisect_left(ok_pos, l)
        if first != len(ok_pos) and ok_pos[first] + nt - 1 <= r:
            print("Yes")
        else:
            print("No")


t = 1
#t = int(input())
for _ in range(t):
    solve()
