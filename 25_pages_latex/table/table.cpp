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
