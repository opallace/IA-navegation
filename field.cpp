enum Factory_type {factory_1, factory_2, factory_3, factory_4, factory_5};
enum Piece_type {battery, solder, bomb, refrigeration, pneumatic};

class Field{

	public:
		int pos_x, pos_y;
		int weight;
		
		bool is_factory;
		bool is_piece;
		bool is_fixed;
		
		Factory_type factory_type;
		Piece_type piece_type;
		
		Field(){}
		
		Field(int _pos_x, int _pos_y){
			this->pos_x  = _pos_x;
			this->pos_y  = _pos_y;
			this->weight = 0;
			this->is_factory = false;
			this->is_piece   = false;
			this->is_fixed   = false;
		}
		
		Field(int _pos_x, int _pos_y, int _map_value){
			this->pos_x = _pos_x;
			this->pos_y = _pos_y;
			
			this->is_factory = false;
			this->is_piece   = false;
			this->is_fixed   = false;
			
			switch(_map_value){
				case 0:
					this->weight = 1;
					break;
				case 1:
					this->weight = 5;
					break;
				case 2:
					this->weight = 10;
					break;
				case 3:
					this->weight = 15;
					break;
			}
		}
		
		bool operator== (const Field &f){
			return (
				this->pos_x        == f.pos_x &&
				this->pos_y        == f.pos_y
			);
		}
		
		void operator= (Field f){
			this->pos_x        = f.pos_x;
			this->pos_y        = f.pos_y;
			this->weight       = f.weight;
			this->is_factory   = f.is_factory;
			this->is_piece     = f.is_piece;
			this->factory_type = f.factory_type;
			this->piece_type   = f.piece_type;
		}
		
		bool operator< (const Field f) const{
			return ((this->pos_y * 42 + this->pos_x) < (f.pos_y * 42 + f.pos_x));
		}
		
		bool inField(int x, int y){
			GLfloat xSize = window_width / game_width;
			GLfloat ySize = window_height / game_height;
			
			return (
				x >= this->pos_x * xSize       &&
				x <= (this->pos_x + 1) * xSize &&
				y >= this->pos_y * ySize       &&
				y <= (this->pos_y + 1) * ySize
			);
		}
		
		static void draw_map(vector<vector<Field*>> map){
			glClear(GL_COLOR_BUFFER_BIT);
			
			GLfloat xSize = window_width / game_width;
			GLfloat ySize = window_height / game_height;
			
			glBegin(GL_QUADS);
			
			for(int j = 0; j < map.size(); j++){
				for(int i = 0; i < map[j].size(); i++){
				
					if(map[j][i]->weight == 1){glColor3f(0.57f, 0.81f, 0.3f);}
					if(map[j][i]->weight == 5){glColor3f(0.57f, 0.54f, 0.32f);}
					if(map[j][i]->weight == 10){glColor3f(0.32f, 0.54f, 0.82f);}
					if(map[j][i]->weight == 15){glColor3f(0.88f, 0.41f, 0.04f);}
					
					if(map[j][i]->is_piece){
						if(map[j][i]->piece_type == battery){glColor3f(0.5f, 0.5f, 0.5f);}
						if(map[j][i]->piece_type == solder){glColor3f(0.5f, 0.5f, 0.5f);}
						if(map[j][i]->piece_type == bomb){glColor3f(0.5f, 0.5f, 0.5f);}
						if(map[j][i]->piece_type == refrigeration){glColor3f(0.5f, 0.5f, 0.5f);}
						if(map[j][i]->piece_type == pneumatic){glColor3f(0.5f, 0.5f, 0.5f);}
					}
					
					if(map[j][i]->is_factory){
						if(map[j][i]->factory_type == factory_1){glColor3f(0.0f, 0.0f, 0.0f);}
						if(map[j][i]->factory_type == factory_2){glColor3f(0.0f, 0.0f, 0.0f);}
						if(map[j][i]->factory_type == factory_3){glColor3f(0.0f, 0.0f, 0.0f);}
						if(map[j][i]->factory_type == factory_4){glColor3f(0.0f, 0.0f, 0.0f);}
						if(map[j][i]->factory_type == factory_5){glColor3f(0.0f, 0.0f, 0.0f);}
					}
					
					glVertex2f(    map[j][i]->pos_x*xSize,    map[j][i]->pos_y*ySize);
					glVertex2f((map[j][i]->pos_x+1)*xSize,    map[j][i]->pos_y*ySize);
					glVertex2f((map[j][i]->pos_x+1)*xSize,(map[j][i]->pos_y+1)*ySize);
					glVertex2f(    map[j][i]->pos_x*xSize,(map[j][i]->pos_y+1)*ySize);
				}
			}
			
			glEnd();
			glFlush();
		}
		
