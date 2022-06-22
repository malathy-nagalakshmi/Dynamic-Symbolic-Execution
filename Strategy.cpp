#include "Strategy.h"

/*
 * Implement your search strategy.
 */
void searchStrategy(z3::expr_vector &OldVec) { /* Add your code here */ 
	if(OldVec.size() != 0){
		auto val = !OldVec.back();
		OldVec.pop_back();
		OldVec.push_back(val);
	}
}
