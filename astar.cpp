#include <GL/glut.h>

#include <iostream>
#include <unistd.h>

#include <vector>
#include <map>
#include <cmath>

GLfloat window_width  = 800;
GLfloat window_height = 800;
GLfloat game_width    = 42;
GLfloat game_height   = 42;

using namespace std;



#include "field.cpp"
vector<vector<Field*>> fields;
int expanded_nodes = 0;
int cost = 0;
#include "agent.cpp"

int battery_inserted       = 0;
int solder_inserted        = 0;
int bomb_inserted          = 0;
int refrigeration_inserted = 0;
int pneumatic_inserted     = 0;

int factory_1_inserted     = 0;
int factory_2_inserted     = 0;
int factory_3_inserted     = 0;
int factory_4_inserted     = 0;
int factory_5_inserted     = 0;

vector<Field*> factory_positions;

Agent agent;
bool loaded = false;

void Init(){
	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, window_height, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	fields = Field::load_map();
}

void onMouseClick(int button, int state, int x, int y){
	
	if(
		button == GLUT_LEFT_BUTTON &&
		state  == GLUT_DOWN &&
		loaded == false
	){
		for(int j = 0; j < fields.size(); j++){
			for(int i = 0; i < fields[j].size(); i++){
				if(fields[j][i]->inField(x, y)){
					
					if(factory_1_inserted == 0){
						fields[j][i]->is_factory   = true;
						fields[j][i]->factory_type = factory_1;
						fields[j][i]->is_fixed     = false;
						
						factory_positions.push_back(fields[j][i]);
						factory_1_inserted = 1;
						
					}else if(factory_2_inserted == 0){
						fields[j][i]->is_factory   = true;
						fields[j][i]->factory_type = factory_2;
						fields[j][i]->is_fixed     = false;
						
						factory_positions.push_back(fields[j][i]);
						factory_2_inserted = 1;
						
					}else if(factory_3_inserted == 0){
						fields[j][i]->is_factory   = true;
						fields[j][i]->factory_type = factory_3;
						fields[j][i]->is_fixed     = false;
						
						factory_positions.push_back(fields[j][i]);
						factory_3_inserted = 1;
						
					}else if(factory_4_inserted == 0){
						fields[j][i]->is_factory   = true;
						fields[j][i]->factory_type = factory_4;
						fields[j][i]->is_fixed     = false;
						
						factory_positions.push_back(fields[j][i]);
						factory_4_inserted = 1;
						
					}else if(factory_5_inserted == 0){
						fields[j][i]->is_factory   = true;
						fields[j][i]->factory_type = factory_5;
						fields[j][i]->is_fixed     = false;
						
						factory_positions.push_back(fields[j][i]);
						factory_5_inserted = 1;
						
					}else {
						agent = Agent(i, j, factory_positions);
						loaded = true;
						
					}
					
				}
			}
		}
	}
	
}

void  MyRun(){
	if(loaded){
		agent.run(fields);
	}else {
		Field::draw_map(fields);
		
	}
}


int main(int argc, char **argv){
	srand(time(NULL));

	glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
    glutInitWindowSize(window_width, window_height);  
    glutCreateWindow("A*");
	
	glutMouseFunc(onMouseClick);
	glutIdleFunc(MyRun);
	glutDisplayFunc(MyRun);

	Init(); 
    glutMainLoop(); 

	return 0;
}
