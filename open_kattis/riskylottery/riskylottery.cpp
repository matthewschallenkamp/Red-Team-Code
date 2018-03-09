#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

double nchoosek(double n, double k)
{
	double res = 1;
	for(int i = 0; i < k; i++)
		res *= (n - i) / (k - i);
	return res;
}

pair<int, double> best_choice(vector<double> &strategy, int ct)
{
	//how can we do this without exploding our complexity?
	//lets say we have a counts left. at each count we are going to store the probability that we have that count left
	vector<double> cts_left_prob(ct + 1, 0.0), winprob(strategy.size(), 0.0);
	cts_left_prob[ct] = 1.0;
	//now, for each item that we can choose and each count that we can choose
	//we are going to calc the probability that the remaining n items choose k of them, and increment that into our
	//cts left
	int bi = strategy.size() - 1;
	double f_chance_sum = 1.0; //total sum of choices forward
	for(int i = 0; i < strategy.size(); i++)
	{
		//per choice, we need to consider all the previous choices gone
		for(int cts_considered = 1; cts_considered <= ct; cts_considered++) // 0 count doesnt matter
		{
			double tot = 0.0;
			//someone takes a single item, no one else takes that item, and 
			double winhere = (strategy[i] / f_chance_sum) * pow(1.0 - (strategy[i] / f_chance_sum), cts_considered - 1);
			// cerr << winhere << " ";
			winhere *= cts_considered * cts_left_prob[cts_considered];
			// cerr << "wh " << i << " " << cts_considered << " " << winhere << " " << cts_left_prob[cts_considered] << " " << f_chance_sum << endl;
			winprob[i] += winhere;
			tot += winhere;
			//chance to take each count
			//chance to take 0: stays
			//chance to take 1: is removed totally -> becomes a win
			//chance to take 2+: moves into some spot with less counts
			for(int totake = 2; totake <= cts_considered; totake++)
			{
				//look ahead x items, we want chance that spot takes totake of strat i, the remainder not strat i, and has different combinations chosen
				double multiple_here = pow(strategy[i] / f_chance_sum, totake) * pow(1.0 - (strategy[i] / f_chance_sum), cts_considered - totake)
						* nchoosek(cts_considered, totake) * cts_left_prob[cts_considered];
				// cerr << "mh " << cts_considered << " " << totake << " " << multiple_here << " " << nchoosek(cts_considered, totake) << endl;
				cts_left_prob[cts_considered - totake] += multiple_here;
				tot += multiple_here; //how many ways? n choose k
			}
			double nonehere = pow(1.0 - (strategy[i] / f_chance_sum), cts_considered) * cts_left_prob[cts_considered]; //chance no one takes anything
			tot += nonehere;
			// if(abs(tot - cts_left_prob[cts_considered]) > 0.001)
			// {
			// 	cerr << "bad probs at " << cts_considered << endl;
			// }
			cts_left_prob[cts_considered] = nonehere;
		}

		f_chance_sum -= strategy[i];

		// for(auto item : cts_left_prob)
		// 	cerr << item << " ";
		// cerr << endl;
			
	}
	for(int i = 0; i < winprob.size(); i++)
		if(winprob[bi] / strategy[bi] < winprob[i] / strategy[i])
			bi = i;
		
	// cerr << "win probs for " << endl;
	// for(auto item : strategy)
	// 	cerr << item << " ";
	// cerr << endl;
	// for(auto item : winprob)
	// 	cerr << item << " ";
	// cerr << endl;
	// for(int i = 0; i < strategy.size(); i++)
	// 	cerr << winprob[i] / strategy[i] / ct << " ";
	// cerr << endl;
	return make_pair(bi, winprob[bi] / strategy[bi]);
}

int main()
{
	int n, m;
	cin >> n >> m;
	
	vector<double> strategy(m, 1.0/m);
	double add = 0.0001;
	double rem = 0.0001 / m;
	int i = 0;
	for(i = 0; i < 20000; i++)
	{
		auto pr1 = best_choice(strategy, n);
		// cerr << pr1.first << endl;
		//try add, check if it helps
		strategy[pr1.first] += add;
		for(auto &item : strategy) item -= rem;
	}
	// cerr << i << endl << endl;
	cout << fixed << setprecision(5) << endl;
	for(auto item : strategy)
		cout << item << endl;
}