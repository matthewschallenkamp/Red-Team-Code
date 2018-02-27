#include <iostream>
#include <vector>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int x, y, n, d;
	scanf("%i%i%i%i", &x, &y, &n, &d);
	vector<vector<int> > table(x, vector<int>(y, 1));
	vector<vector<int> > res(x + 1, vector<int>(y + 1, 0));

	int xs, xe, ys, ye;
	while(n--)
	{
		//ornaments are disjoint, otherwise would need to do corners
		scanf("%i%i%i%i", &xs, &xe, &ys, &ye);
		for(int i = xs; i < xe; i++)
			for(int j = ys; j < ye; j++)
				table[i][j] = 0;
	}

	//we're going to sum down in x to make histograms
	for(int j = 0; j < y; j++)
	{
		for(int i = x - 2; i >= 0; i--)
		{	
			if(table[i][j])
				table[i][j] = table[i+1][j] + 1;
		}
	}

	//now per x row, do histogram sum
	for(auto &col : table)
	{
		vector<pair<int, int> > stack; //pairs of startindex, ht
		stack.push_back(make_pair(0, 0)); //a null pair to be stack base
		col.push_back(0); //we need to pop all at the end
		for(int j = 0; j < col.size(); j++)
		{
			auto sb = make_pair(j, col[j]); // by default we only have our own col
			while(col[j] < stack.back().second) //while there is a larger col to the left
			{
				sb = stack.back(); stack.pop_back();
				//that item ends here
				res[sb.second][j - sb.first]++;
				//and we can remove whichever subrect will be taller, us or the starter
				res[max(col[j], stack.back().second)][j - sb.first]--;
			}
			//now lets add this height subrectangle to the stack
			//we'll start where the last item >= to us started
			stack.push_back(make_pair(sb.first, col[j]));
		}
	}

	for(int i = 0; i <= x; i++)
	{
		for(int j = y; j > 0; j--)
			res[i][j-1] += res[i][j];
		for(int j = y; j > 0; j--)
			res[i][j-1] += res[i][j];
	}
	for(int j = 0; j <= y; j++)
	{
		for(int i = x; i > 0; i--)
			res[i-1][j] += res[i][j];
	}

	int xq, yq;
	while(d--)
	{
		scanf("%i%i", &xq, &yq);
		printf("%i\n", res[xq][yq]);
	}
	return 0;
}