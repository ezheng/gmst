
#include "graph.h"
#include <mex.h>
#include <matrix.h>

//#include 

#include  <stdio.h>

void mexFunction(int			nlhs, 		/* number of expected outputs */
	mxArray		*plhs[],	/* mxArray output pointer array */
	int			nrhs, 		/* number of inputs */
	const mxArray	*prhs[]		/* mxArray input pointer array */)
{


	// get the input. 
	if(nrhs < 2)
		mexErrMsgTxt(" edges = removeEdges[vertices, edges, (fileName)]. The vertices should be 1 1 1; 2 2 2; 3 3 3, where the number is the cluster ID. There should be at least 2 input variables");

	mwSize numOfEdges = mxGetM(prhs[1]);
	mwSize width = mxGetN(prhs[1]);
	mwSize numOfVertices = mxGetM(prhs[0]) * mxGetN(prhs[0]);

	if(width != 3)
		mexErrMsgTxt("The size of variable edges should be [m*3], where m is the number of edges");
	
	// 
	if( !mxIsClass(prhs[0], "int32") || !mxIsClass(prhs[1], "int32") )
		mexErrMsgTxt("The input variables should have int32 type\n");

	int *verticesData = (int*)mxGetData(prhs[0]);
	int *edgeData = (int*)mxGetData(prhs[1]);
	Instance instanceObj(verticesData, numOfVertices, edgeData, numOfEdges);

	Graph::createInstance(instanceObj);
	double percentageEdges = Graph::graph.reductionTests();
	mexPrintf("%%%f percentage of edges left\n", percentageEdges*100);
	// output the reduced edges. I think the best way is to write down

	if(nrhs == 3)
	{		
		size_t buflen = mxGetNumberOfElements(prhs[2]) + 1;
		char *buf = (char *)mxCalloc(buflen, sizeof(char));
		if (mxGetString(prhs[2], buf, buflen) != 0)
			mexErrMsgTxt("Could not convert string data.");
		string fileName(buf, buflen);
		mxFree(buf);
		if(!Graph::graph.writeEdges(fileName.c_str()))
			mexErrMsgTxt("Cannot open files for writing");
	}


	return;
}

