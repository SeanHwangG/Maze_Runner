#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */


	readObjFile(objFilename);
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);


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
	float winLength = 480;//the length of the shorter side of the window
	if (radius > 0)
	{
		float factor = 50;
		float scalePara = winLength / (radius * factor);
		scaleModel(scalePara);
	}
	

	
	// Set the color. 
	color = glm::vec3(1, 0, 0);

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

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, vbos);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	/*
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, points.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
	*/

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0); // mode, count, type, indices
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
}

void PointCloud::rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos)
{
	GLfloat degree;
	glm::vec3 normalVec = glm::cross(prevCursorPos, curCursorPos);
	if (glm::length(normalVec) > 0)
	{
		glm::mat4 rotateMatrix(1);
		normalVec = glm::normalize(normalVec);
		GLfloat cosdeg = glm::dot(prevCursorPos, curCursorPos);
		degree = glm::acos(cosdeg);
		//std::cout << "Cos = " << cosdeg << std::endl;
		//std::cout << "Degree = " << degree << std::endl;
		rotateMatrix = glm::rotate(rotateMatrix, degree, normalVec);
		model = rotateMatrix * model;
		modelCenter = rotateMatrix * glm::vec4(modelCenter,1.0);
	}
	
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	//PointCloud::pointSize = size; // refresh point size
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	//model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

/* Read obj files, 9/29 */
void PointCloud::readObjFile(std::string fileName)
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
				ss >> point.x >> point.y >> point.z;
				pointsColor.push_back(point);

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
				char tempChar;
				unsigned tempNumber;
				ss >> face.x >> tempChar >> tempChar >> tempNumber >> face.y >> tempChar >> tempChar >> tempNumber >> face.z;
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

void PointCloud::scaleModel(GLfloat parameter)
{
	model = glm::scale(model, glm::vec3(parameter, parameter, parameter));
}

void PointCloud::translateModel(glm::vec3 path)
{
	modelCenter = modelCenter + path;
	model = glm::translate(path) * model;
}
