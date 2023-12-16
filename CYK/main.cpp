#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
using Symbol = char;
using Word = std::vector<Symbol>;

struct Grammar {
    std::set<Symbol> m_Nonterminals;
    std::set<Symbol> m_Terminals;
    std::vector<std::pair<Symbol, std::vector<Symbol>>> m_Rules;
    Symbol m_InitialSymbol;
};
#endif

struct Point{ size_t x, y;};
struct Track{size_t R; Point A, B;};

void initFirstColumn(const Grammar & g, const Word & word, vector<vector<map<Symbol, Track>>> & dp)
{
    for(size_t i = 1; i <= word.size(); i++)
        for(size_t r = 0; r < g.m_Rules.size(); r++)
            if(g.m_Rules[r].second.size() == 1 && word[i-1] == g.m_Rules[r].second[0])
                    dp[i][1][g.m_Rules[r].first] = {r, {0, 0}, {0, 0}};

    // empty word
    for(size_t r = 0; r < g.m_Rules.size(); r++)
        if(g.m_Rules[r].second.empty())
            dp[1][0][g.m_Rules[r].first] = {r, {0, 0}, {0, 0}};
}


void fillDpCYK(const Grammar & g, const Word & word, vector<vector<map<Symbol, Track>>> & dp)
{
    initFirstColumn(g, word, dp);
    size_t n = word.size();

    for(size_t i = 2; i <= n; i++)
        for(size_t j = 1; j <= n - i + 1; j++)
            for(size_t k = 1; k <= i - 1; k++)
                for(auto s1 : dp[j][k])
                    for(auto s2 : dp[j+k][i-k])
                        for(size_t r = 0; r < g.m_Rules.size(); r++)
                            if(vector<Symbol> {s1.first, s2.first} == g.m_Rules[r].second)
                                    dp[j][i][g.m_Rules[r].first] = {r, {j, k}, {j+k, i-k}};
}


void backTrack(const std::vector<std::vector<std::map<Symbol, Track>>>& dp, size_t x, size_t y, char nonTerminal, const Grammar & g, vector<size_t> & result)
{
    if(dp[x][y].empty() || !dp[x][y].count(nonTerminal))
        return;

    auto mem = dp[x][y].find(nonTerminal)->second;
    auto rule = g.m_Rules[mem.R];

    result.push_back(mem.R);

    if(rule.second.size() == 2){
        backTrack(dp, mem.A.x, mem.A.y, rule.second[0], g, result);
        backTrack(dp, mem.B.x, mem.B.y, rule.second[1], g, result);
    }
}


std::vector<size_t> trace(const Grammar & g, const Word & word)
{
    size_t n = word.size();
    vector<size_t> result;
    vector<vector<map<Symbol, Track>>> dp(n + 2, vector<map<Symbol, Track>>(n + 2));

    fillDpCYK(g, word, dp);
    backTrack(dp, 1, n, g.m_InitialSymbol, g, result);

    return result;
}

#ifndef __PROGTEST__
int main()
{
//
//    Grammar g0{
//            {'A', 'B', 'C', 'S'},
//            {'a', 'b'},
//            {
//                    {'S', {'A', 'B'}},
//                    {'S', {'B', 'C'}},
//                    {'A', {'B', 'A'}},
//                    {'A', {'a'}},
//                    {'B', {'C', 'C'}},
//                    {'B', {'b'}},
//                    {'C', {'A', 'B'}},
//                    {'C', {'a'}},
//            },
//            'S'};
//
//
//    assert(trace(g0, {'b', 'a', 'a', 'b', 'a'}) == std::vector<size_t>({0, 2, 5, 3, 4, 6, 3, 5, 7}));
//    assert(trace(g0, {'b'}) == std::vector<size_t>({}));
//    assert(trace(g0, {'a'}) == std::vector<size_t>({}));
//    assert(trace(g0, {}) == std::vector<size_t>({}));
//    assert(trace(g0, {'a', 'a', 'a', 'a', 'a'}) == std::vector<size_t>({1, 4, 6, 3, 4, 7, 7, 7, 7}));
//    assert(trace(g0, {'a', 'b'}) == std::vector<size_t>({0, 3, 5}));
//    assert(trace(g0, {'b', 'a'}) == std::vector<size_t>({1, 5, 7}));
//    assert(trace(g0, {'c', 'a'}) == std::vector<size_t>({}));
//
//    Grammar g1{
//            {'A', 'B'},
//            {'x', 'y'},
//            {
//                    {'A', {}},
//                    {'A', {'x'}},
//                    {'B', {'x'}},
//                    {'A', {'B', 'B'}},
//                    {'B', {'B', 'B'}},
//            },
//            'A'};
//
//    assert(trace(g1, {}) == std::vector<size_t>({0}));
//    assert(trace(g1, {'x'}) == std::vector<size_t>({1}));
//    assert(trace(g1, {'x', 'x'}) == std::vector<size_t>({3, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2}));
//
//    Grammar g2{
//            {'A', 'B'},
//            {'x', 'y'},
//            {
//                    {'A', {'x'}},
//                    {'B', {'x'}},
//                    {'A', {'B', 'B'}},
//                    {'B', {'B', 'B'}},
//            },
//            'B'};
//
//    assert(trace(g2, {}).empty());
//    assert(!trace(g2, {'x'}).empty());
//    assert(!trace(g2, {'x', 'x'}).empty());
//    assert(!trace(g2, {'x', 'x', 'x'}).empty());
//
//
//    assert(trace(g2, {}) == std::vector<size_t>({}));
//    assert(trace(g2, {'x'}) == std::vector<size_t>({1}));
//    assert(trace(g2, {'x', 'x'}) == std::vector<size_t>({3, 1, 1}));
//    assert(trace(g2, {'x', 'x', 'x'}) == std::vector<size_t>({3, 3, 1, 1, 1}));
//
//    Grammar g3{
//            {'A', 'B', 'C', 'D', 'E', 'S'},
//            {'a', 'b'},
//            {
//                    {'S', {'A', 'B'}},
//                    {'S', {'S', 'S'}},
//                    {'S', {'a'}},
//                    {'A', {'B', 'S'}},
//                    {'A', {'C', 'D'}},
//                    {'A', {'b'}},
//                    {'B', {'D', 'D'}},
//                    {'B', {'b'}},
//                    {'C', {'D', 'E'}},
//                    {'C', {'b'}},
//                    {'C', {'a'}},
//                    {'D', {'a'}},
//                    {'E', {'S', 'S'}},
//            },
//            'S'};
//    assert(trace(g3, {}).empty());
//    assert(trace(g3, {'b'}).empty());
//    assert(!trace(g3, {'a', 'b', 'a', 'a', 'b'}).empty());
//    assert(!trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}).empty());
//
//    assert(trace(g3, {}) == std::vector<size_t>({}));
//    assert(trace(g3, {'b'}) == std::vector<size_t>({}));
//    assert(trace(g3, {'a', 'b', 'a', 'a', 'b'}) == std::vector<size_t>({1, 2, 0, 3, 7, 1, 2, 2, 7}));
//    assert(trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}) == std::vector<size_t>({1, 1, 0, 4, 8, 11, 12, 0, 5, 6, 11, 11, 0, 4, 9, 11, 7, 11, 7, 2, 2}));
}
#endif

