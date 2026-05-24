# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

// Using this array to easily keep track of
// heap pointers
void* allocatedheap[1024];
int allocatedindex = 0;

typedef struct vector{
	int length;
	int *data;
} vect;

typedef struct node{
	int val;
	vect adjacents;
} Node;

typedef struct graph{
	int vertices;
	Node* nodes;
} Graph;


// Utility functions __

void freeheap(){
	for (int i = 0; i<allocatedindex; i++){
		free(allocatedheap[i]);
	}
}

void printGraph(Graph graph){
	for (int i = 0; i < graph.vertices; i++){
		Node node = graph.nodes[i];
		printf("Node %d: value: %d | adjacents: [", i, node.val);
		if (node.adjacents.length == 0){
			printf("]\n");
		}else{
			for (int j = 0; j<node.adjacents.length; j++){
				if (j == node.adjacents.length-1){
					printf("%d]\n", node.adjacents.data[j]);
					break;
				}
				printf("%d,", node.adjacents.data[j]);
			}
		}
	}
}

bool exists(Node* element, Node* list[], int length){
	for (int i=0; i<length; i++){
		if  (list[i] == element){
			return true;
		}
	}
	return false;
}
//________________________________________________


vect* matrix(int values[],int cpr[],int noOfrows){
	vect* Matrix = malloc(sizeof(vect)*noOfrows);
	allocatedheap[allocatedindex] = (void*)Matrix;
	allocatedindex ++;
	if (Matrix == NULL){
		return NULL;
	}
	int index = 0;
	for(int i = 0; i < noOfrows; i++){
		int rowlen = cpr[i];
		int *row = malloc(sizeof(int)*rowlen);
		if (row == NULL){
			return NULL;
		}
		allocatedheap[allocatedindex] = (void*)row;
		allocatedindex ++;
		for (int col = 0;col< rowlen; col++){
			// -1 represents no adjacents
			if (values[index] == -1){
				row = NULL;
				rowlen = 0;
				break;
			}
			row[col] = values[index];
			index++;
		}
		Matrix[i] = (vect){rowlen, row};
	}
	return Matrix;
}

Graph makeGraph(int values[], vect* edges, int noOfnodes,int startingNode){
	Graph g;
	Node* nodes = malloc(sizeof(Node)*noOfnodes);
	if (nodes == NULL){
		return (Graph){0, NULL};
	}
	allocatedheap[allocatedindex] = (void*)nodes;
	allocatedindex ++;
	// initializing nodes with values
	for (int i = 0; i<noOfnodes; i++){
		int val = values[i];
		nodes[i] = (Node){val};
	}
	// setting adjacents of nodes
	for (int i = 0; i<noOfnodes; i++){
		nodes[i].adjacents = edges[i];
	}
	g.vertices = noOfnodes;
	g.nodes = nodes;
	return g;
	
}

bool dfs(Graph graph, Node* current, Node* target, Node*visited[], Node* path[],  int* visitedTop, int* pathTop){
	
	visited[*visitedTop] = current;
	path[*pathTop] = current;
	(*visitedTop)++;
	(*pathTop)++;
	
	if (current == target){
		return true;
	}
	for (int i = 0; i < current->adjacents.length; i++){
		Node* neighbour = &graph.nodes[current->adjacents.data[i]];
		if (!exists(neighbour, visited, *visitedTop)){
			if (dfs(graph, neighbour, target, visited, path, visitedTop, pathTop)){
				return true;
			}
		}
	}
	(*pathTop)--;
	return false;	
}


int main(){
	int values[] = {1, 2, 3, 4, 5};
	// Creating edges matrix
	int connections[] = {1, 2, 4, 3, 1, 0, -1};
	int cpr[] = {2, 2, 1, 1, 1};
	vect* edges = matrix(connections, cpr, 5);
	// Creating graph
	Graph graph = makeGraph(values, edges, 5, 3);
	if (graph.nodes == NULL){
		printf("error while allocating memory for graph");
	} else{
		printGraph(graph);
		printf("\n");
	}
	// DFS Test
	Node* visited[graph.vertices];
	Node* path[graph.vertices];
	int pathlength = 0;
	int visitedlength = 0;
	bool pathExists = dfs(graph, &graph.nodes[0], &graph.nodes[4], visited, path, &visitedlength, &pathlength);
	if (pathExists){
		printf("Path found: ");
		for (int i = 0; i<pathlength; i++){
			if (i == pathlength-1){
				printf("%d", path[i]->val);
				break;
			}
			printf("%d -> ", path[i]->val);
		}
		printf("\n");
	} else{
		printf("Path doesnt exist");
	}
	//_______________________
	
	freeheap();
	return 0;
}