#pragma once
#include <string>
#include <vector>

namespace TextGame
{
	struct Position
	{
		int X;
		int Y;

		bool operator==(const Position& otherPosition) const
		{
			return X == otherPosition.X && Y == otherPosition.Y;
		}

		bool operator!=(const Position& otherposition) const
		{
			return !(*this == otherposition);
		}
	};

	enum ItemType
	{
		ItemType_KEYCARD,
		ItemType_ACBOOK,
		ItemType_ROUTER,
		ItemType_STUDENTID

	};

	struct InventoryItem
	{
		ItemType Type;
		Position ItemPosition;
	};


	struct PlayerState
	{
		bool WantsToExit;
		bool WantsDescription;
		bool WantsInventoryListed;
		bool WantsToGet;
		bool WantsToUnpack;

		Position CurrentPosition;
		Position DesiredPosition;

		
		int CurrentRoomIndex;
		std::vector<InventoryItem> Inventory;
		
	};

	struct LockedDoorData
	{
		Position DoorPosition;
	};

	struct RoomData
	{
		std::string Name;
		std::string Description;

		int RoomMapWidth;

		Position RoomPosition;
		std::string RoomMap;
		std::vector<InventoryItem> Inventory;
		std::vector<LockedDoorData> LockedDoors;
	};

	struct WorldState
	{
		std::vector<RoomData> Rooms;
	};

	void InitializeGame(PlayerState& playerState, WorldState& worldState);
	void GetInput(PlayerState& playerState, const WorldState& worldState);
	void RenderGame(const PlayerState& playerState, const WorldState& worldState);
	void UpdateGame(PlayerState& playerState, WorldState& worldState);
	void CleanupGame(PlayerState& playerState, WorldState& worldState);

	int PositionToIndex(const Position& position, int roomwidth);
	bool IsSpaceOpenForMovement(const Position& position, const RoomData& currRoom);
	bool IsSpaceOutsideMap(const Position& position, const RoomData& currRoom);

	void TryToUnlockDoor(const Position& position,  RoomData& currRoom, PlayerState& playerState);

	std::string GetItemName(ItemType itemType);
	char GetItemIcon(ItemType itemType);
}