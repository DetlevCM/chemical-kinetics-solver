/*
 * ReportAccuracy.cpp
 *
 *  Created on: 9 Oct 2014
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

void ReportAccuracy(
		int Number_Species,
		vector< string > Species,
		string output_filename,
		string base_concentrations,
		string reduced_concentrations
)
{

	int i;

	ifstream Input_Data_Full;
	Input_Data_Full.open (base_concentrations.c_str());

	ifstream Input_Data_Reduced;
	Input_Data_Reduced.open (reduced_concentrations.c_str());

	// position {0} is time, [Number_Species + 1] is temperature



	vector< double > OutputReview(Number_Species + 2);
	vector< double > MaxDiff(Number_Species + 2);
	vector< double > MinDiff(Number_Species + 2);


	double number_lines = 0;

	if (Input_Data_Full.is_open() && Input_Data_Reduced.is_open())
	{
		string Line1, Line2;

		getline (Input_Data_Full,Line1);
		getline (Input_Data_Reduced,Line2);



		while (Input_Data_Full.good() && Input_Data_Reduced.good())
		{

			//cout << "Loop check \n";
			getline (Input_Data_Full,Line1);
			getline (Input_Data_Reduced,Line2);

			int i;
			//LineIn.resize(4);
			char *cstr1, *cstr2, *p;
			//					string str;
			//string line1;
			//						size_t found;

			cstr1 = new char [Line1.size()+1];
			strcpy (cstr1, Line1.c_str());
			cstr2 = new char [Line2.size()+1];
			strcpy (cstr2, Line2.c_str());

			vector< double > DataFull, DataReduced;

			DataFull.clear(); // make sure storage array is empty
			DataReduced.clear();


			p=strtok (cstr1,"\t"); // break at space or tab
			p=strtok(NULL,"\t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				DataFull.push_back(strtod(p,NULL));
				p=strtok(NULL,"\t");
			}
			delete[] cstr1;
			Line1.clear();


			p=strtok (cstr2,"\t"); // break at space or tab
			p=strtok(NULL,"\t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				DataReduced.push_back(strtod(p,NULL));
				//cout << p << " ";
				p=strtok(NULL,"\t");
			}
			delete[] cstr2;
			Line2.clear();



			if( DataFull.size() != 0 && DataReduced.size() != 0)
			{
				for(i=1;i<=Number_Species;i++)
				{
					double difference;

					if(DataFull[i] > 1.0e-23)
					{
						difference = ((fabs(DataFull[i])-fabs(DataReduced[i]))/fabs(DataFull[i]));

						OutputReview[i] = OutputReview[i] + difference;

						if(MaxDiff[i]< difference)
						{
							MaxDiff[i] = difference;
						}
						if(MinDiff[i] > difference)
						{
							MinDiff[i] = difference;
						}

					}
					number_lines = number_lines + 1;
				}
			}
		}




		Input_Data_Full.close();
		Input_Data_Reduced.clear();

		// average and turn into %
		for(i=1;i<=Number_Species;i++)
		{
			OutputReview[i] = 100 * OutputReview[i] / number_lines;
			MaxDiff[i] = 100 * MaxDiff[i];
			MinDiff[i] = 100 * MinDiff[i];
		}

		ofstream OutputFile;

		OutputFile.open(output_filename.c_str());
		int i;

		OutputFile << "Species Name 	Average Difference in % 	Min Diff in % 	Max Diff in % \n";

		for(i=1;i<=Number_Species;i++)
		{
			OutputFile <<
					Species[i-1] << " 	" <<
					OutputReview[i] << " 	" <<
					MinDiff[i] << " 	" <<
					MaxDiff[i] << " 	" <<
					" \n";
		}


	}
}





