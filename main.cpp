// Title: Exact Cover Problem Solver
// Author: Cody Harrison
// This program will find the exact cover of a set of number through the implementation of Algorithm X by Donald Knuth and
// his Dancing Links technique. The exact cover problem does have a limit of 100 x 100 for the program to work.
// Additionally there is a problem with my logic pertaining to the node counter in headers or my getMinNode() function as
// the program searches an unnecessary additional level. In this specific problem the program tries the set E.
// The results are the same however this version is less efficient. The program requires you manual enter the sets in
// the main method. Sometimes takes multiple runs to fully compile and run correctly depending on IDE.
#include <bits/stdc++.h>

using namespace std;

struct Node
{
public:
    int rowNum;
    int colNum;
    int count;
    struct Node *column;
    struct Node *up;
    struct Node *down;
    struct Node *left;
    struct Node *right;
};

//number of rows and columns in the user provided problem
int nCol = 0;
int nRow = 0;

//Boolean used to determine if a solution has been found
bool solution = false;

//Current level search algorithm is on
int level = 0;

//Header Node used to keep track of entire matrix
struct Node *header = new Node();

// Vector to contain the partial solution
vector <struct Node*> partialSolution;

//Matrix to hold the nodes of linked list max of 100 x 100
struct Node Matrix[100][100];

// Sparse Matrix to hold exact cover problem
bool ProbMatrix[100][100];

//Functions to get next index in any direction
int getRight(int i){
    return (i+1) % nCol;
}

int getLeft(int i){
    if (i-1 < 0){
        return nCol -1;}
    else {
        return i - 1;
    }
} ;

int getUp(int i){
    if (i-1 < 0){
        return nRow-1;
    }
    else {
        return i - 1;
    }
}

int getDown(int i){
    return (i+1) % (nRow + 1);
}

//Get min column for search function
// Traverse the header nodes and return header with lowest node count
Node *getMinNodes(){
    cout << " Getting column with the least nodes";

    struct Node *h = header;
    struct Node *min_col = h->right;

    //set header to right pointer of node to the right
    h = h->right->right;

    //set column with minimal nodes to whichever column h is at as long as h is
    //lower than the current min_col do this until h circles back around
    do{
        if (h->count < min_col->count){
            min_col = h;
        }
        h = h->right;
    }
    while (h != header);
    return min_col;

}


Node *createConstraintMatrix(){


    for (int i = 0; i <= nRow; i++){

        for (int j = 0; j < nCol; j++){
            //If there is a 1 at [i][j] in the Problem Matrix
            if(ProbMatrix[i][j]){

                int a;
                int b;

                //set column number of node
                Matrix[i][j].colNum = j;

                //sets row number of node
                Matrix[i][j].rowNum = i;

                //If there is a 1 increment the node count in the header column by one
                //we do not want to count the nodes in the header row so we skip that row
                if (i) {
                    Matrix[0][j].count += 1;
                }

                //Set the column pointer of the node at [i][j] to the address of the header of column j
                Matrix[i][j].column = &Matrix[0][j];

                //Up Pointer
                //create variables to hold initial position of node
                a = i;
                b = j;
                // loop to keep going up until we find a 1 in the problem matrix
                do {a = getUp(a);}
                while(ProbMatrix[a][b] == 0 && a != i);
                //Once we find the closet 1, set the up pointer of the initial node to the
                //address of the closest node above it
                Matrix[i][j].up = &Matrix[a][j];

                //Down Pointer
                a = i;
                b = j;
                do {a = getDown(a);}
                while (ProbMatrix[a][b] == 0 && a != i);
                Matrix[i][j].down = &Matrix[a][j];

                //Left Pointer
                a = i;
                b = j;
                do {b = getLeft(b);}
                while (ProbMatrix[a][b] == 0 && b != j);
                Matrix[i][j].left = &Matrix[i][b];

                //Right Pointer
                a = i;
                b = j;
                do{b = getRight(b);}
                while (ProbMatrix[a][b] == 0 && b != j);
                Matrix[i][j].right = &Matrix[i][b];



            }
        }
    }

    //Sets right pointer of header to the address at [0][first column]
    header->right = &Matrix[0][0];


    //Sets left pointer of header to the address at [o][last column]
    header->left = &Matrix[0][nCol-1];

    //Sets right pointer of the [0][0] to the header address thus making it doubly linked
    Matrix[0][0].left = header;

    //Sets right pointer of the the last column header to the header address thus making it doubly linked
    Matrix[0][nCol-1].right = header;
    return header;
}

void Cover(struct Node *CurrentNode){


    struct Node *RowNode, *RightNode;

    struct Node *ColNode = CurrentNode->column;

    //Set right pointer of left node to node to right.
    ColNode->left->right = ColNode->right;

    //Set left pointer of right node to node to left
    ColNode->right->left = ColNode->left;

    //Move down column
    for(RowNode = ColNode->down; RowNode!=ColNode; RowNode = RowNode->down) {
        //Traverse right through row
        for (RightNode = RowNode->right; RightNode != RowNode; RightNode = RightNode->right) {
            //Unlink node from bottom node
            RightNode->up->down = RightNode->down;
            //Unlink node from top node
            RightNode->down->up = RightNode->up;
            //Decrement the node count in respective column header by one
            Matrix[0][RightNode->colNum].count -= 1;

        }
    }
}


