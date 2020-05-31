#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<fstream>
#include<string>
using namespace std;
class Cell {
public:
	Cell()
	{
		top = false; bottom = false; left = false; right = false;
		Visited = false;
		cellchar = ' ';
		prevC = -1;
		currentc = 0;
		currentr = 0;
		prevR = -1;
	}
	//setters and getter functions
	bool get_top() { return top; }
	bool get_bottom() { return bottom; }
	bool get_left() { return left; }
	bool get_right() { return right; }
	void set_top(bool t) { top = t; }
	void set_bottom(bool b) { bottom = b; }
	void set_left(bool l) { left = l; }
	void set_right(bool r) { right = r; }
	void setVisited(bool v) { Visited = v; }
	bool getVisited() { return Visited; }
	void setcellchar(char c = ' ') { cellchar = c; }
	char getcellchar() { return cellchar; }
	void setPrevCoordinates(int r, int c) { prevR = r; prevC = c; }
	void setCurrentCoordinates(int r, int c) { currentr = r; currentc = c; }
	int getPrevRowCoordinate() { return prevR; }//for BFS
	int getPrevColCoordinate() { return prevC; }//for BFS
	int getCurrRowCoordinate() { return currentr; }
	int getCurrColCoordinate() { return currentc; }

private:
	bool top, bottom, left, right;
	bool Visited;
	int prevR, prevC, currentr, currentc;
	char cellchar;
};
//non-member fucntions to help in displaying the maze and path
void DisplayTop(vector<Cell>& C) {
	if (!C.empty()) {
		for (auto i : C) {
			cout << "+";
			if (i.get_top()) {
				cout << "---";
			}
			else cout << "   ";
		}
		cout << "+" << endl;
	}
}
void DisplayDown(vector<Cell>& C) {
	if (!C.empty()) {
		for (auto i : C) {
			cout << "+";
			if (i.get_bottom()) {
				cout << "---";
			}
			else cout << "   ";
		}
		cout << "+" << endl;
	}
}
//function to print the maze with path
void DisplaySides(vector<Cell> & C) {
	if (!C.empty()) {
		for (auto i : C) {
			if (i.get_left()) cout << "| ";
			else cout << "  ";
			cout << i.getcellchar() << " ";
		}
		if (C[C.size() - 1].get_right()) cout << "| " << endl;
		else cout << endl;
	}
}
//function to print the maze without path
void DisplaySideswithnoPath(vector<Cell> & C) {
	if (!C.empty()) {
		for (auto i : C) {
			if (i.get_left()) cout << "| ";
			else cout << "  ";
			if (i.getcellchar() == 'S' || i.getcellchar() == 'G') {
				cout << i.getcellchar() << " ";
			}
			else cout << "  ";
		}
		if (C[C.size() - 1].get_right()) cout << "|" << endl;
		else cout << endl;
	}
}
void WriteCoardinates(stack<Cell*>&temp) {//a fucntion to write the coordinates of BFS and DFS
	int count = 0;
	while (!temp.empty()) {
		if (count == 7) {
			cout << endl;
			count = 0;
		}
		cout << "(" << temp.top()->getCurrColCoordinate()+1 << "," << temp.top()->getCurrRowCoordinate()+1 << ")";
		temp.pop();
		count++;
	}
}
class Maze {
public:
	Maze()
	{
		//setting to the coordinates as -1 so they are inaccessible unless we load the mase
		StartCoordC = -1;
		GoalCoordC = -1;
		StartCoordR = -1;
		GoalCoordR = -1;
		StartFlag = false;
		GoalFlag = false;
	}
	void DisplayMaze() {//display the maze without any path
		if (maze.empty()) {
			cout << "No maze loaded " << endl;
			return;
		}
		for (auto i : maze) {
			DisplayTop(i);
			DisplaySideswithnoPath(i);
		}
		DisplayDown(maze[maze.size() - 1]);
	}
	void LoadMaze() {//works assuming that the input file is error free (no extra space) :)
		if (!maze.empty()) {//if a maze has already been loaded, no need to do it again
			cout << "Maze already loaded, no need to do it again lol" << endl;
			return;
		}
		ifstream in("Text1.txt");
		if (in.fail()) {
			cout << "Error cant read the file, quitting program " << endl;
			system("pause");
			exit(0);
		}
		int rowcount = 0;// a counter used for setting coordinates
		char str1[100], str2[100], str3[100];
		in.getline(str1, 100);
		in.getline(str2, 100);
		in.getline(str3, 100);
		while (!in.eof())
		{
			int columncount = 0;// a counter used to set the cell coordinates
			Cell c;//reinitialise everytime we read the next three line
			vector<Cell>M;//reinitialise everytime we read the next three line
			int i = 0;
			int j = 0, k = 1;
			while (i < strlen(str1)-1)
			{
				if (str1[i] == '+') i++; //new cell
				if (str1[i] == '-')c.set_top(true);
				else if (str1[i] == ' ')c.set_top(false);
				i = i + 3; //wall above
				if (str2[j] == '|') c.set_left (true);
				else if (str2[j] == ' ')c.set_left(false);
				j = j + 4; //left wall
				if (str2[j] == '|') c.set_right(true);
				else if (str2[j] == ' ')c.set_right(false); //right wall
				if (str3[k] == ' ') c.set_bottom(false);
				else if (str3[k] == '-') c.set_bottom(true);
				k = k + 4; //wall below
				c.setCurrentCoordinates(rowcount, columncount);//set the coordinates for each cell
				M.push_back(c);//push back into the temporary vector
				columncount++;
			}
			maze.push_back(M);//push back the vecter into maze
			strcpy(str1, str3);
			in.getline(str2, 100);
			in.getline(str3, 100);
			rowcount++;
		}
		if (maze.empty()) {
			cout << "Error no maze was loaded, try again lol " << endl;
		}
		else cout << "Maze Loaded Sucessfully, yaaay" << endl;
	}
	void clearMaze() {//a function to clear the vector if needed
		for (auto i : maze) {
			i.clear();
		}
		maze.clear();
	}
	void DisplayPath() {//A function to display the path that DFS or BFS took to reach the goal
		if (maze.empty()) {
			cout << "No maze loaded " << endl;
			return;
		}
		for (auto i : maze) {
			DisplayTop(i);
			DisplaySides(i);
		}
		DisplayDown(maze[maze.size()-1]);
	}
	void clearPreviousPath() {//a function to clear the path if any before calling DFS or BFS
		for (auto& i : maze) {
			for (auto& j : i) {
				j.setVisited(false);
				if (j.getcellchar() == '@' || j.getcellchar() == '*') {
					j.setcellchar(' ');
				}
			}
		}
	}
	//setters and getter function
	void SetStartc(int x, int y) { StartCoordR = x; StartCoordC = y; }
	void SetGoalc(int x, int y) { GoalCoordR = x; GoalCoordC = y; }
	void SetStartFlag(bool f ) { StartFlag = f; }
	void SetGoalFlag(bool f) { GoalFlag = f; }
	bool getStartFlag() { return StartFlag; }
	bool getGoalFlag() { return GoalFlag; }
	void setStart() {
		if (maze.empty()) {
			cout << "No maze is loaded" << endl;
			cout << endl;
			return;
		}
		if (getStartFlag()) {
			string x;
			cout << "Start is already set, would you like to change, if yes press 1 ";
			getline(cin, x);//give the user option to change if the want
			if (x == "1") {
				maze[StartCoordR][StartCoordC].setcellchar(' ');
				SetStartFlag(false);
			}
			else cout << "Start is not changing" << endl;
		}
		if (!getStartFlag()) {
			int x, y;
			cout << "Please enter the coordinates for Start (Column and then Row)" << endl;
			cin >> x >> y;
			while ((x-1) < 0 || (y-1) < 0 || (x-1) >= maze[0].size() || (y-1) >= maze.size()) {
				cout << "Wrong coordinates enter again " << endl;
				cin >> x >> y;
			}
			if (maze[y - 1][x - 1].getcellchar() == 'G') {//if we want to set Goal as Start have to clear the flag
				cout << "Goal same as start, please set goal again after this" << endl;//let the user know no goal anymore
				SetGoalFlag(false);
				SetGoalc(-1, -1);
			}
			SetStartc(y - 1, x - 1);
			maze[y - 1][x - 1].setcellchar('S');
			SetStartFlag(true);
			DisplayMaze();//show that start has been set where the user wants it to be
		}
	}
	void setGoal(){
		if (maze.empty()) {
			cout << "No maze is loaded" << endl;
			cout << endl;
			return;
		}
		if (getGoalFlag()) {
			string x;
			cout << "Goal is already set, would you like to change, if yes press 1 ";
			getline(cin, x);
			if (x == "1") {//check if the user wants to change
				maze[GoalCoordR][GoalCoordC].setcellchar(' ');
				SetGoalFlag(false);
			}
			else cout << "Goal is not changing" << endl;
		}
		if (!getGoalFlag()) {
			int x, y;
			cout << "Please enter the coordinates for Goal (Column and then Row)" << endl;
			cin >> x >> y;
			while ((x - 1) < 0 || (y - 1) < 0 || (x - 1) >= maze[0].size() || (y - 1) >= maze.size()) {//invalid coordinates
				cout << "Wrong coordinates enter again " << endl;
				cin >> x >> y;//ask the user to input again
			}
			if (maze[y - 1][x - 1].getcellchar() == 'S') {//if the goal is now start we need to set startflag false
				cout << "Goal same as start, so no start anymore, please set one " << endl;//let the user know that no start anymore
				SetStartFlag(false);
				SetStartc(-1, -1);
			}
			SetGoalc(y - 1, x - 1);
			maze[y - 1][x - 1].setcellchar('G');
			SetGoalFlag(true);
			DisplayMaze();//show that Goal has been where the user wants it to be
		}
	}
bool DFS() {
	if (maze.empty()) {
		cout << "No maze loaded " << endl;
		return false;
	}
		clearPreviousPath();//clear if any path has already been set
		if (!getStartFlag()) {
			cout << "No start has been set " << endl;
			return false;
		}
		if (!getGoalFlag()) {
			cout << "No goal has been set" << endl;
			return false;
		}
		bool Goal = false;
		bool Fail = true;
		Cell*Current;//pointer as in stack its pushed by value
		Cell *Temp;//goes to current's neighbours and check if the Goal is there
		stack<Cell*>DFS;
		Current = &maze[StartCoordR][StartCoordC];//point current at the start coordinates
		DFS.push(Current);
		Current->setVisited(true);
		Current = DFS.top();
		while (!DFS.empty()) {
			//check neighbours and go as deep as possible if dead end start popping (go top,bottom,right left)
			if (!Current->get_top() &&(Current->getCurrRowCoordinate() - 1) >= 0 && !maze[Current->getCurrRowCoordinate() - 1][Current->getCurrColCoordinate()].getVisited()) {
				Temp = &maze[Current->getCurrRowCoordinate() - 1][Current->getCurrColCoordinate()];
				Temp->setCurrentCoordinates(Current->getCurrRowCoordinate() - 1, Current->getCurrColCoordinate());
					Temp->setVisited(true);
					if (Temp->getcellchar() == 'G') {
						DFS.push(Temp);
						Goal = true;
					}
					else {
						Temp->setcellchar('*');
						DFS.push(Temp);
						Current = Temp;
						Temp = nullptr;
				}
			}
			else if (!Current->get_bottom() && (Current->getCurrRowCoordinate() + 1) < maze.size() && !maze[Current->getCurrRowCoordinate() + 1][Current->getCurrColCoordinate()].getVisited()) {
				Temp = &maze[Current->getCurrRowCoordinate() + 1][Current->getCurrColCoordinate()];
				Temp->setCurrentCoordinates(Current->getCurrRowCoordinate() + 1, Current->getCurrColCoordinate());
					Temp->setVisited(true);
					if (Temp->getcellchar() == 'G') {
						DFS.push(Temp);
						Goal = true;
					}
					else {
						Temp->setcellchar('*');
						DFS.push(Temp);
						Current = Temp;
						Temp = nullptr;
					}
			}
			else if (!Current->get_right() && (Current->getCurrColCoordinate() + 1) < maze[0].size() && !maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()+1].getVisited()) {
				Temp = &maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate() + 1];
				Temp->setCurrentCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate() + 1);
					Temp->setVisited(true);
					if (Temp->getcellchar() == 'G') {
						DFS.push(Temp);
						Goal = true;
					}
					else {
						Temp->setcellchar('*');
						DFS.push(Temp);
						Current = Temp;
						Temp = nullptr;
				}
			}
			else if (!Current->get_left() &&(Current->getCurrColCoordinate() - 1) >= 0 && !maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()-1].getVisited()) {
				Temp = &maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate() - 1];
				Temp->setCurrentCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate() - 1);
					Temp->setVisited(true);
					if (Temp->getcellchar() == 'G') {
						DFS.push(Temp);
						Goal = true;
					}
					else {
						Temp->setcellchar('*');
						DFS.push(Temp);
						Current = Temp;
						Temp = nullptr;
					}
				}
			else {//dead end so pop
				if (DFS.size() == 1) break;//no solution :(
				DFS.pop();
				Current = DFS.top();
			}
			if (Goal) {
				cout << "Goal Found by DFS" << endl;
				stack<Cell*>goal;
				goal.push(DFS.top());
				DFS.pop();
				while (DFS.size() != 1) {
					DFS.top()->setcellchar('@');
					goal.push(DFS.top());//to write the coordinates later
					DFS.pop();
				}
				goal.push(DFS.top());
				DFS.pop();
				WriteCoardinates(goal);
				Fail = false;
				cout << endl;
				break;
			}
		}
			if (Fail) cout << "Cannot find a path" << endl;
			return (!Fail);
	}
	bool BFS() {
		if (maze.empty()) {//check if a maze is even loaded
			cout << "No maze loaded " << endl;
			return false;
		}
		if (!getStartFlag()) {//check if we have a start or not
			cout << "No start has been set " << endl;
			return false;
		}
		if (!getGoalFlag()) {//check if we have a goal or not
			cout << "No goal has been set" << endl;
			return false;
		}
		bool Goal = false;
		bool Fail = true;
		clearPreviousPath();
		Cell* Current;//pointer as queue is copy by value
		queue<Cell*>BFS;
		Current = &maze[StartCoordR][StartCoordC];
		BFS.push(Current);
		Cell *temp;
		while (!BFS.empty()) {
			Current = BFS.front();
			Current->setVisited(true);
			if (Current->getcellchar() != 'G'&& Current->getcellchar() != 'S')
				Current->setcellchar('*');//set this only if not goal and start
			if (Current->getcellchar() == 'G') {//yaay goal found
				Goal = true;
				break;
			}
			//check all neighbours and push it into queue if not visited and within the range (go top,bottom,right left)
			if (!Current->get_top() && (Current->getCurrRowCoordinate() - 1) >= 0 && !maze[Current->getCurrRowCoordinate() - 1][Current->getCurrColCoordinate()].getVisited()) {
				maze[Current->getCurrRowCoordinate() - 1][Current->getCurrColCoordinate()].setPrevCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate());
				temp = &maze[Current->getCurrRowCoordinate() - 1][Current->getCurrColCoordinate()];
				temp->setCurrentCoordinates(Current->getCurrRowCoordinate() - 1, Current->getCurrColCoordinate());
				BFS.push(temp);
			}
			if (!Current->get_bottom() && (Current->getCurrRowCoordinate() + 1) < maze.size() && !maze[Current->getCurrRowCoordinate() + 1][Current->getCurrColCoordinate()].getVisited()) {
				maze[Current->getCurrRowCoordinate() + 1][Current->getCurrColCoordinate()].setPrevCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate());
				temp = &maze[Current->getCurrRowCoordinate() + 1][Current->getCurrColCoordinate()];
				temp->setCurrentCoordinates(Current->getCurrRowCoordinate() + 1, Current->getCurrColCoordinate());
				BFS.push(temp);
			}
			if (!Current->get_right() && (Current->getCurrColCoordinate() + 1) < maze[0].size() && !maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()+1].getVisited()) {
				maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()+1].setPrevCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate());
				temp = &maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()+1];
				temp->setCurrentCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate()+1);
				BFS.push(temp);
			}
			if (!Current->get_left() && (Current->getCurrColCoordinate() - 1) >= 0 && !maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()-1].getVisited()) {
				maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()-1].setPrevCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate());
				temp = &maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()-1];
				temp->setCurrentCoordinates(Current->getCurrRowCoordinate(), Current->getCurrColCoordinate()-1);
				BFS.push(temp);
			}
			BFS.pop();
		}
		stack<Cell*>tempd;//used so coordinates can be written
		if (Goal) {
			cout << "Goal Found by BFS" << endl;
			while (true) {
				Current = &maze[Current->getCurrRowCoordinate()][Current->getCurrColCoordinate()];
				if (Current->getcellchar() != 'S' && Current->getcellchar() != 'G') {
					Current->setcellchar('@');//change all the visited cells in the right path to @
				}
				tempd.push(Current);
				Current->setCurrentCoordinates(Current->getPrevRowCoordinate(), Current->getPrevColCoordinate());
				if (Current->getCurrRowCoordinate() == StartCoordR && Current->getCurrColCoordinate() == StartCoordC) {
					WriteCoardinates(tempd);
					cout << "(" << GoalCoordC+1 << "," << GoalCoordR+1 << ")";
					cout << endl;
					break;
				}
			}
		}
		else {
			cout << "Failed, cant find a path" << endl;
		}
		return Goal;
	}
