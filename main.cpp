#include <iostream>
#include <stdio.h>

using namespace std;

const int NB_SQUARES = 64;
/* La géométrie du casse-tête : où se trouvent les cubes "tout droit" et les cubes "virage" */
const int isIntersection[NB_SQUARES] = {0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,0,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0};
/* Des lettres pour symboliser les directions */
const char letterDirections[] = {'a','d','r','g','h','b'};
/* Le nombre de positions testées */
long long nbPositionTested = 0;

/* Affiche une succession de directions */
void printDirections(char* directions){
	for(int i=0; i<NB_SQUARES; i++)
		if(directions[i]!='0')
			cout << directions[i];
	cout << endl;
}
void exploreDirections(char* directions, int position);
bool isOkay(char* directions, int size);

int main()
{
	char directions[NB_SQUARES];
	std::fill_n(directions, NB_SQUARES, '0');
	//On fixe les premiers choix
	directions[0] = 'a'; //On commence par aller tout droit, car toutes les directions sont équivalentes
	directions[1] = '0'; //Ce cube ne bouge pas
	directions[2] = 'd'; //On va à droite, sachant que là aussi les directions sont symétriques

	//Pour le reste il faut tout tester
	exploreDirections(directions, 3);
	cout << nbPositionTested << " positions tested" << endl;
	return 0;
}

/* Dit si une succession de directions est possible */
bool isOkay(char* directions, int size){

	//On se place au centre d'un cube de dimension 7.
	//De cette façon on se laisse la possibilité de former un cube de dimension 4 dans toutes les directions à partir du centre
	bool isFree[7][7][7];
	for(int i=0; i<7; i++)
		for(int j=0; j<7; j++)
			for(int k=0; k<7; k++)
				isFree[i][j][k] = true;
	int curX = 3, curY=3, curZ=3;
	char curDir = 'a'; //on commence à explorer le cube tout droit
	for(int i=0; i<=size; i++){
		if(i>0)
			switch(curDir){
			case 'a': curY++; break;
			case 'd': curX++; break;
			case 'r': curY--; break;
			case 'g': curX--; break;
			case 'h': curZ++; break;
			case 'b': curZ--; break;
		}
		if(isIntersection[i]==1){
			char newDir = directions[i];
			if(newDir==curDir){//on ne peut pas aller tout droit s'il y a un virage
				return false;
			}
			curDir=newDir;
		}
		if(curX<0 || curX>6 || curY<0 || curY>6 || curZ<0 || curZ>6) //en dehors du grand cube
			return false;

		if(!isFree[curX][curY][curZ]) // espace deja pris (prend en charge le cas du demi-tour impossible
			return false;

		isFree[curX][curY][curZ] = false;
	}
	//en dehors d'un cube de taille 4
	int minX=3, minY=3, minZ=3, maxX=3, maxY=3, maxZ=3;
	for(int i=0; i<=6; i++)
		for(int j=0; j<=6; j++)
			for(int k=0; k<=6; k++)
				if(!isFree[i][j][k]){
					minX = min(minX, i);
					minY = min(minY, j);
					minZ = min(minZ, k);
					maxX = max(maxX, i);
					maxY = max(maxY, j);
					maxZ = max(maxZ, k);
				}
				if(maxX-minX+1>4 || maxY-minY+1>4 || maxZ-minZ+1>4) //Taille > 4x4
					return false;

				return true;
}

//int time=0;

/* Explore toutes les directions à une position donnée, et après avoir pris les directions indiquées */
void exploreDirections(char* directions, int position){
	//Pour voir l'évolution
	//   if(time++%1000000==0){
	//       //printDirections(directions);
	//	cout << nbPositionTested << endl;
	//}
	if(!isOkay(directions, position)){
		return;
	}
	nbPositionTested++;
	if(position==NB_SQUARES-1){
		cout << "SOLUTION = ";
		printDirections(directions);
	}
	if(!(isIntersection[position]==1)){
		directions[position] = '0';
		exploreDirections(directions, position+1);
	}
	else{
		for(int i=0; i<6; i++){
			directions[position] = letterDirections[i];
			exploreDirections(directions, position+1);
		}
		directions[position] = '0';
	}
}
