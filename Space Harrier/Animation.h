#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	bool loop = true;
	bool randFrame = false;
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
		if (first && randFrame) {
			current_frame = RAND() % frames.size();
			first = false;
		}

		int last_frame = frames.size();

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
			timer += speed;
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
	}
};