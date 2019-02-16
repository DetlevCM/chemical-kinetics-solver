/*
 * Run_Integrator.h
 *
 *  Created on: 20 Oct 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_RUN_INTEGRATOR_H_
#define HEADERS_RUN_INTEGRATOR_H_

// Function to pick the right integration routine
void Choose_Integrator(
		Filenames,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

int Prepare_Integrator_Settings(
		Initial_Data ,
		int ,
		Settings_LSODA& ,
		Settings_Intel&
);

// solve based on concentrations using Intel
void Integrate_Liquid_Phase(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);

// solve based on concentrations using Intel
void Integrate_Liquid_Phase_Intel(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);

// solve based on concentrations and consider headspace gas using Intel
void Integrate_Pressure_Vessel_Liquid_Phase_Intel(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

// solve based on concentrations using Odepack LSODA
void Integrate_Liquid_Phase_Odepack_LSODA(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);

void Integrate_Gas_Phase_Odepack_LSODA(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);

// solve based on concentrations and consider headspace gas using Odepack LSODA
void Integrate_Pressure_Vessel_Liquid_Phase_Odepack_LSODA(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

// solve based on concentrations and consider headspace gas
void Integrate_Pressure_Vessel_Liquid_Phase(
		Filenames,
		vector< double >,
		Reaction_Mechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

#endif /* HEADERS_RUN_INTEGRATOR_H_ */
