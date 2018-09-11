/*
 * Rates_Analysis.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_RATES_ANALYSIS_HPP_
#define HEADERS_RATES_ANALYSIS_HPP_

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

// New function for rates of production

void Prepare_Print_Rates_Per_Species(
		//vector< TrackSpecies > & ,
		//vector< TrackSpecies > & ,
		string,
		//vector< double > & ,
		vector< string > ,
		vector< int > ,
		vector< vector< int > >//,
		//vector< SingleReactionData >&
);


void Print_Rates_Per_Species(
		vector< TrackSpecies > & ,
		vector< TrackSpecies > & ,
		const string,
		vector< double > & ,
		double ,
		vector< string > ,
		vector< int > ,
		vector< vector< int > > //,
		//vector< SingleReactionData >&
);



#endif /* HEADERS_RATES_ANALYSIS_HPP_ */
