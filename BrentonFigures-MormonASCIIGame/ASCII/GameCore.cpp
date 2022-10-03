#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>

namespace TextGame
{
	RoomData CreateRoom(const std::string& inName, const std::string& inDescription)
	{
		RoomData room = {};
		room.Name = inName;
		room.Description = inDescription;

		return room;
	}

	void InitializeGame(PlayerState& playerState, WorldState& worldState)
	{
		printf("Welcome to IGC Housing, it's your first time at your dormitory go and move in.\n");
		printf("Move In Day by Brenton \n\n");
		printf("It's your first day at the dormitory.\n Find everything you need in order to move in.\n");

		playerState.WantsDescription = true;
		/////The game will not auto pick up because the player initializes with wanting to get an item but that does not loop through for each and every turn for the player
		//AKA the player starts wanting to get an item ,but not for every turn for player input. 
		//// playerState.WantsToGet = true;
		playerState.CurrentRoomIndex = { 0 };
		playerState.CurrentPosition.X = 3;
		playerState.CurrentPosition.Y = 2;

		worldState.Rooms.clear();

		// 0 Elevator
		{
			RoomData room = CreateRoom("Elevator", "A small cramped elevator, looks like it's headed to your dorm's floor");
			room.RoomMapWidth = 8;
			room.RoomPosition = { 0, 0 };

			room.RoomMap = 
            "##...###"
			"#......#"
			"#......."
			"........"
			"#......#"
			"####.###";

			
			room.Inventory.push_back({ ItemType_ACBOOK, {4,3} });
		
			worldState.Rooms.push_back(room);

		}
		// 1 Housing Office
		{
			RoomData room = CreateRoom("Housing Office", "IGC's housing office, Everything you need to move in can be found here\n");

			room.RoomMapWidth = 8;
			room.RoomPosition = { 0, 1 };

			room.RoomMap =
				"####.###"
				"##.....#"
				"#......#"
				"#......#"
				"###..###"
				"########";

			room.Inventory.push_back({ ItemType_KEYCARD, {1,2} });

		

			worldState.Rooms.push_back(room);
		}
		// 2 Player's Room
		{
			RoomData room = CreateRoom("Your Room", "Finally! \n Your room is smaller than you thought it'd be.\n Time to start unpacking!!!");
			room.RoomMapWidth = 8;
			room.RoomPosition = { 2, 0 };

			room.RoomMap =
				"########"
				"#......#"
				"#......#"
				".......#"
				"#......#"
				"########";

			room.Inventory.push_back({ ItemType_STUDENTID, {6,4} });

			worldState.Rooms.push_back(room);
		}
		// 3 Friend's Room Tidy
		{
			RoomData room = CreateRoom("Jia's Room", "There are math equations sprawled out all over the walls\n From what you can tell she's an engineering student. There are car posters all over her walls.");
			room.RoomMapWidth = 8;
			room.RoomPosition = { 1, 0 };


			room.RoomMap =
				"########"
				"#......#"
				".......#"
				"........"
				"#......#"
				"########";

			room.Inventory.push_back({ ItemType_ROUTER, {2,1} });
			room.LockedDoors.push_back({ {7,3} });

			worldState.Rooms.push_back(room);
		}
		// 4 Friend's Room Messy
		{
			RoomData room = CreateRoom("Yumi's Room", "Yeesh \n It's only the first day and there's junk everywhere. Who knows what you can find in here.");
			room.RoomMapWidth = 8;
			room.RoomPosition = { -1, 0 };

			room.RoomMap =
				"########"
				"#...##.#"
				"##.....#"
				"#......."
				"#....#.#"
				"##..####";

			worldState.Rooms.push_back(room);
		}
	}

