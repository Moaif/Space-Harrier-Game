#pragma once
#include <vector>
#include "Application.h"
#include "ModuleTime.h"

using namespace std;

class Animation
{
public:
	bool loop = true;
	bool randFrame = false;
	bool timeBased = true;
	float speed = 1.0f;
	vector<SDL_Rect> frames;

private:
	int current_frame = 0;
	int loops = 0;
	float timer=0;
	bool first = true;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames),randFrame(anim.randFrame)
	{}

	SDL_Rect& GetCurrentFrame()
	{
		if (first) {
			if (randFrame) {
				current_frame = RAND() % frames.size();
			}
			if (!timeBased) {
				timer = App->time->GetTimeSinceStart() + (1/speed);
			}
			first = false;
		}

		int last_frame = frames.size();

		if (timeBased) {
			if (timer >= 1) {
				if (randFrame) {
					current_frame = RAND() % frames.size();
				}
				else
				{
					++current_frame;
				}
				timer = 0;
			}
			else
			{
				timer += speed*App->time->GetDeltaTime();
			}
		}
		else {
			if (App->time->GetTimeSinceStart() > timer) {
				if (randFrame) {
					current_frame = RAND() % frames.size();
				}
				else
				{
					++current_frame;
				}
				timer = App->time->GetTimeSinceStart() + (1/speed);
			}
		}

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0 : MAX(last_frame - 1, 0);
			loops++;
		} 

		return frames[current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
		loops = 0;
		timer = 0;
		first = true;
	}

	const SDL_Rect& GetCurrentFrameConst() const{
		return frames[current_frame];
	}

	void SetNextFrame() {
		current_frame=(current_frame+1) %frames.size();
	}
};