class Agent{
	
	public:
		int pos_x, pos_y;
		int range;
		
		int n_battery;
		int n_solder;
		int n_bomb;
		int n_refrigeration;
		int n_pneumatic;
		
		vector<Field*> factory_position;
		vector<int> factory_pieces;

		vector<Field*> objetivos;
		
		Agent(){}
		
		Agent(int _pos_x, int _pos_y, vector<Field*> _factory_position){
			this->pos_x = _pos_x;
			this->pos_y = _pos_y;
			this->range = 4;
			
			this->n_battery       = 0;
			this->n_solder        = 0;
			this->n_bomb          = 0;
			this->n_refrigeration = 0;
			this->n_pneumatic     = 0;
			
			this->factory_position = _factory_position;
			this->factory_pieces   = {0,0,0,0,0};
		}
		
		vector<Field*> verify_range(vector<vector<Field*>> map){
			
			vector<Field*> pieces;
		
			for(int j = -range; j <= range; j++){
				for(int i = -range; i <= range; i++){
					
					int y = this->pos_y + j;
					int x = this->pos_x + i;
					
					if(!(i == x && j == y) && this->is_valid_coordinates(x, y)){
						if(map[y][x]->is_piece){
							pieces.push_back(map[y][x]);
						}
					}
				}
			}
			
			return pieces;
		}
		
		bool is_valid_coordinates(int x, int y){
			if(
				x >= 0 &&
				y >= 0 &&
				x < 42 &&
				y < 42
			){
				return true;
			}

			return false;
		}
		
		void move(){
			vector<Field> neighbors;
			
			int x = this->pos_x;
			int y = this->pos_y;

			if(x - 1 >= 0){
				neighbors.push_back(Field(x - 1, y));
			}

			if(x + 1 < 42){
				neighbors.push_back(Field(x + 1, y));
			}

			if(y - 1 >= 0){
				neighbors.push_back(Field(x, y - 1));
			}
			
			if(y + 1 < 42){
				neighbors.push_back(Field(x, y + 1));
			}
			
			Field new_pos = neighbors[rand() % neighbors.size()];
			
			this->pos_x = new_pos.pos_x;
			this->pos_y = new_pos.pos_y;
			
			cost += fields[this->pos_y][this->pos_x]->weight;
		}
		
		void move(Field field){
			this->pos_x = field.pos_x;
			this->pos_y = field.pos_y;

			cost += fields[this->pos_y][this->pos_x]->weight;
		}
		
		void draw_agent(){
			GLfloat xSize = (window_width / game_width);
			GLfloat ySize = (window_height / game_height);
			
			glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex2f(    this->pos_x*xSize,    this->pos_y*ySize);
				glVertex2f((this->pos_x+1)*xSize,    this->pos_y*ySize);
				glVertex2f((this->pos_x+1)*xSize,(this->pos_y+1)*ySize);
				glVertex2f(    this->pos_x*xSize,(this->pos_y+1)*ySize);
			glEnd();
			glFlush();
		}
		
		void draw_range(){
			GLfloat xSize = (window_width / game_width);
			GLfloat ySize = (window_height / game_height);
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex2f(    (this->pos_x - this->range) * xSize,    (this->pos_y - this->range) * ySize);
				glVertex2f((this->pos_x + this->range + 1)*xSize,    (this->pos_y - this->range) * ySize);
				glVertex2f((this->pos_x + this->range + 1)*xSize,(this->pos_y + this->range + 1)*ySize);
				glVertex2f(    (this->pos_x - this->range)*xSize,(this->pos_y + this->range + 1)*ySize);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
			glFlush();
		}
		
		void draw_path(vector<Field> path){
			GLfloat xSize = (window_width / game_width);
			GLfloat ySize = (window_height / game_height);
			
			glBegin(GL_QUADS);
				glColor3f(0.9f, 0.0f, 1.0f);
				
				for(int i = 0; i < path.size(); i++){
					glVertex2f(    path[i].pos_x*xSize,    path[i].pos_y*ySize);
					glVertex2f((path[i].pos_x+1)*xSize,    path[i].pos_y*ySize);
					glVertex2f((path[i].pos_x+1)*xSize,(path[i].pos_y+1)*ySize);
					glVertex2f(    path[i].pos_x*xSize,(path[i].pos_y+1)*ySize);
				}
				
			glEnd();
			
			glFlush();
		}
		
		bool in(vector<Field*> objetivos, Field *current){
			for (int i = 0; i < objetivos.size(); ++i){
				if(objetivos[i] == current){
					return true;
				}
			}

			return false;
		}

