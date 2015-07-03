/*
 * Namespaces.hpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#ifndef SOURCE_NAMESPACES_HPP_
#define SOURCE_NAMESPACES_HPP_


namespace GlobalArrays{
// Key parameters that define the whole reaction scheme - used globally via namespaces
// Not sure if this is a good place to put it...
extern vector< vector < str_RatesAnalysis > > RatesAnalysisData;
extern vector< TrackSpecies > ProductsForRatesAnalysis;
extern vector< JacobianData > JacobianMatrix;
}






#endif /* SOURCE_NAMESPACES_HPP_ */