	void GetInput(PlayerState& playerState, const WorldState& worldState)
	{
		//This allows the player to quit the game.
		playerState.WantsToExit = false;
		//The player wants to see the room layout alongside the room description. 
		playerState.WantsDescription = true;

		playerState.WantsInventoryListed = false;
		//This enables auto pick up of items
		playerState.WantsToGet = true;
		playerState.WantsToUnpack = false;

		playerState.DesiredPosition = playerState.CurrentPosition;
	
		printf("What do you do?\n");
		printf("> ");
		TextAdventureCommand command = ParseAdventureCommand();

	
		if (command.Verb == "quit" || command.Verb == "q")
		{
			playerState.WantsToExit = true;
		}
		else if (command.Verb == "north" || command.Verb == "n")
		{
			
			playerState.DesiredPosition.Y = playerState.CurrentPosition.Y - 1;
			
			
		}
		else if (command.Verb == "south" || command.Verb == "s")
		{
		
			playerState.DesiredPosition.Y = playerState.CurrentPosition.Y + 1;
	//		playerState.WantsInventoryListed = true;
		
		
		}
		else if (command.Verb == "west" || command.Verb == "w")
		{
			
			playerState.DesiredPosition.X = playerState.CurrentPosition.X - 1;
		
			
		}
		else if (command.Verb == "east" || command.Verb == "e")
		{
			playerState.WantsInventoryListed = true;
			playerState.DesiredPosition.X = playerState.CurrentPosition.X + 1;
	//		playerState.WantsInventoryListed = true;
			
		}
		else if (command.Verb == "look")
		{
			playerState.WantsDescription = true;
		}
		else if (command.Verb == "inventory" || command.Verb == "i")
		{
			playerState.WantsInventoryListed = true;

		}
		else if (command.Verb == "get" || command.Verb == "g")
		{
			playerState.WantsToGet = true;
			playerState.WantsInventoryListed = true;
		}
		else if (command.Verb == "unpack" || command.Verb == "u")
		{
			playerState.WantsToUnpack = true;
		}
		else if (command.Verb == "help")
		{
			printf("Command List: look, quit, inventory, get, unpack, north, south, west, east\n");
			printf("Key:\n");
			printf("  @ - Player\n");
			printf("  i - Item\n");
			printf("  . - Floor\n");
			printf("  # - Wall\n");
			printf("  D - Door (Locked)\n");
			
		}
		else
		{
			printf("I don't understand\n");
		}
		printf("\n");
	}

	void RenderGame(const PlayerState& playerState, const WorldState& worldState)
	{
		if (playerState.WantsDescription)
		{
			const RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
			printf("================================================\n");
			printf("LOCATION: %s\n", currRoom.Name.c_str());
			printf("%s\n\n", currRoom.Description.c_str());
			
			std::string renderedMapString = "";
			unsigned int currentSpace = 0;


			//printf("\n");
			
			while (currentSpace < currRoom.RoomMap.size())
			{
				char characterToDisplay = currRoom.RoomMap[currentSpace];

				//If an item is here replace the characterToDisplay with an i
				for (unsigned int i = 0; i < currRoom.Inventory.size(); ++i)
				{
					const InventoryItem& currItem = currRoom.Inventory[i];
					if (PositionToIndex(currItem.ItemPosition, currRoom.RoomMapWidth) == currentSpace)
					{
						characterToDisplay = GetItemIcon(currItem.Type);
					}
				}

				for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
				{
					const LockedDoorData& currDoor = currRoom.LockedDoors[i];
					if (PositionToIndex(currDoor.DoorPosition, currRoom.RoomMapWidth) == currentSpace)
					{
						characterToDisplay ='D';
					}
				}

				//Rendering a door as D
				for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
				{
					const LockedDoorData& currDoor = currRoom.LockedDoors[i];
					if (PositionToIndex(currDoor.DoorPosition, currRoom.RoomMapWidth) == currentSpace)
					{
						characterToDisplay = 'D';
					}
				}
				//If the player is here replace characterToDisplay with an @
				if (PositionToIndex(playerState.CurrentPosition, currRoom.RoomMapWidth) == currentSpace)
				{
					characterToDisplay = '@';
				}
			
				renderedMapString += characterToDisplay;

				if (currentSpace % currRoom.RoomMapWidth == (currRoom.RoomMapWidth - 1))
				{
					renderedMapString += '\n';
				}

				currentSpace++;
			}
			printf("%s\n", renderedMapString.c_str());
		}
		else if (playerState.WantsInventoryListed)
		{
			printf("================================================\n");
			printf("INVENTORY:\n");

			if (playerState.Inventory.size() == 0)
			{
				printf("You have nothing in your inventory");
			}
			else
			{
				for (unsigned int i = 0; i < playerState.Inventory.size(); i++)
				{
					printf("%s\n", GetItemName(playerState.Inventory[i].Type).c_str());
				}
			}
			printf("\n");
		}
	}

