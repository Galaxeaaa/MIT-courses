#pragma once

class MarchingInfo
{
public:	
	void nextCell();

public:
	float tmin;
	int i, j, k; // current indices for the current grid cell
	float t_next_x, t_next_y, t_next_z;
	float d_tx, d_ty, d_tz;
	int sign_x, sign_y, sign_z;
	int face; // 0:x 1:y 2:z
};
