#include "geometry.h"

//GLuint Geometry::program, Geometry::modelLoc, Geometry::viewLoc, Geometry::projectionLoc;

//GLuint Sphere::sphereFlag(1);
//GLuint Sphere::program, Sphere::modelLoc, Sphere::viewLoc, Sphere::projectionLoc, Sphere::cameraPosLoc;
//GLuint Sphere::type(1), Sphere::typeLoc;

Geometry::Geometry() {}

Geometry::Geometry(std::string objFilename, GLuint programID, Camera* camera)
{
	program = programID;
	glUseProgram(program);
	model = glm::mat4(1);
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projectionLoc = glGetUniformLocation(program, "projection");

	Geometry::camera = camera;

	initialization(objFilename);
}

Geometry::~Geometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, vbos);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::initialization(std::string objFilename)
{
	readObjFile(objFilename);

	//Find max & min
	maxCoordinate = points[0];
	minCoordinate = points[0];
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].x < minCoordinate.x)
		{
			minCoordinate.x = points[i].x;
		}
		else if (points[i].x > maxCoordinate.x)
		{
			maxCoordinate.x = points[i].x;
		}

		if (points[i].y < minCoordinate.y)
		{
			minCoordinate.y = points[i].y;
		}
		else if (points[i].y > maxCoordinate.y)
		{
			maxCoordinate.y = points[i].y;
		}

		if (points[i].z < minCoordinate.z)
		{
			minCoordinate.z = points[i].z;
		}
		else if (points[i].z > maxCoordinate.z)
		{
			maxCoordinate.z = points[i].z;
		}
	}

	modelCenter = 0.5f * (maxCoordinate + minCoordinate);
	//std::cout << "modelCenter" << modelCenter.x << modelCenter.y << modelCenter.z << std::endl;

	float tempRadius = 0;
	for (int i = 0; i < points.size(); i++)
	{
		tempRadius = glm::length(points[i] - modelCenter); // calculate distance
		radius = (tempRadius > radius) ? tempRadius : radius;//find min
		points[i] = points[i] - modelCenter;
	}

	//Centralize
	modelCenter = glm::vec3(0.0, 0.0, 0.0);

	//Scale
	/*
	float winLength = 480;//the length of the shorter side of the window
	if (radius > 0)
	{
		float factor = 50;
		float scalePara = winLength / (radius * factor);
		//model = glm::scale(model, glm::vec3(scalePara, scalePara, scalePara));
	}*/



	// Set the color. 
	//color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO), vertex buffer objects (VBO) and EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);
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

	//a. Bind to the EBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//b. Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 curModel)
{
	glUseProgram(program);
	setMatrix(curModel);

	
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);

	/*
	// Bind to the VAO.
	glBindVertexArray(vao);
	glPointSize(1);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawArrays(GL_TRIANGLES, 0, points.size()); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);
*/

}

void Geometry::update()
{

}

void Geometry::setMatrix(glm::mat4 curModel)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(curModel * model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform1i(glGetUniformLocation(program, "type"), 5);
}


void Geometry::readObjFile(std::string fileName)
{
	std::vector<glm::vec3> pointsTemp;
	std::vector<glm::vec3> pointsNormalTemp;
	std::vector<glm::ivec3> facesNormalTemp;
	std::vector<glm::ivec3> facesTemp;

	std::ifstream objFile("PointObj/" + fileName); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				// Process the point. For example, you can save it to a.
				pointsTemp.push_back(point);

				//coloring
				//ss >> point.x >> point.y >> point.z;
				//pointsColor.push_back(point);

			}
			else if (label == "vn")
			{
				glm::vec3 pointNormal;
				ss >> pointNormal.x >> pointNormal.y >> pointNormal.z;

				pointsNormalTemp.push_back(pointNormal);
			}
			else if (label == "f")
			{
				glm::ivec3 face;
				glm::ivec3 faceNormal;
				char tempChar;
				unsigned tempNumber;
				
				ss >> face.x >> tempChar >> tempNumber >> tempChar >> faceNormal.x >> 
					face.y >> tempChar >>tempNumber>> tempChar >> faceNormal.y >> 
					face.z >> tempChar >> tempNumber >> tempChar >> faceNormal.z;
				
				//start from 0	
				face.x--;
				face.y--;
				face.z--;
				faceNormal.x--;
				faceNormal.y--;
				faceNormal.z--;

				facesTemp.push_back(face);
				facesNormalTemp.push_back(faceNormal);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	objFile.close();

	
	for (int i = 0; i < facesTemp.size(); i++)
	{
		glm::ivec3 tempFace;
		points.push_back(pointsTemp[facesTemp[i].x]);
		points.push_back(pointsTemp[facesTemp[i].y]);
		points.push_back(pointsTemp[facesTemp[i].z]);

		pointsNormal.push_back(pointsNormalTemp[facesNormalTemp[i].x]);
		pointsNormal.push_back(pointsNormalTemp[facesNormalTemp[i].y]);
		pointsNormal.push_back(pointsNormalTemp[facesNormalTemp[i].z]);

		tempFace.x = 3 * i;
		tempFace.y = 3 * i + 1;
		tempFace.z = 3 * i + 2;

		faces.push_back(tempFace);
	}

	return;
}

