#include<iostream>
#include<iomanip>
#include<map>
#include<unordered_map>
#include<set>
#include<unordered_set>
#include<vector>
#include<array>
#include<string>
#include<stack>
#include<queue>
#include<algorithm>
#include<cassert>
#include<functional>
#include<random>
#include<complex>
//#include<boost/multiprecision/cpp_int.hpp>
#define int int64_t
#define REP(i, a, b) for (int64_t i = (int64_t)(a); i < (int64_t)(b); i++)
#define rep(i, a) REP(i, 0, a)
#define EACH(i, a) for (auto i: a)
#define ITR(x, a) for (auto x = a.begin(); x != a.end(); x++)
#define ALL(a) (a.begin()), (a.end())
#define HAS(a, x) (a.find(x) != a.end())
#define Min(x) *min_element(ALL(x))
#define Max(x) *max_element(ALL(x))
#define Unique(L) (L.erase(unique(ALL(L)), L.end()))
using namespace std;
//typedef boost::multiprecision::cpp_int bigint;
const double EPS = 1e-9;
const double PI = acos(-1.0);

class Aho_Corasick {
private:
	vector<unordered_map<int, int>>automaton;
	unordered_multimap<int, int>accept;
	vector<int>failure;
	vector<vector<int>>pattern;

	void build_automaton() {

		// make trie
		rep(i, pattern.size()) {
			int index = 0;
			rep(j, pattern[i].size()) {
				const int c = pattern[i][j];
				const auto itr = automaton[index].find(c);
				if (itr == automaton[index].end()) {
					automaton[index][c] = automaton.size();
					automaton.resize(automaton.size() + 1);
				}
				index = automaton[index][c];
			}
			accept.insert({ index,i });
		}

		// make failure links using bfs
		failure.resize(automaton.size());
		queue<int>Q;
		ITR(x, automaton[0]) {
			failure[x->second] = 0;
			Q.push(x->second);
		}
		while (!Q.empty()) {
			int index = Q.front(); Q.pop();
			ITR(x, automaton[index]) {
				Q.push(x->second);
				int j = failure[index];
				//PMA *r = t->next[0];
				while (j != 0 && automaton[j].find(x->first) == automaton[j].end()) {
					j = failure[j];
				}
				auto itr = automaton[j].find(x->first);
				failure[x->second] = itr == automaton[j].end() ? 0 : itr->second;

				auto range = accept.equal_range(failure[x->second]);
				for (auto itr = range.first; itr != range.second; itr++) {
					accept.insert({ x->second,itr->second });
				}
			}
		}
	}

public:
	Aho_Corasick(const vector<string>&P) :automaton(1) {
		rep(i, P.size()) {
			pattern.push_back(vector<int>(P[i].size()));
			rep(j, P[i].size())pattern[i][j] = P[i][j];
		}
		build_automaton();
	}
	Aho_Corasick(const vector<vector<int>>&P) :automaton(1) {
		pattern = P;
		build_automaton();
	}

	vector<vector<int>>run(const vector<int>&S) {
		vector<vector<int>>ans(pattern.size());
		int index = 0;
		rep(i, S.size()) {
			const int c = S[i];
			while (index != 0 && automaton[index].find(c) == automaton[index].end()) {
				index = failure[index];
			}
			const auto itr = automaton[index].find(c);
			index = itr == automaton[index].end() ? 0 : itr->second;
			auto range = accept.equal_range(index);
			for (auto itr = range.first; itr != range.second; itr++) {
				ans[itr->second].push_back(i - pattern[itr->second].size() + 1);
			}
		}
		return ans;
	}
	vector<vector<int>>run(const string&S) {
		vector<int>T(S.size());
		rep(i, S.size())T[i] = S[i];
		return run(T);
	}
};
