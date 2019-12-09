#include "bezierCurve.h"

glm::mat4 BeizerCurve::B_bez = glm::mat4(glm::vec4(-1,3,-3,1),
							glm::vec4(3,-6,3,0),
							glm::vec4(-3,3,0,0),
							glm::vec4(1,0,0,0));
GLuint BeizerCurve::samplingTimes = 150;
Geometry* BeizerCurve::mark;

BeizerCurve::BeizerCurve(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	GLuint programID, Camera* camera)
{
	G_bez = glm::mat4(glm::vec4(point0, 1),
		glm::vec4(point1, 1),
		glm::vec4(point2, 1),
		glm::vec4(point3, 1));

	program = programID;
	BeizerCurve::camera = camera;

	glUseProgram(program);

	model = glm::mat4(1);
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projectionLoc = glGetUniformLocation(program, "projection");
	cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	typeLoc = glGetUniformLocation(program, "type");

	glGenVertexArrays(3, vaos);
	glGenBuffers(3, vbos);
}

BeizerCurve::~BeizerCurve()
{

}

glm::vec3 BeizerCurve::getPosition(GLfloat t)
{
	glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), pow(t, 1), 1);
	return glm::vec3(G_bez * B_bez * T);
}

glm::vec3 BeizerCurve::getTangent(GLfloat t)
{
	glm::vec4 T = glm::vec4(pow(t, 2), pow(t, 1), 1, 0);
	return glm::vec3(G_bez * B_bez * T);
}

