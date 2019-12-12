#include"Plants.h"


Plants::Plants(GLuint program, std::string start, 
	std::map<char, std::vector<std::string>> rules, int iterNum)
{
	Plants::start = start;
	Plants::rules = rules;
	Plants::iterNum = iterNum;
	Plants::program = program;
}

void Plants::iteration()
{
	std::string::iterator sIt;
	std::map<char, std::vector<std::string>>::iterator mIt;
	result = start;
	for (int i = 0; i < iterNum; i++)
	{
		std::string temp = "";
		for (sIt = result.begin(); sIt != result.end(); sIt++)
		{
			mIt = rules.find(*sIt);
			if (mIt != rules.end())
			{
				int ruleIndex = rand() % (mIt->second.size());
				temp = temp + mIt->second[ruleIndex];
			}
			else
			{
				temp = temp + *sIt;
			}
		}
		result = temp;
	}

	totalLevel = 0;
	int level = 0;
	for (sIt = result.begin(); sIt != result.end(); sIt++)
	{
		if (*sIt == '[')
		{
			level++;
			if (level > totalLevel)
			{
				totalLevel = level;
			}
		}
		else if (*sIt == ']')
		{
			level--;
		}
	}
	//std::cout << "Result String: " << result << std::endl;
}

Transform* Plants::construct()
{
	//Transform(glm::mat4 initial, GLuint animate, int cullingLevel,
		//float cullingRadius, Camera * camera);//Node* child
	float lFactor, rFactor;
	int currLevel = 0;
	glm::mat4 R = glm::mat4(1), T = glm::mat4(1), S = glm::mat4(1);
	Transform* plant = new Transform(T, 0, 0, 0, camera);//Node* child;
	Transform* trunk2tree;
	Trunk* trunk;
	Leaf* leaf;
	GLfloat cosTheta;
	State currState, nextState;
	glm::vec3 nextPos, nextDir, axis;
	glm::vec3 originDir = glm::vec3(0.0f, 1.0f, 0.0f);
	std::stack<State> forks;
	std::string::iterator sIt;
	leaves.clear();

	std::vector<trunkInfo> trunks;
	trunkInfo currTrunkInfo;
	for (sIt = result.begin(); sIt != result.end(); sIt++)
	{
		switch (*sIt)
		{
		case 'D':
		case 'F': // trunk
			if (currLevel < totalLevel) // branch
			{
				lFactor = (float)pow(lengthFactor, currLevel);
				//rFactor = (float)pow(radiusFactor, currLevel);
				nextPos = lFactor * trunkLength * currState.dir + currState.pos;
				//trunk2tree = new Transform(T * R * S, 0, 0, 0, camera);//Node* child
				//trunk2tree->addChild(trunk);
				//trunk = new Trunk(rFactor * radiusFactor * trunkRadius, 
					//rFactor * trunkRadius, lFactor * trunkLength, 
					//currState, trunkTexID, trunkTexLoc);
				//plant->addChild(trunk2tree);
				//plant->addChild(trunk);
				currTrunkInfo.start = currState.pos;
				currTrunkInfo.end = nextPos;
				currTrunkInfo.level = currLevel;
				trunks.push_back(currTrunkInfo);
				currState.pos = nextPos;
			}
			else // leaves
			{
				leaves.push_back(currState);
				//leaf = new Leaf(leafLength, currState, leafTexID, leafTexLoc);
				//plant->addChild(leaf);
			}
			
			break;

		case 'X': // leaf

			break;

		case '+': // Z axis, CCW
			//zRotate++;
			R = glm::rotate(glm::radians(zUnitDegree), glm::vec3(0.0f, 0.0f, 1.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '-': // Z axis, CW
			//zRotate--;
			R = glm::rotate(glm::radians(-zUnitDegree), glm::vec3(0.0f, 0.0f, 1.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '&': // Y axis, CCW
			//yRotate++;
			R = glm::rotate(glm::radians(yUnitDegree), glm::vec3(0.0f, 1.0f, 0.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '^': // Y axis, CW
			//yRotate--;
			R = glm::rotate(glm::radians(-yUnitDegree), glm::vec3(0.0f, 1.0f, 0.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '*': // X axis, CCW
			//xRotate++;
			R = glm::rotate(glm::radians(xUnitDegree), glm::vec3(1.0f, 0.0f, 0.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '/': // X axis, CW
			//xRotate--;
			R = glm::rotate(glm::radians(-xUnitDegree), glm::vec3(1.0f, 0.0f, 0.0f));
			currState.dir = R * glm::vec4(currState.dir, 0);
			currState.dir = glm::normalize(currState.dir);
			break;

		case '[':
			forks.push(currState);
			currLevel++;
			break;

		case ']':
			currState = forks.top();
			forks.pop();
			currLevel--;
			break;

		default:
			//std::cout << "Undifined character: " << *sIt << std::endl;
			break;
		}
	}
	trunk = new Trunk(trunkRadius, lengthFactor, radiusFactor, trunks, trunkTexID, trunkTexLoc);
	plant->addChild(trunk);
	leaf = new Leaf(leafLength, leaves, leafTexID, leafTexLoc);
	plant->addChild(leaf);

	return plant;
}

/****************************************/
Tree::Tree(GLuint program, Camera* camera)
{
	Tree::camera = camera;
	Tree::program = program;
}

Transform* Tree::oak()
{
	Trunk* trunk = new Trunk();
	Leaf* leaf = new Leaf();
	GLuint trunkTexID, trunkTexLoc = 0;
	GLuint leafTexID, leafTexLoc = 1;

	std::string file = "texture/bark_s.jpg";
	trunkTexID = loadTexture(file, trunkTexLoc);
	trunk->setParameter(program, camera);
	
	file = "texture/flower_s.png";
	leafTexID = loadTexture(file, leafTexLoc);
	leaf->setParameter(program, camera);

	srand((unsigned)time(NULL));
	std::string start = "F[*&X][+^X][/^X]";
	//std::string start = "X";
	std::vector<std::string> ruleX
	{
		"F[*^D][/&D][-&D][+^D]",
		"F[/^D][+&D][-&D][*^D]"
		//"F-[[X]+X]+F[+FX]-X"
	};
	std::vector<std::string> ruleF{ "F" };
	std::vector<std::string> ruleD{ "F[*&X][+^FX][/^X]" };
	std::map<char, std::vector<std::string>> rules =
	{
		{'F', ruleF},
		{'X', ruleX},
		{'D', ruleD}
	};
	int iterNum = 7;

	oakTree = new Plants(program, start, rules, iterNum);
	oakTree->leafLength = 0.4;
	//newTree->setTrunk(obj);
	oakTree->setCamera(camera);
	oakTree->setTex(trunkTexID,trunkTexLoc,leafTexID,leafTexLoc);

	oakTree->iteration();
	oakTreeObj = oakTree->construct();

	//Add particles effect
	Particles* fallingLeaves = new Particles();
	fallingLeaves->loadTexture("texture/flower_s.png", 0);
	fallingLeaves->initial(oakTree->getLeaves());
	oakTreeObj->addChild(fallingLeaves);

	return oakTreeObj;
}

Transform* Tree::bush()
{
	Trunk* trunk = new Trunk();
	Leaf* leaf = new Leaf();
	GLuint trunkTexID, trunkTexLoc = 2;
	GLuint leafTexID, leafTexLoc = 3;

	std::string file = "texture/bark_s.jpg";
	trunkTexID = loadTexture(file, trunkTexLoc);
	trunk->setParameter(program, camera);

	file = "texture/flower_s.png";
	leafTexID = loadTexture(file, leafTexLoc);
	leaf->setParameter(program, camera);

	srand((unsigned)time(NULL));
	std::string start = "F[*&X][+^X][/^X]";
	//std::string start = "X";
	std::vector<std::string> ruleX
	{
		"F[*^D][/&D][-&D][+^D]",
		"F[/^D][+&D][-&D][*^D]"
		//"F-[[X]+X]+F[+FX]-X"
	};
	std::vector<std::string> ruleF{ "F" };
	std::vector<std::string> ruleD{ "F[*&X][+^FX][/^X]" };
	std::map<char, std::vector<std::string>> rules =
	{
		{'F', ruleF},
		{'X', ruleX},
		{'D', ruleD}
	};
	int iterNum = 5;

	bushTree = new Plants(program, start, rules, iterNum);
	bushTree->leafLength = 0.4;
	//newTree->setTrunk(obj);
	bushTree->setCamera(camera);
	bushTree->setTex(trunkTexID, trunkTexLoc, leafTexID, leafTexLoc);

	bushTree->iteration();
	bushTreeObj = bushTree->construct();

	return bushTreeObj;
}

Transform* Tree::vine()
{
	Trunk* trunk = new Trunk();
	Leaf* leaf = new Leaf();
	GLuint trunkTexID, trunkTexLoc = 4;
	GLuint leafTexID, leafTexLoc = 5;

	std::string file = "texture/bark_s.jpg";
	trunkTexID = loadTexture(file, trunkTexLoc);
	trunk->setParameter(program, camera);

	file = "texture/leaf.png";
	leafTexID = loadTexture(file, leafTexLoc);
	leaf->setParameter(program, camera);

	srand((unsigned)time(NULL));
	std::string start = "F[*&X][+^X][/^X]";
	//std::string start = "X";
	std::vector<std::string> ruleX
	{
		"F[*^D][/&D][-&D][+^D]",
		"F[/^D][+&D][-&D][*^D]"
		//"F-[[X]+X]+F[+FX]-X"
	};
	std::vector<std::string> ruleF{ "F" };
	std::vector<std::string> ruleD{ "F[*&X][+^FX][/^X]" };
	std::map<char, std::vector<std::string>> rules =
	{
		{'F', ruleF},
		{'X', ruleX},
		{'D', ruleD}
	};
	int iterNum = 4;

	vineTree = new Plants(program, start, rules, iterNum);
	vineTree->leafLength = 2.0;
	//newTree->setTrunk(obj);
	vineTree->setCamera(camera);
	vineTree->setTex(trunkTexID, trunkTexLoc, leafTexID, leafTexLoc);

	vineTree->iteration();
	vineTreeObj = vineTree->construct();

	return vineTreeObj;
}


GLuint Tree::loadTexture(std::string file, GLuint texStoreLoc)
{
	glUseProgram(program);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + texStoreLoc);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = imread(file.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	imfree(data);

	glGenerateMipmap(texture);

	return texture;
}

/******************************************/
GLuint Trunk::program, Trunk::modelLoc, Trunk::viewLoc, Trunk::projectionLoc; 
Camera* Trunk::camera;
Trunk::Trunk(GLfloat radium, GLfloat lFactor, GLfloat rFactor,
	std::vector<trunkInfo> trunks, GLuint textureID, GLuint textureLoc)
{
	Trunk::textureID = textureID;
	Trunk::texStoreLoc = textureLoc;
	glm::mat4 R = glm::mat4(1), T = glm::mat4(1);
	glm::vec3 axis, originDir = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat cosTheta;
	trunkCount = 0;

	int totalNum = trunks.size();
	for (int i = 0; i < totalNum; i++)
	{
		glm::vec3 dir = trunks[i].end - trunks[i].start;
		GLfloat length = glm::length(dir);
		dir = glm::normalize(dir);

		if (originDir.y - dir.y > 0.0001)
		{
			axis = glm::cross(originDir, dir);
			cosTheta = glm::dot(originDir, dir);
			R = glm::rotate(glm::acos(cosTheta), axis);
		}
		else
		{
			R = glm::mat4(1);
		}


		T = glm::translate((float)0.5 * length * dir + trunks[i].start);

		//currentModel = T * R;

		GLfloat lowR = radium * pow(rFactor, trunks[i].level);
		GLfloat upR = lowR * rFactor;
		initialization(upR, lowR, length, T * R);
	}

	


	glUseProgram(program);

	// Generate a vertex array (VAO), vertex buffer objects (VBO) and EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	//d. How to read VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsNormal.size(), pointsNormal.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(1);
	//d. How to read VBO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoord.size(), texCoord.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(2);
	//d. How to read VBO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	//a. Bind to the EBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//b. Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Trunk::~Trunk()
{

}

void Trunk::draw(glm::mat4 model)
{
	glUseProgram(program);
	setShaderPara(model);
	glDisable(GL_CULL_FACE);
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0 + texStoreLoc);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Trunk::update()
{

}

void Trunk::setShaderPara(glm::mat4 model)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));// * currentModel
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform1i(glGetUniformLocation(program, "barkTex"), texStoreLoc);
	glUniform1i(glGetUniformLocation(program, "type"),1);
	
}

void Trunk::setCamera(Camera* camera)
{
	Trunk::camera = camera;
}

void Trunk::initialization(GLfloat upR, GLfloat lowR, GLfloat length, glm::mat4 toWorld)
{
	glm::vec3 upPoint, lowPoint;
	glm::vec3 normal;
	double theta;
	
	for (int i = 0; i < samplingTimes; i++)
	{
		theta = glm::radians((float)i * 360.f / (float)samplingTimes);

		upPoint.x = upR*glm::cos(theta);
		upPoint.y = length / 2;
		upPoint.z = upR * glm::sin(theta);
		points.push_back(toWorld * glm::vec4(upPoint, 1.0));
		texCoord.push_back(glm::vec2((float)i/ (float)samplingTimes, 1.0));

		lowPoint.x = lowR * glm::cos(theta);
		lowPoint.y = -length / 2;
		lowPoint.z = lowR * glm::sin(theta);
		points.push_back(toWorld * glm::vec4(lowPoint, 1.0));
		texCoord.push_back(glm::vec2((float)i / (float)samplingTimes, 0.0));

		normal = glm::cross(upPoint, upPoint - lowPoint);
		normal = glm::cross(upPoint - lowPoint, normal);

		normal = glm::mat3(transpose(inverse(model))) * normal;
		normal = glm::normalize(normal);

		pointsNormal.push_back(normal);
		pointsNormal.push_back(normal);

		if (i != samplingTimes - 1)
		{
			faces.push_back(glm::ivec3(2 * i, 2 * i + 1, 2 * i + 2) + glm::ivec3(trunkCount * samplingTimes * 2));
			faces.push_back(glm::ivec3(2 * i + 3, 2 * i + 2, 2 * i + 1) + glm::ivec3(trunkCount * samplingTimes * 2));
		}
		else
		{
			faces.push_back(glm::ivec3(2 * i, 2 * i + 1, 0) + glm::ivec3(trunkCount * samplingTimes * 2));
			faces.push_back(glm::ivec3(1, 0, 2 * i + 1) + glm::ivec3(trunkCount * samplingTimes * 2));
		}
		
	}
	trunkCount++;
}

void Trunk::setParameter(GLuint programID, Camera* camera)
{
	program = programID;
	Trunk::camera = camera;

	glUseProgram(program);
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projectionLoc = glGetUniformLocation(program, "projection");
}

/**************************************************************************/

GLuint Leaf::program, Leaf::modelLoc, Leaf::viewLoc, Leaf::projectionLoc;
Camera* Leaf::camera;
Leaf::Leaf(GLfloat length, std::vector<State> states, GLuint textureID, GLuint texStoreLoc)
{
	Leaf::textureID = textureID;
	Leaf::texStoreLoc = texStoreLoc;
	glm::mat4 R = glm::mat4(1), T = glm::mat4(1);
	glm::vec3 axis, originDir = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat cosTheta;

	GLfloat halfL = length / 2;
	std::vector<glm::vec3> pointsSet = { glm::vec3(-halfL, +halfL, 0.0),
				glm::vec3(-halfL, -halfL, 0.0),
				glm::vec3(+halfL, +halfL, 0.0),
				glm::vec3(+halfL, -halfL, 0.0) };

	std::vector<glm::vec3> pointsNormalSet = { glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0) };

	std::vector<glm::ivec3> facesSet = { glm::ivec3(0,1,2),glm::ivec3(3,2,1) };

	std::vector<glm::vec2> texCoordSet = { glm::vec2(1.0, 0.0),
				glm::vec2(1.0, 1.0),
				glm::vec2(0.0, 0.0),
				glm::vec2(0.0, 1.0) };

	totalNum = states.size();
	glm::mat4 toWorld;
	for (int i = 0; i < totalNum; i++)
	{
		if (originDir.y - states[i].dir.y > 0.0001)
		{
			axis = glm::cross(originDir, states[i].dir);
			cosTheta = glm::dot(originDir, states[i].dir);
			R = glm::rotate(glm::acos(cosTheta), axis);
		}
		else
		{
			R = glm::mat4(1);
		}
		T = glm::translate((float)0.5 * length * states[i].dir + states[i].pos);
		toWorld = T * R;

		points.push_back(toWorld * glm::vec4(pointsSet[0], 1.0));
		points.push_back(toWorld * glm::vec4(pointsSet[1], 1.0));
		points.push_back(toWorld * glm::vec4(pointsSet[2], 1.0));
		points.push_back(toWorld * glm::vec4(pointsSet[3], 1.0));

		faces.push_back(facesSet[0] + glm::ivec3(4 * i));
		faces.push_back(facesSet[1] + glm::ivec3(4 * i));

		pointsNormal.insert(pointsNormal.end(), pointsNormalSet.begin(), pointsNormalSet.end());
		texCoord.insert(texCoord.end(), texCoordSet.begin(), texCoordSet.end());
	}


	glUseProgram(program);
	//initialization(length);
	//Trunk::state = state;

	// Generate a vertex array (VAO), vertex buffer objects (VBO) and EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	//d. How to read VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsNormal.size(), pointsNormal.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(1);
	//d. How to read VBO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoord.size(), texCoord.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(2);
	//d. How to read VBO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	//a. Bind to the EBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//b. Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Leaf::~Leaf()
{

}

void Leaf::draw(glm::mat4 model)
{
	glUseProgram(program);
	setShaderPara(model);
	glDisable(GL_CULL_FACE);
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0+texStoreLoc);
	glBindTexture(GL_TEXTURE_2D, textureID);

	
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Leaf::update()
{

}

void Leaf::setShaderPara(glm::mat4 model)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));//* currentModel
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform1i(glGetUniformLocation(program, "leafTex"), texStoreLoc);
	glUniform1i(glGetUniformLocation(program, "type"), 2);

}

void Leaf::setCamera(Camera* camera)
{
	Leaf::camera = camera;
}

void Leaf::initialization(GLfloat length)
{
	glm::vec3 upPoint, lowPoint;
	glm::vec3 normal;
	double theta;
	GLfloat halfL = length / 2;
	points = {glm::vec3(-halfL, +halfL, 0.0),
				glm::vec3(-halfL, -halfL, 0.0), 
				glm::vec3(+halfL, +halfL, 0.0), 
				glm::vec3(+halfL, -halfL, 0.0) };

	pointsNormal = { glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0)};

	faces = { glm::ivec3(0,1,2),glm::ivec3(3,2,1) };

	texCoord = { glm::vec2(1.0, 0.0),
				glm::vec2(1.0, 1.0),
				glm::vec2(0.0, 0.0),
				glm::vec2(0.0, 1.0) };
}

void Leaf::setParameter(GLuint programID, Camera* camera)
{
	program = programID;
	Leaf::camera = camera;

	glUseProgram(program);
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projectionLoc = glGetUniformLocation(program, "projection");
}

/****************************************************************/
Camera* Particles::camera;
GLuint Particles::programID, Particles::modelLoc, Particles::viewLoc, Particles::projectionLoc;
void Particles::initial(std::vector<State> sources)
{
	Particles::sources = sources;
	maxParticles = sources.size();
	
	for (int i = 0; i < maxParticles; i++)
	{
		Particle newParticle;
		newParticle.pos = sources[i].pos;
		centers.push_back(sources[i].pos);
		newParticle.speed = glm::vec3(0);

		/*
		glm::vec3 axis, originDir = glm::vec3(0.0f, 1.0f, 0.0f);
		GLfloat cosTheta;
		if (originDir.y - sources[i].dir.y > 0.0001)
		{
			axis = glm::cross(originDir, sources[i].dir);
			cosTheta = glm::dot(originDir, sources[i].dir);
			newParticle.roatateMatrix = glm::rotate(glm::acos(cosTheta), axis);
		}
		else
		{
			newParticle.roatateMatrix = glm::mat4(1);
		}
		*/
		newParticle.life = -1.0;
		particlesContainer.push_back(newParticle);
	}

	float halfL = 0.2;
	points = { glm::vec3(-halfL, +halfL, 0.0),
				glm::vec3(-halfL, -halfL, 0.0),
				glm::vec3(+halfL, +halfL, 0.0),
				glm::vec3(+halfL, -halfL, 0.0) };

	pointsNormal = { glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0),
				glm::vec3(0,0,1.0) };

	faces = { glm::ivec3(0,1,2),glm::ivec3(3,2,1) };

	texCoord = { glm::vec2(1.0, 0.0),
				glm::vec2(1.0, 1.0),
				glm::vec2(0.0, 0.0),
				glm::vec2(0.0, 1.0) };


	glUseProgram(programID);
	
	// Generate a vertex array (VAO), vertex buffer objects (VBO) and EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	//d. How to read VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//normals
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsNormal.size(), pointsNormal.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(1);
	//d. How to read VBO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//texture corrdinate
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	//b. Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoord.size(), texCoord.data(), GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(2);
	//d. How to read VBO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	//centers
	//a. Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
	//b. initialize with empty buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_STATIC_DRAW);
	// c. Enable vertex attribute 0. Creat channel
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(3);
	//d. How to read VBO
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//a. Bind to the EBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//b. Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	lastTime = glfwGetTime();
}

void Particles::draw(glm::mat4 model)
{
	glUseProgram(programID);
	setShaderPara(model);
	glDisable(GL_CULL_FACE);
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	
	glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 0); // normals
	glVertexAttribDivisor(2, 0); // tex coord
	glVertexAttribDivisor(3, 1); // centers : one per quad (its center)
	
	glDrawElementsInstanced(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0, particlesCount);
																		
	// Unbind from the VAO.
	glBindVertexArray(0);
}


void Particles::setShaderPara(glm::mat4 model)
{
	glm::mat4 view = camera->getViewMatrix();
	glm::vec3 camDir = view[2];
	glm::vec3 originDir = model * glm::vec4(0,0,1,0);
	glm::vec3 axis = glm::cross(originDir, camDir);
	glm::mat4 R;
	if (glm::length(axis) > 0.00001)
	{
		//axis = glm::normalize(axis);
		GLfloat cosTheta = glm::dot(originDir, camDir);
		R = glm::rotate(glm::acos(cosTheta), axis);
	}
	else
	{
		R = glm::mat4(1);
	}
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotate"), 1, GL_FALSE, glm::value_ptr(R));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));//camera->getViewMatrix()
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform1i(glGetUniformLocation(programID, "leafTex"), 0);
	//glUniform1i(glGetUniformLocation(programID, "type"), 2);
}

void Particles::update()
{
	GLfloat time = glfwGetTime();
	GLfloat deltaT = time - lastTime;
	lastTime = time;

	int newparticles = (int)(deltaT * 400.0);
	//if (newparticles > (int)(0.016f * 10.0))
		//newparticles = (int)(0.016f * 10.0);

	for (int i = 0; i < newparticles; i++) {
		int particleIndex = findUnusedParticle();
		particlesContainer[particleIndex].life = 200.0f; // This particle will live 5 seconds.
		particlesContainer[particleIndex].pos = sources[rand()% maxParticles].pos;

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(-3.0f, 0.0f, 0.0f);
		
		glm::vec3 randomdir = glm::vec3(
			((float)rand() / (float)RAND_MAX) - 1.0,
			((float)rand() / (float)RAND_MAX) - 1.0,
			((float)rand() / (float)RAND_MAX) - 1.0
		);

		particlesContainer[particleIndex].speed = (maindir + randomdir) * spread;
	}

	particlesCount = 0;
	for (int i = 0; i < maxParticles; i++) {

		Particle& p = particlesContainer[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= deltaT;
			if (p.life > 0.0f) {

				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)deltaT * 0.5f;
				p.pos += p.speed * (float)deltaT;
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;
				if (p.pos.y > 0)
				{
					// Fill the GPU buffer
					centers[particlesCount] = p.pos;
				}
				else
				{
					p.life = -0.1;
				}
			}
			//else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				//p.cameradistance = -1.0f;
			//}

			particlesCount++;
		}
	}

	glUseProgram(programID);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLfloat) * 3, centers.data());

}

void Particles::loadTexture(std::string file, GLuint texStoreLoc)
{
	glUseProgram(programID);
	//GLuint texture;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + texStoreLoc);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = imread(file.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	imfree(data);

	glGenerateMipmap(textureID);

}

int Particles::findUnusedParticle()
{
	for (int i = LastUsedParticle; i < maxParticles; i++) {
		if (particlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < LastUsedParticle; i++) {
		if (particlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0;
}

void Particles::setParameters(GLuint programID, Camera* camera) 
{
	Particles::programID = programID;
	Particles::camera = camera;

	glUseProgram(programID);
	modelLoc = glGetUniformLocation(programID, "model");
	viewLoc = glGetUniformLocation(programID, "view");
	projectionLoc = glGetUniformLocation(programID, "projection");
}