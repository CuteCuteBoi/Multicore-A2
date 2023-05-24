__kernel void encryption(__global char *befVector, __global char *aftVector)
{
	int gid = get_global_id(0);

	switch(befVector[gid])
	{
		case 'A': aftVector[gid] = 'C';
		break;

		case 'B': aftVector[gid] = 'I';
		break;

		case 'C': aftVector[gid] = 'S';
		break;

		case 'D': aftVector[gid] = 'Q';
		break;

		case 'E': aftVector[gid] = 'V';
		break;

		case 'F': aftVector[gid] = 'N';
		break;

		case 'G': aftVector[gid] = 'F';
		break;

		case 'H': aftVector[gid] = 'O';
		break;

		case 'I': aftVector[gid] = 'W';
		break;

		case 'J': aftVector[gid] = 'A';
		break;

		case 'K': aftVector[gid] = 'X';
		break;

		case 'L': aftVector[gid] = 'M';
		break;

		case 'M': aftVector[gid] = 'T';
		break;

		case 'N': aftVector[gid] = 'G';
		break;

		case 'O': aftVector[gid] = 'U';
		break;

		case 'P': aftVector[gid] = 'H';
		break;

		case 'Q': aftVector[gid] = 'P';
		break;

		case 'R': aftVector[gid] = 'B';
		break;

		case 'S': aftVector[gid] = 'K';
		break;

		case 'T': aftVector[gid] = 'L';
		break;

		case 'U': aftVector[gid] = 'R';
		break;

		case 'V': aftVector[gid] = 'E';
		break;

		case 'W': aftVector[gid] = 'Y';
		break;

		case 'X': aftVector[gid] = 'D';
		break;

		case 'Y': aftVector[gid] = 'Z';
		break;

		case 'Z': aftVector[gid] = 'J';
		break;
		
		default:
		aftVector[gid] = befVector[gid];
	}
}