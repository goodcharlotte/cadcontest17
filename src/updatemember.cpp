#include "datatype.h"

void Circuit_t::update_allpi()
{
    queue<int> nodeque;

    for (int pi_i = 0; pi_i < pi.size(); pi_i++) {
        vector<bool> visit_flag(allnodevec.size(), false);
        int pi_node = pi[pi_i];
        nodeque.push(pi_node);
        while (nodeque.size() != 0) {
            int node = nodeque.front();
            if (visit_flag[node] == false) {
                visit_flag[node] = true;
                allnodevec[node].allpi.push_back(pi_node);
                for (int out_i = 0; out_i < allnodevec[node].out.size(); out_i++) {
                    int out_node = allnodevec[node].out[out_i];
                    if (visit_flag[out_node] == false) {
                        nodeque.push(out_node);
                    }
                }
            }
            nodeque.pop();
        }
    }
    
    /* 
    for (int i = 0; i < allnodevec.size(); i++) {
        cout << "========== Debug allpi: " << i << " " << allnodevec[i].name << " ========\n";
        for (int allpi_i = 0; allpi_i < allnodevec[i].allpi.size(); allpi_i++) {
            int node = allnodevec[i].allpi[allpi_i];
            cout << "id:" << node << " " << allnodevec[node].name << endl;
        }
        cout << endl;
    }
    */
    
}
