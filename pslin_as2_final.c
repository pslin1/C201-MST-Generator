/*Submitting student: Shu-Jun Pierre Lin */
/*Collaborating classmates: Chanel Cheng */
/*References: www.tutorialspoint.com , www.stackoverflow.com*/




#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>

#define BLACK 1
#define WHITE 0


struct node {
  int colour;
  int node_number;
  int pos[2];
  int parent;
};
struct edge {
  int u;
  int v;
  int length;
};
  
void read_values(int argc, char*argv[], struct node node_list[]); //function 3 declared up here to prevent implicit declaration in function 2
    
int function1(int argc, char*argv[]) {
  //will be called in main and return value will tell function 4 what to output
  FILE *fp;
  int inputfile = 0, outputfile = 0;
  int i; //used for iterating through for loops
  if (argc==1) {
    return 3;
  }
  for (i=1; i<argc; i++) {
    //iterates until argc is reached and assigns the indices to inputfile and outputfile
    if (strcmp(argv[i], "-i")==0) {
      //input specified
      inputfile=i+1;
    }
    else if (strcmp(argv[i], "-o")==0) {
      //output specified
      outputfile=i+1;
    }
    if ((fp=fopen(argv[inputfile], "r")) == NULL) {
      //input is invalid
      printf("pslin_as1 [-i inputfile [-outputfile]]");
      return -1;
    }
  }
  if (inputfile ==0 && outputfile>0) {
    //if there is an output file but no input file
    printf("pslin_as1 [-i inputfile [-o outputfile]]");
    return -2;
  }
  if (inputfile >0 && outputfile ==0) {
    //input but no output
    return 1;
  }
  if (inputfile >0 && outputfile >0) {
    //input and output specified
    return 2;
  }

  
  return 0;
}


int find_num_pts(int argc, char*argv[]) {
  int num_pt;
  char str[255];
  int linecount;
  int num1;
  FILE *fp;
  if (strcmp(argv[1] , "-i")==0) {
    fp=fopen(argv[2], "r");
  }
  else {
    fp=fopen(argv[4] , "r");
  }
  while (fgets(str, sizeof(str), fp)) {
    if (str[0] != '#') { //line count will increase if it is non comment line
      //if we are at line containing max values
      if (linecount==1) {
	if (sscanf(str,"%d",&num1) == 1) {
	  num_pt=num1;
	  return num_pt;
	}
      }
      
      linecount++;  
    }
    
  }
  return 0;
}
  
void read_values(int argc, char*argv[], struct node node_list[]) { 
  int max_x,max_y,num_pt;
  int *xcoord, *ycoord;
  int num3; //for detecting too many maximum values
  char str[255]; //character to store each line we read
  int linecount=0; //count which line we are on
  int num1,num2; //temp values for storing numbers
  FILE *fp;
  if (strcmp(argv[1] , "-i")==0) {
    fp=fopen(argv[2], "r");
  }
  else {
    fp=fopen(argv[4] , "r");
  }

  while (fgets(str,sizeof(str),fp)) {
    //ignores comment line
    if (str[0] != '#' && str[0] != '\n') { //line count will increase if it is non comment line
      //if we are at line containing max values
      if (linecount==0) {
	if (sscanf(str,"%d %d", &num1, &num2) != 2) {
	  printf("Please enter a maximum y value");
	}
	if (sscanf(str,"%d %d %d",&num1, &num2, &num3)==3) {
	  printf("Too many maximum values");
	}
	max_x=num1; //because we have num1
	max_y=num2; //because we have num2
      }
      //if we are at line containing num_pt
      if (linecount==1) {
	if (sscanf(str,"%d",&num1) !=1) {
	  printf("Please enter a value for the number of points");
	}
	num_pt=num1;
	xcoord = (int *)malloc(num_pt * sizeof(int));
	ycoord = (int *)malloc(num_pt * sizeof(int));
      }
      //if we are at coordinates line
      if (linecount>1){
	int index=linecount-2; //used for keeping track of coord lines
	if (sscanf(str,"%d %d", &num1,&num2)!=2) {
	printf("Please enter valid x and y coordinates");
	}
	if ((num1>=0) && (num1<=max_x) && (num2>=0) && (num2<=max_y)){
	  //checks if coords are in bounds
	  xcoord[index]=num1; //assigns value to correct array index
	  ycoord[index]=num2; //assigns value to correct array index
	}
	
      }
      linecount++;
    }
    
  }
  

  int i;
  for (i=0; i<=num_pt-1; i++) {
    node_list[i].node_number = i;
    node_list[i].colour = WHITE;
    node_list[i].parent = -1;
    node_list[i].pos[0]=xcoord[i];
    node_list[i].pos[1]=ycoord[i];
  }

}

