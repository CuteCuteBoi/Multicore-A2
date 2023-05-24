__kernel void Decipher(int shift, __global char *cipVector, __global char *decVector)
{
	int gid = get_global_id(0);

	if ((cipVector[gid] >= 'A') && (cipVector[gid] <= 'Z'))
		{
			cipVector[gid] = cipVector[gid] - shift;

			if (cipVector[gid] > 'Z')
			{
				cipVector[gid] = cipVector[gid] - 26;
			}
			else if (cipVector[gid] < 'A')
			{
				cipVector[gid] = cipVector[gid] + 26;
			}

			decVector[gid] = cipVector[gid];
		}
		else
		{
			decVector[gid] = cipVector[gid];
		}

}