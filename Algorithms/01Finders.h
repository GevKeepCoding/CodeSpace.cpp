#pragma once
#include <string>

using namespace std;
namespace Finders01 {
	//https://www.youtube.com/watch?v=6TW3JSVEJQE&list=RDCMUCyFWoLmPTgZ3BkHIKMRSV1g&index=7
	// O(N*M) and we need backup of input txt(it can't be stream)
	int search(string pat, string txt) {
		int i, N = txt.length();
		int j, M = pat.length();

		for (i = 0, j = 0; i < N && j < M; ++i) {
			if (txt.at(i) == pat.at(j)) ++j;
			else { i -= j; j = 0; }
		}
		if (j == M) return i - M;
		else		return N;
	}

	// O(N) Linear: Knuth-Morris-Pratt algorithm is using DFA
	class KMP {
		string pat;
		int** dfa;
		int M;
		int search(string txt) {
			int i, j, N = txt.length();
			for (i = 0, j = 0; i < N && j < M; ++i)
			{
				j = dfa[txt.at(i)][j];
			}
			if (j == M) return i - M;
			else		return N;
		}

		// constructing DFA for ABABAC string:
		KMP(string pat) {
			this->pat = pat;
			M = pat.length();
			auto* p = new int* [3];
			dfa = p;
			p[0] = new int[M];
			p[1] = new int[M];
			p[2] = new int[M];

			dfa[pat.at(0)][0] = 1;
			for (int X = 0, j = 1; j < M; ++j) {
				for (int c = 0; c < 3; ++c) {
					dfa[c][j] = dfa[c][X];
				}
				dfa[pat.at(j)][j] = j + 1;
				X = dfa[pat.at(j)][X];
			}

		}
	};
	// Boyer-Moore: N/M in best case but may be N*M in worste case...??
	
	// Rabin-Karp fingerprint search


}