#include "datatype.h"

bool check_redundantset(vector< list<int> >& tempset, list<int>& checkset)
{
    bool redundant_flag = false;

    list<int>::iterator it_i, it_j;
    for (int i = 0; i < tempset.size(); i++) {
        if (tempset[i].size() != checkset.size()) continue;
        else {
            redundant_flag = true;
            it_i = tempset[i].begin();
            it_j = checkset.begin();
            while (it_i != tempset[i].end()) {
                if (*it_i == *it_j) {
                    ++it_i;
                    ++it_j;
                } else {
                    redundant_flag = false;
                    break;
                }
            }
            if (redundant_flag == true) {
                return redundant_flag;
            }
        }
    }

    return redundant_flag;
}

list<int> Circuit_t::check_include(list<int>& subset, int& checknode)
{
    list<int> new_subset(subset);
    list<int>::iterator list_it;
    bool in_flag = false;
    int node = 0;
    for (list_it = new_subset.begin(); list_it != new_subset.end(); ++list_it) {
        node = *list_it;
    }

    for (list_it = new_subset.begin(); list_it != new_subset.end(); ++list_it) {
        node = *list_it;
        if (node == checknode) {
            in_flag = true;
            break;
        }
        if (allnodevec[node].cost == allnodevec[checknode].cost) {
            if (node < checknode) {
                in_flag = true;
                break;
            }
        }
        if (allnodevec[node].cost > allnodevec[checknode].cost) {
            break;
        }
    }

    if (in_flag == true) {
        new_subset.clear();
    } else {
        new_subset.insert(list_it, checknode);
    }
    for (list_it = new_subset.begin(); list_it != new_subset.end(); ++list_it) {
        node = *list_it;
    }

    return new_subset;
}

int Circuit_t::getMaxSum(vector<int>& allcandidate)
{
    int node = 0;
    int costsum = 0;
    for (int i = 0; i < allcandidate.size(); i++) {
        node = allcandidate[i];
        costsum += allnodevec[node].cost;
    }
    return costsum;
}

void Circuit_t::getbaseset(vector<int>& allpatchnode, vector<int>& allcandidate)
{
    sortcost(allcandidate, 0, (allcandidate.size()-1) );
#if 0
    cout << "All base nodes:" << endl;
    for (int i = 0; i < allcandidate.size(); i++) {
        cout << allnodevec[allcandidate[i]].name << " " << allnodevec[allcandidate[i]].cost << endl;
    }
    cout << endl;
#endif


    vector< vector< list<int> > > allbaseset;
    list<int>::iterator list_it;
    allbaseset.push_back(*(new vector< list<int> >));
    
    int MAX_WEIGHT_SUM = getMaxSum(allcandidate);
    int MAX_WEIGHT = allnodevec[allcandidate.back()].cost;
    int weight_sum = 0;
    int candidate_ptr, weight_ptr, set_ptr, choose_ptr;
    bool find_flag = false;
    candidate_ptr = weight_ptr = set_ptr = choose_ptr = 0;

    while (find_flag == false) {
        weight_sum++;
        //cout << "sum: " << weight_sum << endl;
        allbaseset.push_back(*(new vector< list<int> >));
        //Find: weight_sum == node.cost
        while (1) {
            if (candidate_ptr >= allcandidate.size()) {
                break;
            }
            int node = allcandidate[candidate_ptr];
            if (allnodevec[node].cost != weight_sum) {
                break;
            }
            
            allbaseset.back().push_back(*(new list<int>));
            allbaseset.back().back().push_back(node);

            candidate_ptr++;
        }

        //Find: weight_sum == sum of nodes' costs
        choose_ptr = 0;
        for (set_ptr = allbaseset.size()-1; set_ptr >= 0; set_ptr--) {
            weight_ptr = weight_sum - set_ptr;
            if (weight_ptr > set_ptr) break;
            if (weight_ptr > MAX_WEIGHT) break;
            while (1) {
                if (choose_ptr >= allcandidate.size()) {
                    break;
                }
                int node = allcandidate[choose_ptr];
                if (allnodevec[node].cost != weight_ptr) break;
                for (int subset_i = 0; subset_i < allbaseset[set_ptr].size(); subset_i++) {
                    //new item in the subset -> add, or skip
                    list<int> new_subset = check_include(allbaseset[set_ptr][subset_i], allcandidate[choose_ptr]);
                    if (new_subset.size() > 0) {
                        if (check_redundantset(allbaseset.back(), new_subset) == false) {
                            allbaseset.back().push_back(new_subset);
                            //TO DO: check unsat
                            //
                        }
                    }
                }
                choose_ptr++;
            }
        }
        if (weight_sum == MAX_WEIGHT_SUM) {
            break;
        }
        //cout << "size: " << allbaseset.back().size() << endl;
    }
#if 0 
    for (int i = 0; i < allbaseset.size(); i++) {
        cout << "set[" << i << "] size:" << allbaseset[i].size() << endl;
        for (int j = 0; j < allbaseset[i].size(); j++) {
            cout << "{";
            for (list_it = allbaseset[i][j].begin(); list_it != allbaseset[i][j].end(); ++list_it) {
                int node = *list_it;
                cout << allnodevec[node].name << "(" << allnodevec[node].cost << ")";
                cout << " ";
            }
            cout << "}" << endl;
        }
        cout << "===================" << endl;
    }
#endif
}
