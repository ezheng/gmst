/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 30 de Maio de 2006                                          *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "instancegenerator.h"

/**
* Metodo que gera uma instancia com as características passadas
*/
void InstanceGenerator::generateInstance(unsigned int numVertices, unsigned int numClusters){

	//cout << "Gerando instancia com " << numVertices << " vertices e " << numClusters << " clusters... " << endl;
	cout << "Generating instance with" << numVertices << " vertices" << numClusters << " clusters..." << endl;
	srand(time(NULL));
	vector<unsigned int> posX(numVertices), posY(numVertices);	

	vector<int> position(MAX_X, false);
	vector< vector<int> > positions(MAX_Y, position);
	
	//Definindo posicao dos vertices
	//Setting position of vertices
	for(unsigned int i=0; i < numVertices; i++){

		do{
			posX[i] = rand()%MAX_X;
			posY[i] = rand()%MAX_Y;
		} while(positions[posX[i]][posY[i]]);
		positions[posX[i]][posY[i]] = true;
	}
	
	//Criando grupos
	vector<unsigned int> vertices = generateClusters(posX, posY, numClusters);

	//Criando arestas
	vector<unsigned int> edges1, edges2, edgesCosts;
	
	for(unsigned int i=0; i < numVertices; i++){

		for(unsigned int j=i+1; j < numVertices; j++){
			//Se os vertices pertencem a clusters diferentes
			if(vertices[i] != vertices[j]){
				edges1.push_back(i);
				edges2.push_back(j);

				unsigned int cost = (unsigned int)sqrt(pow((double)posX[i]-(double)posX[j],2.0)+pow((double)posY[i]-(double)posY[j],2.0));
				edgesCosts.push_back(cost);
			}
		}
	}

	cout << "... salvando os arquivos..." << endl;
	printToFiles(posX, posY, vertices, edges1, edges2, edgesCosts, numVertices, numClusters);
	
	cout << "... instancia gerada!" << endl;
}

/**
* Metodo que gera todas as instancias
*/
void InstanceGenerator::generateAll(){

	generateInstance(500, 5);
	generateInstance(500, 10);
	generateInstance(500, 50);
	generateInstance(500, 100);

	generateInstance(1000, 10);
	generateInstance(1000, 20);
	generateInstance(1000, 100);
	generateInstance(1000, 200);

	generateInstance(2000, 20);
	generateInstance(2000, 40);
	generateInstance(2000, 200);
	generateInstance(2000, 400);

	generateInstance(5000, 50);
	generateInstance(5000, 100);
	generateInstance(5000, 500);
	generateInstance(5000, 1000);
}

/**
* Metodo que imprime a instancia em dois arquivos: 
* v_c.clu, no formato utilizado pelo programa
* v_c.coo, com as coordenadas da instancia
* onde v e c são o numero de vertices e de clusters da instancia, respectivamente
*/
void InstanceGenerator::printToFiles(vector<unsigned int> posX, vector<unsigned int> posY, vector<unsigned int> vertices, vector<unsigned int> edges1, vector<unsigned int> edges2, vector<unsigned int> edgesCosts, unsigned int numVertices, unsigned int numClusters){

	string cooFile = printToCooFile(posX, posY, numVertices, numClusters);
	string instanceFile = printToCluFile(vertices, edges1, edges2, edgesCosts, numVertices, numClusters);
	generateClusterFiles(instanceFile, cooFile, numVertices, numClusters);
}

/**
* Imprime a instancia em um arquivo com suas coordenadas
* Prints the instance in a file with their coordinates
*/
string InstanceGenerator::printToCooFile(vector<unsigned int> posX, vector<unsigned int> posY, unsigned int numVertices, unsigned int numClusters){

	ostringstream oss;
	oss << "instances/" << numClusters << "inst" << numVertices << ".coo";
	string fileName = oss.str();
	ofstream fout(fileName.c_str(), ios::out);	

	if(fout.fail()){
		cout << "Erro ao criar aquivo " << fileName << "!" << endl;

	} else {
	
		for(unsigned int i=0; i < numVertices; i++){
			fout << posX[i] << " " << posY[i] << endl;
		}
		fout.close();
	}
	return fileName;
}

