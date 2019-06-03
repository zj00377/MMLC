#ifndef SAMPLE_NORMALIZATION_H
#define SAMPLE_NORMALIZATION_H
namespace dpl{

char *stringToChar( std::string fileName ){
	char *name = new char[fileName.length()+1];
	for( int i=0; i<fileName.length(); i++ )
		name[i] = fileName[i];
	name[fileName.length()]='\0';
	return name;
}

void ReadInformation( std::string input_info, int fileNumber, int *sampleNumber, int *featureDim, std::vector<std::string> &sampleFileName, std::vector<std::string> &dictionaryName, std::vector<std::string> &featureFileName){
	FILE *fp;
	fp = fopen( stringToChar(input_info), "r");
	char name[100]; 
        if( fp == NULL ){
		printf("Could not find the input info file.\n");		
		exit(0);
	}
	for( unsigned int i=0; i<fileNumber; i++ ){
		if( fscanf(fp, "%s", name)==0 )
			exit(0);
        	if( fscanf(fp, "%d", &sampleNumber[i])==0 )
			exit(0);
		sampleFileName.push_back(std::string(name));
	}
	for( unsigned int i=0; i<fileNumber; i++ ){
		if( fscanf(fp, "%s", name)==0 )
			exit(0);
        	if( fscanf(fp, "%d", &featureDim[i])==0 )
			exit(0);
		featureFileName.push_back(std::string(name));
	}
	for( unsigned int i=0; i<fileNumber; i++ ){
		if( fscanf(fp, "%s", name)==0 )
			exit(0);
		dictionaryName.push_back(std::string(name));
	}
	fclose(fp);
} 

double ***ReadSample( std::vector<std::string> fileName, int fileNumber, int *sampleNumber, int sampleDim, bool NonNegative ){

	double ***sample = (double***)malloc(fileNumber*sizeof(double**));
        for( unsigned int t=0; t<fileNumber; t++ ){
		sample[t] = (double**)malloc(sampleNumber[t]*sizeof(double*));
		FILE *fp;
		fp = fopen( stringToChar(fileName[t]), "r");
        	if( fp == NULL ){
			printf("Could not find sample file.\n");		
			exit(0);
		}
		for( unsigned int i=0; i<sampleNumber[t]; i++ )
			sample[t][i] = (double*)malloc(sampleDim*sizeof(double));

		for( unsigned int j=0; j<sampleDim; j++ ){
			for( unsigned int i=0; i<sampleNumber[t]; i++ ){
        			if( fscanf(fp, "%lf", &sample[t][i][j])==0 )
					exit(0);
			}
		}
		for( unsigned int i=0; i<sampleNumber[t]; i++ ){
			double mean = 0;
			for( unsigned int j=0; j<sampleDim; j++ )
             			mean += sample[t][i][j];
			mean = mean/sampleDim;

			double sum = 0;	
			for( unsigned int j=0; j<sampleDim; j++ ){
				if( !NonNegative)
             				sample[t][i][j] -= mean;
				sum += sample[t][i][j]*sample[t][i][j];
			}
			double sqrtSum = sqrt(sum);
			if( sqrtSum!=0 ){
				for( unsigned int j=0; j<sampleDim; j++ )
					sample[t][i][j] = (sample[t][i][j]/sqrtSum);
			}
		}	
		fclose(fp);
	}
	return sample;
}

void clearSample( int fileNumber, int *sampleNumber, double ***sample ){
	
	for( unsigned int t=0; t<fileNumber; t++ ){
		for( unsigned int i=0; i<sampleNumber[t]; i++ )
			delete [] sample[t][i];
		delete [] sample[t];
	}
	delete [] sample;
}

}
#endif /* Sample Normalization */
