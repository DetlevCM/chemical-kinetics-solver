/*
 * Processing_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef PROCESSING_FUNCTIONS_H_
#define PROCESSING_FUNCTIONS_H_


vector< ReactionParameter > Process_Reaction_Parameters(
		const vector< SingleReactionData >&
);


vector< TrackSpecies > Reactants_ForReactionRate(
		const vector< SingleReactionData >&
);


vector< TrackSpecies > Products_ForReactionRate(
		const vector< SingleReactionData >&,
		bool
);


vector< TrackSpecies > PrepareSpecies_ForSpeciesLoss(
		const vector< SingleReactionData >&
);



#endif /* PROCESSING_FUNCTIONS_H_ */
