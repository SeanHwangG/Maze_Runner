#include "node.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{

}

/*
void Frustum::setValue(double FOV, float nearP, float farP,
	float winLength, float winHight,
	glm::vec3 center, glm::vec3 up, glm::vec3 eye)
{
	Frustum::FOV = FOV;
	Frustum::nearP = nearP;
	Frustum::farP = farP;
	Frustum::winLength = winLength;
	Frustum::winHight = winHight;
	Frustum::center = center;
	Frustum::up = up;
	Frustum::eye = eye;

	refresh();
}*/

void Camera::refreshCamera()
{
	aspect = winWidth / winHeight;
	direction = glm::normalize(center - eye);
	right = glm::cross(direction, up);

	initialDir = center - eye;

	//8 points
	nearCenter = eye + direction * nearP;
	nearHalfHight = tan(glm::radians(FOV / 2)) * nearP;
	nearHalfLength = aspect * nearHalfHight;
	nearTL = nearCenter + up * nearHalfHight - right * nearHalfLength;
	nearTR = nearCenter + up * nearHalfHight + right * nearHalfLength;
	nearBL = nearCenter - up * nearHalfHight - right * nearHalfLength;
	nearBR = nearCenter - up * nearHalfHight + right * nearHalfLength;

	farCenter = eye + direction * farP;
	farHalfHight = tan(glm::radians(FOV / 2)) * farP;
	farHalfLength = aspect * farHalfHight;
	farTL = farCenter + up * farHalfHight - right * farHalfLength;
	farTR = farCenter + up * farHalfHight + right * farHalfLength;
	farBL = farCenter - up * farHalfHight - right * farHalfLength;
	farBR = farCenter - up * farHalfHight + right * farHalfLength;

	//normal of 6 sides
	nearPNorm = glm::normalize(glm::cross(nearTR - nearTL, nearBL - nearTL));
	farPNorm = glm::normalize(glm::cross(farBL - farTL, farTR - farTL));
	topPNorm = glm::normalize(glm::cross(farTR - farTL, nearTL - farTL));
	bottomPNorm = glm::normalize(glm::cross(nearBL - farBL, farBR - farBL));
	leftPNorm = glm::normalize(glm::cross(nearTL - farTL, farBL - farTL));
	rightPNorm = glm::normalize(glm::cross(farBR - farTR, nearTR - farTR));
}

bool Camera::inFrustum(glm::vec3 position, float radius)
{
	float tempDist = point2PlaneDist(position, nearTL, nearPNorm);
	if (tempDist < -radius)
	{
		return false;
	}
	else
	{
		tempDist = point2PlaneDist(position, farTL, farPNorm);
		if (tempDist < -radius)
		{
			return false;
		}
		else
		{
			tempDist = point2PlaneDist(position, farTL, topPNorm);
			if (tempDist < -radius)
			{
				return false;
			}
			else
			{
				tempDist = point2PlaneDist(position, farBL, bottomPNorm);
				if (tempDist < -radius)
				{
					return false;
				}
				else
				{
					tempDist = point2PlaneDist(position, farBL, leftPNorm);
					if (tempDist < -radius)
					{
						return false;
					}
					else
					{
						tempDist = point2PlaneDist(position, farBR, rightPNorm);
						if (tempDist < -radius)
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

float Camera::point2PlaneDist(glm::vec3 point, glm::vec3 pointOnPlane, glm::vec3 normal)
{
	return glm::dot((point - pointOnPlane), normal);
}

void Camera::rotateCamera(glm::vec3 prevCursorPos, glm::vec3 curCursorPos)
{
	GLfloat degree;


	glm::vec3 normalVec = glm::cross(prevCursorPos,curCursorPos);
	if (glm::length(normalVec) > 0)
	{
		glm::mat4 rotateMatrix(1);
		normalVec = glm::normalize(normalVec);
		GLfloat cosdeg = glm::dot(prevCursorPos, curCursorPos);
		degree = 0.5 * glm::acos(cosdeg);
		//std::cout << "Cos = " << cosdeg << std::endl;
		//std::cout << "Degree = " << degree << std::endl;
		rotateMatrix = glm::rotate(rotateMatrix, degree, normalVec);

		//Way 1
		model = model * rotateMatrix;
		direction = glm::vec3(model * glm::vec4(initialDir, 0));

		//Way2
		//direction =  rotateMatrix * glm::vec4(direction, 0);

		//direction = rotateMatrix * glm::vec4(initialDir, 0);

		center = direction + eye;
	}
}

glm::mat4 Camera::getViewMatrix() 
{ 
	view = glm::lookAt(eye, center, up);
	return view; 
}

glm::mat4 Camera::getProjectionMatrix() 
{ 
	aspect = winWidth / winHeight;
	projection = glm::perspective((double)glm::radians(FOV), (double)aspect,
		(double)nearP, (double)farP);
	return projection; 
}