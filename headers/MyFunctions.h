


#ifndef _MY_OTHER_FUNCTIONS_
#define _MY_OTHER_FUNCTIONS_
//*** Define some of my random hard to classify functions ***//


vector< double > Get_Delta_N(
		const vector< SingleReactionData >&
);

// Making Scheme Irreversible
vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > ,
		vector< ThermodynamicData > ,
		double,
		double
);



#endif /* _MY_OTHER_FUNCTIONS_ */