void BeizerCurve::sampling(glm::mat4 C)
{
	line.clear();
	for (int i = 0; i <= samplingTimes; i++)
	{
		line.push_back(getPosition((GLfloat)i/ (GLfloat)samplingTimes));
	}

	//Sampling points of curve
	// Bind to the VAO.
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * line.size(), line.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	//glUniform1i(typeLoc, 3);
	//handel 1
	handel1.clear();
	handel1.push_back(G_bez[0]);
	handel1.push_back(G_bez[1]);
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * handel1.size(), handel1.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//handel 2
	handel2.clear();
	handel2.push_back(G_bez[2]);
	handel2.push_back(G_bez[3]);
	glBindVertexArray(vaos[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * handel2.size(), handel2.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void BeizerCurve::draw(glm::mat4 C)
{
	glUseProgram(program);
	setMatrix(model);

	glUniform1i(typeLoc, 2);
	glBindVertexArray(vaos[0]);
	glDrawArrays(GL_LINE_STRIP, 0, line.size()); // mode, count, type, indices

	glUniform1i(typeLoc, 3);
	glBindVertexArray(vaos[1]);
	glDrawArrays(GL_LINE_STRIP, 0, handel1.size()); // mode, count, type, indices

	glBindVertexArray(vaos[2]);
	glDrawArrays(GL_LINE_STRIP, 0, handel2.size()); // mode, count, type, indices

	// Unbind from the VAO.
	glBindVertexArray(0);

	//draw mark
	glUniform1i(typeLoc, markType[0]);
	mark->draw(glm::translate(glm::vec3(G_bez[0])));

	glUniform1i(typeLoc, markType[1]);
	mark->draw(glm::translate(glm::vec3(G_bez[1])));

	glUniform1i(typeLoc, markType[2]);
	mark->draw(glm::translate(glm::vec3(G_bez[2])));
}

void BeizerCurve::update()
{

}

void BeizerCurve::setMatrix(glm::mat4 curModel)
{
	model = curModel;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(camera->eye));
}


/**********Track********/

BeizerTrack::BeizerTrack(GLuint programID, Camera* camera)
{
	BeizerTrack::camera = camera;
	BeizerCurve *curve;

	float factor = 1.5;

	Geometry* mark = new Geometry("eyeball_s.obj", programID, camera);

	//curve 1
	curve = new BeizerCurve(glm::vec3(15,10,0), glm::vec3(15,10,-2),
							glm::vec3(0,0,0), glm::vec3(0,0,0),
							programID, camera);
	curve->setMark(mark);
	curve->markType[0] = 5;
	track.push_back(curve);

	//curve 2
	curve = new BeizerCurve(glm::vec3(10.61, 0, -10.61), glm::vec3(9.196, 2, -12.02),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 3
	curve = new BeizerCurve(glm::vec3(0, -5, -15), glm::vec3(-2, -5, -15),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 4
	curve = new BeizerCurve(glm::vec3(-10.61, 0, -10.61), glm::vec3(-12.02, 0, -9.196),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 5
	curve = new BeizerCurve(glm::vec3(-15, 8, 0), glm::vec3(-15, 10, 2),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 6
	curve = new BeizerCurve(glm::vec3(-10.61, 2, 10.61), glm::vec3(-9.196, 0, 12.02),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 7
	curve = new BeizerCurve(glm::vec3(0, 0, 15), glm::vec3(2, -8, 15),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	//curve 8
	curve = new BeizerCurve(glm::vec3(10.61, 3, 10.61), glm::vec3(12.02, 5, 9.196),
							glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
							programID, camera);
	track.push_back(curve);

	for (int i = 0; i < numofCurves; i++)
	{
		continuity((i + 1) % numofCurves, i);
		track[i]->sampling(glm::mat4(1));
	}
}

BeizerTrack::~BeizerTrack()
{

}


void BeizerTrack::continuity(GLuint curveIndex1, GLuint curveIndex2)
{
	GLint diff = (GLint)curveIndex1 - (GLint)curveIndex2;
	if (diff == 1 || diff == -(numofCurves - 1))
	{
		track[curveIndex2]->G_bez[3] = track[curveIndex1]->G_bez[0];
		track[curveIndex2]->G_bez[2] = 2.0f * track[curveIndex1]->G_bez[0] - track[curveIndex1]->G_bez[1];
	}
	else if (diff == -1 || diff == numofCurves - 1)
	{
		track[curveIndex2]->G_bez[0] = track[curveIndex1]->G_bez[3];
		track[curveIndex2]->G_bez[1] = 2.0f * track[curveIndex1]->G_bez[3] - track[curveIndex1]->G_bez[2];
	}
}

void BeizerTrack::draw(glm::mat4 C)
{
	for (int i = 0; i < numofCurves; i++)
	{
		track[i]->draw(glm::mat4(1));
	}
}

void BeizerTrack::update()
{
	/*
	//method 1, nearest
	if (moveFlag == 1)
	{
		GLfloat distance = obj.velocity * glfwGetTime();
		obj.sumDis += distance;
		GLfloat length = glm::length(obj.position - track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes]);
		int next;
		while (obj.sumDis > length)
		{
			obj.sumDis -= length;
			
			next = (obj.nextPoint + numofCurves * samplingTimes + obj.direction) % (numofCurves * samplingTimes);
			obj.position = track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes];
			if (riderFlag == 1)
			{
				camera->eye = obj.position;
				camera->center = track[next / samplingTimes]->line[next % samplingTimes];
			}
			obj.nextPoint = next;
			length = glm::length(obj.position - track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes]);
			obj.obj->setModelMatrix(glm::translate(obj.position));

			
			
		}

		if (physicsFlag == 1)
		{
			obj.velocity = sqrt(2 * (obj.energy / obj.mass - obj.g * obj.position.y));
			if (obj.velocity < 2)
			{
				obj.velocity = 0;
				obj.sumDis = 0;
			}

			if (obj.velocity < 0.001)
			{
				if (track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes].y > obj.position.y)
				{
					obj.direction *= -1;
					obj.velocity = 2.0f;
					obj.energy = 0.5 * obj.mass * pow(obj.velocity, 2) + obj.mass * obj.g * obj.position.y;
					obj.nextPoint = (obj.nextPoint + numofCurves * samplingTimes + obj.direction) % (numofCurves * samplingTimes);
				}
			}
		}
		
	}
	
	glfwSetTime(0);
	*/

	//method 2, bilinear
	if (moveFlag == 1)
	{
		GLfloat distance = obj.velocity * glfwGetTime();
		GLfloat length = glm::length(obj.position - track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes]);
		int next;
		while (distance > length)
		{
			distance -= length;
			next = (obj.nextPoint + numofCurves * samplingTimes + obj.direction) % (numofCurves * samplingTimes);

			obj.position = track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes];

			obj.nextPoint = next;
			length = glm::length(obj.position - track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes]);

			if (riderFlag == 1)
			{
				camera->eye = obj.position;
				camera->center = track[next / samplingTimes]->line[next % samplingTimes];
			}
		}
		if (physicsFlag == 1)
		{
			obj.velocity = sqrt(2 * (obj.energy / obj.mass - obj.g * obj.position.y));
			if (obj.velocity < 1.0)
			{
				obj.velocity = 0;
			}
			if (obj.velocity < 0.00001)
			{
				GLint prev = (obj.nextPoint + numofCurves * samplingTimes - obj.direction) % (numofCurves * samplingTimes);
				obj.position = obj.sumDis * track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes] +
					(1 - obj.sumDis) * track[prev / samplingTimes]->line[prev % samplingTimes];
				obj.energy = 0.5 * obj.mass + obj.mass * obj.g * obj.position.y;

				if (track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes].y > obj.position.y)
				{
					obj.direction *= -1;
					obj.velocity = 1.0f;
					obj.energy = 0.5 * obj.mass * pow(obj.velocity, 2) + obj.mass * obj.g * obj.position.y;
					obj.nextPoint = (obj.nextPoint + numofCurves * samplingTimes + obj.direction) % (numofCurves * samplingTimes);
				}
			}
		}

		glm::vec3 dirVector = track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes] - obj.position;
		if (distance > 0.0000001)
		{
			obj.sumDis = distance / length;
		}
		obj.position = obj.sumDis * dirVector + obj.position;
		obj.obj->setModelMatrix(glm::translate(obj.position));
	}
	else
	{
		GLint prev = (obj.nextPoint + numofCurves * samplingTimes - obj.direction) % (numofCurves * samplingTimes);
		obj.position = obj.sumDis * track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes] +
			(1 - obj.sumDis) * track[prev / samplingTimes]->line[prev % samplingTimes];
		obj.obj->setModelMatrix(glm::translate(obj.position));

		if (riderFlag == 1)
		{
			camera->eye = obj.position;
			camera->center = track[obj.nextPoint / samplingTimes]->line[obj.nextPoint % samplingTimes];
		}
	}
	
	glfwSetTime(0);
	
}

void BeizerTrack::selectPoint(int movement)
{
	track[curControlPoint / 3]->markType[curControlPoint % 3] = 4;
	curControlPoint = (curControlPoint + 3 * numofCurves + movement) % (3 * numofCurves);
	track[curControlPoint / 3]->markType[curControlPoint % 3] = 5;
}

void BeizerTrack::changePosition(int axis, int direction)
{
	float diff = (direction > 0) ? 0.1 : -0.1;
	int curCurve = curControlPoint / 3;
	int pointIndex = curControlPoint % 3;
	int nextCurve;

	switch (pointIndex)
	{
	case 0:
	case 1:
		nextCurve = (curCurve + numofCurves - 1) % numofCurves;
		break;
	case 2:
		nextCurve = (curCurve + 1) % numofCurves;
		break;
	}

	if (axis == 0)
	{
		track[curCurve]->G_bez[pointIndex].x += diff;
	}
	else if (axis == 1)
	{
		track[curCurve]->G_bez[pointIndex].y += diff;
	}
	else if (axis == 2)
	{
		track[curCurve]->G_bez[pointIndex].z += diff;
	}

	continuity(curCurve, nextCurve);
	track[curCurve]->sampling(glm::mat4(1));
	track[nextCurve]->sampling(glm::mat4(1));
}

void BeizerTrack::setObject(Transform* object)
{
	obj.obj = object;
	obj.position = track[0]->line[0];
	obj.energy = 0.5 * obj.mass * pow(obj.velocity, 2) + obj.mass * obj.g * obj.position.y;
	obj.obj->translateModel(obj.position);
	glfwSetTime(0);
}

void BeizerTrack::riderViewEnable()
{ 
	riderFlag = (riderFlag + 1) % 2; 
	if (riderFlag == 1)
	{
		tempCenter = camera->center;
		tempEye = camera->eye;
		tempUp = camera->up;
	}
	else
	{
		camera->center = tempCenter;
		camera->eye = tempEye;
		camera->up = tempUp;
	}
}

void BeizerTrack::physicsSimEnable()
{
	physicsFlag = (physicsFlag + 1) % 2;
	if (physicsFlag == 1)
	{
		tempVelocity = obj.velocity * obj.direction;
		obj.energy = 0.5 * obj.mass * pow(obj.velocity, 2) + obj.mass * obj.g * obj.position.y;
	}
	else
	{
		obj.direction = (tempVelocity > 0) ? 1 : -1;
		obj.velocity = obj.direction * tempVelocity;
	}
}

void BeizerTrack::moveEnable()
{ 
	moveFlag = (moveFlag + 1) % 2; 
	if (moveFlag == 1)
	{
		obj.energy = 0.5 * obj.mass * pow(obj.velocity, 2) + obj.mass * obj.g * obj.position.y;
	}
}
