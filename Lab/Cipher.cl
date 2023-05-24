__kernel void Cipher(int shift, __global char *oriVector, __global char *cipVector)
{
	int gid = get_global_id(0);

	if ((oriVector[gid] >= 'a') && (oriVector[gid] <= 'z'))
	{
			oriVector[gid] = oriVector[gid] - 32 + shift;

			if (oriVector[gid] > 'Z')
			{
				oriVector[gid] = oriVector[gid] - 26;
			}
			else if (oriVector[gid] < 'A')
			{
				oriVector[gid] = oriVector[gid] + 26;
			}

			cipVector[gid] = oriVector[gid];
	}
	else if ((oriVector[gid] >= 'A') && (oriVector[gid] <= 'Z'))
	{
			oriVector[gid] = oriVector[gid] + shift;

			if (oriVector[gid] > 'Z')
			{
				oriVector[gid] = oriVector[gid] - 26;
			}
			else if (oriVector[gid] < 'A')
			{
				oriVector[gid] = oriVector[gid] + 26;
			}

			cipVector[gid] = oriVector[gid];
	}
	else
	{
		cipVector[gid] = oriVector[gid];
	}
}