void print_root_dist(int mode,char *argv[], struct node node_list[], int num_pt, int **adjacency_matrix) {
  //will not run if function 2 returns negative value
  //mode is the int that function 1 returns
  //will define an int in main for mode
  struct node root = node_list[0];
  int x_dist; //x distance from node to root
  int y_dist; //y distance from node to root
  int total_dist; //rectilinear distance from node i to root
  int i; //to keep track of which nodes we compare to root
  int j; //for running through for loop (printing arrays)
  int k; //keeps track of where the output file is 
  int n; //for making adjacency matrix
  if (strcmp(argv[1] , "-o")==0) {
    k=2;
  }
  else {
    k=4;
  }
  //building adjacency matrix
  for (n=1; n<=num_pt; n++) {
    int l=n-1;
    for (i=1; i<=num_pt; i++) {
      x_dist = abs(node_list[i].pos[0] - node_list[n-1].pos[0]);
      y_dist = abs(node_list[i].pos[1] - node_list[n-1].pos[1]);
      total_dist = x_dist + y_dist;
      node_list[i].parent = 0;
      *((int *)adjacency_matrix + l * num_pt + i) = total_dist;
    
    }
  }
  //x=0, y=0 column
  for (i=1; i<num_pt; i++) {
    x_dist = abs(node_list[i].pos[0] - root.pos[0]);
    y_dist = abs(node_list[i].pos[1] - root.pos[1]);
    total_dist = x_dist + y_dist;
    node_list[i].parent = 0;
    *((int *)adjacency_matrix + i * num_pt + 0) = total_dist;
    
  }
  *((int *)adjacency_matrix + 0 * num_pt + 0) = 0;
  
  //input only
  if (mode == 1) {
    printf("Choosing point 0 as the root...\n");
    for (j=0;j<num_pt-1;j++) {
      int f = j+1;
      printf("Point %d has a distance %d to the tree, parent %d;\n", j+1, *((int *)adjacency_matrix +0 * num_pt + f), node_list[j+1].parent);
      //printf("HERE");
    }
    //printf("HERE");
  }
  //printf("HERE");
  //input and output
  if (mode == 2) {
    char *output_name;
    output_name = argv[k];
    FILE *outfile;
    outfile = fopen(output_name, "a");
    fprintf(outfile, "Choosing point 0 as the root...\n");
    for (j=0;j<num_pt-1;j++) {
      fprintf(outfile, "Point %d has a distance %d to the tree, parent %d;\n", j+1, *((int *)adjacency_matrix +0 *num_pt + (j+1)), node_list[j+1].parent);
    }

    fclose(outfile);
      
  }
  //printf("END");
}

int check_if_white_nodes(int num_pt, struct node node_list[]) {
  int i;
  for (i=0;i<=num_pt-1;i++) {
    if (node_list[i].colour == WHITE) {
      return 1; //at least one node is white
    }
    
  }
  return 0; // no more white nodes
}