	void UpdateGame(PlayerState& playerState, WorldState& worldState)
	{
		RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		if (playerState.WantsToGet)
		{
		for (unsigned int i = 0; i < currRoom.Inventory.size(); ++i)
		{
			const InventoryItem& currItem = currRoom.Inventory[i];
			if (currItem.ItemPosition == playerState.CurrentPosition)
			{
				printf("I got a '%s'\n\n\n", GetItemName(currItem.Type).c_str());

				playerState.Inventory.push_back(currItem);

				currRoom.Inventory.erase(currRoom.Inventory.begin() + i);

				break;
			}
		}
		}
		if (playerState.DesiredPosition != playerState.CurrentPosition) 
		{
			TryToUnlockDoor(playerState.DesiredPosition, currRoom, playerState);


			if (IsSpaceOutsideMap(playerState.DesiredPosition, currRoom))
			{
				Position desriedRoomPosition = currRoom.RoomPosition;
				int playerPositionType = 0;

				if (playerState.DesiredPosition.X < 0)
				{
					desriedRoomPosition.X--;
					playerPositionType = 1;
				}
				else if (playerState.DesiredPosition.X >= currRoom.RoomMapWidth)
				{
					desriedRoomPosition.X++;
					playerPositionType = 2;
				}
				else if (playerState.DesiredPosition.Y < 0)
				{
					desriedRoomPosition.Y--;
					playerPositionType = 3;
				}
				else if (playerState.DesiredPosition.Y >= ((int)currRoom.RoomMap.size() / currRoom.RoomMapWidth))
				{
					desriedRoomPosition.Y++;
					playerPositionType = 4;
				}
				bool foundNewRoom = false;

				for (unsigned int i = 0; i < worldState.Rooms.size(); ++i)
				{
					if (worldState.Rooms[i].RoomPosition == desriedRoomPosition)
					{
						playerState.CurrentRoomIndex = i;
						playerState.WantsDescription = true;
						/////////////////////////////
						playerState.WantsToGet = true;
						
						switch (playerPositionType)
						{
						case 1:
							playerState.CurrentPosition.X = worldState.Rooms[playerState.CurrentRoomIndex].RoomMapWidth - 1;
							break;
						case 2:
							playerState.CurrentPosition.X = 0;
							break;
						case 3:
							playerState.CurrentPosition.Y = (worldState.Rooms[playerState.CurrentRoomIndex].RoomMap.size() / worldState.Rooms[playerState.CurrentRoomIndex].RoomMapWidth) - 1;
							break;
						case 4:
							playerState.CurrentPosition.Y = 0;
							break;
						}

						foundNewRoom = true;
					}
				}
				if (!foundNewRoom)
				{
					printf("I thought the world wasn't flat but it certaintly is out there, you can't go any further.\n");
				}
			
			}

			else if (IsSpaceOpenForMovement(playerState.DesiredPosition, currRoom))
			{
				playerState.CurrentPosition = playerState.DesiredPosition;
			}
			else
			{
				printf("Looks like you can't go there\n\n\n");
			}
		}		
		/*else if (playerState.WantsToGet)
		{
			for (unsigned int i = 0; i < currRoom.Inventory.size(); ++i)
			{
				const InventoryItem& currItem = currRoom.Inventory[i];
				if (currItem.ItemPosition == playerState.CurrentPosition)
				{
					printf("I got a '%s'\n", GetItemName(currItem.Type).c_str());

					playerState.Inventory.push_back(currItem);

					currRoom.Inventory.erase(currRoom.Inventory.begin() + i);

					break;
				}
			}
		}*/
		else if (playerState.WantsToUnpack)
		{
			int numberOfRouters = 0;
			int numberOfKeys = 0;
			int numberOfAC = 0;
			int numberOfStud = 0;

			for (unsigned int i = 0; i < playerState.Inventory.size(); ++i)
			{
				const InventoryItem& currItem = playerState.Inventory[i];
				if (currItem.Type == ItemType_ROUTER)
				{
					numberOfRouters++;
				}
				else if (currItem.Type == ItemType_STUDENTID)
				{
					numberOfStud++;
				}
				else if (currItem.Type == ItemType_KEYCARD)
				{
					numberOfKeys++;
				}
				else if (currItem.Type == ItemType_ACBOOK)
				{
					numberOfAC++;
				}

			}
			if (numberOfRouters >=1 && numberOfStud >=1 && numberOfAC >=1)
			{
				printf("Looks like you got everything unpacked\nEven that tacky anime poster.\nAnyways, your new semester is just beginning,\n\nCheers to your new journey");
				playerState.WantsToExit = true;
			
			}
			else
			{
				printf("Not Yet!\n You need all the items in order to fully move in.\n Keep searching!!!");
			}
		
		}
	}