/**
* Imprime a instancia em um arquivo no formato  que sera utilizado pelo programa
* Prints the instance in a file format that will be used by the program
*/
string InstanceGenerator::printToCluFile(vector<unsigned int> vertices, vector<unsigned int> edges1, vector<unsigned int> edges2, vector<unsigned int> edgesCosts, unsigned int numVertices, unsigned int numClusters){

	ostringstream oss;
	oss << "instances/" << numClusters << "inst" << numVertices << ".clu";
	string fileName = oss.str();
	ofstream fout(fileName.c_str(), ios::out);	

	if(fout.fail()){
		cout << "Erro ao criar aquivo " << fileName << "!" << endl;

	} else {

		//Imprimindo grupo de cada vertice
		for(unsigned int i=0; i < numVertices; i++){	
			fout << vertices[i] << endl;
		}

		fout << "" << endl;
		//Imprimindo cada aresta
		for(unsigned int i=0; i < edges1.size(); i++){
			fout << edges1[i] << " " << edges2[i] << " " << edgesCosts[i] << endl;
		}
		
		fout.close();
	}
	return fileName;
}

/**
* Metodo que faz o agrupamento dos vertices, criando os clusters
*/
vector<unsigned int> InstanceGenerator::generateClusters(vector<unsigned int> posX, vector<unsigned int> posY, unsigned int numberOfClusters){

	unsigned int numVertices = posX.size();

	//Define o grupo de que cada vertice fara parte
	vector<unsigned int> vertices(numVertices);
	//Primeiro escolhe 'numberOfClusters' vertices para serem os centros, um para cada cluster
	vector<unsigned int> centers(numberOfClusters);
	//Flag que indica se o vertice ja foi escolhido
	vector<bool> flag(numVertices, false);

	//Escolhe o primeiro centro aleatoriamente
	centers[0] = rand()%numVertices;
	flag[centers[0]] = true;
	vertices[centers[0]] = 1;

	//cout << "\n1o centro: " << posX[centers[0]] << ", " << posY[centers[0]] << endl;
	//Escolhendo os k centros
	for(unsigned int i=1; i < numberOfClusters; i++){

		float positionX = 0;
		float positionY = 0;

		for(unsigned int j=0; j < i; j++){
			positionX += posX[centers[j]];
			positionY += posY[centers[j]];
		}
		positionX /= i;
		positionY /= i;

		//O centro corrente é o vertice mais distante do ponto medio entre os centros escolhidos
		centers[i] = farthestVertex(positionX, positionY, posX, posY, flag);
		flag[centers[i]] = true;
		vertices[centers[i]] = i+1;

		//cout << (i+1) << "o centro: " << posX[centers[i]] << ", " << posY[centers[i]] << endl;
	}

	//Escolhendo os clusters a que vao se unir cada vertice
	for(unsigned int i=0; i < numVertices; i++){

		if(!flag[i]){
			vertices[i] = vertices[closestCenter(i, posX, posY, centers)];
			flag[i] = true;
		}
	}

	return vertices;
}

/**
* Escolhe o centro mais proximo do vertice passado
*/
unsigned int InstanceGenerator::closestCenter(unsigned int vertex, vector<unsigned int> posX, vector<unsigned int> posY, vector<unsigned int> centers){

	unsigned int closestCenter = 0;
	unsigned int closestDistance = INFINITE;

	for(unsigned int i=0; i < centers.size(); i++){
		unsigned int distance = (unsigned int)sqrt(pow((double)posX[vertex]-(double)posX[centers[i]],2.0) + pow((double)posY[vertex]-(double)posY[centers[i]],2.0));
		if(distance < closestDistance){
			closestCenter = centers[i];
			closestDistance = distance;
		}
	}
	//cout << "Vertice (" << posX[vertex] << ", " << posY[vertex] << ") perto do centro (" << posX[closestCenter] << ", " << posY[closestCenter] << ")" << endl;
	return closestCenter;
}