void build_tree(int mode, int num_pt, int **adjacency_matrix, struct edge tree[], struct node node_list[], char *argv[]) {
  int i,j,n; //for running through for loops
  int k=0; //for determining which item in tree array we are currently on
  int b=0; //determining where output file is
  int x,y; //temp variable for storing u and v values of shortest distance
  int value; //temp variable for storing distances
  int difference1, difference2; //used in tie breaker to find difference between y values
  int max_x1, max_x2; //used in tie breaker to find the greatest x value
  if (strcmp(argv[1] , "-o")==0) {
    b=2;
  }
  else {
    b=4;
  }
  //setting all lengths in tree to undetermined value
  for (n=0;n<=num_pt-2;n++) {
    tree[n].length=-1;
  }
  //setting root colour to black
  node_list[0].colour = BLACK;
  while (check_if_white_nodes(num_pt, node_list) == 1) {
    //finding smallest distance
    for (i=0;i<=num_pt-1;i++) {
      if (node_list[i].colour == BLACK) {
	for (j=0;j<=num_pt-1;j++) {
	  if (node_list[j].colour == WHITE) {
	    value = *((int *)adjacency_matrix + i * num_pt + j);
	    x=i; //black node
	    y=j; //white node
	  
	  
	    if (tree[k].length == -1) { //if that tree item doesn't have a length yet
	      tree[k].length = value;
	      tree[k].u = x;
	      tree[k].v = y;
	    }
	    if (tree[k].length > 0 && tree[k].length > value) { //if that tree item has a non minimal length
	      tree[k].length = value;
	      tree[k].u = x;
	      tree[k].v = y;
	    }
	    if (tree[k].length > 0 && tree[k].length == value) { //tiebreaker
	      difference1 =abs(node_list[tree[k].u].pos[1] - node_list[tree[k].v].pos[1]);
	      difference2 =abs(node_list[x].pos[1] - node_list[y].pos[1]);
	      if (difference2 > difference1) { //condition 4.1
		tree[k].length = value;
		tree[k].u = x;
		tree[k].v = y;
	      }
	      if (difference1 == difference2) { //condition 4.2
		if (node_list[tree[k].u].pos[0] > node_list[tree[k].v].pos[0]) {
		  max_x1 = node_list[tree[k].u].pos[0];
		}
		else {
		  max_x1 = node_list[tree[k].v].pos[0];
		}
				     
		if (node_list[x].pos[0] > node_list[y].pos[0]) {
		  max_x2 = node_list[x].pos[0];
		}
		else {
		  max_x2 = node_list[y].pos[0];
		}
		if (max_x1 <= max_x2) {
		  tree[k].length = value;
		  tree[k].u = x;
		  tree[k].v = y;
		}
	      }
	    }
	  }
	}
      }
	
    }
    //setting new node added to tree to black
    node_list[tree[k].v].colour = BLACK;
    //updating parents
    for (i=0;i<=num_pt-1;i++) {
      if (node_list[i].colour == BLACK) {
	for (j=0;j<=num_pt-1;j++) {
	  if (node_list[j].colour == WHITE) {
	    value = *((int *)adjacency_matrix + i * num_pt + j);
	    x=i; //black node
	    y=j; //white node
	    if (tree[k].length > 0 && tree[k].length > value) {
	      node_list[tree[k].v].parent = tree[k].u;
	    }
	    if (tree[k].length > 0 && tree[k].length == value) { //tiebreaker
	      difference1 =abs(node_list[tree[k].u].pos[1] - node_list[tree[k].v].pos[1]);
	      difference2 =abs(node_list[x].pos[1] - node_list[y].pos[1]);
	      if (difference2 > difference1) { //condition 4.1
	        node_list[tree[k].v].parent = tree[k].u;
	      }
	      if (difference1 == difference2) { //condition 4.2
		if (node_list[tree[k].u].pos[0] > node_list[tree[k].v].pos[0]) {
		  max_x1 = node_list[tree[k].u].pos[0];
		}
		else {
		  max_x1 = node_list[tree[k].v].pos[0];
		}
				     
		if (node_list[x].pos[0] > node_list[y].pos[0]) {
		  max_x2 = node_list[x].pos[0];
		}
		else {
		  max_x2 = node_list[y].pos[0];
		}
		if (max_x1 <= max_x2) {
		  node_list[tree[k].v].parent = tree[k].u;
		}
	      }
	    }
	  }
	}
      }
    }
    //iterates k so that the next tree position can be filled
    k++;
  }
    //input only
  if (mode == 1) {
    printf("#Edges of the MST by Prim's algorithm:\n");
    for (j=0;j<=num_pt-2;j++) {
      printf("%d %d %d\n", tree[j].u, tree[j].v, tree[j].length);
    }
  }
  //input and output
  if (mode == 2) {
    char *output_name;
    output_name = argv[b];
    FILE *outfile;
    outfile = fopen(output_name, "a");
    fprintf(outfile, "#Edges of the MST by Prim's algorithm:\n");
    for (j=0;j<=num_pt-2;j++) {
      fprintf(outfile, "%d %d %d\n" ,tree[j].u, tree[j].v, tree[j].length);
    }

    fclose(outfile);
      
  }
}

void find_MST_length(int mode, int num_pt,struct edge tree[], char *argv[]) {
  int i;
  int length=0;
  int k=0;
  if (strcmp(argv[1] , "-o")==0) {
    k=2;
  }
  else {
    k=4;
  }
  for (i=0;i<=num_pt-2;i++) {
    length = length + tree[i].length;
  }
  //input only
  if (mode == 1) {
    printf("The total length of the MST is %d.\n", length);
  }
  //input and output
  if (mode == 2) {
    char *output_name;
    output_name = argv[k];
    FILE *outfile;
    outfile = fopen(output_name, "a");
    fprintf(outfile, "The total length of the MST is %d.\n", length);

    fclose(outfile);
      
  }
}

void print_to_input(int num_pt, struct edge tree[], char *argv[]) {
  FILE *fp; //remember to close file
  int n;
  if (strcmp(argv[1] , "-i")==0) {
    fp=fopen(argv[2], "a");
  }
  else {
    fp=fopen(argv[4] , "a");
  }
  fprintf(fp, "\n#Edges of the MST by Prim's algorithm:\n");
  for (n=0;n<=num_pt -2;n++) {
    fprintf(fp, "%d %d %d\n", tree[n].u, tree[n].v, tree[n].length);
  }
  fclose(fp);
}

int main( int argc, char *argv[]) { //valid==2 and mode==3 if no input
  int mode=function1(argc, argv); //remember to uncomment
  int num_pt = find_num_pts(argc, argv);
  struct node node_list[num_pt];
  struct edge tree[num_pt-1];
  int **adjacency_matrix = malloc(num_pt*sizeof(**adjacency_matrix));
  for (size_t i =0; i<=num_pt; i++) {
    adjacency_matrix[i] = malloc(num_pt*sizeof(int));
  }
  read_values(argc, argv, node_list);
  print_root_dist(mode,argv, node_list,num_pt, adjacency_matrix);
  build_tree(mode, num_pt, adjacency_matrix, tree, node_list,argv);
  find_MST_length(mode, num_pt,tree, argv);
  print_to_input(num_pt, tree, argv);
  

  return 0;
}
    
  
  
  
  
