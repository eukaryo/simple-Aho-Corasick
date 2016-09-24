#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
#include<queue>
#define REP(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rep(i, a) REP(i, 0, a)
#define ITR(x, a) for (auto x = a.begin(); x != a.end(); x++)
using namespace std;

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
					automaton[index][c] = int(automaton.size());
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
				while (j != 0 && automaton[j].find(x->first) == automaton[j].end()) {
					j = failure[j];
				}
				const auto itr = automaton[j].find(x->first);
				failure[x->second] = itr == automaton[j].end() ? 0 : itr->second;

				auto range = accept.equal_range(failure[x->second]);
				for (auto y = range.first; y != range.second; y++) {
					accept.insert({ x->second,y->second });
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
				ans[itr->second].push_back(i - int(pattern[itr->second].size()) + 1);
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
