#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Size of each line of csv file to astarsearch function
#define csv_size 7
using namespace std;


// Utility structures

// Cell coordenates<line,coulmn>
typedef pair<int,int> Pair;

// Cell with f value<f value, (line,colunm)>
typedef pair<double,pair<int,int>> Pairs;

// Cell {f,g,h,(line, coulnm)}
typedef struct{

    int parent_i;
    int parent_j;

    double f,g,h;
} cell;


// FUnction to pick a csv file and convert into a map
map<int,vector<int>> readFileIntoMap(const string& path) 
{
    auto ss = ostringstream{};
    ifstream input_file(path);

    // Map to csv data
    map<int, vector<int>> mat;

    // Vector to each line of csv data
    vector<int> num;

    // String to csv data
    string record;

    // Separator  csv=;
    char delimiter = ';';

    // IF csv file dont open
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();

    // Convert the csv file into strings
    istringstream sstream(ss.str());

    int counter = 0;

    // Get lines
    while (getline(sstream, record)) {
        istringstream line(record);

        // Get each number in line
        while (getline(line, record, delimiter)) {

                // Convert to int
                num.push_back(stoi(record));     
        }

        // Store line in the map
        mat[counter]=num;
        num.clear();
        counter += 1;
    }
    return mat;
}

// Function to show the map
void mostra_csv(map<int,vector<int>> mat)
{

    for(int i = 0;i<mat.size();++i)
    {
       for(int j =0; j<mat[i].size();++j)
       {
           cout<<mat[i][j];
       }
       cout<<"\n";
   }
    return;
}

// Verify if the coordenates are valide
bool verify(int x, int y, int rows, int colunms)
{

    if((x>=0  && x<=rows) &&  (y>=0 && y<=colunms))
    {
        return true;
    }

    return false;
}

// Verify if the the cell is blocked
bool blocked(map<int,vector<int>> map, int row, int col)
{

    // blocked == 1
    // Unblocked == 0
    if(map[row][col]==1){
        return true;
    }
    return false;
}

// Verify if the next cell is destination
bool foundDestination(Pair dest, int row, int col)
{
    if(row == dest.first && col == dest.second)
    {
        printf("\nDestinaton Founded\n");
        return true;
    }
    return false;
}

// calculate the H value using Pitagóras
double calculateH(int row, int col, Pair dest)
{
    double h=0.0;
    h = sqrt(pow(dest.second-col,2)+pow(dest.first-row,2));
    return h;
}