/**
* Procura o vertice mais distance das coordenadas passadas
*/
unsigned int InstanceGenerator::farthestVertex(float positionX, float positionY, vector<unsigned int> posX, vector<unsigned int> posY, vector<bool> flag){

	unsigned int farthestVertex = 0;
	unsigned int farthestDistance = 0;

	for(unsigned int i=0; i < posX.size(); i++){
		//Se o vertice pode ser escolhido...
		if(!flag[i]){
			unsigned int distance = (unsigned int)sqrt(pow((double)positionX - (double)posX[i],2.0)+pow((double)positionY - (double)posY[i],2.0));
			if(distance > farthestDistance){
				farthestVertex = i;
				farthestDistance = distance;
			}
		}
	}

	return farthestVertex;
}

/**clusters[cluster]
* Metodo que gera um arquivo para cada cluster da instancia passada
*/
void InstanceGenerator::generateClusterFiles(string instance, string cooFile, unsigned int numVertices, unsigned int numClusters){
	
	vector<int> posX(numVertices), posY(numVertices);
	//vector<int> vertices(numVertices);
	vector< vector<int> > clusters(numClusters);

	ifstream fin1(instance.c_str(), ios::in);
	//Lendo qual é o cluster de cada vertice do arquivo da instancia
	if(fin1.fail()){
		cout << "Erro ao abrir arquivo " << instance << "!" << endl;
	} else {
		for(unsigned int i=0; i < numVertices; i++){
			int cluster;
			fin1 >> cluster;
			clusters[cluster-1].push_back(i);
		}
		fin1.close();
	}

	ifstream fin2(cooFile.c_str(), ios::in);
	//Lendo a posição de cada vertice no plano
	if(fin2.fail()){
		cout << "Erro ao abrir arquivo " << cooFile << "!" << endl;
	} else {
		
		for(unsigned int i=0; i < numVertices; i++){
			fin2 >> posX[i] >> posY[i];
		}
		fin2.close();
	}	

	//Criando arquivo que conterá as posições dos vértices de cada cluster
	ostringstream oss1;
	oss1 << "plotFiles/" << numClusters << "inst" << numVertices << ".coo";
	string cooFileName = oss1.str();
	ofstream cooFile1(cooFileName.c_str(), ios::out);
	//ofstream cooFile(cooFileName.c_str(), ios::out);

	//Inserindo os dados por linha do arquivo
	unsigned int line = 0;
	while(line < numVertices){

		//Inserindo a posição de um vértice de cada cluster
		for(unsigned int cluster=0; cluster < numClusters; cluster++){
			//Se o cluster ainda tiver algum vértice
			if(clusters[cluster].size() > line){
				cooFile1 << posX[clusters[cluster][line]] << " " << posY[clusters[cluster][line]] << " ";
			}
		}
		cooFile1 << endl;
		line++;
	}	
	cooFile1.close();

	//Gerando arquivo do gnuplot
	ostringstream oss2;
	oss2 << "plotFiles/" << numClusters << "inst" << numVertices <<  ".gnu";
	string gnuFileName = oss2.str();			
	ofstream gnuFile(gnuFileName.c_str(), ios::out);
	gnuFile << "set output \""<< numClusters << "inst" << numVertices << ".eps\"" << endl;
	gnuFile << "set terminal postscript eps color" << endl;
	gnuFile << "plot [0:1000][0:1000]";

	for(unsigned int i=1; i <= numClusters; i++){
		gnuFile << " \"" << numClusters << "inst" << numVertices << ".coo" << "\" using " << i*2-1 << ":" << i*2 << " title \"Grupo "<< i << "\"";
		if(i < numClusters){
			gnuFile << ",";
		}
	}

	gnuFile << endl;
	gnuFile.close();
}