//When uncovering nodes we must be careful to uncover in the reverse order that we covered
void UnCover(struct Node *CurrentNode) {
    struct Node *RowNode, *LeftNode;

    struct Node *ColNode = CurrentNode->column;


    //Move up column
    for(RowNode = ColNode->up; RowNode!=ColNode; RowNode = RowNode->up) {
        //Traverse left through row
        for(LeftNode = RowNode->left; LeftNode!=RowNode; LeftNode = LeftNode->left) {
            //Relink node to bottom node
            LeftNode->up->down = LeftNode;
            //Relink node to top node
            LeftNode->down->up = LeftNode;
            //Increment the node count in respective column header by one
            Matrix[0][LeftNode->colNum].count += 1;
        }
    }

    //Relink column node
    ColNode->left->right = ColNode;
    ColNode->right->left = ColNode;
}


//Use an iterator to print the row numbers from the partial solution
void printSolutions()
{

    vector<struct Node*> :: iterator i;

    for(i = partialSolution.begin(); i!=partialSolution.end(); i++)
        switch ((*i)->rowNum){
            case 1:
                cout << " A";
                break;
            case 2:
                cout << " B";
                break;
            case 3:
                cout << " C";
                break;
            case 4:
                cout << " D";
                break;
            case 5:
                cout << " E";
                break;
            case 6:
                cout << " F";
                break;

        }
}

/*This is where Algorithm X is implemented
1) If matrix A has no columns left, the current partial solution is valid and terminates successfully
2) Otherwise, choose a column c deterministically
3) Choose a r such that A[r] = 1 nondeterministically
4) Put r into the partial solution
5) For each column j such that A[r][j] = 1,
For each row such that A[i][j] = 1,
        Delete row I from matrix A.
Delete column j from matrix A.
6) Repeat this algorithm recursively on the reduced matrix.
 */

//Searches the 0 level of matrix and then recursively searches each level until either solution is found
// or exhausts all possiblilities
void search(int p){


    struct Node *RowNode;
    struct Node *RightNode;
    struct Node *LeftNode;
    struct Node *column;


    //(Step 1) The header points to the header therefore the matrix has no other columns left
    cout << "\nSearching level " << level <<". \nStep 1: ";
    if (header->right == header){
        solution = true;
        cout << "Solution found printing Sets:";
        printSolutions();
        return;
    }
    else cout << "No solution found continuing algorithm ";

    //(Step 2) Choose the column with the least amount of nodes
    cout << "\nStep 2:";
    column = getMinNodes();

    //(Step 3) Cover said column
    cout << "\nStep 3: Covering column and checking to see if valid solution";
    Cover(column);

    for(RowNode = column->down; RowNode != column; RowNode = RowNode->down)
    {
        //(Step 4) Add column to partial solution list
        cout << "\nStep 4: Adding row to partial solution ";
        //inserts row into partial solution
        partialSolution.push_back(RowNode);
        cout << "\nCurrent partial solution:";
        printSolutions();


        //(Step 5) Cover row that share 1's with chosen column
        cout<< "\nStep 5: Covering rows ";
        for (RightNode = RowNode->right; RightNode != RowNode; RightNode = RightNode->right) {
            Cover(RightNode);
        }

        //(Step 6)Recursively apply Algorithm X to smaller matrix (p+1)
        cout << "\nStep 6: Repeating search\n";
        level += 1;
        search(p + 1);


        //If search leads to no solutions back track and remove row from the partial solution list
        partialSolution.pop_back();


        //Restore covered columns and rows in the exact opposite order that they were covered
        column = RowNode->column;
        for (LeftNode = RowNode->left; LeftNode != RowNode; LeftNode = LeftNode->left) {
            UnCover(LeftNode);
            if (solution == false)
                cout<< "\nBackTracking";

        }

        if (solution == false)
            cout << "\nSelecting next row";
    }

    level -= 1;
    if (solution == false) {
        cout << "\nInvalid Solution";
        printSolutions();
    }
    UnCover(column);

}

int main(){

    /*Using the exact cover problem from my report and powerpoint:
      If set S = {A, B, C, D, E, F} and set X = {1, 2, 3, 4, 5, 6, 7} such that:
      A = {1,4,7}
      B = {1,4}
      C = {4,5,7}
      D = {3,5,6}
      E = {2,3,6,7}
      F = {2,7}
      The exact cover S* = {B, D, F}
    */

    // 6 sets = 6 rows
    nRow = 6;

    // 7 elements in the universe set
    nCol = 7;

    // initialize the problem matrix and fill with 0s except for headers
    for(int i=0; i<=nRow; i++)
    {
        for(int j=0; j<nCol; j++)
        {
            if (i == 0) {
                ProbMatrix[i][j] = true;
            }
            else {
                ProbMatrix[i][j] = false;
            }
        }
    }

    //Filling in the 1s in the matrix
    //The row is which set it is and the column is what number it is - 1 to account for starting at 0

    //Set A
    ProbMatrix[1][0] = true;
    ProbMatrix[1][3] = true;
    ProbMatrix[1][6] = true;

    //Set B
    ProbMatrix[2][0] = true;
    ProbMatrix[2][3] = true;

    //Set C
    ProbMatrix[3][3] = true;
    ProbMatrix[3][4] = true;
    ProbMatrix[3][6] = true;

    //Set D
    ProbMatrix[4][2] = true;
    ProbMatrix[4][4] = true;
    ProbMatrix[4][5] = true;

    //Set E
    ProbMatrix[5][1] = true;
    ProbMatrix[5][2] = true;
    ProbMatrix[5][5] = true;
    ProbMatrix[5][6] = true;

    //Set F
    ProbMatrix[6][1] = true;
    ProbMatrix[6][6] = true;

    createConstraintMatrix();

    // initialize the problem matrix and fill with 0s
    for(int i=0; i<=nRow; i++)
    {
        cout << endl;
        for(int j=0; j<nCol; j++)
        {
            if (ProbMatrix[i][j] == true) {
                cout << " 1 ";
            }
            else {
                cout << " 0 ";

            }
        }
    }

    search(0);
    return 0;
}
