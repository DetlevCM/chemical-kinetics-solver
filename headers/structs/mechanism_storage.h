/*
 * mechanism_storage.h
 *
 *  Created on: 30 Oct 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_STRUCTS_MECHANISM_STORAGE_H_
#define HEADERS_STRUCTS_MECHANISM_STORAGE_H_

struct SingleReactionData {
	bool Reversible;
	bool IsDuplicate;
	double paramA;
	double paramN;
	double paramEa;
	vector<double> Reactants;
	vector<double> Products;
};

struct ThermodynamicData {
	double TLow;
	double THigh;
	double TChange;
	double NasaLow1;
	double NasaLow2;
	double NasaLow3;
	double NasaLow4;
	double NasaLow5;
	double NasaLow6;
	double NasaLow7;
	double NasaHigh1;
	double NasaHigh2;
	double NasaHigh3;
	double NasaHigh4;
	double NasaHigh5;
	double NasaHigh6;
	double NasaHigh7;
};




#endif /* HEADERS_STRUCTS_MECHANISM_STORAGE_H_ */
