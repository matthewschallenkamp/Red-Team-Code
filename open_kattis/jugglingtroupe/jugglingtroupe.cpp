#include <iostream>
#include <vector>
#include <stack>

using namespace std;

void processJuggler(vector<int>& jugglers, stack<int>& zeros, int i)
{
  //cerr << i <<": " << jugglers[i] << endl;
  
  //If the juggler has no balls, keep track of his location
  if(jugglers[i] == 0)
    zeros.push(i);
    
  //Any juggler with 2+ balls tosses them to the sides
  while(jugglers[i] > 1)
  {
    //Assume zero is off left side
    int zero = -1;
    
    //Check for closer zero
    if(zeros.size())
    {
      zero = zeros.top();
      zeros.pop();
    }
    
    //Get distance from zero to juggler
    int dist = i - zero;
    //cerr << "Dist: " << dist << endl;
    
    //If the zero is close to the juggler, he
    //drops a bunch of balls and the zero hits the juggler
    //and he drops 2
    if(dist < jugglers[i])
    {
      jugglers[i] -= dist + 1;
      if(i < jugglers.size()-1) jugglers[i+1] += dist;
      if(zero >= 0)jugglers[zero] = 1;
      if(jugglers[i] == 0)
        zeros.push(i);
      
      //cerr << "-" << dist+1 << endl;
    }
    //Otherwise, he drops one ball for each step the zero takes
    //towards the juggler, then the zero goes back in the stack
    else
    {
      int move = jugglers[i]-1;
      jugglers[i] -= move;
      if(i < jugglers.size()-1) jugglers[i+1] += move;
      
      zeros.push(zero + move);
      
      //cerr << "-" << move << endl;
      //cerr << "0 at " << zero+move << endl;
      if(zero >= 0)jugglers[zero] = 1;
      jugglers[zero+move] = 0;
    }
  }
}

ostream& operator << (ostream& out, const vector<int>& nums)
{
  out << "<";
  for(int i=0; i<nums.size()-1; i++)
  {
    out << nums[i] << " ";
  }
  return out << nums.back() << ">";
}

int main()
{
  string in;
  while(cin >> in)
  {
    //List of how many balls each juggler has
    vector<int> jugglers(in.size());

    //Stack containing previous jugglers with no balls
    stack<int> zeros;
    
    for(int i=0; i<in.size(); i++)
    {
      jugglers[i] = in[i] - '0';
    }
    
    //Check each juggler once, left to right
    for(int i=0; i<in.size(); i++)
    {
      processJuggler(jugglers, zeros, i);
      //cerr << jugglers << endl;
    }
    
    for(int i : jugglers)
      cout << i;
    cout << endl;
  }
}