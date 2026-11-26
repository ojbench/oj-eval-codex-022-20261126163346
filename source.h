#ifndef GUESS_SOURCE_H
#define GUESS_SOURCE_H

#include <algorithm>
#include <array>
#include <vector>

int query(int x, int y, int z);

inline int guess(int n, int Taskid) {
    (void)Taskid;
    int sums[5][5][5] = {};
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            for (int k = j + 1; k < 5; ++k)
                sums[i][j][k] = query(i + 1, j + 1, k + 1);
    auto sum = [&](int i, int j, int k) {
        std::array<int, 3> indices = {{i, j, k}};
        std::sort(indices.begin(), indices.end());
        return sums[indices[0]][indices[1]][indices[2]];
    };

    std::vector<long long> values(n + 1);
    std::array<int, 5> order = {{0, 1, 2, 3, 4}};
    // Try each ordering. The extreme-pair sums determine all five values.
    do {
        long long sorted[5];
        long long twice_min = static_cast<long long>(sum(order[0], order[1], order[2]))
                            + sum(order[0], order[1], order[4])
                            - sum(order[2], order[3], order[4]);
        if (twice_min % 2 != 0) continue;
        sorted[0] = twice_min / 2;
        sorted[2] = sum(order[0], order[1], order[2]) - sorted[0];
        sorted[4] = sum(order[0], order[1], order[4]) - sorted[0];
        sorted[1] = sum(order[1], order[2], order[4]) - sorted[4];
        sorted[3] = sum(order[0], order[1], order[3]) - sorted[0];
        bool valid = sorted[0] >= 1 && sorted[4] <= 1000000000;
        for (int i = 1; i < 5; ++i)
            valid = valid && sorted[i - 1] < sorted[i];
        for (int i = 0; i < 5; ++i)
            for (int j = i + 1; j < 5; ++j)
                for (int k = j + 1; k < 5; ++k)
                    valid = valid && sum(order[i], order[j], order[k]) == sorted[i] + sorted[k];
        if (valid) {
            for (int i = 0; i < 5; ++i) values[order[i] + 1] = sorted[i];
            break;
        }
    } while (std::next_permutation(order.begin(), order.end()));

    int left = 1, right = 2;
    long long gap = 1000000001LL;
    for (int i = 1; i <= 5; ++i)
        for (int j = 1; j <= 5; ++j)
            if (values[i] < values[j] && values[j] - values[i] < gap) {
                left = i;
                right = j;
                gap = values[j] - values[i];
            }
    int outside = 1;
    while (outside == left || outside == right) ++outside;

    for (int i = 6; i <= n; ++i) {
        long long answer = query(left, right, i);
        long long pair_sum = values[left] + values[right];
        if (answer < pair_sum) values[i] = answer - values[right];
        else if (answer > pair_sum) values[i] = answer - values[left];
        else {
            // The fixed outside anchor stays outside every nested interval.
            int endpoint = values[outside] < values[left] ? left : right;
            values[i] = query(outside, endpoint, i) - values[outside];
            if (values[i] - values[left] <= values[right] - values[i]) right = i;
            else left = i;
        }
    }
    const long long modulus = 998244353;
    long long result = 0, power = 1;
    for (int i = 1; i <= n; ++i) {
        power = power * 233 % modulus;
        result = (result + values[i] * power) % modulus;
    }
    return static_cast<int>(result);
}

#endif