		void piece_priority(vector<Field*> pieces){
			for(int i = 0; i < pieces.size(); i++){
				if((pieces[i]->piece_type == battery       && this->factory_pieces[0] >= 8) || in(this->objetivos, pieces[i])){continue;}
				if((pieces[i]->piece_type == solder        && this->factory_pieces[1] >= 5) || in(this->objetivos, pieces[i])){continue;}
				if((pieces[i]->piece_type == bomb          && this->factory_pieces[2] >= 2) || in(this->objetivos, pieces[i])){continue;}
				if((pieces[i]->piece_type == refrigeration && this->factory_pieces[3] >= 5) || in(this->objetivos, pieces[i])){continue;}
				if((pieces[i]->piece_type == pneumatic     && this->factory_pieces[4] >= 2) || in(this->objetivos, pieces[i])){continue;}
				
				this->objetivos.insert(this->objetivos.begin(), pieces[i]);
			}
		}
		
		void factory_priority(){
			vector<Field*> priority;

			if(this->factory_pieces[0] >= 8 && factory_position[0]->is_fixed == false && !in(this->objetivos, factory_position[0])){
				this->objetivos.insert(this->objetivos.begin(), factory_position[0]);
			}else
			
			if(this->factory_pieces[1] >= 5 && factory_position[1]->is_fixed == false && !in(this->objetivos, factory_position[1])){
				this->objetivos.insert(this->objetivos.begin(), factory_position[1]);
			}else
			
			if(this->factory_pieces[2] >= 2 && factory_position[2]->is_fixed == false && !in(this->objetivos, factory_position[2])){
				this->objetivos.insert(this->objetivos.begin(), factory_position[2]);
			}else
			
			if(this->factory_pieces[3] >= 5 && factory_position[3]->is_fixed == false && !in(this->objetivos, factory_position[3])){
				this->objetivos.insert(this->objetivos.begin(), factory_position[3]);
			}else
			
			if(this->factory_pieces[4] >= 2 && factory_position[4]->is_fixed == false && !in(this->objetivos, factory_position[4])){
				this->objetivos.insert(this->objetivos.begin(), factory_position[4]);
			}
		}
		
		void run(vector<vector<Field*>> map){
			
			if(
				!factory_position[0]->is_fixed ||
				!factory_position[1]->is_fixed ||
				!factory_position[2]->is_fixed ||
				!factory_position[3]->is_fixed ||
				!factory_position[4]->is_fixed
			){
				
				Field::draw_map(map);
				
				factory_priority();

				while(this->objetivos.empty()){
					this->move();
				
					Field::draw_map(map);
					this->draw_agent();
					this->draw_range();
					
					usleep(100000);
					
					piece_priority(verify_range(map));
				}
				
				factory_priority();
				
				while(!this->objetivos.empty()){
			
					Field::draw_map(map);
					this->draw_agent();
					this->draw_range();
					
					vector<Field> path = a_star(map, Field(this->pos_x, this->pos_y), *this->objetivos[0]);
					
					Field::draw_map(map);
					this->draw_path(path);
					this->draw_agent();
					this->draw_range();
					
					int init_size = this->objetivos.size();

					for (Field field : path){
						this->move(field);
						
 						piece_priority(verify_range(map));

 						if(this->objetivos.size() != init_size){
 							break;
 						}
						
						Field::draw_map(map);
						this->draw_path(path);
						this->draw_agent();
						this->draw_range();
						
						usleep(100000);
					}

					if(this->objetivos.size() != init_size){
 						break;
 					}
					
					if(map[this->pos_y][this->pos_x]->is_piece){
						if(map[this->pos_y][this->pos_x]->piece_type == battery){this->factory_pieces[0]++;}
						if(map[this->pos_y][this->pos_x]->piece_type == solder){this->factory_pieces[1]++;}
						if(map[this->pos_y][this->pos_x]->piece_type == bomb){this->factory_pieces[2]++;}
						if(map[this->pos_y][this->pos_x]->piece_type == refrigeration){this->factory_pieces[3]++;}
						if(map[this->pos_y][this->pos_x]->piece_type == pneumatic){this->factory_pieces[4]++;}
					}
					
					if(map[this->pos_y][this->pos_x]->is_factory){
						if(map[this->pos_y][this->pos_x]->factory_type == factory_1){map[this->pos_y][this->pos_x]->is_fixed = true;}
						if(map[this->pos_y][this->pos_x]->factory_type == factory_2){map[this->pos_y][this->pos_x]->is_fixed = true;}
						if(map[this->pos_y][this->pos_x]->factory_type == factory_3){map[this->pos_y][this->pos_x]->is_fixed = true;}
						if(map[this->pos_y][this->pos_x]->factory_type == factory_4){map[this->pos_y][this->pos_x]->is_fixed = true;}
						if(map[this->pos_y][this->pos_x]->factory_type == factory_5){map[this->pos_y][this->pos_x]->is_fixed = true;}
					}
					
					map[this->pos_y][this->pos_x]->is_piece   = false;
					map[this->pos_y][this->pos_x]->is_factory = false;
					
					this->objetivos.erase(this->objetivos.begin());
				}

				Field::draw_map(map);
			}else {
				cout << "Nós expandidos pelo A*: " << expanded_nodes << endl;
				cout << "Custo total: " << cost << endl;
			}
		}
	
