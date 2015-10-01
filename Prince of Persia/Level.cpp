#include "Level.h";



Level::Level() {
	
}


void Level::setGFX(Graphics* gfx) {
	graphics = gfx;
}


void Level::loadLevel(int l) {

	std::string levelNo = static_cast<std::ostringstream*>( &(std::ostringstream() << l) )->str();

	std::string filePath = "Levels/"; 
	filePath += "level";
	filePath += levelNo;
	filePath += ".txt";

	//open file
	FILE *file = fopen(filePath.c_str(), "rb");
	
	//set initial scene
	fscanf (file, "%d", &scene_x);
	fscanf (file, "%d", &scene_y);
	
	fscanf (file, "%d", &level_height);
	fscanf (file, "%d", &level_width);

	char c;
	//read contents into level array	
	
	for(int i = 0; i < level_height; i++) {
		for(int j = 0; j < level_width; j++) {
			char c = fgetc(file);
			if(c == '\r') {
				c = fgetc(file);
			}
			if(c == '\n') {
				c = fgetc(file);
			}
			level[i][j] = c;
		}
	}

	fclose (file);


	file = fopen("Levels/kkt.txt", "wb");

	for(int i = 0; i < level_height; i++) {
		for(int j = 0; j < level_width; j++) {
			fputc(level[i][j], file);
		}
		fputc('\r\n', file);
	}

	fclose(file);

	/*
	std::ifstream is ("test.txt", std::ifstream::binary);
	if (!is) { return; }

	is>>scene_x;
	is>>scene_y;

	// get length of file:
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);

	char * buffer = new char [length];

	// read data as a block:
	is.read (buffer,length);
	//std::cout << "error: only " << is.gcount() << " could be read";

	is.close();

	// ...buffer contains the entire file...

	delete[] buffer;
	*/
	

	if(DEBUG) {
		
		std::string s;
		s = "Scene_X: " + std::to_string((long double)scene_x) + " | Scene_Y: " + std::to_string((long double)scene_y);
		s += " | ";
		s += "Width: " + std::to_string((long double)level_width) + " | Height: " + std::to_string((long double)level_height);
		s += "\n";

		OutputDebugStringA(s.c_str());
		
	}



}

char Level::getCode(int i, int j) {
	return level[scene_y + i][scene_x + j];
}

void Level::changeScene(direction dir) {

	switch(dir) {

	case U:
		scene_y -= 3;
		if(scene_y < 0) scene_y = 0;
		break;
	case D:
		scene_y += 3;
		if(scene_y > level_height) scene_y = level_height;
		break;
	case L:

		scene_x -= 10;
		if(scene_x < 0) scene_x = 0;
		break;
	case R:
		if(scene_x > level_width) scene_x = level_width;
		scene_x += 10;
		break;
	}

}