		static vector<vector<Field*>> load_map(){
			int mapp[42][42] = {
				{1,0,0,0,0,1,0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0},
				{1,0,1,1,1,1,0,2,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0},
				{1,0,0,0,0,1,0,2,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,3,1,1,1,1,0},
				{1,0,1,1,1,1,0,2,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,1,1,1,1,3,3,3,1,1,1,1},
				{1,0,0,0,0,1,0,2,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,1,3,3,3,3,3,1,1,1},
				{1,1,1,1,0,1,0,2,2,2,2,2,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,1,1,3,3,3,1,1,1,1},
				{0,1,0,0,0,0,0,0,0,0,0,2,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,1,1,3,1,1,1,1,0},
				{0,1,0,0,0,0,1,1,1,1,0,2,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0},
				{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0},
				{0,0,1,0,0,0,0,0,1,1,0,2,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
				{0,0,1,0,1,1,1,0,1,1,0,2,0,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,1,0,0,1,0},
				{0,0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
				{0,0,1,0,0,0,0,0,0,0,0,2,2,2,2,0,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,0},
				{0,0,0,0,0,0,0,0,1,0,0,0,0,0,2,0,1,0,0,1,0,0,1,1,1,2,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0},
				{1,1,1,1,0,1,0,1,1,1,0,0,0,0,2,0,0,0,0,1,0,0,0,1,1,2,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0},
				{0,0,0,0,0,1,0,1,1,1,0,0,2,2,2,2,2,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
				{0,0,1,0,0,1,0,0,1,0,0,2,2,2,2,2,2,2,0,0,0,1,1,1,0,2,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0},
				{1,0,1,0,0,1,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,2,0,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1},
				{1,0,1,0,0,1,0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,1,0,0,0,2,0,0,0,1,0,0,1,0,0,0,0,1,1,1,1,1},
				{1,0,1,0,1,1,1,0,0,0,2,2,2,0,0,0,2,2,2,0,1,1,1,0,0,2,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1},
				{1,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,2,2,2,0,1,1,1,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,1,1},
				{1,1,1,1,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,1,1,1,0,0,0,0,0,0,0,2,0,1,1,1,0,1,1,1,1,1,1},
				{0,0,0,0,1,0,1,0,0,0,0,2,2,2,2,2,2,2,0,0,0,1,0,0,0,1,1,1,0,0,2,0,1,0,0,0,0,1,1,1,1,1},
				{0,0,0,0,1,0,1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,1,1,1,0,0,2,0,1,0,1,1,0,0,0,0,1,1},
				{0,0,1,0,0,0,1,0,1,1,0,0,0,0,2,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,2,0,1,0,1,0,0,0,0,0,0,0},
				{0,1,1,1,0,0,1,0,1,1,1,1,0,0,2,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,2,0,0,0,1,1,1,1,0,0,0,0},
				{0,1,1,1,0,1,1,0,0,0,0,0,0,0,2,0,0,1,1,1,1,0,0,0,1,0,1,0,0,2,2,2,0,0,0,1,1,1,0,0,1,0},
				{0,1,1,1,0,0,0,0,1,1,1,0,0,0,2,0,1,1,1,1,1,1,0,0,1,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,1,0},
				{0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,0,1,1,1,1,1,1,0,0,1,0,0,2,2,2,2,2,2,2,0,0,1,1,1,1,1,0},
				{1,0,0,0,1,0,0,1,2,2,2,2,2,2,2,0,0,1,1,1,1,0,0,0,1,0,0,0,2,2,2,2,2,0,0,0,1,0,0,0,1,0},
				{1,1,0,1,1,1,0,0,2,0,0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0},
				{1,1,0,1,1,1,0,0,2,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0},
				{1,0,0,0,1,0,0,0,2,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,1,0,1,0,1,0,1},
				{0,1,0,1,1,1,1,0,2,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
				{0,1,0,0,0,0,1,0,2,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1},
				{0,1,1,1,0,0,1,0,2,0,0,1,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,1,1,1,3,3,3,3,3,1,1,1,0,0,0,1},
				{0,1,0,0,0,0,1,0,2,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0,1},
				{1,1,0,1,1,1,1,0,2,2,2,2,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0,0},
				{1,0,0,0,0,0,1,0,0,0,0,2,0,0,1,1,1,1,1,1,1,1,0,1,0,1,1,3,3,3,3,3,3,3,3,3,3,3,1,1,0,0},
				{1,0,1,1,1,0,1,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,3,3,3,3,3,3,3,3,3,3,3,1,1,0,0},
				{1,0,0,0,0,0,1,0,0,0,0,2,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,3,3,3,3,3,3,3,3,3,3,3,1,1,0,0}
			};
			
			vector<vector<Field*>> map;
			
			for(int j = 0; j < 42; j++){
				vector<Field*> row;
				
				for(int i = 0; i < 42; i++){
					row.push_back(new Field(i, j, mapp[j][i]));
				}
				
				map.push_back(row);
			}
			
			for(int i = 0; i < 20;){
				int rand_i = rand() % 42;
				int rand_j = rand() % 42;
				
				if(map[rand_j][rand_i]->is_factory == false && map[rand_j][rand_i]->is_piece == false && map[rand_j][rand_i]->weight == 1){
					map[rand_j][rand_i]->is_piece   = true;
					map[rand_j][rand_i]->piece_type = battery;
					
					i++;
				}
			}
			
			for(int i = 0; i < 10;){
				int rand_i = rand() % 42;
				int rand_j = rand() % 42;
				
				if(map[rand_j][rand_i]->is_factory == false && map[rand_j][rand_i]->is_piece == false && map[rand_j][rand_i]->weight == 1){
					map[rand_j][rand_i]->is_piece   = true;
					map[rand_j][rand_i]->piece_type = solder;
					
					i++;
				}
			}
			
			for(int i = 0; i < 8;){
				int rand_i = rand() % 42;
				int rand_j = rand() % 42;
				
				if(map[rand_j][rand_i]->is_factory == false && map[rand_j][rand_i]->is_piece == false && map[rand_j][rand_i]->weight == 1){
					map[rand_j][rand_i]->is_piece   = true;
					map[rand_j][rand_i]->piece_type = bomb;
					
					i++;
				}
			}
			
			for(int i = 0; i < 6;){
				int rand_i = rand() % 42;
				int rand_j = rand() % 42;
				
				if(map[rand_j][rand_i]->is_factory == false && map[rand_j][rand_i]->is_piece == false && map[rand_j][rand_i]->weight == 1){
					map[rand_j][rand_i]->is_piece   = true;
					map[rand_j][rand_i]->piece_type = refrigeration;
					
					i++;
				}
			}
			
			for(int i = 0; i < 4;){
				int rand_i = rand() % 42;
				int rand_j = rand() % 42;
				
				if(map[rand_j][rand_i]->is_factory == false && map[rand_j][rand_i]->is_piece == false && map[rand_j][rand_i]->weight == 1){
					map[rand_j][rand_i]->is_piece   = true;
					map[rand_j][rand_i]->piece_type = pneumatic;
					
					i++;
				}
			}
			
			return map;
		}
};