	private:
		double manhattan_distance(Field start, Field goal){
			int x1 = start.pos_x;
			int y1 = start.pos_y;
			int x2 = goal.pos_x;
			int y2 = goal.pos_y;

			return abs(x1 - x2) + abs(y1 - y2);
		}

		Field min(vector<Field> openSet, map<Field, double> fScore){
			
			Field openSet_min = openSet[0];
			double openSet_min_value = fScore[openSet[0]];
			
			for(int i = 1; i < openSet.size(); i++){
			
				if(fScore[openSet[i]] < openSet_min_value){
					openSet_min       = openSet[i];
					openSet_min_value = fScore[openSet[i]];
				}

			}
			
			return openSet_min;
		}

		map<Field, double> init_gScore(int w, int h){
			map<Field, double> gScore;
			
			for(int j = 0; j < h; j++){
				for(int i = 0; i < w; i++){
					gScore[Field(i, j)] = INFINITY;
				}
			}
			
			return gScore;
		}

		map<Field, double> init_fScore(int w, int h){
			map<Field, double> fScore;
			
			for(int j = 0; j < h; j++){
				for(int i = 0; i < w; i++){
					fScore[Field(i, j)] = INFINITY;
				}
			}
			
			return fScore;
		}

		vector<Field> get_neighbors(Field current){
			int x = current.pos_x;
			int y = current.pos_y;
			
			vector<Field> neighbors;

			if(x - 1 >= 0){
				neighbors.push_back(Field(x - 1, y));
			}

			if(x + 1 < 42){
				neighbors.push_back(Field(x + 1, y));
			}

			if(y - 1 >= 0){
				neighbors.push_back(Field(x, y - 1));
			}
			
			if(y + 1 < 42){
				neighbors.push_back(Field(x, y + 1));
			}
			
			return neighbors;
		}

		void remove(vector<Field> *openSet, Field current){
			for(int i = 0; i < openSet->size(); i++){
				if((*openSet)[i] == current){
					openSet->erase(openSet->begin() + i);
				}
			}
		}

		bool in(vector<Field> openSet, Field current){
			for (int i = 0; i < openSet.size(); ++i){
				if(openSet[i] == current){
					return true;
				}
			}

			return false;
		}
		
		vector<Field> reverse_vector(vector<Field> v){
			vector<Field> reverse;
			
			for(int i = v.size() - 1; i >= 0; i--){
				reverse.push_back(v[i]);
			}
			
			return reverse;
		}

		vector<Field> reconstruct_path(map<Field, Field> cameFrom, Field current){
			vector<Field> reverse_path{current};

			while(cameFrom.count(current) != 0){
				current = cameFrom[current];
				reverse_path.push_back(current);
			}

			return reverse_vector(reverse_path);
		}
		
		vector<Field> a_star(vector<vector<Field*>> mapp, Field start, Field goal){
			vector<Field> openSet{start};
			
			map<Field, Field> cameFrom;
			map<Field, double> gScore = init_gScore(42, 42);
			map<Field, double> fScore = init_fScore(42, 42);

			gScore[start] = 0;
			fScore[start] = manhattan_distance(start, goal);
			
			while(!openSet.empty()){
				expanded_nodes++;
				usleep(10000);
				Field current = min(openSet, fScore);

				if(current == goal){
					return reconstruct_path(cameFrom, current);
				}
				
				remove(&openSet, current);

				GLfloat xSize = window_width / game_width;
				GLfloat ySize = window_height / game_height;
				glBegin(GL_QUADS);
					glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
					glVertex2f(      current.pos_x * xSize,       current.pos_y * ySize);
					glVertex2f((current.pos_x + 1) * xSize,       current.pos_y * ySize);
					glVertex2f((current.pos_x + 1) * xSize, (current.pos_y + 1) * ySize);
					glVertex2f(      current.pos_x * xSize, (current.pos_y + 1) * ySize);
				glEnd();

				glFlush();
				
				for(Field neighbor: get_neighbors(current)){
					double tentative_gScore = gScore[current] + mapp[neighbor.pos_y][neighbor.pos_x]->weight;
					
					if(tentative_gScore < gScore[neighbor]){
						cameFrom[neighbor] = current;
						gScore[neighbor]   = tentative_gScore;
						fScore[neighbor]   = tentative_gScore + manhattan_distance(neighbor, goal);

						if(!in(openSet, neighbor)){
							openSet.push_back(neighbor);
						}
					}
				}

				
			}
		}
};
