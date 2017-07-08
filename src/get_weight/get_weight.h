#ifndef __GET_WEIGHT_H__
#define __GET_WEIGHT_H__

#include <string>

#define EN_DEBUG 0

#if EN_DEBUG
#define debug(x) do { cout << x; } while (0)
#else 
#define debug(x) 
#endif

#define INF_WEIGHT (-1)
using namespace std;

typedef struct 
{
	string node;
	int weight;
}Node_W;

vector<Node_W*> read_weight(char *fname);
int calculate_weight(char *fname, vector<Node_W*> node_w);
#endif
