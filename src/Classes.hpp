/*
 * Classes.h
 *
 *  Created on: 4 Dec 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_CLASSES_HPP_
#define HEADERS_CLASSES_HPP_

class FileNames {
public:
	/*
	 * This class collects the file names for the
	 * input files required during a run.
	 *
	 * To collate data, output files are equally treated in the
	 * same class, together together potentially with output flags.
	 */

	string mechanism;
	string initial_data;
	string species_mapping;

};


/* Centralised Integrator Settings */

class Settings_LSODA{
public:
	int solver_subsettings;

	int JT;
	// LSODA specific settings
	int LRW, LIW;
	int ITOL = 1;
	int ITASK = 1;
	int ISTATE = 1;
	int IOPT = 0;

	// general solver settings
	double RTOL, ATOL;

	// some vectors for LSODA
	vector<int> vector_IWORK;
	vector<double> vector_RWORK;
};

class Settings_Intel{
public:
	int ierr, dpar_size;
	double h, hm, ep, tr;

	int solver_subsettings;

	vector<int> vector_ipar;
	vector<int> vector_kd;
	vector<double> vector_dpar;
	// set the solver:
	solver_type Solver_Type;
};


#endif /* HEADERS_CLASSES_HPP_ */
