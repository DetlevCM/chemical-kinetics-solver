/*
 * Prepare_Jacobian_Matrix.cpp
 *
 *  Created on: 01.03.2015
 *      Author: DetlevCM
 */

#include <Headers.hpp>



/* Column Wise Jacobian Matrix for Intel ODE Solver */



void Prepare_Jacobian_Matrix(
		vector < JacobianData > & JacobianColumnWise,
		const vector< SingleReactionData > & Reactions//,
		//vector< string > Species
)
{
	JacobianColumnWise.clear();

	int Number_Species = (int) Reactions[0].Reactants.size(); // Consider that temperature is included

	// 11 12
	// 21 22

	/*
	 * 1: 11
	 * 2: 21
	 * 3: 12
	 * 4: 22
	 */

	/*
	 *
	struct JacobianSpecies{
	int SpeciesID;
	double power;
	};

	struct JacobianData {
	int ColumnWiseArrayPosition;
	bool IsForward;
	bool IsProduction;
	int ReactionID;
	double coefficient;
	vector< JacobianSpecies > Species;
	};

	 */

	// Column wise order means we first have to check if the species we differentiate with respect to is present
	// then we check if the row species is present and if yes build the input

	int i, j,k,m;


	for(i=0;i<Number_Species;i++) // column
	{

		for(j=0;j<Number_Species;j++) // row
		{
			JacobianData temp;

			JacobianSpecies temp2;

			// Now check every reaction
			for(k=0;k<(int)Reactions.size();k++)
			{
				vector< JacobianSpecies > SpeciesList;

				// species production
				if(Reactions[k].Reactants[i] != 0 && Reactions[k].Products[j] != 0) // differentiation species is present
				{
					temp.ColumnWiseArrayPosition = i*(Number_Species+1) + j; // consider temp array position
					temp.IsForward = true;
					temp.IsProduction = true;
					temp.ReactionID = k; // gets me the k
					temp.coefficient = (Reactions[k].Reactants[i])*(Reactions[k].Reactants[i]);  // correct for A -> 2B as well, is production

					temp2.SpeciesID = i;
					temp2.power = -Reactions[k].Reactants[i] - 1;
					SpeciesList.push_back(temp2);

					for(m=0;m<Number_Species;m++)
					{
						if(Reactions[k].Reactants[m] != 0 && m != i)
						{
							temp2.SpeciesID = m;
							temp2.power = -Reactions[k].Reactants[m];
							SpeciesList.push_back(temp2);
						}
					}

					temp.Species = SpeciesList;
					JacobianColumnWise.push_back(temp);
					SpeciesList.clear();
				}



				// reverse reaction - species production
				if(Reactions[k].Reversible && Reactions[k].Products[i] != 0 && Reactions[k].Products[j] != 0) // differentiation species is present
				{
					temp.ColumnWiseArrayPosition = i*(Number_Species+1) + j;
					temp.IsForward = false;
					temp.IsProduction = false;
					temp.ReactionID = k; // gets me the k
					temp.coefficient = Reactions[k].Products[i];

					temp2.SpeciesID = i;
					temp2.power = Reactions[k].Products[i] - 1;
					SpeciesList.push_back(temp2);

					for(m=0;m<Number_Species;m++)
					{
						if(Reactions[k].Products[m] != 0 && m != i)
						{
							temp2.SpeciesID = m;
							temp2.power = Reactions[k].Products[m];
							SpeciesList.push_back(temp2);
						}
					}

					temp.Species = SpeciesList;
					JacobianColumnWise.push_back(temp);
					SpeciesList.clear();
				}



				// species consumption
				if(Reactions[k].Reversible && Reactions[k].Products[i] != 0 && Reactions[k].Reactants[j] != 0) // differentiation species is present
				{
					temp.ColumnWiseArrayPosition = i*(Number_Species+1) + j;
					temp.ReactionID = k; // gets me the k
					temp.IsForward = false;
					temp.IsProduction = true;
					temp.coefficient = Reactions[k].Products[i]*Reactions[k].Products[i]; // correct for A -> 2B as well, is production

					temp2.SpeciesID = i;
					temp2.power = Reactions[k].Products[i] - 1;
					SpeciesList.push_back(temp2);

					for(m=0;m<Number_Species;m++)
					{
						if(Reactions[k].Products[m] != 0 && m != i)
						{
							temp2.SpeciesID = m;
							temp2.power = Reactions[k].Products[m];
							SpeciesList.push_back(temp2);
						}
					}
					temp.Species = SpeciesList;
					JacobianColumnWise.push_back(temp);
					SpeciesList.clear();
				}



				// reverse reaction - species consumption
				if(Reactions[k].Reactants[i] != 0 && Reactions[k].Reactants[j] != 0) // differentiation species is present
				{
					temp.ColumnWiseArrayPosition = i*(Number_Species+1) + j;
					temp.ReactionID = k; // gets me the k
					temp.IsForward = true;
					temp.IsProduction = false;
					temp.coefficient = -Reactions[k].Reactants[i];

					temp2.SpeciesID = i;
					temp2.power = -Reactions[k].Reactants[i] - 1;
					SpeciesList.push_back(temp2);

					for(m=0;m<Number_Species;m++)
					{
						if(Reactions[k].Reactants[m] != 0 && m != i)
						{
							temp2.SpeciesID = m;
							temp2.power = -Reactions[k].Reactants[m];
							SpeciesList.push_back(temp2);
						}
					}
					temp.Species = SpeciesList;
					JacobianColumnWise.push_back(temp);
					SpeciesList.clear();
				}



			}
		}
	}


	/*
	 * Debug Output
	 */

	/*
	for(i=0;i<(int) JacobianColumnWise.size();i++)
	{

		cout << JacobianColumnWise[i].ColumnWiseArrayPosition << " ";
		if(JacobianColumnWise[i].IsForward) // Forward
		{
			if(JacobianColumnWise[i].IsProduction) // species are gained
			{
				//temp = Kf[JacobianColumnWise[i].ReactionID]*JacobianColumnWise[i].coefficient;
				cout << "Kf" << JacobianColumnWise[i].ReactionID + 1 << "*" << JacobianColumnWise[i].coefficient << " ";
			}
			if(!JacobianColumnWise[i].IsProduction) // species are lost
			{
				//temp = Kf[JacobianColumnWise[i].ReactionID]*JacobianColumnWise[i].coefficient;
				cout << "-Kf" << JacobianColumnWise[i].ReactionID + 1 << "*" << JacobianColumnWise[i].coefficient << " ";
			}
		}

		if(!JacobianColumnWise[i].IsForward) // Reverse
		{
			if(JacobianColumnWise[i].IsProduction) // species are gained
			{
				//temp = Kr[JacobianColumnWise[i].ReactionID]*JacobianColumnWise[i].coefficient;
				cout << "Kr" << JacobianColumnWise[i].ReactionID + 1 << "*" << JacobianColumnWise[i].coefficient << " ";
			}
			if(!JacobianColumnWise[i].IsProduction) // species are lost
			{
				//temp = -Kr[JacobianColumnWise[i].ReactionID]*JacobianColumnWise[i].coefficient;
				cout << "-Kr" << JacobianColumnWise[i].ReactionID + 1 << "*" << JacobianColumnWise[i].coefficient << " ";
			}
		}

		for(j=0;j<(int) JacobianColumnWise[i].Species.size();j++)
		{
			//temp = temp * pow(Concentration[JacobianColumnWise[i].Species[j].SpeciesID],JacobianColumnWise[i].Species[j].power);
			cout << "Spc(" << JacobianColumnWise[i].Species[j].SpeciesID + 1 << ")^" << JacobianColumnWise[i].Species[j].power << " ";
		}
		cout << "\n";
	}
//*/

}



