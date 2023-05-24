__kernel void decryption(__global char *befVector, __global char *aftVector)
{
	int gid = get_global_id(0);

	switch(befVector[gid])
	{
		case 'C': aftVector[gid] = 'A';
		break;

		case 'I': aftVector[gid] = 'B';
		break;

		case 'S': aftVector[gid] = 'C';
		break;

		case 'Q': aftVector[gid] = 'D';
		break;

		case 'V': aftVector[gid] = 'E';
		break;

		case 'N': aftVector[gid] = 'F';
		break;

		case 'F': aftVector[gid] = 'G';
		break;

		case 'O': aftVector[gid] = 'H';
		break;

		case 'W': aftVector[gid] = 'I';
		break;

		case 'A': aftVector[gid] = 'J';
		break;

		case 'X': aftVector[gid] = 'K';
		break;

		case 'M': aftVector[gid] = 'L';
		break;

		case 'T': aftVector[gid] = 'M';
		break;

		case 'G': aftVector[gid] = 'N';
		break;

		case 'U': aftVector[gid] = 'O';
		break;

		case 'H': aftVector[gid] = 'P';
		break;

		case 'P': aftVector[gid] = 'Q';
		break;

		case 'B': aftVector[gid] = 'R';
		break;

		case 'K': aftVector[gid] = 'S';
		break;

		case 'L': aftVector[gid] = 'T';
		break;

		case 'R': aftVector[gid] = 'U';
		break;

		case 'E': aftVector[gid] = 'V';
		break;

		case 'Y': aftVector[gid] = 'W';
		break;

		case 'D': aftVector[gid] = 'X';
		break;

		case 'Z': aftVector[gid] = 'Y';
		break;

		case 'J': aftVector[gid] = 'Z';
		break;
		
		default:
		aftVector[gid] = befVector[gid];
	}
}