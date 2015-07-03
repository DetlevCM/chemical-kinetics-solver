/*
 * Rates_Analysis.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_RATES_ANALYSIS_H_
#define HEADERS_RATES_ANALYSIS_H_

void MaxRatesAnalysis(
		vector< vector < str_RatesAnalysis > >& ,
		const vector< TrackSpecies > & ,
		const vector< TrackSpecies > & ,
		const vector< double > ,
		const double
);


void RatesAnalysisAtTimes(
		const vector< TrackSpecies > & ,
		const vector< TrackSpecies > & ,
		const vector< double > & ,
		const double ,
		const vector< string > ,
		const vector< SingleReactionData >&
);


void WriteMaxRatesAnalysis(
		vector< vector < str_RatesAnalysis > >&,
		const vector< string >&,
		const vector< SingleReactionData >&,
		string
);

void PrepareStreamRatesAnalysis(
		const vector< string >&,
		string
		);

void StreamRatesAnalysis(
		const string,
		const vector< TrackSpecies > & ,
		const vector< TrackSpecies > & ,
		const vector< double > & ,
		const double ,
		const int
);


#endif /* HEADERS_RATES_ANALYSIS_H_ */
