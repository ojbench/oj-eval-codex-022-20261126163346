# Solution

Query all ten triples among the first five positions. Enumerate their 120
possible increasing orders. For a proposed order with values a < b < c < d < e,
the answers include a+c, a+e, and c+e, which recover a, c, and e by subtraction
and division by two. The answers b+e and a+d recover b and d. Check every
triple and the value bounds to identify the valid assignment.

These answers uniquely determine the five values: the unique minimum-answer
triple contains the lowest three positions, and the unique maximum-answer
triple contains the highest three. Their intersection identifies the middle
position. Queries containing the middle, one low, and one high position
distinguish the two low positions and the two high positions. Thus the order,
and consequently the recovered values, are unique.

Choose the closest pair L < R among these five values and a third known value
C. No known value lies strictly between L and R, so C is outside the interval.
For each remaining position with unknown value X, query the pair and X:

- An answer below L+R means X < L, and X = answer-R.
- An answer above L+R means X > R, and X = answer-L.
- An answer equal to L+R means L < X < R. If C < L, query C,L,X;
  otherwise query C,R,X. This returns C+X. Keep the shorter of [L,X]
  and [X,R] as the new pair.

The fixed anchor C remains outside every subsequent interval. Each extra query
at least halves the positive integer gap. Initially the closest of the four
adjacent gaps is at most floor((10^9-1)/4) = 249999999, so there are at most
27 extra queries. The total is at most n+32. There is also at most one extra
query per remaining position, giving a total of at most 2n. Hence the solution
always meets the full-score threshold min(n+35, 2n).

Time complexity is O(n), with a constant-size permutation search. Storage is
O(n). All state is local to each invocation; arithmetic uses 64-bit integers.

## Local validation

Run `g++ -std=c++11 -O2 -Wall -Wextra -pedantic tests/local.cpp -o /tmp/guess-test`
and `/tmp/guess-test`. The simulator checks valid queries, the returned hash,
and the full-score query bound. It covers every ordered five-element subset
of 1 through 9, 2,000 deterministic random cases (including length 10,000),
and repeatedly halving intervals. The published sample-data path is absent
from this environment.

`source.h` is the original grader interface. `src.hpp` forwards to it for
drivers using the filename mentioned in the repository submission instructions.
