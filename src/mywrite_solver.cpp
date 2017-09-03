#include "core/Solver.h"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "mtl/Heap.h"
#include "mtl/Alg.h"
#include "utils/Options.h"
#include "mtl/Sort.h"

using namespace Minisat;

//Do not od map
#define mapVar(var, map, max) ((var) - 1)


void Solver::toDimacs_nomap(const char *file, const vec<Lit>& assumps)
{
    FILE* f = fopen(file, "wr");
    if (f == NULL)
        fprintf(stderr, "could not open file %s\n", file), exit(1);
    toDimacs_nomap(f, assumps);
    fclose(f);
}


void Solver::toDimacs_nomap(const char *file, const vec<Lit>& assumps, const int after)
{
    FILE* f = fopen(file, "wr");
    if (f == NULL)
        fprintf(stderr, "could not open file %s\n", file), exit(1);
    fprintf(f, "after %d\n", after);
	toDimacs_nomap(f, assumps);
    fclose(f);
}


void Solver::toDimacs_nomap(FILE* f, Clause& c, vec<Var>& map, Var& max)
{
    if (satisfied(c)) return;

    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) != l_False)
            fprintf(f, "%s%d ", sign(c[i]) ? "-" : "", mapVar(var(c[i]), map, max)+1);
    fprintf(f, "0\n");
}


void Solver::toDimacs_nomap(FILE* f, const vec<Lit>& assumps)
{
    // Handle case when solver is in contradictory state:
    if (!ok){
        fprintf(f, "p cnf 1 2\n1 0\n-1 0\n");
        return; }

    vec<Var> map; Var max = 0;

    // Cannot use removeClauses here because it is not safe
    // to deallocate them at this point. Could be improved.
    int cnt = 0;
    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]]))
            cnt++;
        
    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]])){
            Clause& c = ca[clauses[i]];
            for (int j = 0; j < c.size(); j++)
                if (value(c[j]) != l_False)
                    mapVar(var(c[j]), map, max);
        }

    // Assumptions are added as unit clauses:
	assumps.copyTo(assumptions);
    cnt += assumptions.size();

	//only write 0
    //fprintf(f, "p cnf %d %d\n", max, cnt);
	fprintf(f, "p cnf %d %d\n", 0, 0);


    for (int i = 0; i < clauses.size(); i++)
        toDimacs_nomap(f, ca[clauses[i]], map, max);
   
   
	//I move the assumptions to the back
	for (int i = 0; i < assumptions.size(); i++){
        assert(value(assumptions[i]) != l_False);
        fprintf(f, "%s%d 0\n", sign(assumptions[i]) ? "-" : "", mapVar(var(assumptions[i]), map, max)+1);
    }
    if (verbosity > 0)
        printf("Wrote %d clauses with %d variables.\n", cnt, max);
}
