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
        if (allnodevec[node].cost != INF) {
            costsum += allnodevec[node].cost;
        }
    }
    return costsum;
}

int Circuit_t::getMaxIndex(vector<int>& allcandidate, int costsum)
{
    int can_index = 0;
    for (int i = 0; i < allcandidate.size(); i++) {
        if (allnodevec[allcandidate[i]].cost <= costsum) {
            can_index = i;
        } else {
            break;
        }
    }
    return can_index;
}

vector< vector<int> > Circuit_t::getSumSet(vector<int>& allcandidate, int costsum)
{
    int start_index = getMaxIndex(allcandidate, costsum);
    vector< vector<int> > allsumset;
    vector<int> temp_set;
    recur_CEV(allcandidate, allsumset, temp_set, start_index, costsum);
    return allsumset;
}

int Circuit_t::getCostSum(vector<int>& allcandidate, int start_index)
{
    int costsum = 0;
    int node = 0;
    while (start_index >= 0) {
        node = allcandidate[start_index];
        costsum += allnodevec[node].cost;
        start_index--;
    }
    return costsum;
}

void Circuit_t::recur_CEV(vector<int>& allcandidate, vector< vector<int> >& allsumset, vector<int> temp_set, int start_index, int costsum)
{
    clock_t temp_clk = clock();
    double time_sec = double(temp_clk - start_clk)/CLOCKS_PER_SEC;
    if ( time_sec > TIME_LIMIT) {
        cout << "time out:" << time_sec << endl;
        return;
    }


    int node = 0;
    if (costsum == 0) {
        allsumset.push_back(temp_set);
        return;
    } else if (costsum < 0) {
        return;
    }

    if (start_index < 0) {
        return;
    } else {
        node = allcandidate[start_index];
    }

    if (getCostSum(allcandidate, start_index) < costsum) {
        return;
    }

    vector<int> index_on;
    vector<int> index_off;

    for (int i = 0; i < temp_set.size(); i++) {
        index_on.push_back(temp_set[i]);
        index_off.push_back(temp_set[i]);
    }
    index_on.push_back(node);

    recur_CEV(allcandidate, allsumset, index_on, start_index-1, costsum-allnodevec[node].cost);
    recur_CEV(allcandidate, allsumset, index_off, start_index-1, costsum);
}

vector<int> Circuit_t::getbaseset(vector<int>& allcandidate, vector<int>& allpatchnode, Circuit_t& patchckt_off, vector<int>& allpatchnode_off)
{
    clock_t temp_clk;
    sortcost(allcandidate, 0, (allcandidate.size()-1) );
#if 0
    cout << "All base nodes:" << endl;
    for (int i = 0; i < allcandidate.size(); i++) {
        cout << allnodevec[allcandidate[i]].name << " " << allnodevec[allcandidate[i]].cost << endl;
    }
    cout << endl;
#endif
    vector<int> choosebase;
    vector< vector< list<int> > > allbaseset;
    list<int>::iterator list_it;
    allbaseset.push_back(*(new vector< list<int> >));
    
    int MAX_WEIGHT_SUM = getMaxSum(allcandidate);
    int MAX_WEIGHT = allnodevec[allcandidate.back()].cost;
    int weight_sum = 0;
    int candidate_ptr, weight_ptr, set_ptr, choose_ptr;
    bool finish_flag = false;
    bool find_flag = false;
    candidate_ptr = weight_ptr = set_ptr = choose_ptr = 0;

    //TODO: patch_onset + patch_offset
    Circuit_t twopatchckt;
    twopatchckt.readfile("patch.v");
    twopatchckt.readfile2("patch2.v");


    while (finish_flag == false) {
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
            if (finish_flag == true) break;
            if (weight_ptr > set_ptr) break;
            if (weight_ptr > MAX_WEIGHT) break;
            while (finish_flag == false) {
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
                            copy(allbaseset.back().back().begin(), allbaseset.back().back().end(), back_inserter(choosebase));
                            //TODO: check unsat
                            //find_flag = checkUNSAT(twopatchckt, choosebase, allpatchnode, patchckt_off, allpatchnode_off);
                            if (find_flag == false) {
                                choosebase.clear();
                            } else {
                                finish_flag = true;
                                break;
                            }
                        }
                    }
                    temp_clk = clock();
                    double time_sec = double(temp_clk - start_clk)/CLOCKS_PER_SEC;
                    if ( time_sec > 1500) {
                        finish_flag = true;
                        //cout << "time out:" << time_sec << endl;
                    }

                }
                choose_ptr++;
            }
        }
        if (weight_sum == MAX_WEIGHT_SUM) {
            finish_flag = true;
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

    return choosebase;
}
