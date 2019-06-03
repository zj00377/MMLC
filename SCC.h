#ifndef SPARSE_COORDINATE_CODING_H
#define SPARSE_COORDINATE_CODING_H

namespace dpl{

static unsigned int myseed;

double getAbs( double value ){
	if( value < 0 )
		return -value;
	else
		return value;
}

double ***FeatureInitialization( int fileNumber, int sampleDim, int *featureDim, int *sampleNumber ){

	double ***feature = (double***)malloc(fileNumber*sizeof(double**));
	for( unsigned int t=0; t<fileNumber; t++ ){
		feature[t] = (double**)malloc(sampleNumber[t]*sizeof(double*));
        	for( unsigned int i=0; i<sampleNumber[t]; i++ ){
			feature[t][i] = (double*)malloc(featureDim[t]*sizeof(double));
			for( unsigned int j=0; j<featureDim[t]; j++ )
				feature[t][i][j] = 0;
		}
	}
	return feature;
}

std::vector<double> *Initialize_R( int fileNumber, int *sampleNumber ){
	std::vector<double> *R = new std::vector<double> [fileNumber];
	for( unsigned int t=0; t<fileNumber; t++ )
		R[t].assign(sampleNumber[t], 0);
	return R;
}

std::vector<std::vector<int> > *NonZeroIndexInitialization( int fileNumber, int *sampleNumber ){
	std::vector<std::vector<int> > *nonZeroIndex = new std::vector<std::vector<int> > [fileNumber];
	for( unsigned int t=0; t<fileNumber; t++ )
		nonZeroIndex[t].resize(sampleNumber[t]);
	return nonZeroIndex;
}

double ShrinkageFunction( double value, double theta ){

	if( value < -theta )
		return value+theta;
	else if( value > theta )
		return value-theta;
	else
		return 0;
}

double **Initialize_D( int dictionarySize, int sampleDim, double **Wd ){
	double **D = (double**)malloc(dictionarySize*sizeof(double*));
	for( unsigned int i=0; i<dictionarySize; i++ ){
		D[i] = (double*)malloc(sampleDim*sizeof(double));
		memcpy(D[i], Wd[i], sampleDim);
	}
	return D;
}

void CopyDictionary( double **Wd, double **D, int dictionarySize, int sampleDim ){
	for( unsigned int i=0; i<dictionarySize; i++ ){
		memcpy(Wd[i], D[i], sampleDim);
	}
}

void UpdateDictionary( double **D, double **Wd, int dictionarySize, int sampleDim ){
	for( unsigned int i=0; i<dictionarySize; i++ ){
		memcpy(D[i], Wd[i], sampleDim);
	}
}

double **Initialize_A(int fileNumber, int *featureDim ){
	double **A = (double**)malloc(fileNumber*sizeof(double*));
	for( unsigned int t=0; t<fileNumber; t++ ){
		A[t] = (double*)malloc(featureDim[t]*sizeof(double));
		memset(A[t], 0, featureDim[t]);
	}
	return A;
}

double **Initialize_A_Copy(int fileNumber, int *featureDim ){
	double **A_Copy = (double**)malloc(fileNumber*sizeof(double*));
	for( unsigned int t=0; t<fileNumber; t++ ){
		A_Copy[t] = (double*)malloc(featureDim[t]*sizeof(double));
		memset(A_Copy[t], 0, featureDim[t]);
	}
	return A_Copy;
}

void Initialize_A( double *A, double *A_Copy, int featureDim ){
	for( unsigned int i=0; i<featureDim; i++ ){
		A[i]=A_Copy[i];
		A_Copy[i]=0;
    	}
}

void Update_A( double *A, double *A_Copy, double *feature, std::vector<int> &nonZeroIndex ){
	for( unsigned int i=0; i<nonZeroIndex.size(); i++ ){
		A[nonZeroIndex[i]] += feature[nonZeroIndex[i]]*feature[nonZeroIndex[i]];
		A_Copy[nonZeroIndex[i]] += feature[nonZeroIndex[i]]*feature[nonZeroIndex[i]];
	}
}

double getNonNegativeFeature( double featureElement, double optimalT ){
	if( featureElement+optimalT>=0 )
		return optimalT;
	else
		return -1*featureElement;
}

int *getRandomIndex( int size ){
	std::vector<int> index (size);
	int *data=(int*)malloc(size*sizeof(int));
	for( unsigned int i=0; i<size; i++ )
	        index[i] = i;

	for( unsigned int i=0; i<size; i++ ){
    		int randomIndex = rand_r(&myseed)%index.size();
        	data[i] = index[randomIndex];
        	index.erase(index.begin()+randomIndex);
    	}
    	return data;
}

int getIterationNumber( int epoches, int fileNumber, int *sampleNumber ){
	int iteration = 0;
	for( unsigned int i=0; i<fileNumber; i++ )
		iteration = iteration>sampleNumber[i]? iteration:sampleNumber[i];	
	return iteration*epoches;
}

void UpdateFeature( double **Wd, double *sample, std::vector<double> &residuals, double *feature, std::vector<int> &nonZeroIndex, double lambda, int layers, int featureDim, int sampleDim, bool NonNegative ){

    	for( unsigned int i = 0; i<sampleDim; i++ ){
  		residuals[i] = -sample[i];
        	for( unsigned int j = 0; j<nonZeroIndex.size(); j++ )
        		residuals[i] += Wd[nonZeroIndex[j]][i]*feature[nonZeroIndex[j]];
    	}

	nonZeroIndex.resize(0);
	int *randomIndex = getRandomIndex(featureDim );

	for ( unsigned int i = 0; i < featureDim; i++ ){

        	double optimalT;
        	double derivative = 0;

        	for (unsigned int j = 0;j < sampleDim; j++)
                	derivative += (residuals[j]*Wd[randomIndex[i]][j]);

		optimalT = ShrinkageFunction( feature[randomIndex[i]]-derivative, lambda )-feature[randomIndex[i]];

		if( NonNegative )
			optimalT = getNonNegativeFeature( feature[randomIndex[i]], optimalT );

		feature[randomIndex[i]] += optimalT;

        	if ( optimalT!=0 ){
            		for (unsigned int j = 0;j < sampleDim; j++)
                		residuals[j] += optimalT*Wd[randomIndex[i]][j];
        	}

		if( feature[randomIndex[i]]!=0 )
			nonZeroIndex.push_back(randomIndex[i]);

	}

	for ( unsigned int k = 1; k < layers; k++ ){
		for ( unsigned int i = 0; i < nonZeroIndex.size(); i++ ){
        		double optimalT;
        		double derivative = 0;
        		for (unsigned int j = 0;j < sampleDim; j++)
                		derivative += (residuals[j]*Wd[nonZeroIndex[i]][j]);

			optimalT = ShrinkageFunction( feature[nonZeroIndex[i]]-derivative, lambda )-feature[nonZeroIndex[i]];

			if( NonNegative )
				optimalT = getNonNegativeFeature( feature[nonZeroIndex[i]], optimalT );

			feature[nonZeroIndex[i]] += optimalT;

        		if ( optimalT!=0 ){
            			for (unsigned int j = 0;j < sampleDim; j++)
                			residuals[j] += optimalT*Wd[nonZeroIndex[i]][j];
        		}
		}
	}

	nonZeroIndex.resize(0);
	for ( unsigned int i = 0; i < featureDim; i++ ){
		if( feature[i]!=0 )
			nonZeroIndex.push_back(i);
	}
	free(randomIndex);
}

void SCD( double **Wd, double *sample, std::vector<double> &residuals, double *feature, std::vector<int> &nonZeroIndex, double lambda, int layers, int featureDim, int sampleDim, bool NonNegative ){

    	for( unsigned int i = 0; i<sampleDim; i++ ){
  		residuals[i] = -sample[i];
        	for( unsigned int j = 0; j<nonZeroIndex.size(); j++ )
        		residuals[i] += Wd[nonZeroIndex[j]][i]*feature[nonZeroIndex[j]];
    	}
	int *randomIndex = getRandomIndex(featureDim );
	for ( unsigned int k = 1; k < layers; k++ ){
		for ( unsigned int i = 0; i < featureDim; i++ ){
        		double optimalT;
        		double derivative = 0;

        		for (unsigned int j = 0;j < sampleDim; j++)
                		derivative += (residuals[j]*Wd[randomIndex[i]][j]);

			optimalT = ShrinkageFunction( feature[randomIndex[i]]-derivative, lambda )-feature[randomIndex[i]];

			if( NonNegative )
				optimalT = getNonNegativeFeature( feature[randomIndex[i]], optimalT );

			feature[randomIndex[i]] += optimalT;

        		if ( optimalT!=0 ){
            			for (unsigned int j = 0;j < sampleDim; j++)
                			residuals[j] += optimalT*Wd[randomIndex[i]][j];
        		}
		}
	}
	free(randomIndex);
}

void UpdateWd( double **Wd, std::vector<double> &residuals, double *feature, double *A, std::vector<int> &nonZeroIndex, int sampleDim, bool NonNegative ){

    	for ( unsigned int i = 0; i < sampleDim; i++ ){
        	for ( unsigned int j = 0; j < nonZeroIndex.size(); j++ ){
                if( NonNegative && Wd[nonZeroIndex[j]][i] - feature[nonZeroIndex[j]]*residuals[i]*dpl::learningRate(A,nonZeroIndex[j])<0 )
                    Wd[nonZeroIndex[j]][i] = 0;
                else
                    Wd[nonZeroIndex[j]][i] = Wd[nonZeroIndex[j]][i] - feature[nonZeroIndex[j]]*residuals[i]*dpl::learningRate(A,nonZeroIndex[j]);
            }
    	}
}

void NormalizeWd( double **Wd, std::vector<int> &nonZeroIndex, int sampleDim ){
	for( unsigned int i=0; i<nonZeroIndex.size(); i++ ){
		double sum = 0;
		for( unsigned int j=0; j<sampleDim; j++ )
			sum += Wd[nonZeroIndex[i]][j]*Wd[nonZeroIndex[i]][j];
		sum = sqrt(sum);

		if( sum!=0 ){
			for( unsigned int j=0; j<sampleDim; j++ )
				Wd[nonZeroIndex[i]][j] = Wd[nonZeroIndex[i]][j]/sum;
		}
	}
}

void saveFeature( int fileNumber, int *featureDim, int *sampleNumber, std::vector<std::string> featureFileName, double ***feature ){

	for( unsigned int t=0; t<fileNumber; t++ ){
		FILE *fp;
        	fp = fopen( dpl::stringToChar(featureFileName[t]), "w");
        	if( fp == NULL ){
			printf("could not find dictionary file: %s\n", featureFileName[t].c_str());
            		exit(0);
		}
		for( unsigned int i=0; i<sampleNumber[t]; i++ ){
			for( unsigned int j=0; j<featureDim[t]; j++)
	        		fprintf(fp, "%.15lf ", feature[t][i][j]);
			fprintf(fp, "\n");
		}
		fclose(fp);
		printf("Save dictionary file in %s\n", featureFileName[t].c_str());
	}
}

void saveNonZeroIndex( std::vector<int> *nonZeroIndex, char *IndexFileName, int featureDim, int sampleNumber ){

	printf("Save nonZero index in %s\n", IndexFileName);

	FILE *fp;
    	fp = fopen( IndexFileName, "w");
    	if( fp == NULL ){
		printf("could not find index file %s\n", IndexFileName);
        	exit(0);
	}

	for( unsigned int i=0; i<sampleNumber; i++ ){
		for( unsigned int j=0; j<nonZeroIndex[i].size(); j++)
	        	fprintf(fp, "%d ", nonZeroIndex[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void clearFeature( int fileNumber, int *sampleNumber, double ***feature ){
	for( unsigned int t=0; t<fileNumber; t++ ){
        	for( unsigned int i=0; i<sampleNumber[t]; i++ )
			delete [] feature[t][i];	
		delete [] feature[t];
	}
	delete [] feature;
}

double computeLassoResult( double **Wd, double *sample, double *feature, double lambda, int sampleDim, int featureDim ){

	double LassoResult = 0;
	double residuals;
	for( unsigned int i=0; i<sampleDim; i++ ){
		residuals = -sample[i];
		for( unsigned int j=0; j<featureDim; j++ )
			residuals += Wd[j][i]*feature[j];

		LassoResult += residuals*residuals;
	}

	double sum_feature = 0;
	for( unsigned int j=0; j<featureDim; j++ )
		sum_feature += getAbs(feature[j]);

    	return 0.5*LassoResult+lambda*sum_feature;
}


void CalculateError(  double **Wd,  double **sample, double **feature, double lambda, int sampleNumber, int sampleDim, int featureDim ) {

	double TotalDecError = 0;
	for( unsigned int t=0; t<sampleNumber; t++ ){
		TotalDecError += computeLassoResult( Wd, sample[t], feature[t], lambda, sampleDim, featureDim);
	}
	TotalDecError /= sampleNumber;
	std::cout<<"Total Decode Error is "<<TotalDecError<<std::endl;
}

void trainDecoder( double ***Wd, double ***feature, double ***sample, double lambda, int layers, int *featureDim, int *sampleNumber, int fileNumber, int sampleDim, int dictionarySize, int epoches, bool NonNegative ){

	std::vector<double> *residuals = Initialize_R( fileNumber, sampleNumber );
	double **A = Initialize_A( fileNumber, featureDim );
	double **A_Copy = Initialize_A_Copy( fileNumber, featureDim );
	double **D = Initialize_D( dictionarySize, sampleDim, Wd[0] );
	int iterationNumber = getIterationNumber( epoches, fileNumber, sampleNumber );
	std::vector<std::vector<int> > *nonZeroIndex = NonZeroIndexInitialization( fileNumber, sampleNumber );

	srand((unsigned)time(0));
	myseed = (unsigned int) RAND_MAX * rand();
	std::cout<<"Train decoder"<<std::endl;
	double ComputionalTime = 0;
	clock_t BeginTime = clock();
    	for( unsigned int it=0; it<iterationNumber; it++ ){
	    	for( unsigned int t=0; t<fileNumber; t++ ){
	    	    int i = it%sampleNumber[t];
		    if( i==0 )
			Initialize_A( A[t], A_Copy[t], featureDim[t] );

		    CopyDictionary( Wd[t], D, dictionarySize, sampleDim );

		    UpdateFeature( Wd[t], sample[t][i], residuals[t], feature[t][i], nonZeroIndex[t][i], lambda, layers, featureDim[t], sampleDim, NonNegative );

		    Update_A( A[t], A_Copy[t], feature[t][i], nonZeroIndex[t][i] );

		    UpdateWd( Wd[t], residuals[t], feature[t][i], A[t], nonZeroIndex[t][i], sampleDim, NonNegative );

		    NormalizeWd( Wd[t], nonZeroIndex[t][i], sampleDim );

		    UpdateDictionary( D, Wd[t], dictionarySize, sampleDim );

		    if( it%sampleNumber[t]==sampleNumber[t]-1 ){
	        	std::cout<<"Sample "<<t<<" epoch "<<(it+1)/sampleNumber[t]<<" finished"<<std::endl;
	        	//CalculateError( Wd[t], sample[t], feature[t], lambda, sampleNumber[t], sampleDim, featureDim[t] );
	    	    }
		}
        }

	for( unsigned int t=0; t<fileNumber; t++ ){
		CopyDictionary( Wd[t], D, dictionarySize, sampleDim );
		for( unsigned int i=0; i<sampleNumber[t]; i++ )
			UpdateFeature( Wd[t], sample[t][i], residuals[t], feature[t][i], nonZeroIndex[t][i], lambda, 20, featureDim[t], sampleDim, NonNegative );
		std::cout<<"Sample "<<t<<" ";
		//CalculateError(Wd[t], sample[t], feature[t], lambda, sampleNumber[t], sampleDim, featureDim[t]);
	}

	clock_t EndTime = clock();
	ComputionalTime = (double)(EndTime-BeginTime)/CLOCKS_PER_SEC;

   	std::cout<<"Finish decoding process:"<<std::endl;
	std::cout<<"Train Decode Time is "<<ComputionalTime<<" seconds."<<std::endl;
}


}

#endif /* Sparse Coordinate Coding */