// Print the path  --  cellWeights[][csv_size]: csv_size need be a variable, dont fixed
void storepath(cell cellWeights[][csv_size], Pair dest)
{

int row = dest.first;
int col=dest.second;

// Vector to store cells coordenates of the path
vector<Pair> Path;

// Loop to verify if the next cell is destination
while(!(cellWeights[row][col].parent_i== row && cellWeights[row][col].parent_j==col))
{
        // insert in the Path container
        Path.push_back(make_pair(row, col));
        int temp_row = cellWeights[row][col].parent_i;
        int temp_col = cellWeights[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    // insert destination in the Path container
    Path.push_back(make_pair(row, col));
    while (!Path.empty()) 
    {
        // Print the path
        pair<int, int> p = Path.back();
        Path.pop_back();
        printf("The Path: ");
        printf("-> (%d,%d) ", p.first, p.second);
    }
 
    return;
}

// TO do the A* Search 
void astarsearch( map<int,vector<int>> map, Pair src, Pair dest)
{

int rows = map.size();
int cols = map[0].size();

// Begin closedlist setting all to false
bool closedlist[map.size()][map[0].size()];
memset(closedlist, false, sizeof(closedlist));

// Size need be variable
cell cellWeights[4][7];

// Initiate the cells
for(int m=0; m<map.size();m++){
    for(int l = 0; l<map[0].size();l++)
    {
        cellWeights[m][l].parent_i=0;
        cellWeights[m][l].parent_j=0;
        cellWeights[m][l].g=0.0;
        cellWeights[m][l].h=0.0;
        cellWeights[m][l].f=0.0;
    }
}

// Open list
vector<Pairs> openlist;

// Store the source cell
int i = src.first;
int j = src.second;
cellWeights[i][j].parent_i=i;
cellWeights[i][j].parent_j=j;
cellWeights[i][j].g=0.0;
cellWeights[i][j].h=0.0;
cellWeights[i][j].f=0.0;

// Store the soourc cell data
openlist.push_back(make_pair(0,make_pair(i,j)));
bool destFound=false;

// While Destination isnt found
while(!destFound)
{

    // Try do this to each neightbour( North(-1,0), East(0,1), South(+1,0) and West(0,-1))
    for(int nb =0 ; nb<4;nb++)
    {
    int a=0;
    int b=0;

    if (nb == 0)
    {
        //----North----
        a=-1;
        b=0;
    }
    else if(nb == 1){
        //----East----
        a=0;
        b=1;

    }
    else if(nb ==2 )
    {
        //----SOuth----
        a=1;
        b=0;
    }

    else {
        //--- West----
        a=0;
        b=-1;
    }

    // Fixed to south
    a=1;
    b=0;

    {
        // Pick the current cell
        Pairs current = *openlist.begin();
        openlist.erase(openlist.begin());

        //current cell indexes
        int curr_i = current.second.first;
        int curr_j = current.second.second;

        closedlist[curr_i][curr_j]=true;

        //Next cell indexes
        int next_i=curr_i+a;
        int next_j=curr_j+b;

        // Next cell f,g,h
        double gnew,fnew,hnew;

            // If next cell is valid
            if(verify(next_i,next_j,rows,cols)){

                // If next cell is destination
                if(curr_i == dest.first && curr_j == dest.second)
                {
                    printf("Found Destination");
                    cellWeights[next_i][next_j].parent_i=curr_i;
                    cellWeights[next_i][next_j].parent_j=curr_j;
                    storepath(cellWeights,dest);
                    destFound=true;
                    return;

                }
                
                // If next cell is unblocked(0) or if not be evaluated yet
                else if(!blocked(map,next_i,next_j) || closedlist[next_i][next_j]==false)
                {
                    // Calculate Next cell f,g,h
                    gnew=cellWeights[curr_i][curr_j].g+1.0;
                    hnew = calculateH(next_i,next_j,dest);
                    fnew = gnew+hnew;

                    // If new f is bigger than old cell f
                    if(cellWeights[next_i][next_j].f==0.0 || cellWeights[next_i][next_j].f>fnew){
                        openlist.push_back(make_pair(fnew, make_pair(next_i,next_j)));

                        // Next cell store the current(parent) cell coordenates
                        cellWeights[next_i][next_j].parent_i=curr_i;
                        cellWeights[next_i][next_j].parent_j=curr_j;

                        // Store next cell f,g,h
                        cellWeights[next_i][next_j].f=fnew;
                        cellWeights[next_i][next_j].h=hnew;
                        cellWeights[next_i][next_j].g=gnew;
                    }    
                }
            }
        }
        }
    }
}


int main()
{

    //Filename
    string filename("mapa.csv");
    string file_contents;

    map<int,vector<int>> map;
    Pair pair_src;
    Pair pair_dest;

    // Convert csv dat into a map
    map = readFileIntoMap(filename);
    cout<<"-----Map-----"<<endl;
    mostra_csv(map);

    int rows=map.size();
    int columns=map[0].size();

    int src_x,src_y;

    printf("\n  Write the Source coordenates:\n");
    cin>>src_x;
    cin>>src_y;
    printf("Source: (%i,%i)\n",src_x,src_y);
 
    // Destination 
    int dest_x,dest_y;
    printf("\n Write the Destination coordenates: \n");
    cin>>dest_x;
    cin>>dest_y;
    printf("Destination: (%i,%i) \n",dest_x,dest_y);

    // Verify

    if(src_x==dest_x && src_y==dest_y){
        printf("\nThe Source cell and the destination cell are the same!\n");
        return 0;
    }

    if(!verify(src_x,src_y,rows,columns)){
        printf("\nThe source cell is invalide\n");
        return 0;
    }
    
    if(!verify(dest_x,dest_y,rows,columns)){
        printf("\nThe destination cell is invalide!\n");
        return 0;
    }

    if(blocked(map, src_x,src_y)){
        printf("\nThe source cell is blocked!\n");
        return 0;
    }

    if(blocked(map, dest_x,dest_y)){

        printf("\nThe destination cell is blocked!\n");
        return 0;
    }

    pair_src = make_pair(src_x,src_y);
    pair_dest = make_pair(dest_x,dest_y);

    astarsearch(map,pair_src,pair_dest);

    return 0;
}


