/*
	Multi Task Sparse Coordinate Coding
*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <iomanip>
#include <string.h>
#include "SampleNormalization.h"
#include "DictionaryGeneration.h"
#include "LR.h"
#include "SCC.h"

int main(int argc, char* argv[])
{
	std::string input_info="Input_info.txt";
	int layers = 3;
	int epochNumber = 10;
	double lambda = 0.13;
	int fileNumber = 4;
	int sampleDim = 400;
	int dictionarySize = 1000;
	bool NonNegative = false;

	std::vector<std::string> sampleFileName;
	std::vector<std::string> dictionaryName;
	std::vector<std::string> featureFileName;
	int *sampleNumber = new int[fileNumber];
	int *featureDim = new int[fileNumber];
	dpl::ReadInformation( input_info, fileNumber, sampleNumber, featureDim, sampleFileName, dictionaryName, featureFileName);
	
	double ***Wd;
	double ***feature;
	double ***sample;
	sample = dpl::ReadSample( sampleFileName, fileNumber, sampleNumber, sampleDim, NonNegative );
	
	std::cout<<"Begin to initialize dictionary."<<std::endl;
	Wd = dpl::GenerateRandomPatchDictionary( fileNumber, dictionarySize, sampleNumber, sampleDim, featureDim, sample );
	dpl::DictionaryNormalization( featureDim, fileNumber, sampleDim, Wd );
	feature = dpl::FeatureInitialization( fileNumber, sampleDim, featureDim, sampleNumber );
	
	std::cout<<"Start training "<<std::endl;
	dpl::trainDecoder( Wd, feature, sample, lambda, layers, featureDim, sampleNumber, fileNumber, sampleDim, dictionarySize, epochNumber, NonNegative );
	std::cout<<"Finish training "<<std::endl;

	dpl::saveDictionary( fileNumber, featureDim, sampleDim, Wd, dictionaryName );
	dpl::saveFeature( fileNumber, featureDim, sampleNumber, featureFileName, feature );

	//dpl::clearSample( fileNumber, sampleNumber, sample );
	//dpl::clearFeature( fileNumber, sampleNumber, feature );
	//dpl::clearDictionary( fileNumber, featureDim, Wd );
	return 0;
}
