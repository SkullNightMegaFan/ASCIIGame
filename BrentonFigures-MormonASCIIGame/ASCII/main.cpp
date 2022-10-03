#include <stdlib.h>
#include "GameCore.h"

using namespace TextGame;

int main()
{
	PlayerState playerState = {};
	WorldState worldState = {};

	InitializeGame(playerState, worldState);

	while (!playerState.WantsToExit)
	{
		RenderGame(playerState, worldState);
		GetInput(playerState, worldState);
		UpdateGame(playerState, worldState);
	}

	CleanupGame(playerState, worldState);

	//Not sure why I originally had system pause. If the player types quit that's good enough for me. 
	// //Probably just didn't know what it did and just left it in there out of fear the exe file would crash. 
	//system("pause");

	return 0;
}