	void CleanupGame(PlayerState& playerState, WorldState& worldState)
	{
		worldState.Rooms.clear();
	}
	int PositionToIndex(const Position& position, int roomwidth)
	{
		return position.Y * roomwidth + position.X;
	}
	bool IsSpaceOpenForMovement(const Position& position, const RoomData& currRoom)
	{
		for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
		{
			const LockedDoorData& currDoor = currRoom.LockedDoors[i];
			if (currDoor.DoorPosition == position)
			{
				return false;
			}
		}
		int spaceIndex = PositionToIndex(position, currRoom.RoomMapWidth);
		return currRoom.RoomMap[spaceIndex] == '.';
	}
	bool IsSpaceOutsideMap(const Position& position, const RoomData& currRoom)
	{
		return position.X < 0 ||
			position.X >= currRoom.RoomMapWidth ||
			position.Y < 0 ||
			position.Y >= ((int)currRoom.RoomMap.size() / currRoom.RoomMapWidth);
	}

	void TryToUnlockDoor(const Position& position,  RoomData& currRoom, PlayerState& playerState)
	{
		for (unsigned int i = 0; i < currRoom.LockedDoors.size(); ++i)
		{
			const LockedDoorData& currDoor = currRoom.LockedDoors[i];
			if (currDoor.DoorPosition == position)
			{
				//Door is found/exists
				for (unsigned int j = 0; j < playerState.Inventory.size(); ++j)
				{
					const InventoryItem& currItem = playerState.Inventory[j];
					if (currItem.Type == ItemType_KEYCARD)
					{
						//Found Room Key
						currRoom.LockedDoors.erase(currRoom.LockedDoors.begin() + i);
						playerState.Inventory.erase(playerState.Inventory.begin() + j);

						return;
					}
				}
			}
		}
	}
	//Invent items
	std::string GetItemName(ItemType itemType)
	{
		switch (itemType)
		{
		case ItemType_ROUTER:
			return "An iptime router, it's surprisingly small. ";
			break;
		case ItemType_ACBOOK:
			return "Instructions for setting up AC";
			break;
		case ItemType_KEYCARD:
			return "Room Key";
				break;
		case ItemType_STUDENTID:
			return "Your official Mason Korea ID";
			break;
		}

		return "UNKNOWN ITEM";
	}
	//How items appear in game
	char GetItemIcon(ItemType itemType)
	{
		switch (itemType)
		{
		case TextGame::ItemType_KEYCARD:
			return 'i'; ////originally k
			break;
		case TextGame::ItemType_ACBOOK:
			return 'i'; ////originally s
			break;
		case TextGame::ItemType_ROUTER:
			return 'i'; ////originally r
			break;
		case TextGame::ItemType_STUDENTID:
			return 'i'; ////originally c
			break;
		}
		return 'i';
	}
}
