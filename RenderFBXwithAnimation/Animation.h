#pragma once
#include"Struct.h"
class Joint;
class Animation
{
	Joint* joint;
	std::vector<std::vector<KeyFrame>> keyframes;
	float totaltime;
	float framerate;
	float framerate_inv;
	float currtime = 0.0f;
	int totalKeyframes;
public:
	void initializeBinaryAnimation(const char * path, Joint* _joint);
	void sentToJoint(int _key);
	void Interpolate(float delta);

	int GetTotalKeyframes() { return totalKeyframes; };
};