private:
	vector<vector<Cell>> maze;// a 2D vector to save the cells
	int StartCoordR, StartCoordC, GoalCoordR, GoalCoordC;//variables to store the coordinates
	bool StartFlag, GoalFlag;//flags to see if start and goal were set or not
};
void showmenu() {
	cout << "1. Load the maze" << endl;
	cout << "2. Display maze" << endl;
	cout << "3. SetStart" << endl;
	cout << "4. SetGoal" << endl;
	cout << "5. Find Path (BFS)" << endl;
	cout << "6. Find Path (DFS)" << endl;
	cout << "7. Display Path of the last search done" << endl;
	cout << "8. Exit" << endl;
	cout << "Choose an option: ";
}
int main()
{
	Maze m;
	string option;
	while (true) {
		showmenu();
		getline(cin, option);
		if (option == "1") {
			m.LoadMaze();
		}
		else if (option == "2") {
			m.DisplayMaze();
		}
		else if (option == "3") {
			m.setStart();
		}
		else if (option == "4") {
			m.setGoal();
		}
		else if (option == "5") {
			bool d;
			d = m.BFS();
			if (d) m.DisplayPath();
		}
		else if (option == "6") {
			bool d;
			d = m.DFS();
			if (d) m.DisplayPath();
		}
		else if (option == "7") {
			m.DisplayPath();
		}
		else if (option == "8") {
			cout << "Okay, bye" << endl;
			system("pause");
			return 0;
		}
		else {
			cout << "No such option, try again" << endl;
		}
	}
}