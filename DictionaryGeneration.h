#ifndef DICTIONARY_GENERATION_H
#define DICTIONARY_GENERATION_H

namespace dpl{

double ***InitializeDictionary( int fileNumber, int *featureDim, int sampleDim ){
	
	double ***Wd = (double***)malloc(fileNumber*sizeof(double**));
	for( unsigned int t=0; t<fileNumber; t++ ){
		Wd[t] = (double**)malloc(featureDim[t]*sizeof(double*));
		for( unsigned int j=0; j<featureDim[t]; j++ )
			Wd[t][j] = (double*)malloc(sampleDim*sizeof(double));
	}
	return Wd;
}

double ***GenerateRandomPatchDictionary( int fileNumber, int dictionarySize, int *sampleNumber, int sampleDim,  int *featureDim, double ***sample ){

	double ***Wd = InitializeDictionary( fileNumber, featureDim, sampleDim );
	srand((unsigned)time(0));
	unsigned int myseed = (unsigned int) RAND_MAX * rand();
	int totalSample = 0;
	for( unsigned int i=0; i<fileNumber; i++ )
		totalSample += sampleNumber[i];

	for( unsigned int i=0; i<dictionarySize; i++ ){
        	unsigned int index = rand_r(&myseed)%totalSample;
		unsigned int sum = 0;
		unsigned int file_id = 0;
		for( unsigned int j=0; j<fileNumber; j++ ){
			if( index>=sum && index<(sum+sampleNumber[j]) ){
				file_id = j;
				index -= sum;
				break;
			}
			sum += sampleNumber[j];
		}
        	for( unsigned int j=0; j<sampleDim; j++ )
            		Wd[0][i][j]=sample[file_id][index][j];
	}
	for( unsigned int t=0; t<fileNumber; t++ ){
		for( unsigned int i=0; i<dictionarySize; i++ ){
			for( unsigned int j=0; j<sampleDim; j++ )
            			Wd[t][i][j]=Wd[0][i][j];
		}
		for( unsigned int i=dictionarySize; i<featureDim[t]; i++ ){
        		unsigned int index = rand_r(&myseed)%sampleNumber[t];
        		for( unsigned int j=0; j<sampleDim; j++ )
            			Wd[t][i][j]=sample[t][index][j];
		}
	}
	return Wd;
}

void DictionaryNormalization( int *featureDim, int fileNumber, int sampleDim, double ***Wd ){
	for( unsigned int t=0; t<fileNumber; t++ ){
		for( unsigned int i=0; i<featureDim[t]; i++ ){
			double sum = 0;
			for( unsigned int j=0; j<sampleDim; j++ )
				sum += Wd[t][i][j]*Wd[t][i][j];
			sum = sqrt(sum);

			if( sum!=0 ){
				for( unsigned int j=0; j<sampleDim; j++ )
					Wd[t][i][j] = Wd[t][i][j]/sum;
			}
		}
	}
}

void saveDictionary( int fileNumber, int *featureDim, int sampleDim, double ***Wd, std::vector<std::string> dictionaryName ){

	for( unsigned int t=0; t<fileNumber; t++ ){
		FILE *fp;
        	fp = fopen( dpl::stringToChar(dictionaryName[t]), "w");
        	if( fp == NULL ){
			printf("could not find dictionary file: %s\n", dictionaryName[t].c_str());
            		exit(0);
		}
		for( unsigned int i=0; i<sampleDim; i++ ){
			for( unsigned int j=0; j<featureDim[t]; j++)
	        		fprintf(fp, "%.15lf ", Wd[t][j][i]);
			fprintf(fp, "\n");
		}
		fclose(fp);
		printf("Save dictionary file in %s\n", dictionaryName[t].c_str());
	}
}


void clearDictionary( int fileNumber, int *featureDim, double ***Wd ){
	for( unsigned int t=0; t<fileNumber; t++ ){
		for( unsigned int j=0; j<featureDim[t]; j++ )
			delete [] Wd[t][j];
		delete [] Wd[t];
	}
	delete [] Wd;
}

}

#endif /* Dictionary Generation*/
