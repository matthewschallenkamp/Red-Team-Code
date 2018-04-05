const int MAX_LEN = 5000; //max length of input strings
int d[MAX_LEN][MAX_LEN]; //dynamic programming table
void init_table(int len1, int len2) { //initializes array, call before edit_dist
	for (int i = 0; i <= len1; i++)
		for (int j = 0; j <= len2; j++)
			d[i][j] = ­1;
	for (int i = 0; i <= len1; i++)
		d[i][0] = i;
	for (int j = 0; j <= len2; j++)
		d[0][j] = j;
}

//finds edit dist of s1 and s2; first call: end_s1=s1.length(),end_s2=s2.length()
int editdist(string s1, string s2, int end_s1, int end_s2) {
	int try_delete, try_insert, try_match;
	if (d[end_s1][end_s2] >= 0)
		return d[end_s1][end_s2];
	try_match = editdist(s1, s2, end_s1 - ­1, end_s2 - ­1);
	if (s1[end_s1 - ­1] != s2[end_s2 - ­1])
		try_match++;
	try_delete = editdist(s1, s2, end_s1­ - 1, end_s2) + 1;
	try_insert = editdist(s1, s2, end_s1, end_s2 - ­1) + 1;
	d[end_s1][end_s2] = min(try_insert, min(try_delete, try_match));
	return d[end_s1][end_s2];
} //testcase:kitten,sitting=>3