void Geometry::setCamera(Camera *camera) 
{ 
	Geometry::camera = camera;
}

/********************************************************************/

Sphere::Sphere(std::string objFilename, GLuint programID, Camera* camera)
{ 
	program = programID;
	glUseProgram(program);
	model = glm::mat4(1);
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projectionLoc = glGetUniformLocation(program, "projection");
	cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	typeLoc = glGetUniformLocation(program, "type");

	Sphere::camera = camera;

	readObjFile(objFilename);

	//Find max & min

	maxCoordinate = points[0];
	minCoordinate = points[0];
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].x < minCoordinate.x)
		{
			minCoordinate.x = points[i].x;
		}
		else if (points[i].x > maxCoordinate.x)
		{
			maxCoordinate.x = points[i].x;
		}

		if (points[i].y < minCoordinate.y)
		{
			minCoordinate.y = points[i].y;
		}
		else if (points[i].y > maxCoordinate.y)
		{
			maxCoordinate.y = points[i].y;
		}

		if (points[i].z < minCoordinate.z)
		{
			minCoordinate.z = points[i].z;
		}
		else if (points[i].z > maxCoordinate.z)
		{
			maxCoordinate.z = points[i].z;
		}
	}

	modelCenter = 0.5f * (maxCoordinate + minCoordinate);
	//std::cout << "modelCenter" << modelCenter.x << modelCenter.y << modelCenter.z << std::endl;

	float tempRadius = 0;
	for (int i = 0; i < points.size(); i++)
	{
		//tempRadius = glm::length(points[i] - modelCenter); // calculate distance
		//radius = (tempRadius > radius) ? tempRadius : radius;//find min
		points[i] = points[i] - modelCenter;
	}

	// Set the color. 
	//color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO), vertex buffer objects (VBO) and EBO.
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);
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

	//a. Bind to the EBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//b. Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Sphere::readObjFile(std::string fileName)
{

	std::ifstream objFile("PointObj/" + fileName); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				// Process the point. For example, you can save it to a.
				points.push_back(point);

				//coloring
				//ss >> point.x >> point.y >> point.z;
				//pointsColor.push_back(point);

			}
			else if (label == "vn")
			{
				glm::vec3 pointNormal;
				ss >> pointNormal.x >> pointNormal.y >> pointNormal.z;

				pointsNormal.push_back(pointNormal);
			}
			else if (label == "f")
			{
				glm::ivec3 face;
				glm::ivec3 faceNormal;
				char tempChar;
				unsigned tempNumber;

				ss >> face.x >> tempChar >> tempChar >> faceNormal.x >>
					face.y >> tempChar >> tempChar >> faceNormal.y >>
					face.z >> tempChar >> tempChar >> faceNormal.z;

				//start from 0	
				face.x--;
				face.y--;
				face.z--;

				faces.push_back(face);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	objFile.close();

	return;
}

void Sphere::draw(glm::mat4 curModel)
{
	if (sphereFlag == 0)
	{
		return;
	}

	glUseProgram(program);
	setMatrix(curModel);

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Sphere::setSphereFlag(GLuint flag)
{ 
	if (flag == 1)
	{
		sphereFlag = (sphereFlag + 1) % 2;
	}
}

void Sphere::setMatrix(glm::mat4 curModel)
{
	model = curModel;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(camera->eye));
	glUniform1i(typeLoc, type);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
}