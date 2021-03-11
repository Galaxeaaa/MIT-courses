#include "MarchingInfo.h"

void MarchingInfo::nextCell()
{
	if (t_next_x > t_next_y && t_next_x > t_next_z)
	{
		i += sign_x;
		tmin = t_next_x;
		t_next_x += d_tx;
	}
	else if (t_next_y > t_next_z)
	{
		j += sign_y;
		tmin = t_next_y;
		t_next_y += d_ty;
	}
	else
	{
		k += sign_z;
		tmin = t_next_z;
		t_next_z += d_tz;
	}
}