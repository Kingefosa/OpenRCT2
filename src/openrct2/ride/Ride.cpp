#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include <climits>
#include <cstdlib>

#include "../audio/audio.h"
#include "../audio/AudioMixer.h"
#include "../Cheats.h"
#include "../common.h"
#include "../config/Config.h"
#include "../Context.h"
#include "../Editor.h"
#include "../Game.h"
#include "../Input.h"
#include "../interface/Window.h"
#include "../interface/Window_internal.h"
#include "../localisation/Date.h"
#include "../localisation/Localisation.h"
#include "../management/Finance.h"
#include "../management/Marketing.h"
#include "../management/NewsItem.h"
#include "../network/network.h"
#include "../object/ObjectList.h"
#include "../object/ObjectManager.h"
#include "../OpenRCT2.h"
#include "../paint/VirtualFloor.h"
#include "../peep/Peep.h"
#include "../peep/Staff.h"
#include "../rct1/RCT1.h"
#include "../scenario/Scenario.h"
#include "../util/Util.h"
#include "../windows/Intent.h"
#include "../world/Banner.h"
#include "../world/Climate.h"
#include "../world/Footpath.h"
#include "../world/Map.h"
#include "../world/MapAnimation.h"
#include "../world/Park.h"
#include "../world/Scenery.h"
#include "../world/Sprite.h"
#include "CableLift.h"
#include "MusicList.h"
#include "Ride.h"
#include "RideData.h"
#include "RideGroupManager.h"
#include "Station.h"
#include "Track.h"
#include "TrackData.h"
#include "../core/Math.hpp"
#include "../core/Util.hpp"

#pragma region Ride classification table

/** rct2: 0x0097C3AF */
// clang-format off
const uint8 gRideClassifications[255] = {
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_SHOP_OR_STALL,
    RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_RIDE, RIDE_CLASS_SHOP_OR_STALL,
    RIDE_CLASS_KIOSK_OR_FACILITY, RIDE_CLASS_KIOSK_OR_FACILITY, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_KIOSK_OR_FACILITY, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_KIOSK_OR_FACILITY, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_RIDE, RIDE_CLASS_SHOP_OR_STALL,
    RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_SHOP_OR_STALL, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE,
    RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE, RIDE_CLASS_RIDE
};
// clang-format on

uint8 gTypeToRideEntryIndexMap[TYPE_TO_RIDE_ENTRY_SLOTS];

#pragma endregion

static constexpr const sint32 RideInspectionInterval[] = {
    10, 20, 30, 45, 60, 120, 0, 0
};

Ride gRideList[MAX_RIDES];

rct_ride_measurement gRideMeasurements[MAX_RIDE_MEASUREMENTS];

uint16 gRideCount;
bool gGotoStartPlacementMode = false;
sint32 gRideRemoveTrackPieceCallbackX;
sint32 gRideRemoveTrackPieceCallbackY;
sint32 gRideRemoveTrackPieceCallbackZ;
sint32 gRideRemoveTrackPieceCallbackDirection;
sint32 gRideRemoveTrackPieceCallbackType;

money16 gTotalRideValueForMoney;
uint32 gSamePriceThroughoutParkA;
uint32 gSamePriceThroughoutParkB;

money32 _currentTrackPrice;

uint16 _numCurrentPossibleRideConfigurations;
uint16 _numCurrentPossibleSpecialTrackPieces;

uint16 _currentTrackCurve;
uint16 _currentTrackEndX;
uint16 _currentTrackEndY;
uint8 _rideConstructionState;
uint8 _currentRideIndex;
uint16 _currentTrackBeginX;
uint16 _currentTrackBeginY;
uint16 _currentTrackBeginZ;
uint8 _currentTrackPieceDirection;
uint8 _currentTrackPieceType;
uint8 _currentTrackSelectionFlags;
sint8 _rideConstructionArrowPulseTime;
uint8 _currentTrackSlopeEnd;
uint8 _currentTrackBankEnd;
uint8 _currentTrackLiftHill;
uint8 _currentTrackAlternative;
uint8 _selectedTrackType;

uint8 _previousTrackBankEnd;
uint8 _previousTrackSlopeEnd;

uint16 _previousTrackPieceX;
uint16 _previousTrackPieceY;
uint16 _previousTrackPieceZ;

uint8 _currentBrakeSpeed2;
uint8 _currentSeatRotationAngle;

LocationXYZ16 _unkF44188;


CoordsXYZD _unkF440C5;

uint8 gRideEntranceExitPlaceType;
uint8 gRideEntranceExitPlaceRideIndex;
uint8 gRideEntranceExitPlaceStationIndex;
uint8 gRideEntranceExitPlacePreviousRideConstructionState;
uint8 gRideEntranceExitPlaceDirection;

uint8 gLastEntranceStyle;

// Static function declarations
rct_peep *find_closest_mechanic(sint32 x, sint32 y, sint32 forInspection);
static void ride_breakdown_status_update(sint32 rideIndex);
static void ride_breakdown_update(sint32 rideIndex);
static void ride_call_closest_mechanic(sint32 rideIndex);
static void ride_call_mechanic(sint32 rideIndex, rct_peep *mechanic, sint32 forInspection);
static void ride_chairlift_update(Ride *ride);
static void ride_entrance_exit_connected(Ride* ride, sint32 ride_idx);
static void ride_set_name_to_vehicle_default(Ride * ride, rct_ride_entry * rideEntry);
static sint32 ride_get_new_breakdown_problem(Ride *ride);
static void ride_inspection_update(Ride *ride);
static void ride_mechanic_status_update(sint32 rideIndex, sint32 mechanicStatus);
static void ride_music_update(sint32 rideIndex);
static void ride_shop_connected(Ride* ride, sint32 ride_idx);
static void ride_spiral_slide_update(Ride *ride);
static void ride_update(sint32 rideIndex);
static void ride_update_vehicle_colours(sint32 rideIndex);
void loc_6DDF9C(Ride *ride, rct_tile_element *tileElement);

Ride *get_ride(sint32 index)
{
    if (index < 0 || index >= MAX_RIDES)
    {
        log_error("invalid index %d for ride", index);
        return nullptr;
    }
    return &gRideList[index];
}

rct_ride_entry * get_ride_entry(sint32 index)
{
    rct_ride_entry * result = nullptr;
    auto objMgr = OpenRCT2::GetContext()->GetObjectManager();
    if (objMgr != nullptr)
    {
        auto obj = objMgr->GetLoadedObject(OBJECT_TYPE_RIDE, index);
        if (obj != nullptr)
        {
            result = (rct_ride_entry *)obj->GetLegacyData();
        }
    }
    return result;
}

void get_ride_entry_name(char *name, sint32 index)
{
    if (index < 0 || index >= object_entry_group_counts[OBJECT_TYPE_RIDE])
    {
        log_error("invalid index %d for ride type", index);
        return;
    }

    const auto entryName = object_entry_get_entry(OBJECT_TYPE_RIDE, index)->name;
    memcpy(name, entryName, 8);
    name[8] = '\0';
}

rct_ride_measurement *get_ride_measurement(sint32 index)
{
    return &gRideMeasurements[index];
}

rct_ride_entry * get_ride_entry_by_ride(Ride *ride)
{
    rct_ride_entry *type = get_ride_entry(ride->subtype);
    if (type == nullptr)
    {
        char oldname[128];
        format_string(oldname, 128, ride->name, &ride->name_arguments);
        log_error("Invalid ride subtype for ride %s", oldname);
    }
    return type;
}

/**
*
*  rct2: 0x006DED68
*/
void reset_type_to_ride_entry_index_map()
{
    size_t stride = MAX_RIDE_OBJECTS + 1;
    uint8 * entryTypeTable = (uint8 *)malloc(RIDE_TYPE_COUNT * stride);
    memset(entryTypeTable, 0xFF, RIDE_TYPE_COUNT * stride);

    for (uint8 i = 0; i < MAX_RIDE_OBJECTS; i++) {
        rct_ride_entry * rideEntry = get_ride_entry(i);
        if (rideEntry == nullptr) {
            continue;
        }
        for (uint8 j = 0; j < MAX_RIDE_TYPES_PER_RIDE_ENTRY; j++) {
            uint8 rideType = rideEntry->ride_type[j];
            if (rideType < RIDE_TYPE_COUNT) {
                uint8 * entryArray = &entryTypeTable[rideType * stride];
                uint8 * nextEntry = (uint8 *)memchr(entryArray, 0xFF, stride);
                *nextEntry = i;
            }
        }
    }

    uint8 * dst = gTypeToRideEntryIndexMap;
    for (uint8 i = 0; i < RIDE_TYPE_COUNT; i++) {
        uint8 * entryArray = &entryTypeTable[i * stride];
        uint8 * entry = entryArray;
        while (*entry != 0xFF) {
            *dst++ = *entry++;
        }
        *dst++ = 0xFF;
    }

    free(entryTypeTable);
}

uint8 *get_ride_entry_indices_for_ride_type(uint8 rideType)
{
    uint8 *entryIndexList = gTypeToRideEntryIndexMap;
    while (rideType > 0) {
        do {
            entryIndexList++;
        } while (*(entryIndexList - 1) != 255);
        rideType--;
    }
    return entryIndexList;
}

sint32 ride_get_count()
{
    Ride *ride;
    sint32 i, count = 0;

    FOR_ALL_RIDES(i, ride)
        count++;

    return count;
}

sint32 ride_get_total_queue_length(Ride *ride)
{
    sint32 i, queueLength = 0;
    for (i = 0; i < MAX_STATIONS; i++)
        if (!ride_get_entrance_location(ride, i).isNull())
            queueLength += ride->queue_length[i];
    return queueLength;
}

sint32 ride_get_max_queue_time(Ride *ride)
{
    uint8 i, queueTime = 0;
    for (i = 0; i < MAX_STATIONS; i++)
        if (!ride_get_entrance_location(ride, i).isNull())
            queueTime = Math::Max(queueTime, ride->queue_time[i]);
    return (sint32)queueTime;
}

rct_peep * ride_get_queue_head_guest(Ride * ride, sint32 stationIndex)
{
    rct_peep * peep;
    rct_peep * result = nullptr;
    uint16 spriteIndex = ride->last_peep_in_queue[stationIndex];
    while ((peep = try_get_guest(spriteIndex)) != nullptr) {
        spriteIndex = peep->next_in_queue;
        result = peep;
    }
    return result;
}

static void ride_update_queue_length(Ride * ride, sint32 stationIndex)
{
    uint16 count = 0;
    rct_peep * peep;
    uint16 spriteIndex = ride->last_peep_in_queue[stationIndex];
    while ((peep = try_get_guest(spriteIndex)) != nullptr) {
        spriteIndex = peep->next_in_queue;
        count++;
    }
    ride->queue_length[stationIndex] = count;
}

void ride_queue_insert_guest_at_front(Ride * ride, sint32 stationIndex, rct_peep * peep)
{
    assert(ride != nullptr);
    assert(stationIndex < MAX_STATIONS);
    assert(peep != nullptr);

    peep->next_in_queue = SPRITE_INDEX_NULL;
    rct_peep * queueHeadGuest = ride_get_queue_head_guest(ride, peep->current_ride_station);
    if (queueHeadGuest == nullptr) {
        ride->last_peep_in_queue[peep->current_ride_station] = peep->sprite_index;
    } else {
        queueHeadGuest->next_in_queue = peep->sprite_index;
    }
    ride_update_queue_length(ride, peep->current_ride_station);
}

/**
 *
 *  rct2: 0x006AC916
 */
void ride_update_favourited_stat()
{
    sint32 i;
    Ride *ride;
    uint16 spriteIndex;
    rct_peep* peep;

    FOR_ALL_RIDES(i, ride)
        ride->guests_favourite = 0;

    FOR_ALL_PEEPS(spriteIndex, peep) {
        if (peep->linked_list_type_offset != SPRITE_LIST_PEEP * 2)
            return;
        if (peep->favourite_ride != RIDE_ID_NULL) {
            ride = &gRideList[peep->favourite_ride];
            ride->guests_favourite++;
            ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_CUSTOMER;

        }

    }

    window_invalidate_by_class(WC_RIDE_LIST);
}

money32 get_shop_item_cost(sint32 shopItem)
{
    return ShopItemStats[shopItem].cost;
}

money16 get_shop_base_value(sint32 shopItem)
{
    return ShopItemStats[shopItem].base_value;
}

money16 get_shop_cold_value(sint32 shopItem)
{
    return ShopItemStats[shopItem].cold_value;
}

money16 get_shop_hot_value(sint32 shopItem)
{
    return ShopItemStats[shopItem].hot_value;
}

/**
 *
 *  rct2: 0x006AC3AB
 */
static money32 ride_calculate_income_per_hour(Ride *ride)
{
    // Get entry by ride to provide better reporting
    rct_ride_entry *entry = get_ride_entry_by_ride(ride);
    if (entry == nullptr)
    {
        return 0;
    }
    money32 customersPerHour = ride_customers_per_hour(ride);
    money32 priceMinusCost = ride_get_price(ride);

    sint32 currentShopItem = entry->shop_item;
    if (currentShopItem != SHOP_ITEM_NONE) {
        priceMinusCost -= get_shop_item_cost(currentShopItem);
    }

    currentShopItem = (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_RIDE_PHOTO) ?
        RidePhotoItems[ride->type] :
        entry->shop_item_secondary;

    if (currentShopItem != SHOP_ITEM_NONE) {
        priceMinusCost += ride->price_secondary;
        priceMinusCost -= get_shop_item_cost(currentShopItem);

        if (entry->shop_item != SHOP_ITEM_NONE)
            priceMinusCost /= 2;
    }

    return customersPerHour * priceMinusCost;
}

/**
 *
 *  rct2: 0x006CAF80
 * ax result x
 * bx result y
 * dl ride index
 * esi result map element
 */
bool ride_try_get_origin_element(sint32 rideIndex, CoordsXYE *output)
{
    rct_tile_element *resultTileElement = nullptr;

    tile_element_iterator it;
    tile_element_iterator_begin(&it);
    do {
        if (tile_element_get_type(it.element) != TILE_ELEMENT_TYPE_TRACK)
            continue;
        if (rideIndex != track_element_get_ride_index(it.element))
            continue;

        // Found a track piece for target ride

        // Check if it's not the station or ??? (but allow end piece of station)
        bool specialTrackPiece = (
            track_element_get_type(it.element) != TRACK_ELEM_BEGIN_STATION &&
            track_element_get_type(it.element) != TRACK_ELEM_MIDDLE_STATION &&
            (TrackSequenceProperties[track_element_get_type(it.element)][0] & TRACK_SEQUENCE_FLAG_ORIGIN)
        );

        // Set result tile to this track piece if first found track or a ???
        if (resultTileElement == nullptr || specialTrackPiece) {
            resultTileElement = it.element;

            if (output != nullptr) {
                output->element = resultTileElement;
                output->x = it.x * 32;
                output->y = it.y * 32;
            }
        }

        if (specialTrackPiece) {
            return true;
        }
    } while (tile_element_iterator_next(&it));

    return resultTileElement != nullptr;
}

/**
*
* rct2: 0x006C6096
* Gets the next track block coordinates from the
* coordinates of the first of element of a track block.
* Use track_block_get_next if you are unsure if you are
* on the first element of a track block
*/
bool track_block_get_next_from_zero(sint16 x, sint16 y, sint16 z_start, uint8 rideIndex, uint8 direction_start, CoordsXYE *output, sint32 *z, sint32 *direction)
{
    Ride* ride = get_ride(rideIndex);

    if (!(direction_start & (1 << 2))){
        x += TileDirectionDelta[direction_start].x;
        y += TileDirectionDelta[direction_start].y;
    }

    rct_tile_element* tileElement = map_get_first_element_at(x / 32, y / 32);
    if (tileElement == nullptr){
        output->element = nullptr;
        output->x = LOCATION_NULL;
        return 0;
    }

    do{
        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
            continue;

        if (track_element_get_ride_index(tileElement) != rideIndex)
            continue;

        if (tile_element_get_track_sequence(tileElement) != 0)
            continue;

        const rct_preview_track* nextTrackBlock = get_track_def_from_ride(ride, track_element_get_type(tileElement));
        const rct_track_coordinates* nextTrackCoordinate = get_track_coord_from_ride(ride, track_element_get_type(tileElement));

        uint8 nextRotation =
            tile_element_get_direction_with_offset(tileElement, nextTrackCoordinate->rotation_begin) |
            (nextTrackCoordinate->rotation_begin & (1 << 2));

        if (nextRotation != direction_start)
            continue;

        sint16 nextZ = nextTrackCoordinate->z_begin - nextTrackBlock->z + tileElement->base_height * 8;
        if (nextZ != z_start)
            continue;

        if (z != nullptr) *z = tileElement->base_height * 8;
        if (direction != nullptr) *direction = nextRotation;
        output->x = x;
        output->y = y;
        output->element = tileElement;
        return 1;
    } while (!tile_element_is_last_for_tile(tileElement++));

    if (direction != nullptr) *direction = direction_start;
    if (z != nullptr) *z = z_start;
    output->x = x;
    output->y = y;
    output->element = --tileElement;
    return 0;
}

/**
 *
 *  rct2: 0x006C60C2
 */
bool track_block_get_next(CoordsXYE *input, CoordsXYE *output, sint32 *z, sint32 *direction)
{
    uint8 rideIndex = track_element_get_ride_index(input->element);
    Ride* ride = get_ride(rideIndex);

    const rct_preview_track* trackBlock = get_track_def_from_ride(ride, track_element_get_type(input->element));
    uint8 sequence = tile_element_get_track_sequence(input->element);
    trackBlock += sequence;

    const rct_track_coordinates* trackCoordinate = get_track_coord_from_ride(ride, track_element_get_type(input->element));

    sint32 x = input->x;
    sint32 y = input->y;
    sint32 OriginZ = input->element->base_height * 8;

    uint8 rotation = tile_element_get_direction(input->element);
    switch (rotation){
    case 0:
        x += trackCoordinate->x;
        x -= trackBlock->x;
        y += trackCoordinate->y;
        y -= trackBlock->y;
        break;
    case 1:
        x += trackCoordinate->y;
        x -= trackBlock->y;
        y -= trackCoordinate->x;
        y += trackBlock->x;
        break;
    case 2:
        x -= trackCoordinate->x;
        x += trackBlock->x;
        y -= trackCoordinate->y;
        y += trackBlock->y;
        break;
    case 3:
        x -= trackCoordinate->y;
        x += trackBlock->y;
        y += trackCoordinate->x;
        y -= trackBlock->x;
        break;
    }

    OriginZ -= trackBlock->z;
    OriginZ += trackCoordinate->z_end;

    uint8 directionStart = ((trackCoordinate->rotation_end + rotation) & TILE_ELEMENT_DIRECTION_MASK) |
        (trackCoordinate->rotation_end & (1 << 2));

    return track_block_get_next_from_zero(x, y, OriginZ, rideIndex, directionStart, output, z, direction);
}

/**
 * Returns the begin position / direction and end position / direction of the
 * track piece that proceeds the given location. Gets the previous track block
 * coordinates from the coordinates of the first of element of a track block.
 * Use track_block_get_previous if you are unsure if you are on the first
 * element of a track block
 *  rct2: 0x006C63D6
 */
bool track_block_get_previous_from_zero(sint16 x, sint16 y, sint16 z, uint8 rideIndex, uint8 direction, track_begin_end *outTrackBeginEnd){
    Ride* ride = get_ride(rideIndex);

    uint8 directionStart = direction;
    direction ^= (1 << 1);

    if (!(direction & (1 << 2))){
        x += TileDirectionDelta[direction].x;
        y += TileDirectionDelta[direction].y;
    }

    rct_tile_element* tileElement = map_get_first_element_at(x / 32, y / 32);
    if (tileElement == nullptr){
        outTrackBeginEnd->end_x = x;
        outTrackBeginEnd->end_y = y;
        outTrackBeginEnd->begin_element = nullptr;
        outTrackBeginEnd->begin_direction = directionStart ^ (1 << 1);
        return 0;
    }

    do{
        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
            continue;

        if (track_element_get_ride_index(tileElement) != rideIndex)
            continue;

        const rct_preview_track* nextTrackBlock = get_track_def_from_ride(ride, track_element_get_type(tileElement));
        const rct_track_coordinates* nextTrackCoordinate = get_track_coord_from_ride(ride, track_element_get_type(tileElement));

        nextTrackBlock += tile_element_get_track_sequence(tileElement);
        if ((nextTrackBlock + 1)->index != 255)
            continue;

        uint8 nextRotation =
            tile_element_get_direction_with_offset(tileElement, nextTrackCoordinate->rotation_end) |
            (nextTrackCoordinate->rotation_end & (1 << 2));

        if (nextRotation != directionStart)
            continue;

        sint16 nextZ = nextTrackCoordinate->z_end - nextTrackBlock->z + tileElement->base_height * 8;
        if (nextZ != z)
            continue;

        nextRotation =
            tile_element_get_direction_with_offset(tileElement, nextTrackCoordinate->rotation_begin) |
            (nextTrackCoordinate->rotation_begin & (1 << 2));
        outTrackBeginEnd->begin_element = tileElement;
        outTrackBeginEnd->begin_x = x;
        outTrackBeginEnd->begin_y = y;
        outTrackBeginEnd->end_x = x;
        outTrackBeginEnd->end_y = y;
        switch (nextRotation & 3){
        case 0:
            outTrackBeginEnd->begin_x -= nextTrackCoordinate->x;
            outTrackBeginEnd->begin_y -= nextTrackCoordinate->y;
            break;
        case 1:
            outTrackBeginEnd->begin_x -= nextTrackCoordinate->y;
            outTrackBeginEnd->begin_y += nextTrackCoordinate->x;
            break;
        case 2:
            outTrackBeginEnd->begin_x += nextTrackCoordinate->x;
            outTrackBeginEnd->begin_y += nextTrackCoordinate->y;
            break;
        case 3:
            outTrackBeginEnd->begin_x += nextTrackCoordinate->y;
            outTrackBeginEnd->begin_y -= nextTrackCoordinate->x;
            break;
        }

        outTrackBeginEnd->begin_z = tileElement->base_height * 8;
        outTrackBeginEnd->begin_z +=
            get_track_def_from_ride(ride, track_element_get_type(tileElement))->z -
            nextTrackBlock->z;

        outTrackBeginEnd->begin_direction = nextRotation;
        outTrackBeginEnd->end_direction = directionStart ^ (1 << 1);
        return 1;
    } while (!tile_element_is_last_for_tile(tileElement++));

    outTrackBeginEnd->end_x = x;
    outTrackBeginEnd->end_y = y;
    outTrackBeginEnd->begin_z = z;
    outTrackBeginEnd->begin_element = nullptr;
    outTrackBeginEnd->end_direction = directionStart ^ (1 << 1);
    return 0;
}

/**
 *
 *  rct2: 0x006C6402
 *
 * @remarks outTrackBeginEnd.begin_x and outTrackBeginEnd.begin_y will be in the
 * higher two bytes of ecx and edx where as outTrackBeginEnd.end_x and
 * outTrackBeginEnd.end_y will be in the lower two bytes (cx and dx).
 */
bool track_block_get_previous(sint32 x, sint32 y, rct_tile_element *tileElement, track_begin_end *outTrackBeginEnd)
{
    uint8 rideIndex = track_element_get_ride_index(tileElement);
    Ride* ride = get_ride(rideIndex);

    const rct_preview_track* trackBlock = get_track_def_from_ride(ride, track_element_get_type(tileElement));
    uint8 sequence = tile_element_get_track_sequence(tileElement);
    trackBlock += sequence;

    const rct_track_coordinates* trackCoordinate = get_track_coord_from_ride(ride, track_element_get_type(tileElement));

    sint32 z = tileElement->base_height * 8;

    uint8 rotation = tile_element_get_direction(tileElement);
    switch (rotation){
    case 0:
        x -= trackBlock->x;
        y -= trackBlock->y;
        break;
    case 1:
        x -= trackBlock->y;
        y += trackBlock->x;
        break;
    case 2:
        x += trackBlock->x;
        y += trackBlock->y;
        break;
    case 3:
        x += trackBlock->y;
        y -= trackBlock->x;
        break;
    }

    z -= trackBlock->z;
    z += trackCoordinate->z_begin;

    rotation =
        ((trackCoordinate->rotation_begin + rotation) & TILE_ELEMENT_DIRECTION_MASK) |
        (trackCoordinate->rotation_begin & (1 << 2));

    return track_block_get_previous_from_zero(x, y, z, rideIndex, rotation, outTrackBeginEnd);
}

/**
 *
 * Make sure to pass in the x and y of the start track element too.
 *  rct2: 0x006CB02F
 * ax result x
 * bx result y
 * esi input / output map element
 */
sint32 ride_find_track_gap(CoordsXYE *input, CoordsXYE *output)
{
    rct_window *w;
    Ride *ride;
    track_circuit_iterator it = { 0 }, slowIt;
    sint32 rideIndex;

    assert(tile_element_get_type(input->element) == TILE_ELEMENT_TYPE_TRACK);
    rideIndex = track_element_get_ride_index(input->element);
    ride = get_ride(rideIndex);

    if (ride->type == RIDE_TYPE_MAZE)
        return 0;

    w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && _currentRideIndex == rideIndex)
        ride_construction_invalidate_current_track();

    bool moveSlowIt = true;
    track_circuit_iterator_begin(&it, *input);
    slowIt = it;
    while (track_circuit_iterator_next(&it)) {
        if (!track_is_connected_by_shape(it.last.element, it.current.element)) {
            *output = it.current;
            return 1;
        }
        //#2081: prevent an infinite loop
        moveSlowIt = !moveSlowIt;
        if (moveSlowIt)
        {
            track_circuit_iterator_next(&slowIt);
            if (track_circuit_iterators_match(&it, &slowIt))
            {
                *output = it.current;
                return 1;
            }
        }

    }
    if (!it.looped) {
        *output = it.last;
        return 1;
    }

    return 0;
}

/**
 *
 *  rct2: 0x006AF561
 */
void ride_get_status(sint32 rideIndex, rct_string_id *formatSecondary, sint32 *argument)
{
    Ride *ride = get_ride(rideIndex);

    if (ride->lifecycle_flags & RIDE_LIFECYCLE_CRASHED) {
        *formatSecondary = STR_CRASHED;
        return;
    }
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
        *formatSecondary = STR_BROKEN_DOWN;
        return;
    }
    if (ride->status == RIDE_STATUS_CLOSED) {
        *formatSecondary = STR_CLOSED;

        if (!ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_IS_SHOP)) {
            *argument = ride->num_riders;

            if (*argument == 1) {
                *formatSecondary = STR_CLOSED_WITH_PERSON;
            } else if (*argument > 1) {
                *formatSecondary = STR_CLOSED_WITH_PEOPLE;
            }
        }

        return;
    }
    if (ride->status == RIDE_STATUS_TESTING) {
        *formatSecondary = STR_TEST_RUN;
        return;
    }
    if (ride->mode == RIDE_MODE_RACE && !(ride->lifecycle_flags & RIDE_LIFECYCLE_PASS_STATION_NO_STOPPING) && ride->race_winner != SPRITE_INDEX_NULL && (GET_PEEP(ride->race_winner))->sprite_identifier == SPRITE_IDENTIFIER_PEEP) {
        rct_peep *peep = GET_PEEP(ride->race_winner);
        if (peep->name_string_idx == STR_GUEST_X) {
            *argument = peep->id;
            *formatSecondary = STR_RACE_WON_BY_GUEST;
        } else {
            *argument = peep->name_string_idx;
            *formatSecondary = STR_RACE_WON_BY;
        }
    } else {
        if (!ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_IS_SHOP)) {
            *argument = ride->num_riders;
            *formatSecondary = STR_PERSON_ON_RIDE;
            if (*argument != 1)
                *formatSecondary = STR_PEOPLE_ON_RIDE;

        } else {
            *formatSecondary = STR_OPEN;
        }
    }
}

sint32 ride_get_total_length(Ride *ride)
{
    sint32 i, totalLength = 0;
    for (i = 0; i < ride->num_stations; i++)
        totalLength += ride->length[i];
    return totalLength;
}

sint32 ride_get_total_time(Ride *ride)
{
    sint32 i, totalTime = 0;
    for (i = 0; i < ride->num_stations; i++)
        totalTime += ride->time[i];
    return totalTime;
}

sint32 ride_can_have_multiple_circuits(Ride *ride)
{
    if (!(RideData4[ride->type].flags & RIDE_TYPE_FLAG4_ALLOW_MULTIPLE_CIRCUITS))
        return 0;

    // Only allow circuit or launch modes
    if (
        ride->mode != RIDE_MODE_CONTINUOUS_CIRCUIT &&
        ride->mode != RIDE_MODE_REVERSE_INCLINE_LAUNCHED_SHUTTLE &&
        ride->mode != RIDE_MODE_POWERED_LAUNCH_PASSTROUGH
    ) {
        return 0;
    }

    // Must have no more than one vehicle and one station
    if (ride->num_vehicles > 1 || ride->num_stations > 1)
        return 0;

    return 1;
}

#pragma region Initialisation functions

/**
 *
 *  rct2: 0x006ACA89
 */
void ride_init_all()
{
    for (sint32 i = 0; i < MAX_RIDES; i++) {
        Ride *ride = get_ride(i);
        ride->type = RIDE_TYPE_NULL;
    }

    for (sint32 i = 0; i < MAX_RIDE_MEASUREMENTS; i++) {
        rct_ride_measurement *ride_measurement = get_ride_measurement(i);
        ride_measurement->ride_index = 255;
    }
}

/**
*
*  rct2: 0x006B7A38
*/
void reset_all_ride_build_dates()
{
    sint32 i;
    Ride *ride;

    FOR_ALL_RIDES(i, ride)
        ride->build_date -= gDateMonthsElapsed;
}

#pragma endregion

#pragma region Construction

static sint32 ride_check_if_construction_allowed(Ride *ride)
{
    rct_ride_entry * rideEntry = get_ride_entry_by_ride(ride);
    if (rideEntry == nullptr) {
        context_show_error(STR_INVALID_RIDE_TYPE, STR_CANT_EDIT_INVALID_RIDE_TYPE);
        return 0;
    }
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
        set_format_arg(6, rct_string_id, ride->name);
        set_format_arg(8, uint32, ride->name_arguments);
        context_show_error(STR_CANT_START_CONSTRUCTION_ON, STR_HAS_BROKEN_DOWN_AND_REQUIRES_FIXING);
        return 0;
    }

    if (ride->status != RIDE_STATUS_CLOSED) {
        set_format_arg(6, rct_string_id, ride->name);
        set_format_arg(8, uint32, ride->name_arguments);
        context_show_error(STR_CANT_START_CONSTRUCTION_ON, STR_MUST_BE_CLOSED_FIRST);
        return 0;
    }

    return 1;
}

static rct_window *ride_create_or_find_construction_window(sint32 rideIndex)
{
    rct_window *w;

    w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w == nullptr || w->number != rideIndex) {
        window_close_construction_windows();
        _currentRideIndex = rideIndex;
        w = context_open_window(WC_RIDE_CONSTRUCTION);
    } else {
        ride_construction_invalidate_current_track();
        _currentRideIndex = rideIndex;
    }

    return w;
}

/**
 *
 *  rct2: 0x006B4857
 */
void ride_construct(sint32 rideIndex)
{
    CoordsXYE trackElement;

    if (ride_try_get_origin_element(rideIndex, &trackElement)) {
        ride_find_track_gap(&trackElement, &trackElement);

        rct_window *w = window_get_main();
        if (w != nullptr && ride_modify(&trackElement))
            window_scroll_to_location(w, trackElement.x, trackElement.y, trackElement.element->base_height * 8);
    } else {
        ride_initialise_construction_window(rideIndex);
    }
}

/**
 *
 *  rct2: 0x006DD4D5
 */
static void ride_remove_cable_lift(Ride *ride)
{
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT) {
        ride->lifecycle_flags &= ~RIDE_LIFECYCLE_CABLE_LIFT;
        uint16 spriteIndex = ride->cable_lift;
        do {
            rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
            invalidate_sprite_2((rct_sprite*)vehicle);
            sprite_remove((rct_sprite*)vehicle);
            spriteIndex = vehicle->next_vehicle_on_train;
        } while (spriteIndex != SPRITE_INDEX_NULL);
    }
}

/**
 *
 *  rct2: 0x006DD506
 */
static void ride_remove_vehicles(Ride *ride)
{
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK) {
        ride->lifecycle_flags &= ~RIDE_LIFECYCLE_ON_TRACK;
        ride->lifecycle_flags &= ~(RIDE_LIFECYCLE_TEST_IN_PROGRESS | RIDE_LIFECYCLE_HAS_STALLED_VEHICLE);

        for (size_t i = 0; i < MAX_VEHICLES_PER_RIDE; i++) {
            uint16 spriteIndex = ride->vehicles[i];
            while (spriteIndex != SPRITE_INDEX_NULL) {
                rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
                invalidate_sprite_2((rct_sprite*)vehicle);
                sprite_remove((rct_sprite*)vehicle);
                spriteIndex = vehicle->next_vehicle_on_train;
            }

            ride->vehicles[i] = SPRITE_INDEX_NULL;
        }

        for (size_t i = 0; i < MAX_STATIONS; i++)
            ride->train_at_station[i] = 255;
    }
}

/**
 *
 *  rct2: 0x006DD4AC
 */
void ride_clear_for_construction(sint32 rideIndex)
{
    Ride *ride;
    rct_window *w;

    ride = get_ride(rideIndex);

    ride_measurement_clear(ride);

    ride->lifecycle_flags &= ~(RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN);
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAIN | RIDE_INVALIDATE_RIDE_LIST;

    // Open circuit rides will go directly into building mode (creating ghosts) where it would normally clear the stats,
    // however this causes desyncs since it's directly run from the window and other clients would not get it.
    // To prevent these problems, unconditionally invalidate the test results on all clients in multiplayer games.
    if (network_get_mode() != NETWORK_MODE_NONE) {
        invalidate_test_results(rideIndex);
    }

    ride_remove_cable_lift(ride);
    ride_remove_vehicles(ride);

    w = window_find_by_number(WC_RIDE, rideIndex);
    if (w != nullptr)
        window_event_resize_call(w);
}

/**
 *
 *  rct2: 0x006664DF
 */
void ride_remove_peeps(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    // Find first station
    sint8 stationIndex = ride_get_first_valid_station_start(ride);

    // Get exit position and direction
    sint32 exitX = 0;
    sint32 exitY = 0;
    sint32 exitZ = 0;
    sint32 exitDirection = 255;
    if (stationIndex != -1) {
        TileCoordsXYZD location = ride_get_exit_location(ride, stationIndex);
        if (!location.isNull()) {
            exitX = location.x;
            exitY = location.y;
            exitZ = location.z;
            exitDirection = location.direction;

            exitX = (exitX * 32) - (word_981D6C[exitDirection].x * 20) + 16;
            exitY = (exitY * 32) - (word_981D6C[exitDirection].y * 20) + 16;
            exitZ = (exitZ * 8) + 2;

            // Reverse direction
            exitDirection ^= 2;

            exitDirection *= 8;
        }
    }

    // Place all the peeps at exit
    uint16 spriteIndex;
    rct_peep *peep;
    FOR_ALL_PEEPS(spriteIndex, peep) {
        if (
            peep->state == PEEP_STATE_QUEUING_FRONT ||
            peep->state == PEEP_STATE_ENTERING_RIDE ||
            peep->state == PEEP_STATE_LEAVING_RIDE ||
            peep->state == PEEP_STATE_ON_RIDE
        ) {
            if (peep->current_ride != rideIndex)
                continue;

            peep_decrement_num_riders(peep);
            if (peep->state == PEEP_STATE_QUEUING_FRONT && peep->sub_state == PEEP_RIDE_AT_ENTRANCE)
                remove_peep_from_queue(peep);

            invalidate_sprite_2((rct_sprite*)peep);

            if (exitDirection == 255) {
                sint32 x = peep->next_x + 16;
                sint32 y = peep->next_y + 16;
                sint32 z = peep->next_z * 8;
                if (peep->next_var_29 & 4)
                    z += 8;
                z++;
                sprite_move(x, y, z, (rct_sprite*)peep);
            } else {
                sprite_move(exitX, exitY, exitZ, (rct_sprite*)peep);
                peep->sprite_direction = exitDirection;
            }

            invalidate_sprite_2((rct_sprite*)peep);
            peep->state = PEEP_STATE_FALLING;
            peep_switch_to_special_sprite(peep, 0);

            peep->happiness = Math::Min(peep->happiness, peep->happiness_target) / 2;
            peep->happiness_target = peep->happiness;
            peep->window_invalidate_flags |= PEEP_INVALIDATE_PEEP_STATS;
        }
    }

    ride->num_riders = 0;
    ride->slide_in_use = 0;
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAIN;
}

/**
 * Gets the origin track element (sequence 0). Seems to do more than that though and even invalidates track.
 *  rct2: 0x006C683D
 * ax : x
 * bx : direction << 8, type
 * cx : y
 * dx : z
 * si : extra_params
 * di : output_element
 * bp : flags
 */
sint32 sub_6C683D(sint32* x, sint32* y, sint32* z, sint32 direction, sint32 type, uint16 extra_params, rct_tile_element** output_element, uint16 flags)
{
    rct_tile_element *tileElement = map_get_first_element_at(*x / 32, *y / 32);
    rct_tile_element *successTileElement = nullptr;

    do {
        if (tileElement->base_height != *z / 8)
            continue;

        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
            continue;

        if ((tile_element_get_direction(tileElement)) != direction)
            continue;

        if (type != track_element_get_type(tileElement))
            continue;

        successTileElement = tileElement;
        if (tile_element_get_track_sequence(tileElement) == 0)
            break;
    } while (!tile_element_is_last_for_tile(tileElement++));

    tileElement = successTileElement;
    if (tileElement == nullptr)
        return 1;

    // Possibly z should be & 0xF8
    const rct_preview_track * trackBlock = get_track_def_from_ride_index(track_element_get_ride_index(tileElement), type);

    sint32 sequence = tile_element_get_track_sequence(tileElement);
    uint8 mapDirection = tile_element_get_direction(tileElement);

    switch (mapDirection){
    case TILE_ELEMENT_DIRECTION_WEST:
        *x -= trackBlock[sequence].x;
        *y -= trackBlock[sequence].y;
        break;
    case TILE_ELEMENT_DIRECTION_NORTH:
        *x -= trackBlock[sequence].y;
        *y += trackBlock[sequence].x;
        break;
    case TILE_ELEMENT_DIRECTION_EAST:
        *x += trackBlock[sequence].x;
        *y += trackBlock[sequence].y;
        break;
    case TILE_ELEMENT_DIRECTION_SOUTH:
        *x += trackBlock[sequence].y;
        *y -= trackBlock[sequence].x;
        break;
    }
    *z -= trackBlock[sequence].z;

    sint32 start_x = *x, start_y = *y, start_z = *z;
    *z += trackBlock[0].z;
    for (sint32 i = 0; trackBlock[i].index != 0xFF; ++i){
        sint32 cur_x = start_x, cur_y = start_y, cur_z = start_z;
        switch (mapDirection){
        case TILE_ELEMENT_DIRECTION_WEST:
            cur_x += trackBlock[i].x;
            cur_y += trackBlock[i].y;
            break;
        case TILE_ELEMENT_DIRECTION_NORTH:
            cur_x += trackBlock[i].y;
            cur_y -= trackBlock[i].x;
            break;
        case TILE_ELEMENT_DIRECTION_EAST:
            cur_x -= trackBlock[i].x;
            cur_y -= trackBlock[i].y;
            break;
        case TILE_ELEMENT_DIRECTION_SOUTH:
            cur_x -= trackBlock[i].y;
            cur_y += trackBlock[i].x;
            break;
        }
        cur_z += trackBlock[i].z;

        map_invalidate_tile_full(cur_x, cur_y);

        tileElement = map_get_first_element_at(cur_x / 32, cur_y / 32);
        successTileElement = nullptr;
        do {
            if (tileElement->base_height != cur_z / 8)
                continue;

            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
                continue;

            if ((tile_element_get_direction(tileElement)) != direction)
                continue;

            if (tile_element_get_track_sequence(tileElement) != trackBlock[i].index)
                continue;

            if (type == track_element_get_type(tileElement)) {
                successTileElement = tileElement;
                break;
            }
        }
        while (!tile_element_is_last_for_tile(tileElement++));

        if (successTileElement == nullptr)
        {
            return 1;
        }
        if (i == 0 && output_element != nullptr)
        {
            *output_element = tileElement;
        }
        if (flags & (1 << 0))
        {
            tileElement->type &= ~TILE_ELEMENT_TYPE_FLAG_HIGHLIGHT;
        }
        if (flags & (1 << 1))
        {
            tileElement->type |= TILE_ELEMENT_TYPE_FLAG_HIGHLIGHT;
        }
        if (flags & (1 << 2))
        {
            track_element_set_colour_scheme(tileElement, (uint8)(extra_params & 0xFF));
        }
        if (flags & (1 << 5))
        {
            track_element_set_seat_rotation(tileElement, (uint8)(extra_params & 0xFF));
        }

        if (flags & (1 << 3)) {
            track_element_set_cable_lift(tileElement);
        }
        if (flags & (1 << 4))
        {
            track_element_clear_cable_lift(tileElement);
        }
    }

    return 0;
}

void ride_restore_provisional_track_piece()
{
    if (_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_TRACK) {
        sint32 x, y, z, direction, type, rideIndex, liftHillAndAlternativeState;
        if (window_ride_construction_update_state(&type, &direction, &rideIndex, &liftHillAndAlternativeState, &x, &y, &z, nullptr)) {
            ride_construction_remove_ghosts();
        } else {
            _currentTrackPrice = place_provisional_track_piece(rideIndex, type, direction, liftHillAndAlternativeState, x, y, z);
            window_ride_construction_update_active_elements();
        }
    }
}

void ride_remove_provisional_track_piece()
{
    if (!(_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_TRACK)) {
        return;
    }
    Ride *ride;
    sint32 rideIndex, x, y, z, direction;

    rideIndex = _currentRideIndex;

    x = _unkF440C5.x;
    y = _unkF440C5.y;
    z = _unkF440C5.z;

    ride = get_ride(rideIndex);
    if (ride->type == RIDE_TYPE_MAZE)
    {
        sint32 flags = GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED | GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_GHOST;
        maze_set_track(x, y, z, flags, false, 0, rideIndex, GC_SET_MAZE_TRACK_FILL);
        maze_set_track(x, y + 16, z, flags, false, 1, rideIndex, GC_SET_MAZE_TRACK_FILL);
        maze_set_track(x + 16, y + 16, z, flags, false, 2, rideIndex, GC_SET_MAZE_TRACK_FILL);
        maze_set_track(x + 16, y, z, flags, false, 3, rideIndex, GC_SET_MAZE_TRACK_FILL);
    }
    else
    {
        direction = _unkF440C5.direction;
        if (!(direction & 4)) {
            x -= TileDirectionDelta[direction].x;
            y -= TileDirectionDelta[direction].y;
        }
        CoordsXYE next_track;
        if (track_block_get_next_from_zero(x, y, z, rideIndex, direction, &next_track, &z, &direction)) {
            sint32 flags = GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED | GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_GHOST;
            game_do_command(
                next_track.x,
                flags | ((direction & 3) << 8),
                next_track.y,
                track_element_get_type(next_track.element) | (tile_element_get_track_sequence(next_track.element) << 8),
                GAME_COMMAND_REMOVE_TRACK,
                z,
                0
            );
        }
    }
}

/**
 *
 *  rct2: 0x006C96C0
 */
void ride_construction_remove_ghosts()
{
    if (_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_ENTRANCE_OR_EXIT) {
        ride_entrance_exit_remove_ghost();
        _currentTrackSelectionFlags &= ~TRACK_SELECTION_FLAG_ENTRANCE_OR_EXIT;
    }
    if (_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_TRACK) {
        ride_remove_provisional_track_piece();
        _currentTrackSelectionFlags &= ~TRACK_SELECTION_FLAG_TRACK;
    }
}

/*
*  rct2: 0x006C9627
*/
void ride_construction_invalidate_current_track()
{
    sint32 x, y, z;

    switch (_rideConstructionState) {
    case RIDE_CONSTRUCTION_STATE_SELECTED:
        x = _currentTrackBeginX;
        y = _currentTrackBeginY;
        z = _currentTrackBeginZ;
        sub_6C683D(
            &x,
            &y,
            &z,
            _currentTrackPieceDirection & 3,
            _currentTrackPieceType,
            0,
            nullptr,
            1
        );
        break;
    case RIDE_CONSTRUCTION_STATE_MAZE_BUILD:
    case RIDE_CONSTRUCTION_STATE_MAZE_MOVE:
    case RIDE_CONSTRUCTION_STATE_MAZE_FILL:
        if (_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_ARROW) {
            map_invalidate_tile_full(
                _currentTrackBeginX & 0xFFE0,
                _currentTrackBeginY & 0xFFE0
            );
            gMapSelectFlags &= ~MAP_SELECT_FLAG_ENABLE_ARROW;
        }
        break;
    default:
        if (_currentTrackSelectionFlags & TRACK_SELECTION_FLAG_ARROW) {
            _currentTrackSelectionFlags &= ~TRACK_SELECTION_FLAG_ARROW;
            gMapSelectFlags &= ~MAP_SELECT_FLAG_ENABLE_ARROW;
            map_invalidate_tile_full(_currentTrackBeginX, _currentTrackBeginY);
        }
        ride_construction_remove_ghosts();
        break;
    }
}

/**
 *
 *  rct2: 0x006C9B19
 */
static void ride_construction_reset_current_piece()
{
    Ride *ride;

    ride = get_ride(_currentRideIndex);
    if (!ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_NO_TRACK) || ride->num_stations == 0) {
        _currentTrackCurve = RideConstructionDefaultTrackType[ride->type] | 0x100;
        _currentTrackSlopeEnd = 0;
        _currentTrackBankEnd = 0;
        _currentTrackLiftHill = 0;
        _currentTrackAlternative = RIDE_TYPE_NO_ALTERNATIVES;
        if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_START_CONSTRUCTION_INVERTED) {
            _currentTrackAlternative |= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
        }
        _previousTrackSlopeEnd = 0;
        _previousTrackBankEnd = 0;
    } else {
        _currentTrackCurve = 0xFFFF;
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_0;
    }
}

/**
 *
 *  rct2: 0x006C9800
 */
void ride_construction_set_default_next_piece()
{
    sint32 x, y, z, direction, rideIndex, trackType, curve, bank, slope;
    Ride *ride;
    track_begin_end trackBeginEnd;
    CoordsXYE xyElement;
    rct_tile_element *tileElement;

    _currentTrackPrice = MONEY32_UNDEFINED;
    switch (_rideConstructionState) {
    case RIDE_CONSTRUCTION_STATE_FRONT:
        rideIndex = _currentRideIndex;
        ride = get_ride(rideIndex);

        x = _currentTrackBeginX;
        y = _currentTrackBeginY;
        z = _currentTrackBeginZ;
        direction = _currentTrackPieceDirection;
        if (!track_block_get_previous_from_zero(x, y, z, rideIndex, direction, &trackBeginEnd)) {
            ride_construction_reset_current_piece();
            return;
        }
        tileElement = trackBeginEnd.begin_element;
        trackType = track_element_get_type(tileElement);

        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_NO_TRACK)) {
            ride_construction_reset_current_piece();
            return;
        }

        // Set whether track is covered
        _currentTrackAlternative &= ~RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
        if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_HAS_ALTERNATIVE_TRACK_TYPE)
        {
            if (track_element_is_inverted(tileElement))
            {
                _currentTrackAlternative |= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
            }
        }

        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_FLAT_RIDE)) {
            curve = gFlatRideTrackCurveChain[trackType].next;
            bank = FlatRideTrackDefinitions[trackType].bank_end;
            slope = FlatRideTrackDefinitions[trackType].vangle_end;
        } else {
            if (track_element_is_booster(ride->type, trackType)) {
                curve = 0x100 | TRACK_ELEM_BOOSTER;
            } else {
                curve = gTrackCurveChain[trackType].next;
            }
            bank = TrackDefinitions[trackType].bank_end;
            slope = TrackDefinitions[trackType].vangle_end;
        }

        // Set track curve
        _currentTrackCurve = curve;

        // Set track banking
        if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_HAS_ALTERNATIVE_TRACK_TYPE) {
            if (bank == TRACK_BANK_UPSIDE_DOWN) {
                bank = TRACK_BANK_NONE;
                _currentTrackAlternative ^= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
            }
        }
        _currentTrackBankEnd = bank;
        _previousTrackBankEnd = bank;

        // Set track slope and lift hill
        _currentTrackSlopeEnd = slope;
        _previousTrackSlopeEnd = slope;
        _currentTrackLiftHill = ((tileElement->type & 0x80) && slope != TRACK_SLOPE_DOWN_25 && slope != TRACK_SLOPE_DOWN_60) != 0;
        break;
    case RIDE_CONSTRUCTION_STATE_BACK:
        rideIndex = _currentRideIndex;
        ride = get_ride(rideIndex);

        x = _currentTrackBeginX;
        y = _currentTrackBeginY;
        z = _currentTrackBeginZ;
        direction = _currentTrackPieceDirection ^ 2;
        if (!track_block_get_next_from_zero(x, y, z, rideIndex, direction, &xyElement, &z, &direction)) {
            ride_construction_reset_current_piece();
            return;
        }
        tileElement = xyElement.element;
        trackType = track_element_get_type(tileElement);

        // Set whether track is covered
        _currentTrackAlternative &= ~RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
        if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_HAS_ALTERNATIVE_TRACK_TYPE)
        {
            if (track_element_is_inverted(tileElement))
            {
                _currentTrackAlternative |= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
            }
        }

        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_FLAT_RIDE))
        {
            curve = gFlatRideTrackCurveChain[trackType].previous;
            bank = FlatRideTrackDefinitions[trackType].bank_start;
            slope = FlatRideTrackDefinitions[trackType].vangle_start;
        }
        else
        {
            if (track_element_is_booster(ride->type, trackType)) {
                curve = 0x100 | TRACK_ELEM_BOOSTER;
            } else {
                curve = gTrackCurveChain[trackType].previous;
            }
            bank = TrackDefinitions[trackType].bank_start;
            slope = TrackDefinitions[trackType].vangle_start;
        }

        // Set track curve
        _currentTrackCurve = curve;

        // Set track banking
        if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_HAS_ALTERNATIVE_TRACK_TYPE) {
            if (bank == TRACK_BANK_UPSIDE_DOWN) {
                bank = TRACK_BANK_NONE;
                _currentTrackAlternative ^= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;
            }
        }
        _currentTrackBankEnd = bank;
        _previousTrackBankEnd = bank;

        // Set track slope and lift hill
        _currentTrackSlopeEnd = slope;
        _previousTrackSlopeEnd = slope;
        if (!gCheatsEnableChainLiftOnAllTrack) {
            _currentTrackLiftHill = track_element_is_lift_hill(tileElement);
        }
        break;
    }
}

/**
 *
 *  rct2: 0x006C9296
 */
void ride_select_next_section()
{
    if (_rideConstructionState == RIDE_CONSTRUCTION_STATE_SELECTED) {
        ride_construction_invalidate_current_track();
        sint32 x = _currentTrackBeginX;
        sint32 y = _currentTrackBeginY;
        sint32 z = _currentTrackBeginZ;
        sint32 direction = _currentTrackPieceDirection;
        sint32 type = _currentTrackPieceType;
        rct_tile_element *tileElement;
        if (sub_6C683D(&x, &y, &z, direction & 3, type, 0, &tileElement, 0)) {
            _rideConstructionState = RIDE_CONSTRUCTION_STATE_0;
            window_ride_construction_update_active_elements();
            return;
        }

        // Invalidate previous track piece (we may not be changing height!)
        virtual_floor_invalidate();

        CoordsXYE inputElement, outputElement;
        inputElement.x = x;
        inputElement.y = y;
        inputElement.element = tileElement;
        if (track_block_get_next(&inputElement, &outputElement, &z, &direction)) {
            x = outputElement.x;
            y = outputElement.y;
            tileElement = outputElement.element;
            if (!scenery_tool_is_active())
            {
                // Set next element's height.
                virtual_floor_set_height(tileElement->base_height << 3);
            }
        } else {
            _rideConstructionState = RIDE_CONSTRUCTION_STATE_FRONT;
            _currentTrackBeginX = outputElement.x;
            _currentTrackBeginY = outputElement.y;
            _currentTrackBeginZ = z;
            _currentTrackPieceDirection = direction;
            _currentTrackPieceType = track_element_get_type(tileElement);
            _currentTrackSelectionFlags = 0;
            _rideConstructionArrowPulseTime = 0;
            ride_construction_set_default_next_piece();
            window_ride_construction_update_active_elements();
            return;
        }

        _currentTrackBeginX = x;
        _currentTrackBeginY = y;
        _currentTrackBeginZ = z;
        _currentTrackPieceDirection = tile_element_get_direction(tileElement);
        _currentTrackPieceType = track_element_get_type(tileElement);
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
        window_ride_construction_update_active_elements();
    } else if (_rideConstructionState == RIDE_CONSTRUCTION_STATE_BACK) {
        if (ride_select_forwards_from_back()) {
            window_ride_construction_update_active_elements();
        }
    }
}

/**
 *
 *  rct2: 0x006C93B8
 */
void ride_select_previous_section()
{
    if (_rideConstructionState == RIDE_CONSTRUCTION_STATE_SELECTED) {
        ride_construction_invalidate_current_track();
        sint32 x = _currentTrackBeginX;
        sint32 y = _currentTrackBeginY;
        sint32 z = _currentTrackBeginZ;
        sint32 direction = _currentTrackPieceDirection;
        sint32 type = _currentTrackPieceType;
        rct_tile_element *tileElement;
        if (sub_6C683D(&x, &y, &z, direction & 3, type, 0, &tileElement, 0)) {
            _rideConstructionState = RIDE_CONSTRUCTION_STATE_0;
            window_ride_construction_update_active_elements();
            return;
        }

        // Invalidate previous track piece (we may not be changing height!)
        virtual_floor_invalidate();

        track_begin_end trackBeginEnd;
        if (track_block_get_previous(x, y, tileElement, &trackBeginEnd)) {
            _currentTrackBeginX = trackBeginEnd.begin_x;
            _currentTrackBeginY = trackBeginEnd.begin_y;
            _currentTrackBeginZ = trackBeginEnd.begin_z;
            _currentTrackPieceDirection = trackBeginEnd.begin_direction;
            _currentTrackPieceType = track_element_get_type(trackBeginEnd.begin_element);
            _currentTrackSelectionFlags = 0;
            _rideConstructionArrowPulseTime = 0;
            if (!scenery_tool_is_active())
            {
                // Set previous element's height.
                virtual_floor_set_height(trackBeginEnd.begin_element->base_height << 3);
            }
            window_ride_construction_update_active_elements();
        } else {
            _rideConstructionState = RIDE_CONSTRUCTION_STATE_BACK;
            _currentTrackBeginX = trackBeginEnd.end_x;
            _currentTrackBeginY = trackBeginEnd.end_y;
            _currentTrackBeginZ = trackBeginEnd.begin_z;
            _currentTrackPieceDirection = trackBeginEnd.end_direction;
            _currentTrackPieceType = track_element_get_type(tileElement);
            _currentTrackSelectionFlags = 0;
            _rideConstructionArrowPulseTime = 0;
            ride_construction_set_default_next_piece();
            window_ride_construction_update_active_elements();
        }
    } else if (_rideConstructionState == RIDE_CONSTRUCTION_STATE_FRONT) {
        if (ride_select_backwards_from_front()) {
            window_ride_construction_update_active_elements();
        }
    }
}

/**
 *
 *  rct2: 0x006CC2CA
 */
static sint32 ride_modify_entrance_or_exit(rct_tile_element *tileElement, sint32 x, sint32 y)
{
    sint32 rideIndex, entranceType;
    rct_window *constructionWindow;

    rideIndex = tileElement->properties.entrance.ride_index;

    entranceType = tileElement->properties.entrance.type;
    if (entranceType != ENTRANCE_TYPE_RIDE_ENTRANCE && entranceType != ENTRANCE_TYPE_RIDE_EXIT)
        return 0;

    sint32 bl = (tileElement->properties.entrance.index & 0x70) >> 4;

    // Get or create construction window for ride
    constructionWindow = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (constructionWindow == nullptr) {
        if (!ride_initialise_construction_window(rideIndex))
            return 0;

        constructionWindow = window_find_by_class(WC_RIDE_CONSTRUCTION);
        if (constructionWindow == nullptr)
            return 0;
    }

    ride_construction_invalidate_current_track();
    if (
        _rideConstructionState != RIDE_CONSTRUCTION_STATE_ENTRANCE_EXIT ||
        !(input_test_flag(INPUT_FLAG_TOOL_ACTIVE)) ||
        gCurrentToolWidget.window_classification != WC_RIDE_CONSTRUCTION
    ) {
        // Replace entrance / exit
        tool_set(constructionWindow, entranceType == ENTRANCE_TYPE_RIDE_ENTRANCE ? WC_RIDE_CONSTRUCTION__WIDX_ENTRANCE : WC_RIDE_CONSTRUCTION__WIDX_EXIT, TOOL_CROSSHAIR);
        gRideEntranceExitPlaceType = entranceType;
        gRideEntranceExitPlaceRideIndex = rideIndex;
        gRideEntranceExitPlaceStationIndex = bl;
        input_set_flag(INPUT_FLAG_6, true);
        if (_rideConstructionState != RIDE_CONSTRUCTION_STATE_ENTRANCE_EXIT) {
            gRideEntranceExitPlacePreviousRideConstructionState = _rideConstructionState;
            _rideConstructionState = RIDE_CONSTRUCTION_STATE_ENTRANCE_EXIT;
        }

        window_ride_construction_update_active_elements();
        gMapSelectFlags &= ~MAP_SELECT_FLAG_ENABLE_CONSTRUCT;
    } else {
        // Remove entrance / exit
        game_do_command(x, (GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED | GAME_COMMAND_FLAG_APPLY), y, rideIndex, GAME_COMMAND_REMOVE_RIDE_ENTRANCE_OR_EXIT, bl, 0);
        gCurrentToolWidget.widget_index = entranceType == ENTRANCE_TYPE_RIDE_ENTRANCE ? WC_RIDE_CONSTRUCTION__WIDX_ENTRANCE : WC_RIDE_CONSTRUCTION__WIDX_EXIT;
        gRideEntranceExitPlaceType = entranceType;
    }

    window_invalidate_by_class(WC_RIDE_CONSTRUCTION);
    return 1;
}

/**
 *
 *  rct2: 0x006CC287
 */
static sint32 ride_modify_maze(rct_tile_element *tileElement, sint32 x, sint32 y)
{
    _currentRideIndex = track_element_get_ride_index(tileElement);
    _rideConstructionState = RIDE_CONSTRUCTION_STATE_MAZE_BUILD;
    _currentTrackBeginX = x;
    _currentTrackBeginY = y;
    _currentTrackBeginZ = tileElement->base_height * 8;
    _currentTrackSelectionFlags = 0;
    _rideConstructionArrowPulseTime = 0;

    auto intent = Intent(INTENT_ACTION_UPDATE_MAZE_CONSTRUCTION);
    context_broadcast_intent(&intent);

    return 1;
}

/**
 *
 *  rct2: 0x006CC056
 */
sint32 ride_modify(CoordsXYE *input)
{
    sint32 rideIndex, x, y, z, direction, type;
    CoordsXYE tileElement, endOfTrackElement;
    Ride *ride;
    rct_ride_entry *rideEntry;

    tileElement = *input;
    rideIndex = track_element_get_ride_index(tileElement.element);
    ride = get_ride(rideIndex);
    if (ride == nullptr) {
        return 0;
    }
    rideEntry = get_ride_entry_by_ride(ride);

    if ((rideEntry == nullptr) || !ride_check_if_construction_allowed(ride))
        return 0;

    if (ride->lifecycle_flags & RIDE_LIFECYCLE_INDESTRUCTIBLE) {
        set_format_arg(6, rct_string_id, ride->name);
        set_format_arg(8, uint32, ride->name_arguments);
        context_show_error(STR_CANT_START_CONSTRUCTION_ON, STR_LOCAL_AUTHORITY_FORBIDS_DEMOLITION_OR_MODIFICATIONS_TO_THIS_RIDE);
        return 0;
    }

    // Stop the ride again to clear all vehicles and peeps (compatible with network games)
    ride_set_status(rideIndex, RIDE_STATUS_CLOSED);

    // Check if element is a station entrance or exit
    if (tile_element_get_type(tileElement.element) == TILE_ELEMENT_TYPE_ENTRANCE)
        return ride_modify_entrance_or_exit(tileElement.element, tileElement.x, tileElement.y);

    ride_create_or_find_construction_window(rideIndex);

    if (ride->type == RIDE_TYPE_MAZE)
        return ride_modify_maze(tileElement.element, tileElement.x, tileElement.y);

    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_CANNOT_HAVE_GAPS)) {
        if (ride_find_track_gap(&tileElement, &endOfTrackElement))
            tileElement = endOfTrackElement;
    }

    x = tileElement.x;
    y = tileElement.y;
    z = tileElement.element->base_height * 8;
    direction = tile_element_get_direction(tileElement.element);
    type = track_element_get_type(tileElement.element);

    if (sub_6C683D(&x, &y, &z, direction, type, 0, nullptr, 0))
        return 0;

    _currentRideIndex = rideIndex;
    _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
    _currentTrackBeginX = x;
    _currentTrackBeginY = y;
    _currentTrackBeginZ = z;
    _currentTrackPieceDirection = direction;
    _currentTrackPieceType = type;
    _currentTrackSelectionFlags = 0;
    _rideConstructionArrowPulseTime = 0;

    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_NO_TRACK)) {
        window_ride_construction_update_active_elements();
        return 1;
    }

    ride_select_next_section();
    if (_rideConstructionState == RIDE_CONSTRUCTION_STATE_FRONT) {
        window_ride_construction_update_active_elements();
        return 1;
    }

    _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
    _currentTrackBeginX = x;
    _currentTrackBeginY = y;
    _currentTrackBeginZ = z;
    _currentTrackPieceDirection = direction;
    _currentTrackPieceType = type;
    _currentTrackSelectionFlags = 0;
    _rideConstructionArrowPulseTime = 0;

    ride_select_previous_section();

    if (_rideConstructionState != RIDE_CONSTRUCTION_STATE_BACK) {
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
        _currentTrackBeginX = x;
        _currentTrackBeginY = y;
        _currentTrackBeginZ = z;
        _currentTrackPieceDirection = direction;
        _currentTrackPieceType = type;
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
    }

    window_ride_construction_update_active_elements();
    return 1;
}

/**
 *
 *  rct2: 0x006CC3FB
 */
sint32 ride_initialise_construction_window(sint32 rideIndex)
{
    Ride *ride;
    rct_window *w;

    tool_cancel();
    ride = get_ride(rideIndex);

    if (!ride_check_if_construction_allowed(ride))
        return 0;

    ride_clear_for_construction(rideIndex);
    ride_remove_peeps(rideIndex);

    w = ride_create_or_find_construction_window(rideIndex);

    tool_set(w, WC_RIDE_CONSTRUCTION__WIDX_CONSTRUCT, TOOL_CROSSHAIR);
    input_set_flag(INPUT_FLAG_6, true);

    ride = get_ride(_currentRideIndex);

    _currentTrackCurve = RideConstructionDefaultTrackType[ride->type] | 0x100;
    _currentTrackSlopeEnd = 0;
    _currentTrackBankEnd = 0;
    _currentTrackLiftHill = 0;
    _currentTrackAlternative = RIDE_TYPE_NO_ALTERNATIVES;

    if (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_START_CONSTRUCTION_INVERTED)
        _currentTrackAlternative |= RIDE_TYPE_ALTERNATIVE_TRACK_TYPE;

    _previousTrackBankEnd = 0;
    _previousTrackSlopeEnd = 0;

    _currentTrackPieceDirection = 0;
    _rideConstructionState = RIDE_CONSTRUCTION_STATE_PLACE;
    _currentTrackSelectionFlags = 0;
    _rideConstructionArrowPulseTime = 0;

    window_ride_construction_update_active_elements();
    return 1;
}

#pragma endregion

#pragma region Update functions

/**
 *
 *  rct2: 0x006ABE4C
 */
void ride_update_all()
{
    Ride *ride;
    sint32 i;

    // Remove all rides if scenario editor
    if (gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR) {
        if (gS6Info.editor_step <= EDITOR_STEP_INVENTIONS_LIST_SET_UP)
            FOR_ALL_RIDES(i, ride)
                ride->type = RIDE_TYPE_NULL;
        return;
    }

    window_update_viewport_ride_music();

    // Update rides
    FOR_ALL_RIDES(i, ride)
        ride_update(i);

    ride_music_update_final();
}

/**
 *
 *  rct2: 0x006ABE73
 */
static void ride_update(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    if (ride->vehicle_change_timeout != 0)
        ride->vehicle_change_timeout--;

    ride_music_update(rideIndex);

    // Update stations
    if (ride->type != RIDE_TYPE_MAZE)
        for (sint32 i = 0; i < MAX_STATIONS; i++)
            ride_update_station(ride, i);

    // Update financial statistics
    ride->num_customers_timeout++;

    if (ride->num_customers_timeout >= 960) {
        // This is meant to update about every 30 seconds
        ride->num_customers_timeout = 0;

        // Shift number of customers history, start of the array is the most recent one
        for (sint32 i = CUSTOMER_HISTORY_SIZE - 1; i > 0; i--)
        {
            ride->num_customers[i] = ride->num_customers[i - 1];
        }
        ride->num_customers[0] = ride->cur_num_customers;

        ride->cur_num_customers = 0;
        ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_CUSTOMER;

        ride->income_per_hour = ride_calculate_income_per_hour(ride);
        ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_INCOME;

        if (ride->upkeep_cost != MONEY16_UNDEFINED)
            ride->profit = (ride->income_per_hour - ((money32)ride->upkeep_cost * 16));
    }

    // Ride specific updates
    if (ride->type == RIDE_TYPE_CHAIRLIFT)
        ride_chairlift_update(ride);
    else if (ride->type == RIDE_TYPE_SPIRAL_SLIDE)
        ride_spiral_slide_update(ride);

    ride_breakdown_update(rideIndex);

    // Various things include news messages
    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_DUE_INSPECTION))
        if (((gCurrentTicks >> 1) & 255) == (uint32)rideIndex)
            ride_breakdown_status_update(rideIndex);

    ride_inspection_update(ride);

    if (ride->status == RIDE_STATUS_TESTING && gConfigGeneral.no_test_crashes) {
        for (sint32 i = 0; i < ride->num_vehicles; i++) {
            uint16 spriteIndex = ride->vehicles[i];
            if (spriteIndex == SPRITE_INDEX_NULL)
                continue;

            rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);

            if (vehicle->status == VEHICLE_STATUS_CRASHED || vehicle->status == VEHICLE_STATUS_CRASHING) {
                ride_set_status(rideIndex, RIDE_STATUS_CLOSED);
                ride_set_status(rideIndex, RIDE_STATUS_CLOSED);
                ride_set_status(rideIndex, RIDE_STATUS_TESTING);
                break;
            }
        }
    }
}

/**
 *
 *  rct2: 0x006AC489
 */
static void ride_chairlift_update(Ride *ride)
{
    sint32 x, y, z;

    if (!(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK))
        return;
    if ((ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_CRASHED)) &&
        ride->breakdown_reason_pending == 0)
        return;

    uint16 old_chairlift_bullwheel_rotation = ride->chairlift_bullwheel_rotation >> 14;
    ride->chairlift_bullwheel_rotation += ride->speed * 2048;
    if (old_chairlift_bullwheel_rotation == ride->speed / 8)
        return;

    x = ride->chairlift_bullwheel_location[0].x * 32;
    y = ride->chairlift_bullwheel_location[0].y * 32;
    z = ride->chairlift_bullwheel_z[0] * 8;
    map_invalidate_tile_zoom1(x, y, z, z + (4 * 8));

    x = ride->chairlift_bullwheel_location[1].x * 32;
    y = ride->chairlift_bullwheel_location[1].y * 32;
    z = ride->chairlift_bullwheel_z[1] * 8;
    map_invalidate_tile_zoom1(x, y, z, z + (4 * 8));
}

/**
 *
 *  rct2: 0x0069A3A2
 * edi: ride (in code as bytes offset from start of rides list)
 * bl: happiness
 */
void ride_update_satisfaction(Ride* ride, uint8 happiness) {
    ride->satisfaction_next += happiness;
    ride->satisfaction_time_out++;
    if (ride->satisfaction_time_out >= 20) {
        ride->satisfaction = ride->satisfaction_next >> 2;
        ride->satisfaction_next = 0;
        ride->satisfaction_time_out = 0;
        ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_CUSTOMER;

    }
}

/**
 *
 *  rct2: 0x0069A3D7
 * Updates the ride popularity
 * edi : ride
 * bl  : pop_amount
 * pop_amount can be zero if peep visited but did not purchase.
 */
void ride_update_popularity(Ride* ride, uint8 pop_amount){
    ride->popularity_next += pop_amount;
    ride->popularity_time_out++;
    if (ride->popularity_time_out < 25)return;

    ride->popularity = ride->popularity_next;
    ride->popularity_next = 0;
    ride->popularity_time_out = 0;
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_CUSTOMER;
}

/** rct2: 0x0098DDB8, 0x0098DDBA */
static constexpr const LocationXY16 ride_spiral_slide_main_tile_offset[][4] = {
    {
        {  32,  32 },
        {   0,  32 },
        {   0,   0 },
        {  32,   0 },
    }, {
        {  32,   0 },
        {   0,   0 },
        {   0, -32 },
        {  32, -32 },
    }, {
        {   0,   0 },
        { -32,   0 },
        { -32, -32 },
        {   0, -32 },
    }, {
        {   0,   0 },
        {   0,  32 },
        { -32,  32 },
        { -32,   0 },
    },
};

/**
 *
 *  rct2: 0x006AC545
 */
static void ride_spiral_slide_update(Ride *ride)
{
    if (gCurrentTicks & 3)
        return;
    if (ride->slide_in_use == 0)
        return;

    ride->spiral_slide_progress++;
    if (ride->spiral_slide_progress >= 48) {
        ride->slide_in_use--;

        rct_peep *peep = GET_PEEP(ride->slide_peep);
        peep->destination_x++;
    }

    const uint8 current_rotation = get_current_rotation();
    // Invalidate something related to station start
    for (sint32 i = 0; i < MAX_STATIONS; i++) {
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
            continue;

        sint32 x = ride->station_starts[i].x;
        sint32 y = ride->station_starts[i].y;

        rct_tile_element *tileElement = ride_get_station_start_track_element(ride, i);
        if (tileElement == nullptr)
            continue;

        sint32 rotation = tile_element_get_direction(tileElement);
        x *= 32;
        y *= 32;
        x += ride_spiral_slide_main_tile_offset[rotation][current_rotation].x;
        y += ride_spiral_slide_main_tile_offset[rotation][current_rotation].y;

        map_invalidate_tile_zoom0(x, y, tileElement->base_height * 8, tileElement->clearance_height * 8);
    }
}

#pragma endregion

#pragma region Breakdown and inspection functions

static uint8 _breakdownProblemProbabilities[] = {
    25,     // BREAKDOWN_SAFETY_CUT_OUT
    12,     // BREAKDOWN_RESTRAINTS_STUCK_CLOSED
    10,     // BREAKDOWN_RESTRAINTS_STUCK_OPEN
    13,     // BREAKDOWN_DOORS_STUCK_CLOSED
    10,     // BREAKDOWN_DOORS_STUCK_OPEN
    6,      // BREAKDOWN_VEHICLE_MALFUNCTION
    0,      // BREAKDOWN_BRAKES_FAILURE
    3       // BREAKDOWN_CONTROL_FAILURE
};

/**
 *
 *  rct2: 0x006AC7C2
 */
static void ride_inspection_update(Ride *ride)
{
    if (gCurrentTicks & 2047)
        return;
    if (gScreenFlags & SCREEN_FLAGS_TRACK_DESIGNER)
        return;

    ride->last_inspection++;
    if (ride->last_inspection == 0)
        ride->last_inspection--;

    sint32 inspectionIntervalMinutes = RideInspectionInterval[ride->inspection_interval];
    if (inspectionIntervalMinutes == 0)
        return;

    if (RideAvailableBreakdowns[ride->type] == 0)
        return;

    if (inspectionIntervalMinutes > ride->last_inspection)
        return;

    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_DUE_INSPECTION | RIDE_LIFECYCLE_CRASHED))
        return;

    // Inspect the first station that has an exit
    ride->lifecycle_flags |= RIDE_LIFECYCLE_DUE_INSPECTION;
    ride->mechanic_status = RIDE_MECHANIC_STATUS_CALLING;

    sint8 stationIndex = ride_get_first_valid_station_exit(ride);
    ride->inspection_station = (stationIndex != -1) ? stationIndex : 0;
}

static sint32 get_age_penalty(Ride *ride) {
    sint32 years;
    years = date_get_year(gDateMonthsElapsed - ride->build_date);
    switch (years) {
    case 0:
        return 0;
    case 1:
        return ride->unreliability_factor / 8;
    case 2:
        return ride->unreliability_factor / 4;
    case 3:
    case 4:
        return ride->unreliability_factor / 2;
    case 5:
    case 6:
    case 7:
        return ride->unreliability_factor;
    default:
        return ride->unreliability_factor * 2;
    }
}

/**
 *
 *  rct2: 0x006AC622
 */
static void ride_breakdown_update(sint32 rideIndex)
{
    if (gCurrentTicks & 255)
        return;
    if (gScreenFlags & SCREEN_FLAGS_TRACK_DESIGNER)
        return;

    Ride *ride = get_ride(rideIndex);
    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_CRASHED))
        ride->downtime_history[0]++;

    if (!(gCurrentTicks & 8191))
    {
        sint32 totalDowntime = 0;

        for (sint32 i = 0; i < DOWNTIME_HISTORY_SIZE; i++)
        {
            totalDowntime += ride->downtime_history[i];
        }

        ride->downtime = Math::Min(totalDowntime / 2, 100);

        for (sint32 i = DOWNTIME_HISTORY_SIZE - 1; i > 0; i--)
        {
            ride->downtime_history[i] = ride->downtime_history[i - 1];
        }
        ride->downtime_history[0] = 0;

        ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE;
    }

    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_CRASHED))
        return;
    if (ride->status == RIDE_STATUS_CLOSED)
        return;

    // Calculate breakdown probability?
    sint32 unreliabilityAccumulator = ride->unreliability_factor + get_age_penalty(ride);
    ride->reliability = (uint16) Math::Max(0, (ride->reliability - unreliabilityAccumulator));
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE;

    // Random probability of a breakdown. Roughly this is 1 in
    //
    // (25000 - reliability) / 3 000 000
    //
    // a 0.8% chance, less the breakdown factor which accumulates as the game
    // continues.
    if ((ride->reliability == 0 || (sint32)(scenario_rand() & 0x2FFFFF) <= 1 + RIDE_INITIAL_RELIABILITY - ride->reliability)
            && !gCheatsDisableAllBreakdowns) {
        sint32 breakdownReason = ride_get_new_breakdown_problem(ride);
        if (breakdownReason != -1)
            ride_prepare_breakdown(rideIndex, breakdownReason);
    }
}

/**
 *
 *  rct2: 0x006B7294
 */
static sint32 ride_get_new_breakdown_problem(Ride *ride)
{
    sint32 availableBreakdownProblems, monthsOld, totalProbability, randomProbability, problemBits, breakdownProblem;
    rct_ride_entry *entry;

    // Brake failure is more likely when it's raining
    _breakdownProblemProbabilities[BREAKDOWN_BRAKES_FAILURE] = climate_is_raining() ? 20 : 3;

    entry = get_ride_entry_by_ride(ride);
    if (entry == nullptr || entry->flags & RIDE_ENTRY_FLAG_CANNOT_BREAK_DOWN)
        return -1;

    availableBreakdownProblems = RideAvailableBreakdowns[ride->type];

    // Calculate the total probability range for all possible breakdown problems
    totalProbability = 0;
    problemBits = availableBreakdownProblems;
    while (problemBits != 0) {
        breakdownProblem = bitscanforward(problemBits);
        problemBits &= ~(1 << breakdownProblem);
        totalProbability += _breakdownProblemProbabilities[breakdownProblem];
    }
    if (totalProbability == 0)
        return -1;

    // Choose a random number within this range
    randomProbability = scenario_rand() % totalProbability;

    // Find which problem range the random number lies
    problemBits = availableBreakdownProblems;
    do {
        breakdownProblem = bitscanforward(problemBits);
        problemBits &= ~(1 << breakdownProblem);
        randomProbability -= _breakdownProblemProbabilities[breakdownProblem];
    } while (randomProbability >= 0);

    if (breakdownProblem != BREAKDOWN_BRAKES_FAILURE)
        return breakdownProblem;

    // Brakes failure can not happen if block brakes are used (so long as there is more than one vehicle)
    // However if this is the case, brake failure should be taken out the equation, otherwise block brake
    // rides have a lower probability to break down due to a random implementation reason.
    if (ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED || ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED)
        if (ride->num_vehicles != 1)
            return -1;

    // If brakes failure is disabled, also take it out of the equation (see above comment why)
    if (gCheatsDisableBrakesFailure)
        return -1;

    monthsOld = gDateMonthsElapsed - ride->build_date;
    if (monthsOld < 16 || ride->reliability_percentage > 50)
        return -1;

    return BREAKDOWN_BRAKES_FAILURE;
}

static void choose_random_train_to_breakdown_safe(Ride * ride)
{
    ride->broken_vehicle = scenario_rand() % ride->num_vehicles;

    // Prevent crash caused by accessing SPRITE_INDEX_NULL on hacked rides.
    // This should probably be cleaned up on import instead.
    while (ride->vehicles[ride->broken_vehicle] == SPRITE_INDEX_NULL && ride->broken_vehicle != 0)
    {
        --ride->broken_vehicle;
    }
}

/**
 *
 *  rct2: 0x006B7348
 */
void ride_prepare_breakdown(sint32 rideIndex, sint32 breakdownReason)
{
    sint32 i;
    Ride *ride;
    rct_vehicle *vehicle;

    ride = get_ride(rideIndex);
    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_CRASHED))
        return;

    ride->lifecycle_flags |= RIDE_LIFECYCLE_BREAKDOWN_PENDING;

    ride->breakdown_reason_pending = breakdownReason;
    ride->breakdown_sound_modifier = 0;
    ride->not_fixed_timeout = 0;

    switch (breakdownReason) {
    case BREAKDOWN_SAFETY_CUT_OUT:
    case BREAKDOWN_CONTROL_FAILURE:
        // Inspect first station with an exit
        i = ride_get_first_valid_station_exit(ride);
        if (i != -1)
        {
            ride->inspection_station = i;
        }

        break;
    case BREAKDOWN_RESTRAINTS_STUCK_CLOSED:
    case BREAKDOWN_RESTRAINTS_STUCK_OPEN:
    case BREAKDOWN_DOORS_STUCK_CLOSED:
    case BREAKDOWN_DOORS_STUCK_OPEN:
        // Choose a random train and car
        choose_random_train_to_breakdown_safe(ride);
        ride->broken_car = scenario_rand() % ride->num_cars_per_train;

        // Set flag on broken car
        vehicle = GET_VEHICLE(ride->vehicles[ride->broken_vehicle]);
        for (i = ride->broken_car; i > 0; i--) {
            if (vehicle->next_vehicle_on_train == SPRITE_INDEX_NULL) {
                vehicle = nullptr;
                break;
            }
            else {
                vehicle = GET_VEHICLE(vehicle->next_vehicle_on_train);
            }
        }
        if (vehicle != nullptr)
            vehicle->update_flags |= VEHICLE_UPDATE_FLAG_BROKEN_CAR;
        break;
    case BREAKDOWN_VEHICLE_MALFUNCTION:
        // Choose a random train
        choose_random_train_to_breakdown_safe(ride);
        ride->broken_car = 0;

        // Set flag on broken train, first car
        vehicle = GET_VEHICLE(ride->vehicles[ride->broken_vehicle]);
        vehicle->update_flags |= VEHICLE_UPDATE_FLAG_BROKEN_TRAIN;
        break;
    case BREAKDOWN_BRAKES_FAILURE:
        // Original code generates a random number but does not use it
        // Unsure if this was supposed to choose a random station (or random station with an exit)
        i = ride_get_first_valid_station_exit(ride);
        if (i != -1)
        {
            ride->inspection_station = i;
        }
        break;
    }
}

/**
 *
 *  rct2: 0x006B74FA
 */
void ride_breakdown_add_news_item(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    set_format_arg(0, rct_string_id, ride->name);
    set_format_arg(2, uint32, ride->name_arguments);
    if (gConfigNotifications.ride_broken_down) {
        news_item_add_to_queue(NEWS_ITEM_RIDE, STR_RIDE_IS_BROKEN_DOWN, rideIndex);
    }
}

/**
 *
 *  rct2: 0x006B75C8
 */
static void ride_breakdown_status_update(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    // Warn player if ride hasn't been fixed for ages
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
        ride->not_fixed_timeout++;
        // When there has been a full 255 timeout ticks this
        // will force timeout ticks to keep issuing news every
        // 16 ticks. Note there is no reason to do this.
        if (ride->not_fixed_timeout == 0)
            ride->not_fixed_timeout -= 16;

        if (
            !(ride->not_fixed_timeout & 15) &&
            ride->mechanic_status != RIDE_MECHANIC_STATUS_FIXING &&
            ride->mechanic_status != RIDE_MECHANIC_STATUS_HAS_FIXED_STATION_BRAKES
        ) {
            set_format_arg(0, rct_string_id, ride->name);
            set_format_arg(2, uint32, ride->name_arguments);
            if (gConfigNotifications.ride_warnings) {
                news_item_add_to_queue(NEWS_ITEM_RIDE, STR_RIDE_IS_STILL_NOT_FIXED, rideIndex);
            }
        }
    }

    ride_mechanic_status_update(rideIndex, ride->mechanic_status);
}

/**
 *
 *  rct2: 0x006B762F
 */
static void ride_mechanic_status_update(sint32 rideIndex, sint32 mechanicStatus)
{
    sint32 breakdownReason;
    Ride *ride;
    rct_peep *mechanic;

    ride = get_ride(rideIndex);

    // Turn a pending breakdown into a breakdown.
    if ((mechanicStatus == RIDE_MECHANIC_STATUS_UNDEFINED ||
        mechanicStatus == RIDE_MECHANIC_STATUS_CALLING ||
        mechanicStatus == RIDE_MECHANIC_STATUS_HEADING) &&
        (ride->lifecycle_flags & RIDE_LIFECYCLE_BREAKDOWN_PENDING) &&
        !(ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN))
    {
        breakdownReason = ride->breakdown_reason_pending;
        if (
            breakdownReason == BREAKDOWN_SAFETY_CUT_OUT ||
            breakdownReason == BREAKDOWN_BRAKES_FAILURE ||
            breakdownReason == BREAKDOWN_CONTROL_FAILURE
        ) {
            ride->lifecycle_flags |= RIDE_LIFECYCLE_BROKEN_DOWN;
            ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE | RIDE_INVALIDATE_RIDE_LIST | RIDE_INVALIDATE_RIDE_MAIN;
            ride->breakdown_reason = breakdownReason;
            ride_breakdown_add_news_item(rideIndex);
        }
    }
    switch (mechanicStatus) {
    case RIDE_MECHANIC_STATUS_UNDEFINED:
        if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN)
        {
            ride->mechanic_status = RIDE_MECHANIC_STATUS_CALLING;
        }
        break;
    case RIDE_MECHANIC_STATUS_CALLING:
        if (RideAvailableBreakdowns[ride->type] == 0) {
            ride->lifecycle_flags &= ~(RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN | RIDE_LIFECYCLE_DUE_INSPECTION);
            break;
        }

        ride_call_closest_mechanic(rideIndex);
        break;
    case RIDE_MECHANIC_STATUS_HEADING:
        mechanic = nullptr;
        if (ride->mechanic != SPRITE_INDEX_NULL) {
            mechanic = &(get_sprite(ride->mechanic)->peep);
        }
        if (mechanic == nullptr ||
            !peep_is_mechanic(mechanic) ||
            (mechanic->state != PEEP_STATE_HEADING_TO_INSPECTION && mechanic->state != PEEP_STATE_ANSWERING) ||
            mechanic->current_ride != rideIndex
        ) {
            ride->mechanic_status = RIDE_MECHANIC_STATUS_CALLING;
            ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE;
            ride_mechanic_status_update(rideIndex, RIDE_MECHANIC_STATUS_CALLING);
        }
        break;
    case RIDE_MECHANIC_STATUS_FIXING:
        mechanic = nullptr;
        if (ride->mechanic != SPRITE_INDEX_NULL) {
            mechanic = &(get_sprite(ride->mechanic)->peep);
        }
        if (mechanic == nullptr ||
            !peep_is_mechanic(mechanic) ||
            (
                mechanic->state != PEEP_STATE_HEADING_TO_INSPECTION &&
                mechanic->state != PEEP_STATE_FIXING &&
                mechanic->state != PEEP_STATE_INSPECTING &&
                mechanic->state != PEEP_STATE_ANSWERING
            )
        ) {
            ride->mechanic_status = RIDE_MECHANIC_STATUS_CALLING;
            ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE;
            ride_mechanic_status_update(rideIndex, RIDE_MECHANIC_STATUS_CALLING);
        }
        break;
    }
}

/**
 *
 *  rct2: 0x006B796C
 */
static void ride_call_mechanic(sint32 rideIndex, rct_peep *mechanic, sint32 forInspection)
{
    Ride *ride;

    ride = get_ride(rideIndex);
    peep_decrement_num_riders(mechanic);
    mechanic->state = forInspection ? PEEP_STATE_HEADING_TO_INSPECTION : PEEP_STATE_ANSWERING;
    peep_window_state_update(mechanic);
    mechanic->sub_state = 0;
    ride->mechanic_status = RIDE_MECHANIC_STATUS_HEADING;
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAINTENANCE;
    ride->mechanic = mechanic->sprite_index;
    mechanic->current_ride = rideIndex;
    mechanic->current_ride_station = ride->inspection_station;
}

/**
 *
 *  rct2: 0x006B76AB
 */
static void ride_call_closest_mechanic(sint32 rideIndex)
{
    Ride *ride;
    rct_peep *mechanic;
    sint32 forInspection;

    ride = get_ride(rideIndex);
    forInspection = (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN)) == 0;

    mechanic = ride_find_closest_mechanic(ride, forInspection);
    if (mechanic != nullptr)
        ride_call_mechanic(rideIndex, mechanic, forInspection);
}

rct_peep *ride_find_closest_mechanic(Ride *ride, sint32 forInspection)
{
    sint32 x, y, z, stationIndex;
    TileCoordsXYZD location;
    rct_tile_element *tileElement;

    // Get either exit position or entrance position if there is no exit
    stationIndex = ride->inspection_station;
    location = ride_get_exit_location(ride, stationIndex);
    if (location.isNull()) {
        location = ride_get_entrance_location(ride, stationIndex);
        if (location.isNull())
            return nullptr;
    }

    // Get station start track element and position
    x = location.x;
    y = location.y;
    z = location.z;
    tileElement = ride_get_station_exit_element(ride, x, y, z);
    if (tileElement == nullptr)
        return nullptr;

    x *= 32;
    y *= 32;

    // Set x,y to centre of the station exit for the mechanic search.
    x += 16;
    y += 16;

    return find_closest_mechanic(x, y, forInspection);
}

/**
 *
 *  rct2: 0x006B774B (forInspection = 0)
 *  rct2: 0x006B78C3 (forInspection = 1)
 */
rct_peep *find_closest_mechanic(sint32 x, sint32 y, sint32 forInspection)
{
    uint32 closestDistance, distance;
    uint16 spriteIndex;
    rct_peep *peep, *closestMechanic = nullptr;

    closestDistance = UINT_MAX;
    FOR_ALL_STAFF(spriteIndex, peep) {
        if (peep->staff_type != STAFF_TYPE_MECHANIC)
            continue;

        if (!forInspection) {
            if (peep->state == PEEP_STATE_HEADING_TO_INSPECTION){
                if (peep->sub_state >= 4)
                    continue;
            }
            else if (peep->state != PEEP_STATE_PATROLLING)
                continue;

            if (!(peep->staff_orders & STAFF_ORDERS_FIX_RIDES))
                continue;
        } else {
            if (peep->state != PEEP_STATE_PATROLLING || !(peep->staff_orders & STAFF_ORDERS_INSPECT_RIDES))
                continue;
        }

        if (map_is_location_in_park(x, y))
            if (!staff_is_location_in_patrol(peep, x & 0xFFE0, y & 0xFFE0))
                continue;

        if (peep->x == LOCATION_NULL)
            continue;

        // Manhattan distance
        distance = std::abs(peep->x - x) + std::abs(peep->y - y);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestMechanic = peep;
        }
    }

    return closestMechanic;
}

rct_peep *ride_get_assigned_mechanic(Ride *ride)
{
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
        if (
            ride->mechanic_status == RIDE_MECHANIC_STATUS_HEADING ||
            ride->mechanic_status == RIDE_MECHANIC_STATUS_FIXING ||
            ride->mechanic_status == RIDE_MECHANIC_STATUS_HAS_FIXED_STATION_BRAKES
        ) {
            rct_peep *peep = &(get_sprite(ride->mechanic)->peep);
            if (peep_is_mechanic(peep))
                return peep;
        }
    }

    return nullptr;
}

#pragma endregion

#pragma region Music functions

/**
 *
 *  rct2: 0x006ABE85
 */
static void ride_music_update(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    if (
        !(RideData4[ride->type].flags & RIDE_TYPE_FLAG4_MUSIC_ON_DEFAULT) &&
        !(RideData4[ride->type].flags & RIDE_TYPE_FLAG4_ALLOW_MUSIC)
    ) {
        return;
    }

    if (ride->status != RIDE_STATUS_OPEN || !(ride->lifecycle_flags & RIDE_LIFECYCLE_MUSIC)) {
        ride->music_tune_id = 255;
        return;
    }

    if (ride->type == RIDE_TYPE_CIRCUS) {
        rct_vehicle *vehicle = GET_VEHICLE(ride->vehicles[0]);
        if (vehicle->status != VEHICLE_STATUS_DOING_CIRCUS_SHOW) {
            ride->music_tune_id = 255;
            return;
        }
    }

    // Oscillate parameters for a power cut effect when breaking down
    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN)) {
        if (ride->breakdown_reason_pending == BREAKDOWN_CONTROL_FAILURE) {
            if (!(gCurrentTicks & 7))
                if (ride->breakdown_sound_modifier != 255)
                    ride->breakdown_sound_modifier++;
        } else {
            if (
                (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) ||
                ride->breakdown_reason_pending == BREAKDOWN_BRAKES_FAILURE ||
                ride->breakdown_reason_pending == BREAKDOWN_CONTROL_FAILURE
            ) {
                if (ride->breakdown_sound_modifier != 255)
                    ride->breakdown_sound_modifier++;
            }

            if (ride->breakdown_sound_modifier == 255) {
                ride->music_tune_id = 255;
                return;
            }
        }
    }

    // Select random tune from available tunes for a music style (of course only merry-go-rounds have more than one tune)
    if (ride->music_tune_id == 255) {
        const auto &musicStyleTunes = gRideMusicStyleTuneIds[ride->music];
        auto numTunes = musicStyleTunes.size();
        ride->music_tune_id = musicStyleTunes[util_rand() % numTunes];
        ride->music_position = 0;
        return;
    }

    sint32 x = ride->station_starts[0].x * 32 + 16;
    sint32 y = ride->station_starts[0].y * 32 + 16;
    sint32 z = ride->station_heights[0] * 8;

    sint32 sampleRate = 22050;

    // Alter sample rate for a power cut effect
    if (ride->lifecycle_flags & (RIDE_LIFECYCLE_BREAKDOWN_PENDING | RIDE_LIFECYCLE_BROKEN_DOWN)) {
        sampleRate = ride->breakdown_sound_modifier * 70;
        if (ride->breakdown_reason_pending != BREAKDOWN_CONTROL_FAILURE)
            sampleRate *= -1;
        sampleRate += 22050;
    }

    ride->music_position = ride_music_params_update(x, y, z, rideIndex, sampleRate, ride->music_position, &ride->music_tune_id);
}

#pragma endregion

#pragma region Measurement functions

/**
 *
 *  rct2: 0x006B642B
 */
void ride_measurement_clear(Ride *ride)
{
    rct_ride_measurement *measurement;

    if (ride->measurement_index == 255)
        return;

    measurement = get_ride_measurement(ride->measurement_index);
    measurement->ride_index = 255;
    ride->measurement_index = 255;
}

/**
 *
 *  rct2: 0x006B64F2
 */
static void ride_measurement_update(rct_ride_measurement *measurement)
{
    uint16 spriteIndex;
    Ride *ride;
    rct_vehicle *vehicle;
    sint32 velocity, altitude, verticalG, lateralG;

    ride = get_ride(measurement->ride_index);
    spriteIndex = ride->vehicles[measurement->vehicle_index];
    if (spriteIndex == SPRITE_INDEX_NULL)
        return;

    vehicle = GET_VEHICLE(spriteIndex);

    if (measurement->flags & RIDE_MEASUREMENT_FLAG_UNLOADING) {
        if (vehicle->status != VEHICLE_STATUS_DEPARTING && vehicle->status != VEHICLE_STATUS_TRAVELLING_CABLE_LIFT)
            return;

        measurement->flags &= ~RIDE_MEASUREMENT_FLAG_UNLOADING;
        if (measurement->current_station == vehicle->current_station)
            measurement->current_item = 0;
    }

    if (vehicle->status == VEHICLE_STATUS_UNLOADING_PASSENGERS) {
        measurement->flags |= RIDE_MEASUREMENT_FLAG_UNLOADING;
        return;
    }

    uint8 trackType = (vehicle->track_type >> 2) & 0xFF;
    if (trackType == TRACK_ELEM_BLOCK_BRAKES ||
        trackType == TRACK_ELEM_CABLE_LIFT_HILL ||
        trackType == TRACK_ELEM_25_DEG_UP_TO_FLAT ||
        trackType == TRACK_ELEM_60_DEG_UP_TO_FLAT ||
        trackType == TRACK_ELEM_DIAG_25_DEG_UP_TO_FLAT ||
        trackType == TRACK_ELEM_DIAG_60_DEG_UP_TO_FLAT)
        if (vehicle->velocity == 0)
            return;

    if (measurement->current_item >= RIDE_MEASUREMENT_MAX_ITEMS)
        return;

    if (measurement->flags & RIDE_MEASUREMENT_FLAG_G_FORCES) {
        vehicle_get_g_forces(vehicle, &verticalG, &lateralG);
        verticalG = Math::Clamp(-127, verticalG / 8, 127);
        lateralG = Math::Clamp(-127, lateralG / 8, 127);

        if (gScenarioTicks & 1) {
            verticalG = (verticalG + measurement->vertical[measurement->current_item]) / 2;
            lateralG = (lateralG + measurement->lateral[measurement->current_item]) / 2;
        }

        measurement->vertical[measurement->current_item] = verticalG & 0xFF;
        measurement->lateral[measurement->current_item] = lateralG & 0xFF;
    }

    velocity = Math::Min(std::abs((vehicle->velocity * 5) >> 16), 255);
    altitude = Math::Min(vehicle->z / 8, 255);

    if (gScenarioTicks & 1) {
        velocity = (velocity + measurement->velocity[measurement->current_item]) / 2;
        altitude = (altitude + measurement->altitude[measurement->current_item]) / 2;
    }

    measurement->velocity[measurement->current_item] = velocity & 0xFF;
    measurement->altitude[measurement->current_item] = altitude & 0xFF;

    if (gScenarioTicks & 1) {
        measurement->current_item++;
        measurement->num_items = Math::Max(measurement->num_items, measurement->current_item);
    }
}

/**
 *
 *  rct2: 0x006B6456
 */
void ride_measurements_update()
{
    if (gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR)
        return;

    // For each ride measurement
    for (sint32 i = 0; i < MAX_RIDE_MEASUREMENTS; i++) {
        rct_ride_measurement *measurement = get_ride_measurement(i);
        if (measurement->ride_index == 255)
            continue;

        Ride *ride = get_ride(measurement->ride_index);
        if (!(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK))
            continue;

        if (measurement->flags & RIDE_MEASUREMENT_FLAG_RUNNING) {
            ride_measurement_update(measurement);
        } else {
            // For each vehicle
            for (sint32 j = 0; j < ride->num_vehicles; j++) {
                uint16 spriteIndex = ride->vehicles[j];
                if (spriteIndex == SPRITE_INDEX_NULL)
                    continue;

                rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
                if (vehicle->status == VEHICLE_STATUS_DEPARTING || vehicle->status == VEHICLE_STATUS_TRAVELLING_CABLE_LIFT) {
                    measurement->vehicle_index = j;
                    measurement->current_station = vehicle->current_station;
                    measurement->flags |= RIDE_MEASUREMENT_FLAG_RUNNING;
                    measurement->flags &= ~RIDE_MEASUREMENT_FLAG_UNLOADING;
                    ride_measurement_update(measurement);
                    break;
                }
            }

        }
    }
}

static rct_ride_measurement *ride_get_existing_measurement(sint32 rideIndex)
{
    for (sint32 i = 0; i < MAX_RIDE_MEASUREMENTS; i++) {
        rct_ride_measurement *measurement = get_ride_measurement(i);
        if (measurement->ride_index == rideIndex)
            return measurement;
    }

    return nullptr;
}

static sint32 ride_get_free_measurement()
{
    for (sint32 i = 0; i < MAX_RIDE_MEASUREMENTS; i++) {
        rct_ride_measurement *measurement = get_ride_measurement(i);
        if (measurement->ride_index == 255)
            return i;
    }

    return -1;
}

/**
 *
 *  rct2: 0x006B66D9
 */
rct_ride_measurement *ride_get_measurement(sint32 rideIndex, rct_string_id *message)
{
    Ride *ride = get_ride(rideIndex);

    // Check if ride type supports data logging
    if (!ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_DATA_LOGGING)) {
        if (message != nullptr) *message = STR_DATA_LOGGING_NOT_AVAILABLE_FOR_THIS_TYPE_OF_RIDE;
        return nullptr;
    }

    // Check if a measurement already exists for this ride
    rct_ride_measurement *measurement = ride_get_existing_measurement(rideIndex);
    if (measurement == nullptr) {
        // Find a free measurement
        sint32 i = ride_get_free_measurement();
        if (i == -1) {
            // Use last recently used measurement for some other ride
            sint32 lruIndex = 0;
            uint32 lruTicks = 0xFFFFFFFF;
            for (i = 0; i < MAX_RIDE_MEASUREMENTS; i++) {
                measurement = get_ride_measurement(i);

                if (measurement->last_use_tick <= lruTicks) {
                    lruTicks = measurement->last_use_tick;
                    lruIndex = i;
                }
            }

            i = lruIndex;
            measurement = get_ride_measurement(i);
            get_ride(measurement->ride_index)->measurement_index = 255;
        } else {
            measurement = get_ride_measurement(i);
        }

        measurement->ride_index = rideIndex;
        ride->measurement_index = i;
        measurement->flags = 0;
        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_G_FORCES))
            measurement->flags |= RIDE_MEASUREMENT_FLAG_G_FORCES;
        measurement->num_items = 0;
        measurement->current_item = 0;
    }

    measurement->last_use_tick = gScenarioTicks;
    if (measurement->flags & 1) {
        if (message != nullptr) *message = 0;
        return measurement;
    } else {
        set_format_arg(0, rct_string_id, RideComponentNames[RideNameConvention[ride->type].vehicle].singular);
        set_format_arg(2, rct_string_id, RideComponentNames[RideNameConvention[ride->type].station].singular);
        if (message != nullptr) *message = STR_DATA_LOGGING_WILL_START_WHEN_NEXT_LEAVES;
        return nullptr;
    }
}

#pragma endregion

#pragma region Colour functions

track_colour ride_get_track_colour(Ride *ride, sint32 colourScheme)
{
    track_colour result;
    result.main = ride->track_colour_main[colourScheme];
    result.additional = ride->track_colour_additional[colourScheme];
    result.supports = ride->track_colour_supports[colourScheme];
    return result;
}

vehicle_colour ride_get_vehicle_colour(Ride *ride, sint32 vehicleIndex)
{
    vehicle_colour result;

    //Prevent indexing array out of bounds
    if (vehicleIndex > 31) {
        vehicleIndex = 31;
    }

    result.main = ride->vehicle_colours[vehicleIndex].body_colour;
    result.additional_1 = ride->vehicle_colours[vehicleIndex].trim_colour;
    result.additional_2 = ride->vehicle_colours_extended[vehicleIndex];
    return result;
}

static bool ride_does_vehicle_colour_exist(uint8 ride_sub_type, vehicle_colour *vehicleColour)
{
    sint32 i;
    Ride *ride2;
    FOR_ALL_RIDES(i, ride2) {
        if (ride2->subtype != ride_sub_type) continue;
        if (ride2->vehicle_colours[0].body_colour != vehicleColour->main) continue;
        return false;
    }
    return true;
}

sint32 ride_get_unused_preset_vehicle_colour(uint8 ride_type, uint8 ride_sub_type)
{
    if (ride_sub_type >= 128)
    {
        return 0;
    }
    rct_ride_entry *rideEntry = get_ride_entry(ride_sub_type);
    if (rideEntry == nullptr)
    {
        return 0;
    }
    vehicle_colour_preset_list *presetList = rideEntry->vehicle_preset_list;
    if (presetList->count == 0)
        return 0;
    if (presetList->count == 255)
        return 255;

    for (sint32 attempt = 0; attempt < 200; attempt++) {
        uint8 numColourConfigurations = presetList->count;
        sint32 randomConfigIndex = util_rand() % numColourConfigurations;
        vehicle_colour *preset = &presetList->list[randomConfigIndex];

        if (ride_does_vehicle_colour_exist(ride_sub_type, preset)) {
            return randomConfigIndex;
        }
    }
    return 0;
}

/**
 *
 *  rct2: 0x006DE52C
 */
void ride_set_vehicle_colours_to_random_preset(Ride *ride, uint8 preset_index)
{
    rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
    vehicle_colour_preset_list *presetList = rideEntry->vehicle_preset_list;

    if (presetList->count != 0 && presetList->count != 255) {
        assert(preset_index < presetList->count);

        ride->colour_scheme_type = RIDE_COLOUR_SCHEME_ALL_SAME;
        vehicle_colour *preset = &presetList->list[preset_index];
        ride->vehicle_colours[0].body_colour = preset->main;
        ride->vehicle_colours[0].trim_colour = preset->additional_1;
        ride->vehicle_colours_extended[0] = preset->additional_2;
    } else {
        ride->colour_scheme_type = RIDE_COLOUR_SCHEME_DIFFERENT_PER_TRAIN;
        uint32 count = Math::Min(presetList->count, (uint8)32);
        for (uint32 i = 0; i < count; i++) {
            vehicle_colour *preset = &presetList->list[i];
            ride->vehicle_colours[i].body_colour = preset->main;
            ride->vehicle_colours[i].trim_colour = preset->additional_1;
            ride->vehicle_colours_extended[i] = preset->additional_2;
        }
    }
}

#pragma endregion

#pragma region Reachability

/**
 *
 *  rct2: 0x006B7A5E
 */
void ride_check_all_reachable()
{
    Ride *ride;
    sint32 i;

    FOR_ALL_RIDES(i, ride) {
        if (ride->connected_message_throttle != 0)
            ride->connected_message_throttle--;
        if (ride->status != RIDE_STATUS_OPEN || ride->connected_message_throttle != 0)
            continue;

        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_IS_SHOP))
            ride_shop_connected(ride, i);
        else
            ride_entrance_exit_connected(ride, i);
    }
}

/**
 *
 *  rct2: 0x006B7C59
 * @return 1 if the coordinate is reachable or has no entrance, 0 otherwise
 */
static sint32 ride_entrance_exit_is_reachable(TileCoordsXYZD coordinates)
{
    sint32 x, y, z;

    if (coordinates.isNull())
        return 1;

    x = coordinates.x;
    y = coordinates.y;
    z = coordinates.z;
    uint8 face_direction = coordinates.direction;

    x *= 32;
    y *= 32;
    x -= TileDirectionDelta[face_direction].x;
    y -= TileDirectionDelta[face_direction].y;
    x /= 32;
    y /= 32;

    return map_coord_is_connected(x, y, z, face_direction);
}

static void ride_entrance_exit_connected(Ride* ride, sint32 ride_idx)
{
    for (sint32 i = 0; i < MAX_STATIONS; ++i)
    {
        LocationXY8 station_start = ride->station_starts[i];
        TileCoordsXYZD entrance = ride_get_entrance_location(ride_idx, i);
        TileCoordsXYZD exit = ride_get_exit_location(ride_idx, i);

        if (station_start.xy == RCT_XY8_UNDEFINED )
            continue;
        if (!entrance.isNull() && !ride_entrance_exit_is_reachable(entrance))
        {
            // name of ride is parameter of the format string
            set_format_arg(0, rct_string_id, ride->name);
            set_format_arg(2, uint32, ride->name_arguments);
            if (gConfigNotifications.ride_warnings) {
            news_item_add_to_queue(1, STR_ENTRANCE_NOT_CONNECTED, ride_idx);
            }
            ride->connected_message_throttle = 3;
        }

        if (!exit.isNull() && !ride_entrance_exit_is_reachable(exit))
        {
            // name of ride is parameter of the format string
            set_format_arg(0, rct_string_id, ride->name);
            set_format_arg(2, uint32, ride->name_arguments);
            if (gConfigNotifications.ride_warnings) {
            news_item_add_to_queue(1, STR_EXIT_NOT_CONNECTED, ride_idx);
            }
            ride->connected_message_throttle = 3;
        }

    }
}

static void ride_shop_connected(Ride* ride, sint32 ride_idx)
{
    sint32 x, y, count;
    rct_tile_element *tileElement;

    LocationXY8 coordinates = ride->station_starts[0];
    if (coordinates.xy == RCT_XY8_UNDEFINED)
        return;

    x = coordinates.x;
    y = coordinates.y;

    tileElement = map_get_first_element_at(x, y);
    do {
        if (tile_element_get_type(tileElement) == TILE_ELEMENT_TYPE_TRACK && track_element_get_ride_index(tileElement) == ride_idx)
            break;
    } while (!tile_element_is_last_for_tile(tileElement++));

    uint16 entrance_directions = 0;
    uint8 track_type = track_element_get_type(tileElement);
    ride = get_ride(track_element_get_ride_index(tileElement));
    if (ride == nullptr)
    {
        return;
    }
    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_FLAT_RIDE)) {
        entrance_directions = FlatRideTrackSequenceProperties[track_type][0];
    } else {
        entrance_directions = TrackSequenceProperties[track_type][0];
    }

    uint8 tile_direction = tile_element_get_direction(tileElement);
    entrance_directions <<= tile_direction;
    entrance_directions = ((entrance_directions >> 12) | entrance_directions) & 0xF;

    // Now each bit in entrance_directions stands for an entrance direction to check
    if (entrance_directions == 0)
        return;

    // Turn x, y from tiles into units
    x *= 32;
    y *= 32;

    for (count = 0; entrance_directions != 0; count++) {
        if (!(entrance_directions & 1)) {
            entrance_directions >>= 1;
            continue;
        }
        entrance_directions >>= 1;

        // Flip direction north<->south, east<->west
        uint8 face_direction = count ^ 2;

        sint32 y2 = y - TileDirectionDelta[face_direction].y;
        sint32 x2 = x - TileDirectionDelta[face_direction].x;

        if (map_coord_is_connected(x2 / 32, y2 / 32, tileElement->base_height, face_direction))
            return;
    }

    // Name of ride is parameter of the format string
    set_format_arg(0, rct_string_id, ride->name);
    set_format_arg(2, uint32, ride->name_arguments);
    if (gConfigNotifications.ride_warnings) {
    news_item_add_to_queue(1, STR_ENTRANCE_NOT_CONNECTED, ride_idx);
    }

    ride->connected_message_throttle = 3;
}

#pragma endregion

#pragma region Interface

static void ride_track_set_map_tooltip(rct_tile_element *tileElement)
{
    sint32 rideIndex;
    Ride *ride;

    rideIndex = track_element_get_ride_index(tileElement);
    ride = get_ride(rideIndex);

    set_map_tooltip_format_arg(0, rct_string_id, STR_RIDE_MAP_TIP);
    set_map_tooltip_format_arg(2, rct_string_id, ride->name);
    set_map_tooltip_format_arg(4, uint32, ride->name_arguments);

    rct_string_id formatSecondary;
    sint32 arg1 = 0;
    ride_get_status(rideIndex, &formatSecondary, &arg1);
    set_map_tooltip_format_arg(8, rct_string_id, formatSecondary);
    set_map_tooltip_format_arg(10, uint32, arg1);
}

static void ride_station_set_map_tooltip(rct_tile_element *tileElement)
{
    sint32 i, rideIndex, stationIndex;
    Ride *ride;

    rideIndex = track_element_get_ride_index(tileElement);
    ride = get_ride(rideIndex);

    stationIndex = tile_element_get_station(tileElement);
    for (i = stationIndex; i >= 0; i--)
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
            stationIndex--;

    set_map_tooltip_format_arg(0, rct_string_id, STR_RIDE_MAP_TIP);
    set_map_tooltip_format_arg(2, rct_string_id, ride->num_stations <= 1 ? STR_RIDE_STATION : STR_RIDE_STATION_X);
    set_map_tooltip_format_arg(4, rct_string_id, ride->name);
    set_map_tooltip_format_arg(6, uint32, ride->name_arguments);
    set_map_tooltip_format_arg(10, rct_string_id, RideComponentNames[RideNameConvention[ride->type].station].capitalised);
    set_map_tooltip_format_arg(12, uint16, stationIndex + 1);

    rct_string_id formatSecondary;
    sint32 arg1;
    ride_get_status(rideIndex, &formatSecondary, &arg1);
    set_map_tooltip_format_arg(14, rct_string_id, formatSecondary);
    set_map_tooltip_format_arg(16, uint32, arg1);
}

static void ride_entrance_set_map_tooltip(rct_tile_element *tileElement)
{
    sint32 i, rideIndex, stationIndex;
    Ride *ride;

    rideIndex = track_element_get_ride_index(tileElement);
    ride = get_ride(rideIndex);

    // Get the station
    stationIndex = tile_element_get_station(tileElement);
    for (i = stationIndex; i >= 0; i--)
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
            stationIndex--;

    if (tileElement->properties.entrance.type == ENTRANCE_TYPE_RIDE_ENTRANCE) {
        // Get the queue length
        sint32 queueLength = 0;
        if (!ride_get_entrance_location(ride, stationIndex).isNull())
            queueLength = ride->queue_length[stationIndex];

        set_map_tooltip_format_arg(0, rct_string_id, STR_RIDE_MAP_TIP);
        set_map_tooltip_format_arg(2, rct_string_id, ride->num_stations <= 1 ? STR_RIDE_ENTRANCE : STR_RIDE_STATION_X_ENTRANCE);
        set_map_tooltip_format_arg(4, rct_string_id, ride->name);
        set_map_tooltip_format_arg(6, uint32, ride->name_arguments);
        set_map_tooltip_format_arg(12, uint16, stationIndex + 1);
        if (queueLength == 0) {
            set_map_tooltip_format_arg(14, rct_string_id, STR_QUEUE_EMPTY);
        } else if (queueLength == 1) {
            set_map_tooltip_format_arg(14, rct_string_id, STR_QUEUE_ONE_PERSON);
        } else {
            set_map_tooltip_format_arg(14, rct_string_id, STR_QUEUE_PEOPLE);
        }
        set_map_tooltip_format_arg(16, uint16, queueLength);
    } else {
        // Get the station
        stationIndex = tile_element_get_station(tileElement);
        for (i = stationIndex; i >= 0; i--)
            if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
                stationIndex--;

        set_map_tooltip_format_arg(0, rct_string_id, ride->num_stations <= 1 ? STR_RIDE_EXIT : STR_RIDE_STATION_X_EXIT);
        set_map_tooltip_format_arg(2, rct_string_id, ride->name);
        set_map_tooltip_format_arg(4, uint32, ride->name_arguments);
        set_map_tooltip_format_arg(10, uint16, stationIndex + 1);
    }
}

void ride_set_map_tooltip(rct_tile_element *tileElement)
{
    if (tile_element_get_type(tileElement) == TILE_ELEMENT_TYPE_ENTRANCE)
    {
        ride_entrance_set_map_tooltip(tileElement);
    }
    else
    {
        if (track_element_is_station(tileElement))
        {
            ride_station_set_map_tooltip(tileElement);
        }
        else
        {
            ride_track_set_map_tooltip(tileElement);
        }
    }
}

static sint32 ride_music_params_update_label_51(uint32 a1, uint8 * tuneId, uint8 rideIndex, sint32 v32, sint32 pan_x, uint16 sampleRate)
{
    if (a1 < gRideMusicInfoList[*tuneId].length)
    {
        rct_ride_music_params * ride_music_params = gRideMusicParamsListEnd;
        if (ride_music_params < &gRideMusicParamsList[Util::CountOf(gRideMusicParamsList)])
        {
            ride_music_params->ride_id   = rideIndex;
            ride_music_params->tune_id   = *tuneId;
            ride_music_params->offset    = a1;
            ride_music_params->volume    = v32;
            ride_music_params->pan       = pan_x;
            ride_music_params->frequency = sampleRate;
            gRideMusicParamsListEnd++;
        }

        return a1;
    }
    else
    {
        *tuneId = 0xFF;
        return 0;
    }
}

static sint32 ride_music_params_update_label_58(uint32 position, uint8 * tuneId)
{
    rct_ride_music_info * ride_music_info = &gRideMusicInfoList[*tuneId];
    position += ride_music_info->offset;
    if (position < ride_music_info->length)
    {
        return position;
    }
    else
    {
        *tuneId = 0xFF;
        return 0;
    }
}

/**
 *
 *  rct2: 0x006BC3AC
 * Update ride music parameters
 * @param x (ax)
 * @param y (cx)
 * @param z (dx)
 * @param sampleRate (di)
 * @param rideIndex (bl)
 * @param position (ebp)
 * @param tuneId (bh)
 * @returns new position (ebp)
 */
sint32 ride_music_params_update(sint16 x, sint16 y, sint16 z, uint8 rideIndex, uint16 sampleRate, uint32 position, uint8 * tuneId)
{
    if (!(gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR) && !gGameSoundsOff && g_music_tracking_viewport != nullptr)
    {
        const LocationXY16 rotatedCoords = ride_get_rotated_coords(x, y, z);
        rct_viewport * viewport = g_music_tracking_viewport;
        sint16 view_width  = viewport->view_width;
        sint16 view_width2 = view_width * 2;
        sint16 view_x      = viewport->view_x - view_width2;
        sint16 view_y      = viewport->view_y - view_width;
        sint16 view_x2     = view_width2 + view_width2 + viewport->view_width + view_x;
        sint16 view_y2     = view_width + view_width + viewport->view_height + view_y;

        if (view_x >= rotatedCoords.x ||
            view_y >= rotatedCoords.y ||
            view_x2 < rotatedCoords.x ||
            view_y2 < rotatedCoords.y)
        {
            return ride_music_params_update_label_58(position, tuneId);
        }

        sint32 x2 = viewport->x + ((rotatedCoords.x - viewport->view_x) >> viewport->zoom);
        x2 *= 0x10000;
        uint16 screenwidth = context_get_width();
        if (screenwidth < 64)
        {
            screenwidth = 64;
        }
        sint32 pan_x = ((x2 / screenwidth) - 0x8000) >> 4;

        sint32 y2 = viewport->y + ((rotatedCoords.y - viewport->view_y) >> viewport->zoom);
        y2 *= 0x10000;
        uint16 screenheight = context_get_height();
        if (screenheight < 64)
        {
            screenheight = 64;
        }
        sint32 pan_y = ((y2 / screenheight) - 0x8000) >> 4;

        uint8  vol1  = -1;
        uint8  vol2  = -1;
        sint32 panx2 = pan_x;
        sint32 pany2 = pan_y;
        if (pany2 < 0)
        {
            pany2 = -pany2;
        }
        if (pany2 > 6143)
        {
            pany2 = 6143;
        }
        pany2 -= 2048;
        if (pany2 > 0)
        {
            pany2 = -((pany2 / 4) - 1024) / 4;
            vol1  = (uint8) pany2;
            if (pany2 >= 256)
            {
                vol1 = -1;
            }
        }

        if (panx2 < 0)
        {
            panx2 = -panx2;
        }
        if (panx2 > 6143)
        {
            panx2 = 6143;
        }
        panx2 -= 2048;
        if (panx2 > 0)
        {
            panx2 = -((panx2 / 4) - 1024) / 4;
            vol2  = (uint8) panx2;
            if (panx2 >= 256)
            {
                vol2 = -1;
            }
        }
        if (vol1 >= vol2)
        {
            vol1 = vol2;
        }
        if (vol1 < gVolumeAdjustZoom * 3)
        {
            vol1 = 0;
        }
        else
        {
            vol1 = vol1 - (gVolumeAdjustZoom * 3);
        }
        sint32 v32 = -(((uint8) (-vol1 - 1) * (uint8) (-vol1 - 1)) / 16) - 700;
        if (vol1 && v32 >= -4000)
        {
            if (pan_x > 10000)
            {
                pan_x = 10000;
            }
            if (pan_x < -10000)
            {
                pan_x = -10000;
            }
            rct_ride_music * ride_music = &gRideMusicList[0];
            sint32 channel = 0;
            uint32 a1;
            while (ride_music->ride_id != rideIndex || ride_music->tune_id != *tuneId)
            {
                ride_music++;
                channel++;
                if (channel >= AUDIO_MAX_RIDE_MUSIC)
                {
                    rct_ride_music_info * ride_music_info = &gRideMusicInfoList[*tuneId];
                    a1 = position + ride_music_info->offset;

                    return ride_music_params_update_label_51(a1, tuneId, rideIndex, v32, pan_x, sampleRate);
                }
            }
            sint32 playing = Mixer_Channel_IsPlaying(gRideMusicList[channel].sound_channel);
            if (!playing)
            {
                *tuneId = 0xFF;
                return 0;
            }
            a1 = (uint32) Mixer_Channel_GetOffset(gRideMusicList[channel].sound_channel);

            return ride_music_params_update_label_51(a1, tuneId, rideIndex, v32, pan_x, sampleRate);
        }
        else
        {
            return ride_music_params_update_label_58(position, tuneId);
        }
    }
    return position;
}

/**
*  Play/update ride music based on structs updated in 0x006BC3AC
*  rct2: 0x006BC6D8
*/
void ride_music_update_final()
{
    rct_ride_music_params * edi = nullptr;
    sint32 ebx = 0;
    if (!(gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR))
    {
        // TODO Allow circus music (CSS24) to play if ride music is disabled (that should be sound)
        if (!gGameSoundsOff && gConfigSound.ride_music_enabled && !(gScreenFlags & SCREEN_FLAGS_TITLE_DEMO))
        {
            while (1)
            {
                sint32 v8 = 0;
                sint32 v9 = 1;
                rct_ride_music_params * ride_music_params = &gRideMusicParamsList[0];
                while (ride_music_params < gRideMusicParamsListEnd)
                {
                    if (ride_music_params->ride_id != (uint8) -1)
                    {
                        v8++;
                        if (v9 >= ride_music_params->volume)
                        {
                            v9  = ride_music_params->volume;
                            edi = ride_music_params;
                        }
                    }
                    ride_music_params++;
                }
                if (v8 <= 2)
                {
                    break;
                }
                edi->ride_id = -1;
            }

            // stop currently playing music that is not in music params list or not playing?
            rct_ride_music * ride_music = &gRideMusicList[0];
            sint32 channel = 0;
            do
            {
                if (ride_music->ride_id != RIDE_ID_NULL)
                {
                    rct_ride_music_params * ride_music_params = &gRideMusicParamsList[0];
                    sint32 isplaying = 0;
                    while (ride_music_params < gRideMusicParamsListEnd && !isplaying)
                    {
                        if (ride_music_params->ride_id == ride_music->ride_id && ride_music_params->tune_id == ride_music->tune_id)
                        {
                            isplaying = Mixer_Channel_IsPlaying(gRideMusicList[channel].sound_channel);
                            break;
                        }
                        ride_music_params++;
                    }
                    if (!isplaying)
                    {
                        Mixer_Stop_Channel(gRideMusicList[channel].sound_channel);
                        ride_music->ride_id = RIDE_ID_NULL;
                    }

                }
                ride_music++;
                channel++;
            }
            while (channel < AUDIO_MAX_RIDE_MUSIC);

            for (rct_ride_music_params * ride_music_params = &gRideMusicParamsList[0]; ride_music_params < gRideMusicParamsListEnd; ride_music_params++)
            {
                if (ride_music_params->ride_id != RIDE_ID_NULL)
                {
                    rct_ride_music * ride_music_2 = &gRideMusicList[0];
                    sint32 channel2 = 0;
                    while (ride_music_params->ride_id != ride_music_2->ride_id || ride_music_params->tune_id != ride_music_2->tune_id)
                    {
                        if (ride_music_2->ride_id == RIDE_ID_NULL)
                        {
                            ebx = channel2;
                        }
                        ride_music_2++;
                        channel2++;
                        if (channel2 >= AUDIO_MAX_RIDE_MUSIC)
                        {
                            rct_ride_music_info * ride_music_info = &gRideMusicInfoList[ride_music_params->tune_id];
                            rct_ride_music      * ride_music_3    = &gRideMusicList[ebx];
                            ride_music_3->sound_channel = Mixer_Play_Music(ride_music_info->path_id, MIXER_LOOP_NONE, true);
                            if (ride_music_3->sound_channel)
                            {
                                ride_music_3->volume    = ride_music_params->volume;
                                ride_music_3->pan       = ride_music_params->pan;
                                ride_music_3->frequency = ride_music_params->frequency;
                                ride_music_3->ride_id   = ride_music_params->ride_id;
                                ride_music_3->tune_id   = ride_music_params->tune_id;
                                Mixer_Channel_Volume(ride_music_3->sound_channel, DStoMixerVolume(ride_music_3->volume));
                                Mixer_Channel_Pan(ride_music_3->sound_channel, DStoMixerPan(ride_music_3->pan));
                                Mixer_Channel_Rate(ride_music_3->sound_channel, DStoMixerRate(ride_music_3->frequency));
                                sint32 offset = ride_music_params->offset - 10000;
                                if (offset < 0)
                                {
                                    offset = 0;
                                }
                                Mixer_Channel_SetOffset(ride_music_3->sound_channel, offset);

                                // Move circus music to the sound mixer group
                                if (ride_music_info->path_id == PATH_ID_CSS24)
                                {
                                    Mixer_Channel_SetGroup(ride_music_3->sound_channel, MIXER_GROUP_SOUND);
                                }
                            }
                            return;
                        }
                    }

                    if (ride_music_params->volume != ride_music_2->volume)
                    {
                        ride_music_2->volume = ride_music_params->volume;
                        Mixer_Channel_Volume(ride_music_2->sound_channel, DStoMixerVolume(ride_music_2->volume));
                    }
                    if (ride_music_params->pan != ride_music_2->pan)
                    {
                        ride_music_2->pan = ride_music_params->pan;
                        Mixer_Channel_Pan(ride_music_2->sound_channel, DStoMixerPan(ride_music_2->pan));
                    }
                    if (ride_music_params->frequency != ride_music_2->frequency)
                    {
                        ride_music_2->frequency = ride_music_params->frequency;
                        Mixer_Channel_Rate(ride_music_2->sound_channel, DStoMixerRate(ride_music_2->frequency));
                    }

                }
            }
        }
    }
}

#pragma endregion

static bool ride_is_mode_valid(Ride *ride, uint8 mode)
{
    const uint8 * availableModes = ride_seek_available_modes(ride);

    for (; *availableModes != 0xFF; availableModes++)
    {
        if (*availableModes == mode)
        {
            return true;
        }
    }

    return false;
}

static bool ride_is_valid_lift_hill_speed(Ride *ride, sint32 speed)
{
    sint32 minSpeed = gCheatsFastLiftHill ? 0   : RideLiftData[ride->type].minimum_speed;
    sint32 maxSpeed = gCheatsFastLiftHill ? 255 : RideLiftData[ride->type].maximum_speed;
    return speed >= minSpeed && speed <= maxSpeed;
}

static bool ride_is_valid_num_circuits(sint32 numCircuits)
{
    sint32 minNumCircuits = 1;
    sint32 maxNumCircuits = gCheatsFastLiftHill ? 255 : 20;
    return numCircuits >= minNumCircuits && numCircuits <= maxNumCircuits;
}

static bool ride_is_valid_operation_option(Ride *ride, uint8 value)
{
    uint8 minValue = RideProperties[ride->type].min_value;
    uint8 maxValue = RideProperties[ride->type].max_value;
    if (gCheatsFastLiftHill) {
        minValue = 0;
        maxValue = 255;
    }

    return value >= minValue && value <= maxValue;
}

static money32 ride_set_setting(uint8 rideIndex, uint8 setting, uint8 value, uint8 flags)
{
    gCommandExpenditureType = RCT_EXPENDITURE_TYPE_RIDE_RUNNING_COSTS;

    Ride *ride = get_ride(rideIndex);
    if (ride == nullptr || ride->type == RIDE_TYPE_NULL) {
        log_warning("Invalid ride: #%u.", rideIndex);
        return MONEY32_UNDEFINED;
    }

    switch (setting) {
    case RIDE_SETTING_MODE:
        if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
            gGameCommandErrorText = STR_HAS_BROKEN_DOWN_AND_REQUIRES_FIXING;
            return MONEY32_UNDEFINED;
        }

        if (ride->status != RIDE_STATUS_CLOSED) {
            gGameCommandErrorText = STR_MUST_BE_CLOSED_FIRST;
            return MONEY32_UNDEFINED;
        }

        if (!ride_is_mode_valid(ride, value)) {
            log_warning("Invalid ride mode.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            invalidate_test_results(rideIndex);
            ride_clear_for_construction(rideIndex);
            ride_remove_peeps(rideIndex);

            ride->mode = value;
            ride_update_max_vehicles(rideIndex);
        }
        break;
    case RIDE_SETTING_DEPARTURE:
        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->depart_flags = value;
        }
        break;
    case RIDE_SETTING_MIN_WAITING_TIME:
        if (value > 250) {
            log_warning("Invalid minimum waiting time.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->min_waiting_time = value;
            ride->max_waiting_time = Math::Max(value, ride->max_waiting_time);
        }
        break;
    case RIDE_SETTING_MAX_WAITING_TIME:
        if (value > 250) {
            log_warning("Invalid maximum waiting time.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->max_waiting_time = value;
            ride->min_waiting_time = Math::Min(value, ride->min_waiting_time);
        }
        break;
    case RIDE_SETTING_OPERATION_OPTION:
        if (!ride_is_valid_operation_option(ride, value)) {
            log_warning("Invalid operation option value.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            invalidate_test_results(rideIndex);
            ride->operation_option = value;
        }
        break;
    case RIDE_SETTING_INSPECTION_INTERVAL:
        if (value > RIDE_INSPECTION_NEVER) {
            log_warning("Invalid inspection interval.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->inspection_interval = value;
        }
        break;
    case RIDE_SETTING_MUSIC:
        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->lifecycle_flags &= ~RIDE_LIFECYCLE_MUSIC;
            if (value) {
                ride->lifecycle_flags |= RIDE_LIFECYCLE_MUSIC;
            }
        }
        break;
    case RIDE_SETTING_MUSIC_TYPE:
        if (value >= MUSIC_STYLE_COUNT) {
            log_warning("Invalid music style.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            if (value != ride->music) {
                ride->music = value;
                ride->music_tune_id = 0xFF;
            }
        }
        break;
    case RIDE_SETTING_LIFT_HILL_SPEED:
        if (!ride_is_valid_lift_hill_speed(ride, value)) {
            log_warning("Invalid lift hill speed.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            if (value != ride->lift_hill_speed) {
                ride->lift_hill_speed = value;
                invalidate_test_results(rideIndex);
            }
        }
        break;
    case RIDE_SETTING_NUM_CIRCUITS:
        if (ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT && value > 1) {
            gGameCommandErrorText = STR_MULTICIRCUIT_NOT_POSSIBLE_WITH_CABLE_LIFT_HILL;
            return MONEY32_UNDEFINED;
        }

        if (!ride_is_valid_num_circuits(value)) {
            log_warning("Invalid number of circuits.");
            return MONEY32_UNDEFINED;
        }

        if (flags & GAME_COMMAND_FLAG_APPLY) {
            if (value != ride->num_circuits) {
                ride->num_circuits = value;
                invalidate_test_results(rideIndex);
            }
        }
        break;
    case RIDE_SETTING_RIDE_TYPE:
        if (!gCheatsAllowArbitraryRideTypeChanges) {
            return MONEY32_UNDEFINED;
        }
        if (flags & GAME_COMMAND_FLAG_APPLY) {
            ride->type = value;
        }
        break;
    }

    if (flags & GAME_COMMAND_FLAG_APPLY) {
        if (ride->overall_view.xy != RCT_XY8_UNDEFINED) {
            LocationXYZ16 coord;
            coord.x = ride->overall_view.x * 32 + 16;
            coord.y = ride->overall_view.y * 32 + 16;
            coord.z = tile_element_height(coord.x, coord.y);
            network_set_player_last_action_coord(network_get_player_index(game_command_playerid), coord);
        }

        window_invalidate_by_number(WC_RIDE, rideIndex);
    }

    return 0;
}

/**
 *
 *  rct2: 0x006B5559
 */
void game_command_set_ride_setting(sint32 *eax, sint32 *ebx, sint32 *ecx, sint32 *edx, sint32 *esi, sint32 *edi, sint32 *ebp)
{
    uint8 rideIndex = *edx & 0xFF;
    uint8 setting = (*edx >> 8) & 0xFF;
    uint8 newValue = (*ebx >> 8) & 0xFF;
    uint8 flags = *ebx & 0xFF;
    *ebx = ride_set_setting(rideIndex, setting, newValue, flags);
}

/**
 *
 *  rct2: 0x006B4CC1
 */
static sint32 ride_mode_check_valid_station_numbers(Ride *ride)
{
    uint8 no_stations = 0;
    for (uint8 station_index = 0; station_index < MAX_STATIONS; ++station_index){
        if (ride->station_starts[station_index].xy != RCT_XY8_UNDEFINED)
        {
            no_stations++;
        }
    }

    switch (ride->mode){
    case RIDE_MODE_REVERSE_INCLINE_LAUNCHED_SHUTTLE:
    case RIDE_MODE_POWERED_LAUNCH_PASSTROUGH:
    case RIDE_MODE_POWERED_LAUNCH:
    case RIDE_MODE_LIM_POWERED_LAUNCH:
        if (no_stations <= 1) return 1;
        gGameCommandErrorText = STR_UNABLE_TO_OPERATE_WITH_MORE_THAN_ONE_STATION_IN_THIS_MODE;
        return 0;
    case RIDE_MODE_SHUTTLE:
        if (no_stations >= 2) return 1;
        gGameCommandErrorText = STR_UNABLE_TO_OPERATE_WITH_LESS_THAN_TWO_STATIONS_IN_THIS_MODE;
        return 0;
    }

    if (ride->type == RIDE_TYPE_GO_KARTS || ride->type == RIDE_TYPE_MINI_GOLF){
        if (no_stations <= 1) return 1;
        gGameCommandErrorText = STR_UNABLE_TO_OPERATE_WITH_MORE_THAN_ONE_STATION_IN_THIS_MODE;
        return 0;
    }

    return 1;
}

/**
 * returns stationIndex of first station on success
 * -1 on failure.
 */
static sint32 ride_mode_check_station_present(Ride* ride){
    sint32 stationIndex = ride_get_first_valid_station_start(ride);

    if (stationIndex == -1) {
        gGameCommandErrorText = STR_NOT_YET_CONSTRUCTED;
        if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_HAS_NO_TRACK))
            return -1;

        if (ride->type == RIDE_TYPE_MAZE)
            return -1;

        gGameCommandErrorText = STR_REQUIRES_A_STATION_PLATFORM;
        return -1;
    }

    return stationIndex;
}

/**
 *
 *  rct2: 0x006B5872
 */
static sint32 ride_check_for_entrance_exit(sint32 rideIndex)
{
    Ride* ride = get_ride(rideIndex);

    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_IS_SHOP))
        return 1;

    sint32 i;
    uint8 entrance = 0;
    uint8 exit = 0;
    for (i = 0; i < MAX_STATIONS; i++) {
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
            continue;

        if (!ride_get_entrance_location(ride, i).isNull()) {
            entrance = 1;
        }

        if (!ride_get_exit_location(ride, i).isNull()) {
            exit = 1;
        }

        // If station start and no entrance/exit
        // Sets same error message as no entrance
        if (ride_get_exit_location(ride, i).isNull() &&
            ride_get_entrance_location(ride, i).isNull())
        {
            entrance = 0;
            break;
        }
    }

    if (entrance == 0){
        gGameCommandErrorText = STR_ENTRANCE_NOT_YET_BUILT;
        return 0;
    }

    if (exit == 0){
        gGameCommandErrorText = STR_EXIT_NOT_YET_BUILT;
        return 0;
    }

    return 1;
}

/**
 *
 *  rct2: 0x006B5952
 */
static void sub_6B5952(sint32 rideIndex)
{
    for (sint32 i = 0; i < MAX_STATIONS; i++)
    {
        TileCoordsXYZD location = ride_get_entrance_location(rideIndex, i);
        if (location.isNull())
            continue;

        sint32 x = location.x * 32;
        sint32 y = location.y * 32;
        sint32 z = location.z;

        // This will fire for every entrance on this x, y and z, regardless whether that actually belongs to
        // the ride or not.
        rct_tile_element * tileElement = map_get_first_element_at(location.x, location.y);
        do {
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_ENTRANCE)
                continue;
            if (tileElement->base_height != z)
                continue;

            sint32 direction = tile_element_get_direction(tileElement);
            footpath_chain_ride_queue(rideIndex, i, x, y, tileElement, direction ^ 2);
        } while (!tile_element_is_last_for_tile(tileElement++));
    }
}

/**
 *
 *  rct2: 0x006D3319
 */
static sint32 ride_check_block_brakes(CoordsXYE *input, CoordsXYE *output)
{
    rct_window *w;
    track_circuit_iterator it;
    sint32 rideIndex, type;

    rideIndex = track_element_get_ride_index(input->element);
    w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && _currentRideIndex == rideIndex)
        ride_construction_invalidate_current_track();

    track_circuit_iterator_begin(&it, *input);
    while (track_circuit_iterator_next(&it)) {
        if (track_element_get_type(it.current.element) == TRACK_ELEM_BLOCK_BRAKES) {
            type = track_element_get_type(it.last.element);
            if (type == TRACK_ELEM_END_STATION) {
                gGameCommandErrorText = STR_BLOCK_BRAKES_CANNOT_BE_USED_DIRECTLY_AFTER_STATION;
                *output = it.current;
                return 0;
            }
            if (type == TRACK_ELEM_BLOCK_BRAKES) {
                gGameCommandErrorText = STR_BLOCK_BRAKES_CANNOT_BE_USED_DIRECTLY_AFTER_EACH_OTHER;
                *output = it.current;
                return 0;
            }
            if ((it.last.element->type & 0x80) && type != TRACK_ELEM_LEFT_CURVED_LIFT_HILL && type != TRACK_ELEM_RIGHT_CURVED_LIFT_HILL) {
                gGameCommandErrorText = STR_BLOCK_BRAKES_CANNOT_BE_USED_DIRECTLY_AFTER_THE_TOP_OF_THIS_LIFT_HILL;
                *output = it.current;
                return 0;
            }
        }
    }
    if (!it.looped) {
        // Not sure why this is the case...
        gGameCommandErrorText = STR_BLOCK_BRAKES_CANNOT_BE_USED_DIRECTLY_AFTER_STATION;
        *output = it.last;
        return 0;
    }

    return 1;
}

/**
 * Iterates along the track until an inversion (loop, corkscrew, barrel roll etc.) track piece is reached.
 * @param input The start track element and position.
 * @param output The first track element and position which is classified as an inversion.
 * @returns true if an inversion track piece is found, otherwise false.
 *  rct2: 0x006CB149
 */
static bool ride_check_track_contains_inversions(CoordsXYE *input, CoordsXYE *output)
{
    sint32 rideIndex = track_element_get_ride_index(input->element);
    Ride *ride = get_ride(rideIndex);
    if (ride->type == RIDE_TYPE_MAZE)
        return true;

    rct_window *w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && rideIndex == _currentRideIndex) {
        ride_construction_invalidate_current_track();
    }

    bool moveSlowIt = true;
    track_circuit_iterator it, slowIt;
    track_circuit_iterator_begin(&it, *input);
    slowIt = it;

    while (track_circuit_iterator_next(&it)) {
        sint32 trackType = track_element_get_type(it.current.element);
        if (TrackFlags[trackType] & TRACK_ELEM_FLAG_INVERSION_TO_NORMAL) {
            *output = it.current;
            return true;
        }

        // Prevents infinite loops
        moveSlowIt = !moveSlowIt;
        if (moveSlowIt)
        {
            track_circuit_iterator_next(&slowIt);
            if (track_circuit_iterators_match(&it, &slowIt))
            {
                return false;
            }
        }
    }
    return false;
}

/**
 * Iterates along the track until a banked track piece is reached.
 * @param input The start track element and position.
 * @param output The first track element and position which is banked.
 * @returns true if a banked track piece is found, otherwise false.
 *  rct2: 0x006CB1D3
 */
static bool ride_check_track_contains_banked(CoordsXYE *input, CoordsXYE *output)
{
    sint32 rideIndex = track_element_get_ride_index(input->element);
    Ride *ride = get_ride(rideIndex);
    if (ride->type == RIDE_TYPE_MAZE)
        return true;

    rct_window *w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && rideIndex == _currentRideIndex) {
        ride_construction_invalidate_current_track();
    }

    bool moveSlowIt = true;
    track_circuit_iterator it, slowIt;
    track_circuit_iterator_begin(&it, *input);
    slowIt = it;

    while (track_circuit_iterator_next(&it)) {
        sint32 trackType = track_element_get_type(output->element);
        if (TrackFlags[trackType] & TRACK_ELEM_FLAG_BANKED) {
            *output = it.current;
            return true;
        }

        // Prevents infinite loops
        moveSlowIt = !moveSlowIt;
        if (moveSlowIt)
        {
            track_circuit_iterator_next(&slowIt);
            if (track_circuit_iterators_match(&it, &slowIt))
            {
                return false;
            }
        }
    }
    return false;
}

/**
 *
 *  rct2: 0x006CB25D
 */
static sint32 ride_check_station_length(CoordsXYE *input, CoordsXYE *output)
{
    rct_window* w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr &&
        _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 &&
        _currentRideIndex == track_element_get_ride_index(input->element))
    {
        ride_construction_invalidate_current_track();
    }

    output->x = input->x;
    output->y = input->y;
    output->element = input->element;
    track_begin_end trackBeginEnd;
    while (track_block_get_previous(output->x, output->y, output->element, &trackBeginEnd)){
        output->x = trackBeginEnd.begin_x;
        output->y = trackBeginEnd.begin_y;
        output->element = trackBeginEnd.begin_element;
    }

    sint32 num_station_elements = 0;
    CoordsXYE last_good_station = *output;

    do{
        if (TrackSequenceProperties[track_element_get_type(output->element)][0] & TRACK_SEQUENCE_FLAG_ORIGIN){
            num_station_elements++;
            last_good_station = *output;
        }
        else{
            if (num_station_elements == 0)
                continue;
            if (num_station_elements == 1){
                return 0;
            }
            num_station_elements = 0;
        }
    } while (track_block_get_next(output, output, nullptr, nullptr));

    // Prevent returning a pointer to a map element with no track.
    *output = last_good_station;
    if (num_station_elements == 1)
        return 0;

    return 1;
}

/**
 *
 *  rct2: 0x006CB2DA
 */
static bool ride_check_start_and_end_is_station(CoordsXYE * input, CoordsXYE * output)
{
    rct_window *w;
    Ride *ride;
    sint32 rideIndex, trackType;
    CoordsXYE trackBack, trackFront;

    rideIndex = track_element_get_ride_index(input->element);
    ride = get_ride(rideIndex);

    w = window_find_by_class(WC_RIDE_CONSTRUCTION);
    if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && rideIndex == _currentRideIndex) {
        ride_construction_invalidate_current_track();
    }

    // Check back of the track
    track_get_back(input, &trackBack);
    trackType = track_element_get_type(trackBack.element);
    if (!(TrackSequenceProperties[trackType][0] & TRACK_SEQUENCE_FLAG_ORIGIN)) {
        return false;
    }
    ride->chairlift_bullwheel_location[0].x = trackBack.x >> 5;
    ride->chairlift_bullwheel_location[0].y = trackBack.y >> 5;
    ride->chairlift_bullwheel_z[0] = trackBack.element->base_height;

    // Check front of the track
    track_get_front(input, &trackFront);
    trackType = track_element_get_type(trackFront.element);
    if (!(TrackSequenceProperties[trackType][0] & TRACK_SEQUENCE_FLAG_ORIGIN)) {
        return false;
    }
    ride->chairlift_bullwheel_location[1].x = trackFront.x >> 5;
    ride->chairlift_bullwheel_location[1].y = trackFront.y >> 5;
    ride->chairlift_bullwheel_z[1] = trackFront.element->base_height;
    return true;
}

/**
 * Sets the position and direction of the returning point on the track of a boat hire ride. This will either be the end of the
 * station or the last track piece from the end of the direction.
 *  rct2: 0x006B4D39
 */
static void ride_set_boat_hire_return_point(Ride * ride, CoordsXYE * startElement)
{
    sint32 trackType = -1;
    sint32 returnX = startElement->x;
    sint32 returnY = startElement->y;
    sint32 startX = returnX;
    sint32 startY = returnY;
    rct_tile_element *returnTrackElement = startElement->element;
    track_begin_end trackBeginEnd;
    while (track_block_get_previous(returnX, returnY, returnTrackElement, &trackBeginEnd)) {
        // If previous track is back to the starting x, y, then break loop (otherwise possible infinite loop)
        if (trackType != -1 && startX == trackBeginEnd.begin_x && startY == trackBeginEnd.begin_y) break;

        sint32 x = trackBeginEnd.begin_x;
        sint32 y = trackBeginEnd.begin_y;
        sint32 z = trackBeginEnd.begin_z;
        sint32 direction = trackBeginEnd.begin_direction;
        trackType = track_element_get_type(trackBeginEnd.begin_element);
        sub_6C683D(&x, &y, &z, direction, trackType, 0, &returnTrackElement, 0);
        returnX = x;
        returnY = y;
    };

    trackType = track_element_get_type(returnTrackElement);
    sint32 elementReturnDirection = TrackCoordinates[trackType].rotation_begin;
    ride->boat_hire_return_direction = (returnTrackElement->type + elementReturnDirection) & 3;
    ride->boat_hire_return_position.x = returnX >> 5;
    ride->boat_hire_return_position.y = returnY >> 5;
}

/**
 *
 *  rct2: 0x006B4D39
 */
static void ride_set_maze_entrance_exit_points(Ride *ride)
{
    // Needs room for an entrance and an exit per station, plus one position for the list terminator.
    TileCoordsXYZD positions[(MAX_STATIONS * 2) + 1];

    // Create a list of all the entrance and exit positions
    TileCoordsXYZD * position = positions;
    for (sint32 i = 0; i < MAX_STATIONS; i++)
    {
        const auto entrance = ride_get_entrance_location(ride, i);
        const auto exit = ride_get_exit_location(ride, i);

        if (!entrance.isNull()) {
            *position++ = entrance;
        }
        if (!exit.isNull()) {
            *position++ = exit;
        }
    }
    (*position++).x = COORDS_NULL;

    // Enumerate entrance and exit positions
    for (position = positions; !(*position).isNull(); position++)
    {
        sint32 x = (*position).x << 5;
        sint32 y = (*position).y << 5;
        sint32 z = (*position).z;

        rct_tile_element *tileElement = map_get_first_element_at((*position).x, (*position).y);
        do {
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_ENTRANCE) continue;
            if (
                tileElement->properties.entrance.type != ENTRANCE_TYPE_RIDE_ENTRANCE &&
                tileElement->properties.entrance.type != ENTRANCE_TYPE_RIDE_EXIT
            ) {
                continue;
            }
            if (tileElement->base_height != z) continue;

            maze_entrance_hedge_removal(x, y, tileElement);
        } while (!tile_element_is_last_for_tile(tileElement++));
    }
}

/**
 * Sets a flag on all the track elements that can be the start of a circuit block. i.e. where a train can start.
 *  rct2: 0x006B4E6B
 */
static void ride_set_block_points(CoordsXYE *startElement)
{
    CoordsXYE currentElement = *startElement;
    do {
        sint32 trackType = track_element_get_type(currentElement.element);
        switch (trackType) {
        case TRACK_ELEM_END_STATION:
        case TRACK_ELEM_CABLE_LIFT_HILL:
        case TRACK_ELEM_25_DEG_UP_TO_FLAT:
        case TRACK_ELEM_60_DEG_UP_TO_FLAT:
        case TRACK_ELEM_DIAG_25_DEG_UP_TO_FLAT:
        case TRACK_ELEM_DIAG_60_DEG_UP_TO_FLAT:
        case TRACK_ELEM_BLOCK_BRAKES:
            currentElement.element->flags &= ~(1 << 5);
            break;
        }
    } while (track_block_get_next(&currentElement, &currentElement, nullptr, nullptr) && currentElement.element != startElement->element);
}

/**
 *
 *  rct2: 0x006B4D26
 */
static void ride_set_start_finish_points(sint32 rideIndex, CoordsXYE *startElement)
{
    Ride *ride = get_ride(rideIndex);

    switch (ride->type) {
    case RIDE_TYPE_BOAT_HIRE:
        ride_set_boat_hire_return_point(ride, startElement);
        break;
    case RIDE_TYPE_MAZE:
        ride_set_maze_entrance_exit_points(ride);
        break;
    }

    if (ride_is_block_sectioned(ride) && !(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK)) {
        ride_set_block_points(startElement);
    }
}

/**
 *
 *  rct2: 0x0069ED9E
 */
static sint32 count_free_misc_sprite_slots()
{
    sint32 miscSpriteCount = gSpriteListCount[SPRITE_LIST_MISC];
    sint32 remainingSpriteCount = gSpriteListCount[SPRITE_LIST_NULL];
    return Math::Max(0, miscSpriteCount + remainingSpriteCount - 300);
}

static constexpr const LocationXY16 word_9A3AB4[4] = {
    {   0,   0 },
    {   0, -96 },
    { -96, -96 },
    { -96,   0 },
};

static constexpr const LocationXY16 word_9A2A60[] = {
    {   0,  16 },
    {  16,  31 },
    {  31,  16 },
    {  16,   0 },
    {  16,  16 },
    {  64,  64 },
    {  64, -32 },
    { -32, -32 },
    { -32,  64 },
};

/**
 *
 *  rct2: 0x006DD90D
 */
static rct_vehicle *vehicle_create_car(
    sint32 rideIndex,
    sint32 vehicleEntryIndex,
    sint32 carIndex,
    sint32 vehicleIndex,
    sint32 x,
    sint32 y,
    sint32 z,
    sint32 *remainingDistance,
    rct_tile_element *tileElement
) {
    Ride *ride = get_ride(rideIndex);
    rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
    rct_ride_entry_vehicle *vehicleEntry = &rideEntry->vehicles[vehicleEntryIndex];
    sint32 edx;

    rct_vehicle *vehicle = (rct_vehicle*)create_sprite(1);
    vehicle->sprite_identifier = SPRITE_IDENTIFIER_VEHICLE;
    vehicle->ride = rideIndex;
    vehicle->ride_subtype = ride->subtype;

    vehicle->vehicle_type = vehicleEntryIndex;
    vehicle->is_child = carIndex == 0 ? 0 : 1;
    vehicle->var_44 = ror32(vehicleEntry->spacing, 10) & 0xFFFF;
    edx = vehicleEntry->spacing >> 1;
    *remainingDistance -= edx;
    vehicle->remaining_distance = *remainingDistance;
    if (!(vehicleEntry->flags & VEHICLE_ENTRY_FLAG_GO_KART)) {
        *remainingDistance -= edx;
    }

    // loc_6DD9A5:
    vehicle->sprite_width = vehicleEntry->sprite_width;
    vehicle->sprite_height_negative = vehicleEntry->sprite_height_negative;
    vehicle->sprite_height_positive = vehicleEntry->sprite_height_positive;
    vehicle->mass = vehicleEntry->car_mass;
    vehicle->num_seats = vehicleEntry->num_seats;
    vehicle->speed = vehicleEntry->powered_max_speed;
    vehicle->powered_acceleration = vehicleEntry->powered_acceleration;
    vehicle->velocity = 0;
    vehicle->acceleration = 0;
    vehicle->swing_sprite = 0;
    vehicle->swinging_car_var_0 = 0;
    vehicle->var_4E = 0;
    vehicle->restraints_position = 0;
    vehicle->spin_sprite = 0;
    vehicle->spin_speed = 0;
    vehicle->sound2_flags = 0;
    vehicle->sound1_id = RCT12_SOUND_ID_NULL;
    vehicle->sound2_id = RCT12_SOUND_ID_NULL;
    vehicle->next_vehicle_on_train = SPRITE_INDEX_NULL;
    vehicle->var_C4 = 0;
    vehicle->animation_frame = 0;
    vehicle->var_C8 = 0;
    vehicle->scream_sound_id = 255;
    vehicle->vehicle_sprite_type = 0;
    vehicle->bank_rotation = 0;
    vehicle->target_seat_rotation = 4;
    vehicle->seat_rotation = 4;
    for (sint32 i = 0; i < 32; i++) {
        vehicle->peep[i] = SPRITE_INDEX_NULL;
    }

    if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_DODGEM_CAR_PLACEMENT) {
        // loc_6DDCA4:
        vehicle->var_CD = 0;
        sint32 direction = tile_element_get_direction(tileElement);
        x += word_9A3AB4[direction].x;
        y += word_9A3AB4[direction].y;
        z = tileElement->base_height * 8;
        vehicle->track_x = x;
        vehicle->track_y = y;
        vehicle->track_z = z;
        vehicle->current_station = tile_element_get_station(tileElement);

        z += RideData5[ride->type].z_offset;

        vehicle->track_type = track_element_get_type(tileElement) << 2;
        vehicle->track_progress = 0;
        vehicle->status = 0;
        vehicle->sub_state = 0;
        vehicle->update_flags = 0;

        LocationXY16 chosenLoc;
        // loc_6DDD26:
        do {
            vehicle->sprite_direction = scenario_rand() & 0x1E;
            chosenLoc.y = y + (scenario_rand() & 0xFF);
            chosenLoc.x = x + (scenario_rand() & 0xFF);
        } while (vehicle_update_dodgems_collision(vehicle, chosenLoc.x, chosenLoc.y, nullptr));

        sprite_move(chosenLoc.x, chosenLoc.y, z, (rct_sprite*)vehicle);
    } else {
        sint16 dl = 0;
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_CHAIRLIFT) {
            dl = 1;
        }

        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_GO_KART) {
            // Choose which lane Go Kart should start in
            dl = 5;
            if (vehicleIndex & 1) {
                dl = 6;
            }
        }
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_MINI_GOLF) {
            dl = 9;
            vehicle->var_D3 = 0;
            vehicle->mini_golf_current_animation = 0;
            vehicle->mini_golf_flags = 0;
        }
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_4) {
            if (!vehicle->is_child) {
                dl = 15;
            }
        }
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_5) {
            dl = 16;
        }
        vehicle->var_CD = dl;

        vehicle->track_x = x;
        vehicle->track_y = y;

        sint32 direction = tile_element_get_direction(tileElement);
        vehicle->sprite_direction = direction << 3;

        if (ride->type == RIDE_TYPE_SPACE_RINGS) {
            direction = 4;
        } else {
            if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_16)) {
                if (RideConstructionDefaultTrackType[ride->type] != FLAT_TRACK_ELEM_1_X_4_B) {
                    if (RideConstructionDefaultTrackType[ride->type] != FLAT_TRACK_ELEM_1_X_4_A) {
                        if (ride->type == RIDE_TYPE_ENTERPRISE) {
                            direction += 5;
                        } else {
                            direction = 4;
                        }
                    }
                }
            }
        }

        x += word_9A2A60[direction].x;
        y += word_9A2A60[direction].y;
        vehicle->track_z = tileElement->base_height * 8;

        vehicle->current_station = tile_element_get_station(tileElement);
        z = tileElement->base_height * 8;
        z += RideData5[ride->type].z_offset;

        sprite_move(x, y, z, (rct_sprite*)vehicle);
        vehicle->track_type = (track_element_get_type(tileElement) << 2) | (vehicle->sprite_direction >> 3);
        vehicle->track_progress = 31;
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_MINI_GOLF) {
            vehicle->track_progress = 15;
        }
        vehicle->update_flags = VEHICLE_UPDATE_FLAG_1;
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_HAS_INVERTED_SPRITE_SET)
        {
            if (track_element_is_inverted(tileElement))
            {
                vehicle->update_flags |= VEHICLE_UPDATE_FLAG_USE_INVERTED_SPRITES;
            }
        }
        vehicle->status = VEHICLE_STATUS_MOVING_TO_END_OF_STATION;
        vehicle->sub_state = 0;
    }

    // loc_6DDD5E:
    vehicle->num_peeps = 0;
    vehicle->next_free_seat = 0;
    return vehicle;
}

/**
 *
 *  rct2: 0x006DD84C
 */
static train_ref vehicle_create_train(sint32 rideIndex, sint32 x, sint32 y, sint32 z, sint32 vehicleIndex, sint32 *remainingDistance, rct_tile_element *tileElement)
{
    Ride *ride = get_ride(rideIndex);

    train_ref train = { nullptr, nullptr };
    for (sint32 carIndex = 0; carIndex < ride->num_cars_per_train; carIndex++) {
        const uint8 vehicle = ride_entry_get_vehicle_at_position(ride->subtype, ride->num_cars_per_train, carIndex);
        rct_vehicle *car = vehicle_create_car(rideIndex, vehicle, carIndex, vehicleIndex, x, y, z, remainingDistance, tileElement);
        if (carIndex == 0) {
            train.head = car;
        } else {
            // Link the previous car with this car
            train.tail->next_vehicle_on_train = car->sprite_index;
            train.tail->next_vehicle_on_ride = car->sprite_index;
            car->prev_vehicle_on_ride = train.tail->sprite_index;
        }
        train.tail = car;
    }
    return train;
}

static void vehicle_create_trains(sint32 rideIndex, sint32 x, sint32 y, sint32 z, rct_tile_element *tileElement)
{
    Ride *ride = get_ride(rideIndex);
    train_ref firstTrain = { nullptr }, lastTrain = { nullptr };
    sint32 remainingDistance = 0;

    for (sint32 vehicleIndex = 0; vehicleIndex < ride->num_vehicles; vehicleIndex++) {
        if (ride_is_block_sectioned(ride)) {
            remainingDistance = 0;
        }
        train_ref train = vehicle_create_train(rideIndex, x, y, z, vehicleIndex, &remainingDistance, tileElement);
        if (vehicleIndex == 0) {
            firstTrain = train;
        } else {
            // Link the end of the previous train with the front of this train
            lastTrain.tail->next_vehicle_on_ride = train.head->sprite_index;
            train.head->prev_vehicle_on_ride = lastTrain.tail->sprite_index;
        }
        lastTrain = train;

        // Add train to ride vehicle list
        move_sprite_to_list((rct_sprite*)train.head, SPRITE_LIST_TRAIN * 2);
        for (sint32 i = 0; i < MAX_VEHICLES_PER_RIDE; i++) {
            if (ride->vehicles[i] == SPRITE_INDEX_NULL) {
                ride->vehicles[i] = train.head->sprite_index;
                break;
            }
        }
    }

    // Link the first train and last train together. Nullptr checks are there to keep Clang happy.
    if (lastTrain.tail != nullptr)
        firstTrain.head->prev_vehicle_on_ride = lastTrain.tail->sprite_index;
    if (firstTrain.head != nullptr)
        lastTrain.tail->next_vehicle_on_ride = firstTrain.head->sprite_index;
}

static void vehicle_unset_update_flag_b1(rct_vehicle *head)
{
    rct_vehicle *vehicle = head;
    while (true) {
        vehicle->update_flags &= ~VEHICLE_UPDATE_FLAG_1;
        uint16 spriteIndex = vehicle->next_vehicle_on_train;
        if (spriteIndex == SPRITE_INDEX_NULL) {
            break;
        }
        vehicle = GET_VEHICLE(spriteIndex);
    }
}

/**
 *
 *  rct2: 0x006DDE9E
 */
static void ride_create_vehicles_find_first_block(Ride *ride, CoordsXYE *outXYElement)
{
    rct_vehicle *vehicle = GET_VEHICLE(ride->vehicles[0]);
    sint32 firstX = vehicle->track_x;
    sint32 firstY = vehicle->track_y;
    sint32 firstZ = vehicle->track_z;
    rct_tile_element *firstElement = map_get_track_element_at(firstX, firstY, firstZ / 8);

    assert(firstElement != nullptr);

    sint32 x = firstX;
    sint32 y = firstY;
    rct_tile_element *trackElement = firstElement;
    track_begin_end trackBeginEnd;
    while (track_block_get_previous(x, y, trackElement, &trackBeginEnd)) {
        x = trackBeginEnd.end_x;
        y = trackBeginEnd.end_y;
        trackElement = trackBeginEnd.begin_element;
        if (x == firstX && y == firstY && trackElement == firstElement) {
            break;
        }

        sint32 trackType = track_element_get_type(trackElement);
        switch (trackType) {
        case TRACK_ELEM_25_DEG_UP_TO_FLAT:
        case TRACK_ELEM_60_DEG_UP_TO_FLAT:
            if (track_element_is_lift_hill(trackElement)) {
                outXYElement->x = x;
                outXYElement->y = y;
                outXYElement->element = trackElement;
                return;
            }
            break;
        case TRACK_ELEM_DIAG_25_DEG_UP_TO_FLAT:
        case TRACK_ELEM_DIAG_60_DEG_UP_TO_FLAT:
            if (track_element_is_lift_hill(trackElement)) {
                rct_tile_element *tileElement = map_get_first_element_at(trackBeginEnd.begin_x >> 5, trackBeginEnd.begin_y >> 5);
                do {
                    if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
                    if (track_element_get_type(tileElement) != trackType) continue;
                    if (tile_element_get_track_sequence(tileElement) != 0) continue;
                    if (tileElement->base_height != trackBeginEnd.begin_z / 8) continue;
                    outXYElement->x = trackBeginEnd.begin_x;
                    outXYElement->y = trackBeginEnd.begin_y;
                    outXYElement->element = tileElement;
                    return;
                } while (!tile_element_is_last_for_tile(tileElement++));
            }
            break;
        case TRACK_ELEM_END_STATION:
        case TRACK_ELEM_CABLE_LIFT_HILL:
        case TRACK_ELEM_BLOCK_BRAKES:
            outXYElement->x = x;
            outXYElement->y = y;
            outXYElement->element = trackElement;
            return;
        }
    }

    outXYElement->x = firstX;
    outXYElement->y = firstY;
    outXYElement->element = firstElement;
}

/**
 *
 *  rct2: 0x006DD84C
 */
static bool ride_create_vehicles(Ride *ride, sint32 rideIndex, CoordsXYE *element, sint32 isApplying)
{
    ride_update_max_vehicles(rideIndex);
    if (ride->subtype == RIDE_ENTRY_INDEX_NULL) {
        return true;
    }

    // Check if there are enough free sprite slots for all the vehicles
    sint32 totalCars = ride->num_vehicles * ride->num_cars_per_train;
    if (totalCars > count_free_misc_sprite_slots()) {
        gGameCommandErrorText = STR_UNABLE_TO_CREATE_ENOUGH_VEHICLES;
        return false;
    }

    if (!isApplying) {
        return true;
    }

    rct_tile_element *tileElement = element->element;
    sint32 x = element->x;
    sint32 y = element->y;
    sint32 z = element->element->base_height;
    sint32 direction = tile_element_get_direction(tileElement);

    //
    if (ride->mode == RIDE_MODE_STATION_TO_STATION) {
        x = element->x - TileDirectionDelta[direction].x;
        y = element->y - TileDirectionDelta[direction].y;

        tileElement = map_get_first_element_at(x >> 5, y >> 5);
        do {
            if (tileElement->base_height != z) continue;
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
            break;
        } while (!tile_element_is_last_for_tile(tileElement++));

        z = tileElement->base_height;
        direction = tile_element_get_direction(tileElement);
    }

    vehicle_create_trains(rideIndex, x, y, z, tileElement);
    // return true;

    // Initialise station departs
// 006DDDD0:
    ride->lifecycle_flags |= RIDE_LIFECYCLE_ON_TRACK;
    for (sint32 i = 0; i < MAX_STATIONS; i++) {
        ride->station_depart[i] = (ride->station_depart[i] & 0x80) | 1;
    }

    //
    if (ride->type != RIDE_TYPE_SPACE_RINGS && !ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_16)) {
        if (ride_is_block_sectioned(ride)) {
            CoordsXYE firstBlock;
            ride_create_vehicles_find_first_block(ride, &firstBlock);
            loc_6DDF9C(ride, firstBlock.element);
        } else {
            for (sint32 i = 0; i < ride->num_vehicles; i++) {
                rct_vehicle *vehicle = GET_VEHICLE(ride->vehicles[i]);

                rct_ride_entry_vehicle *vehicleEntry = vehicle_get_vehicle_entry(vehicle);

                if (!(vehicleEntry->flags & VEHICLE_ENTRY_FLAG_DODGEM_CAR_PLACEMENT)) {
                    vehicle_update_track_motion(vehicle, nullptr);
                }

                vehicle_unset_update_flag_b1(vehicle);
            }
        }
    }
    ride_update_vehicle_colours(rideIndex);
    return true;
}

/**
 *
 *  rct2: 0x006DDF9C
 */
void loc_6DDF9C(Ride *ride, rct_tile_element *tileElement)
{
    rct_vehicle *train, *car;

    for (sint32 i = 0; i < ride->num_vehicles; i++) {
        train = GET_VEHICLE(ride->vehicles[i]);
        if (i == 0) {
            vehicle_update_track_motion(train, nullptr);
            vehicle_unset_update_flag_b1(train);
            continue;
        }

        vehicle_update_track_motion(train, nullptr);

        do {
            tileElement->flags |= TILE_ELEMENT_FLAG_BLOCK_BRAKE_CLOSED;
            car = train;
            while (true) {
                car->velocity = 0;
                car->acceleration = 0;
                car->swing_sprite = 0;
                car->remaining_distance += 13962;

                uint16 spriteIndex = car->next_vehicle_on_train;
                if (spriteIndex == SPRITE_INDEX_NULL) {
                    break;
                }
                car = GET_VEHICLE(spriteIndex);
            }
        } while (!(vehicle_update_track_motion(train, nullptr) & VEHICLE_UPDATE_MOTION_TRACK_FLAG_10));

        tileElement->flags |= TILE_ELEMENT_FLAG_BLOCK_BRAKE_CLOSED;
        car = train;
        while (true) {
            car->update_flags &= ~VEHICLE_UPDATE_FLAG_1;
            car->status = VEHICLE_STATUS_TRAVELLING;
            if ((car->track_type >> 2) == TRACK_ELEM_END_STATION) {
                car->status = VEHICLE_STATUS_MOVING_TO_END_OF_STATION;
            }

            uint16 spriteIndex = car->next_vehicle_on_train;
            if (spriteIndex == SPRITE_INDEX_NULL) {
                break;
            }
            car = GET_VEHICLE(spriteIndex);
        }
    }
}

/**
 * Checks and initialises the cable lift track returns false if unable to find
 * appropriate track.
 *  rct2: 0x006D31A6
 */
static bool ride_initialise_cable_lift_track(Ride *ride, bool isApplying)
{
    LocationXY8 location;
    sint32 stationIndex;
    for (stationIndex = 0; stationIndex < MAX_STATIONS; stationIndex++) {
        location = ride->station_starts[stationIndex];
        if (location.xy != RCT_XY8_UNDEFINED) break;
        if (stationIndex == 3) {
            gGameCommandErrorText = STR_CABLE_LIFT_HILL_MUST_START_IMMEDIATELY_AFTER_STATION;
            return false;
        }
    }

    sint32 x = location.x * 32;
    sint32 y = location.y * 32;
    sint32 z = ride->station_heights[stationIndex];

    bool success = false;
    rct_tile_element *tileElement = map_get_first_element_at(location.x, location.y);
    do {
        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
        if (tileElement->base_height != z) continue;

        if (!(TrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN)) {
            continue;
        }
        success = true;
        break;
    } while (!tile_element_is_last_for_tile(tileElement++));

    if (!success)
        return false;

    enum {
        STATE_FIND_CABLE_LIFT,
        STATE_FIND_STATION,
        STATE_REST_OF_TRACK
    };
    sint32 state = STATE_FIND_CABLE_LIFT;

    track_circuit_iterator it;
    track_circuit_iterator_begin(&it, { x, y, tileElement });
    while (track_circuit_iterator_previous(&it)) {
        tileElement = it.current.element;
        sint32 trackType = track_element_get_type(tileElement);

        uint16 flags = 16;
        switch (state) {
        case STATE_FIND_CABLE_LIFT:
            // Search for a cable lift hill track element
            if (trackType == TRACK_ELEM_CABLE_LIFT_HILL) {
                flags = 8;
                state = STATE_FIND_STATION;
            }
            break;
        case STATE_FIND_STATION:
            // Search for the start of the hill
            switch (trackType) {
            case TRACK_ELEM_FLAT:
            case TRACK_ELEM_25_DEG_UP:
            case TRACK_ELEM_60_DEG_UP:
            case TRACK_ELEM_FLAT_TO_25_DEG_UP:
            case TRACK_ELEM_25_DEG_UP_TO_FLAT:
            case TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP:
            case TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP:
            case TRACK_ELEM_FLAT_TO_60_DEG_UP_LONG_BASE:
                flags = 8;
                break;
            case TRACK_ELEM_END_STATION:
                state = STATE_REST_OF_TRACK;
                break;
            default:
                gGameCommandErrorText = STR_CABLE_LIFT_HILL_MUST_START_IMMEDIATELY_AFTER_STATION;
                return false;
            }
            break;
        }
        if (isApplying) {
            z = tileElement->base_height * 8;
            sint32 direction = tile_element_get_direction(tileElement);
            trackType = track_element_get_type(tileElement);
            x = it.current.x;
            y = it.current.y;
            sub_6C683D(&x, &y, &z, direction, trackType, 0, &tileElement, flags);
        }
    }
    return true;
}

/**
 *
 *  rct2: 0x006DF4D4
 */
static bool ride_create_cable_lift(sint32 rideIndex, bool isApplying)
{
    Ride *ride = get_ride(rideIndex);

    if (ride->mode != RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED &&
        ride->mode != RIDE_MODE_CONTINUOUS_CIRCUIT
    ) {
        gGameCommandErrorText = STR_CABLE_LIFT_UNABLE_TO_WORK_IN_THIS_OPERATING_MODE;
        return false;
    }

    if (ride->num_circuits > 1) {
        gGameCommandErrorText = STR_MULTICIRCUIT_NOT_POSSIBLE_WITH_CABLE_LIFT_HILL;
        return false;
    }

    if (count_free_misc_sprite_slots() <= 5) {
        gGameCommandErrorText = STR_UNABLE_TO_CREATE_ENOUGH_VEHICLES;
        return false;
    }

    if (!ride_initialise_cable_lift_track(ride, isApplying)) {
        return false;
    }

    if (!isApplying) {
        return true;
    }

    sint32 x = ride->cable_lift_x;
    sint32 y = ride->cable_lift_y;
    sint32 z = ride->cable_lift_z;
    rct_tile_element *tileElement = map_get_first_element_at(x >> 5, y >> 5);
    do {
        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
        if (tileElement->base_height != z) continue;
        break;
    } while (!tile_element_is_last_for_tile(tileElement++));
    sint32 direction = tile_element_get_direction(tileElement);

    rct_vehicle *head = nullptr;
    rct_vehicle *tail = nullptr;
    uint32 ebx = 0;
    for (sint32 i = 0; i < 5; i++) {
        uint32 edx = ror32(0x15478, 10);
        uint16 var_44 = edx & 0xFFFF;
        edx = rol32(edx, 10) >> 1;
        ebx -= edx;
        sint32 remaining_distance = ebx;
        ebx -= edx;

        rct_vehicle *current = cable_lift_segment_create(rideIndex, x, y, z, direction, var_44, remaining_distance, i == 0);
        current->next_vehicle_on_train = SPRITE_INDEX_NULL;
        if (i == 0) {
            head = current;
        } else {
            tail->next_vehicle_on_train = current->sprite_index;
            tail->next_vehicle_on_ride = current->sprite_index;
            current->prev_vehicle_on_ride = tail->sprite_index;
        }
        tail = current;
    }
    head->prev_vehicle_on_ride = tail->sprite_index;
    tail->next_vehicle_on_ride = head->sprite_index;

    ride->lifecycle_flags |= RIDE_LIFECYCLE_CABLE_LIFT;
    cable_lift_update_track_motion(head);
    return true;
}

/**
 *
 *  rct2: 0x006B51C0
 */
static void loc_6B51C0(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    if (gUnk141F568 != gUnk13CA740)
        return;

    rct_window *w = window_get_main();
    if (w == nullptr)
        return;

    sint8 entranceOrExit = -1;
    sint32 i;
    for (i = 0; i < MAX_STATIONS; i++) {
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
            continue;

        if (ride_get_entrance_location(rideIndex, i).isNull()) {
            entranceOrExit = 0;
            break;
        }

        if (ride_get_exit_location(rideIndex, i).isNull()) {
            entranceOrExit = 1;
            break;
        }
    }

    if (entranceOrExit == -1)
        return;

    if (ride->type != RIDE_TYPE_MAZE) {
        sint32 x = ride->station_starts[i].x * 32;
        sint32 y = ride->station_starts[i].y * 32;
        sint32 z = ride->station_heights[i] * 8;
        window_scroll_to_location(w, x, y, z);

        CoordsXYE trackElement;
        ride_try_get_origin_element(rideIndex, &trackElement);
        ride_find_track_gap(&trackElement, &trackElement);
        sint32 ok = ride_modify(&trackElement);
        if (ok == 0) {
            return;
        }

        w = window_find_by_class(WC_RIDE_CONSTRUCTION);
        if (w != nullptr)
            window_event_mouse_up_call(w, WC_RIDE_CONSTRUCTION__WIDX_ENTRANCE + entranceOrExit);
    }
}

/**
 *
 *  rct2: 0x006B528A
 */
static void ride_scroll_to_track_error(CoordsXYE *trackElement)
{
    if (!gGameCommandIsNetworked && gUnk141F568 == gUnk13CA740) {
        rct_window *w = window_get_main();
        if (w != nullptr) {
            window_scroll_to_location(w, trackElement->x, trackElement->y, trackElement->element->base_height * 8);
            ride_modify(trackElement);
        }
    }
}

/**
 *
 *  rct2: 0x006B4F6B
 */
static rct_tile_element *loc_6B4F6B(sint32 rideIndex, sint32 x, sint32 y)
{
    Ride * ride = get_ride(rideIndex);
    rct_tile_element *tileElement = map_get_first_element_at(x / 32, y / 32);
    do {
        if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
            continue;

        if (RideProperties[ride->type].flags & RIDE_TYPE_FLAG_FLAT_RIDE) {
            if (!(FlatRideTrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN))
                continue;
        } else {
            if (!(TrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN))
                continue;
        }

        if (track_element_get_ride_index(tileElement) == rideIndex)
            return tileElement;
    } while (!tile_element_is_last_for_tile(tileElement++));

    return nullptr;
}

sint32 ride_is_valid_for_test(sint32 rideIndex, sint32 goingToBeOpen, sint32 isApplying)
{
    sint32 stationIndex;
    Ride *ride;
    CoordsXYE trackElement, problematicTrackElement = { 0 };

    ride = get_ride(rideIndex);
    if (ride->type == RIDE_TYPE_NULL)
    {
        log_warning("Invalid ride type for ride %u", rideIndex);
        return 0;
    }

    window_close_by_number(WC_RIDE_CONSTRUCTION, rideIndex);

    stationIndex = ride_mode_check_station_present(ride);
    if (stationIndex == -1)return 0;

    if (!ride_mode_check_valid_station_numbers(ride))
        return 0;

    if (!ride_check_for_entrance_exit(rideIndex)) {
        loc_6B51C0(rideIndex);
        return 0;
    }

    if (goingToBeOpen && isApplying) {
        sub_6B5952(rideIndex);
        ride->lifecycle_flags |= RIDE_LIFECYCLE_EVER_BEEN_OPENED;
    }

    // z = ride->station_heights[i] * 8;
    trackElement.x = ride->station_starts[stationIndex].x * 32;
    trackElement.y = ride->station_starts[stationIndex].y * 32;
    trackElement.element = loc_6B4F6B(rideIndex, trackElement.x, trackElement.y);
    if (trackElement.element == nullptr) {
        // Maze is strange, station start is 0... investigation required
        if (ride->type != RIDE_TYPE_MAZE)
            return 0;
    }

    if (
        ride->type == RIDE_TYPE_AIR_POWERED_VERTICAL_COASTER ||
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT ||
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED
        ) {
        if (ride_find_track_gap(&trackElement, &problematicTrackElement) && (!gConfigGeneral.test_unfinished_tracks ||
            ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED || ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED)) {
            gGameCommandErrorText = STR_TRACK_IS_NOT_A_COMPLETE_CIRCUIT;
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED
        ) {
        if (!ride_check_block_brakes(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (ride->subtype != 255) {
        rct_ride_entry *rideType = get_ride_entry(ride->subtype);
        if (rideType->flags & RIDE_ENTRY_FLAG_NO_INVERSIONS) {
            gGameCommandErrorText = STR_TRACK_UNSUITABLE_FOR_TYPE_OF_TRAIN;
            if (ride_check_track_contains_inversions(&trackElement, &problematicTrackElement)) {
                ride_scroll_to_track_error(&problematicTrackElement);
                return 0;
            }
        }
        if (rideType->flags & RIDE_ENTRY_FLAG_NO_BANKED_TRACK) {
            gGameCommandErrorText = STR_TRACK_UNSUITABLE_FOR_TYPE_OF_TRAIN;
            if (ride_check_track_contains_banked(&trackElement, &problematicTrackElement)) {
                ride_scroll_to_track_error(&problematicTrackElement);
                return 0;
            }
        }
    }

    if (ride->mode == RIDE_MODE_STATION_TO_STATION) {
        if (!ride_find_track_gap(&trackElement, &problematicTrackElement)) {
            gGameCommandErrorText = STR_RIDE_MUST_START_AND_END_WITH_STATIONS;
            return 0;
        }

        gGameCommandErrorText = STR_STATION_NOT_LONG_ENOUGH;
        if (!ride_check_station_length(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }

        gGameCommandErrorText = STR_RIDE_MUST_START_AND_END_WITH_STATIONS;
        if (!ride_check_start_and_end_is_station(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (isApplying)
        ride_set_start_finish_points(rideIndex, &trackElement);

    if (
        !ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_NO_VEHICLES) &&
        !(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK)
    ) {
        if (!ride_create_vehicles(ride, rideIndex, &trackElement, isApplying)) {
            return 0;
        }
    }

    if (
        (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_ALLOW_CABLE_LIFT_HILL) &&
        (ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT_HILL_COMPONENT_USED) &&
        !(ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT)
        ) {
        if (!ride_create_cable_lift(rideIndex, isApplying))
            return 0;
    }

    return 1;
}
/**
 *
 *  rct2: 0x006B4EEA
 */
sint32 ride_is_valid_for_open(sint32 rideIndex, sint32 goingToBeOpen, sint32 isApplying)
{
    sint32 stationIndex;
    Ride *ride;
    CoordsXYE trackElement, problematicTrackElement = { 0 };

    ride = get_ride(rideIndex);

    // Check to see if construction tool is in use. If it is close the construction window
    // to set the track to its final state and clean up ghosts.
    // We can't just call close as it would cause a stack overflow during shop creation
    // with auto open on.
    if (WC_RIDE_CONSTRUCTION == gCurrentToolWidget.window_classification &&
        rideIndex == gCurrentToolWidget.window_number &&
        (input_test_flag(INPUT_FLAG_TOOL_ACTIVE)))
        window_close_by_number(WC_RIDE_CONSTRUCTION, rideIndex);

    stationIndex = ride_mode_check_station_present(ride);
    if (stationIndex == -1)return 0;

    if (!ride_mode_check_valid_station_numbers(ride))
        return 0;

    if (!ride_check_for_entrance_exit(rideIndex)) {
        loc_6B51C0(rideIndex);
        return 0;
    }

    if (goingToBeOpen && isApplying) {
        sub_6B5952(rideIndex);
        ride->lifecycle_flags |= RIDE_LIFECYCLE_EVER_BEEN_OPENED;
    }

    // z = ride->station_heights[i] * 8;
    trackElement.x = ride->station_starts[stationIndex].x * 32;
    trackElement.y = ride->station_starts[stationIndex].y * 32;
    trackElement.element = loc_6B4F6B(rideIndex, trackElement.x, trackElement.y);
    if (trackElement.element == nullptr) {
        // Maze is strange, station start is 0... investigation required
        if (ride->type != RIDE_TYPE_MAZE)
            return 0;
    }

    if (
        ride->type == RIDE_TYPE_AIR_POWERED_VERTICAL_COASTER ||
        ride->mode == RIDE_MODE_RACE ||
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT ||
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED
    ) {
        if (ride_find_track_gap(&trackElement, &problematicTrackElement)) {
            gGameCommandErrorText = STR_TRACK_IS_NOT_A_COMPLETE_CIRCUIT;
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED
    ) {
        if (!ride_check_block_brakes(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (ride->subtype != 255) {
        rct_ride_entry *rideType = get_ride_entry(ride->subtype);
        if (rideType->flags & RIDE_ENTRY_FLAG_NO_INVERSIONS) {
            gGameCommandErrorText = STR_TRACK_UNSUITABLE_FOR_TYPE_OF_TRAIN;
            if (ride_check_track_contains_inversions(&trackElement, &problematicTrackElement)) {
                ride_scroll_to_track_error(&problematicTrackElement);
                return 0;
            }
        }
        if (rideType->flags & RIDE_ENTRY_FLAG_NO_BANKED_TRACK) {
            gGameCommandErrorText = STR_TRACK_UNSUITABLE_FOR_TYPE_OF_TRAIN;
            if (ride_check_track_contains_banked(&trackElement, &problematicTrackElement)) {
                ride_scroll_to_track_error(&problematicTrackElement);
                return 0;
            }
        }
    }

    if (ride->mode == RIDE_MODE_STATION_TO_STATION) {
        if (!ride_find_track_gap(&trackElement, &problematicTrackElement)) {
            gGameCommandErrorText = STR_RIDE_MUST_START_AND_END_WITH_STATIONS;
            return 0;
        }

        gGameCommandErrorText = STR_STATION_NOT_LONG_ENOUGH;
        if (!ride_check_station_length(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }

        gGameCommandErrorText = STR_RIDE_MUST_START_AND_END_WITH_STATIONS;
        if (!ride_check_start_and_end_is_station(&trackElement, &problematicTrackElement)) {
            ride_scroll_to_track_error(&problematicTrackElement);
            return 0;
        }
    }

    if (isApplying)
        ride_set_start_finish_points(rideIndex, &trackElement);

    if (
        !ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_NO_VEHICLES) &&
        !(ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK)
    ) {
        if (!ride_create_vehicles(ride, rideIndex, &trackElement, isApplying)) {
            return 0;
        }
    }

    if (
        (RideData4[ride->type].flags & RIDE_TYPE_FLAG4_ALLOW_CABLE_LIFT_HILL) &&
        (ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT_HILL_COMPONENT_USED) &&
        !(ride->lifecycle_flags & RIDE_LIFECYCLE_CABLE_LIFT)
    ) {
        if (!ride_create_cable_lift(rideIndex, isApplying))
            return 0;
    }

    return 1;
}

/**
 *
 *  rct2: 0x006CB7FB
 */
sint32 ride_get_refund_price(sint32 ride_id)
{
    uint8 oldpaused = gGamePaused;
    gGamePaused = 0;
    money32 refundPrice = 0;

    tile_element_iterator it;

    tile_element_iterator_begin(&it);
    while (tile_element_iterator_next(&it)) {
        if (tile_element_get_type(it.element) != TILE_ELEMENT_TYPE_TRACK)
            continue;

        if (track_element_get_ride_index(it.element) != ride_id)
            continue;

        sint32 x = it.x * 32, y = it.y * 32;
        sint32 z = it.element->base_height * 8;

        uint8 rotation = tile_element_get_direction(it.element);
        uint8 type = track_element_get_type(it.element);

        if (type != TRACK_ELEM_INVERTED_90_DEG_UP_TO_FLAT_QUARTER_LOOP){
            money32 removePrice = game_do_command(
                x,
                GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_APPLY | (rotation << 8),
                y,
                type | (tile_element_get_track_sequence(it.element) << 8),
                GAME_COMMAND_REMOVE_TRACK,
                z,
                0);
            if (removePrice == MONEY32_UNDEFINED) {
                tile_element_remove(it.element);
            } else {
                refundPrice += removePrice;
            }
            tile_element_iterator_restart_for_tile(&it);
            continue;
        }

        // Using GAME_COMMAND_FLAG_2 for below commands as a HACK to stop fences from being removed
        refundPrice += game_do_command(
            x,
            GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_2 | (0 << 8),
            y,
            ride_id | (2 << 8),
            GAME_COMMAND_SET_MAZE_TRACK,
            z,
            0);
        // Above gamecommand may remove the tile element which will cause the next game command to
        // return MONEY32_UNDEFINED as it does not need to be called.

        money32 removePrice = game_do_command(
            x,
            GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_2 | (1 << 8),
            y + 16,
            ride_id | (2 << 8),
            GAME_COMMAND_SET_MAZE_TRACK,
            z,
            0);

        if (removePrice == MONEY32_UNDEFINED &&
            gGameCommandErrorText == 0)
        {
            tile_element_iterator_restart_for_tile(&it);
            continue;
        }

        refundPrice += removePrice;

        removePrice = game_do_command(
            x + 16,
            GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_2 | (2 << 8),
            y + 16,
            ride_id | (2 << 8),
            GAME_COMMAND_SET_MAZE_TRACK,
            z,
            0);
        if (refundPrice == MONEY32_UNDEFINED &&
            gGameCommandErrorText == 0)
        {
            tile_element_iterator_restart_for_tile(&it);
            continue;
        }

        refundPrice += removePrice;

        removePrice = game_do_command(
            x + 16,
            GAME_COMMAND_FLAG_5 | GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_2 | (3 << 8),
            y,
            ride_id | (2 << 8),
            GAME_COMMAND_SET_MAZE_TRACK,
            z,
            0);

        if (removePrice != MONEY32_UNDEFINED)
        {
            refundPrice += removePrice;
        }
        tile_element_iterator_restart_for_tile(&it);
    }
    gGamePaused = oldpaused;
    return refundPrice;
}

/**
 *
 *  rct2: 0x00696707
 */
void ride_stop_peeps_queuing(sint32 rideIndex)
{
    uint16 spriteIndex;
    rct_peep *peep;

    FOR_ALL_PEEPS(spriteIndex, peep) {
        if (peep->state != PEEP_STATE_QUEUING)
            continue;
        if (peep->current_ride != rideIndex)
            continue;

        remove_peep_from_queue(peep);
        peep_decrement_num_riders(peep);

        peep->state = PEEP_STATE_FALLING;

        peep_window_state_update(peep);
    }
}

sint32 ride_get_empty_slot()
{
    for (sint32 i = 0; i < MAX_RIDES; i++) {
        Ride *ride = get_ride(i);
        if (ride->type == RIDE_TYPE_NULL) {
            return i;
        }
    }
    return -1;
}

sint32 ride_get_default_mode(Ride *ride)
{
    const rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
    const uint8 *availableModes = RideAvailableModes;

    for (sint32 i = 0; i < ride->type; i++) {
        while (*(availableModes++) != 255) {}
    }
    // Since this only selects a default mode and does not prevent other modes from being used, there is no need
    // to check if select-by-track-type or the all-ride-modes cheat have been enabled.
    if (rideEntry->flags & RIDE_ENTRY_DISABLE_FIRST_TWO_OPERATING_MODES)
    {
        availableModes += 2;
    }
    return availableModes[0];
}

static bool ride_with_colour_config_exists(uint8 ride_type, const track_colour *colours)
{
    Ride *ride;
    sint32 i;

    FOR_ALL_RIDES(i, ride) {
        if (ride->type != ride_type) continue;
        if (ride->track_colour_main[0] != colours->main) continue;
        if (ride->track_colour_additional[0] != colours->additional) continue;
        if (ride->track_colour_supports[0] != colours->supports) continue;

        return true;
    }
    return false;
}

static bool ride_name_exists(char *name)
{
    char buffer[256];
    Ride *ride;
    sint32 i;
    FOR_ALL_RIDES(i, ride) {
        format_string(buffer, 256, ride->name, &ride->name_arguments);
        if ((strcmp(buffer, name) == 0) && ride_has_any_track_elements(i)) {
            return true;
        }
    }
    return false;
}

/**
 *
 *  Based on rct2: 0x006B4776
 */
sint32 ride_get_random_colour_preset_index(uint8 ride_type)
{
    if (ride_type >= 128)
    {
        return 0;
    }

    const track_colour_preset_list *colourPresets = &RideColourPresets[ride_type];

    // 200 attempts to find a colour preset that hasn't already been used in the park for this ride type
    for (sint32 i = 0; i < 200; i++) {
        sint32 listIndex = util_rand() % colourPresets->count;
        const track_colour *colours = &colourPresets->list[listIndex];

        if (!ride_with_colour_config_exists(ride_type, colours)) {
            return listIndex;
        }
    }
    return 0;
}

/**
 *
 *  Based on rct2: 0x006B4776
 */
void ride_set_colour_preset(Ride *ride, uint8 index)
{
    const track_colour_preset_list * colourPresets = &RideColourPresets[ride->type];
    track_colour colours = { COLOUR_BLACK, COLOUR_BLACK, COLOUR_BLACK };
    if (index < colourPresets->count) {
        colours = colourPresets->list[index];
    }
    for (sint32 i = 0; i < NUM_COLOUR_SCHEMES; i++) {
        ride->track_colour_main[i] = colours.main;
        ride->track_colour_additional[i] = colours.additional;
        ride->track_colour_supports[i] = colours.supports;
    }
    ride->colour_scheme_type = 0;
}

money32 ride_get_common_price(Ride *forRide)
{
    Ride *ride;
    sint32 i;

    FOR_ALL_RIDES(i, ride) {
        if (ride->type == forRide->type && ride != forRide) {
            return ride->price;
        }
    }

    return MONEY32_UNDEFINED;
}

money32 shop_item_get_common_price(Ride *forRide, sint32 shopItem)
{
    rct_ride_entry *rideEntry;
    Ride *ride;
    sint32 i;

    FOR_ALL_RIDES(i, ride) {
        if (ride != forRide) {
            rideEntry = get_ride_entry(ride->subtype);
            if (rideEntry == nullptr) {
                continue;
            }
            if (rideEntry->shop_item == shopItem) {
                return ride->price;
            }
            if (rideEntry->shop_item_secondary == shopItem) {
                return ride->price_secondary;
            }
            if (shop_item_is_photo(shopItem) && (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_RIDE_PHOTO)) {
                return ride->price_secondary;
            }
        }
    }

    return MONEY32_UNDEFINED;
}

bool shop_item_is_photo(sint32 shopItem)
{
    return (
        shopItem == SHOP_ITEM_PHOTO || shopItem == SHOP_ITEM_PHOTO2 ||
        shopItem == SHOP_ITEM_PHOTO3 || shopItem == SHOP_ITEM_PHOTO4
    );
}

bool shop_item_has_common_price(sint32 shopItem)
{
    if (shopItem < 32) {
        return gSamePriceThroughoutParkA & (1u << shopItem);
    } else {
        return gSamePriceThroughoutParkB & (1u << (shopItem - 32));
    }
}

void ride_set_name_to_default(Ride * ride, rct_ride_entry * rideEntry)
{
    if (RideGroupManager::RideTypeIsIndependent(ride->type))
    {
        ride_set_name_to_vehicle_default(ride, rideEntry);
    }
    else
    {
        ride_set_name_to_track_default(ride, rideEntry);
    }
}

void ride_set_name_to_track_default(Ride *ride, rct_ride_entry * rideEntry)
{
    char rideNameBuffer[256];
    ride_name_args name_args;

    ride->name = STR_NONE;

    if (RideGroupManager::RideTypeHasRideGroups(ride->type))
    {
        const RideGroup * rideGroup = RideGroupManager::GetRideGroup(ride->type, rideEntry);
        name_args.type_name = rideGroup->Naming.name;
    }
    else
    {
        name_args.type_name = RideNaming[ride->type].name;
    }

    name_args.number = 0;
    do
    {
        name_args.number++;
        format_string(rideNameBuffer, 256, 1, &name_args);
    }
    while (ride_name_exists(rideNameBuffer));

    ride->name = 1;
    ride->name_arguments_type_name = name_args.type_name;
    ride->name_arguments_number = name_args.number;
}

static void ride_set_name_to_vehicle_default(Ride * ride, rct_ride_entry * rideEntry)
{
    char rideNameBuffer[256];
    ride_name_args name_args;

    ride->name = 1;
    ride->name_arguments_type_name = rideEntry->naming.name;
    rct_string_id rideNameStringId = 0;
    name_args.type_name = rideEntry->naming.name;
    name_args.number = 0;

    do {
        name_args.number++;
        format_string(rideNameBuffer, 256, ride->name, &name_args);
    } while (ride_name_exists(rideNameBuffer));

    ride->name_arguments_type_name = name_args.type_name;
    ride->name_arguments_number = name_args.number;

    rideNameStringId = user_string_allocate(USER_STRING_HIGH_ID_NUMBER | USER_STRING_DUPLICATION_PERMITTED, rideNameBuffer);
    if (rideNameStringId != 0) {
        ride->name = rideNameStringId;
    } else {
        ride_set_name_to_track_default(ride, rideEntry);
    }
}

/**
 * This will return the name of the ride, as seen in the New Ride window.
 */
rct_ride_name get_ride_naming(const uint8 rideType, rct_ride_entry * rideEntry)
{
    if (RideGroupManager::RideTypeHasRideGroups(rideType))
    {
        const RideGroup * rideGroup = RideGroupManager::GetRideGroup(rideType, rideEntry);
        return rideGroup->Naming;
    }
    else if (!RideGroupManager::RideTypeIsIndependent(rideType))
    {
        return RideNaming[rideType];
    }
    else
    {
        return rideEntry->naming;
    }
}

/**
 *
 * Network client callback when placing ride pieces
 *   Client does execute placing the piece on the same tick as mouse_up - waits for server command
 * Re-executes function from ride_construction - window_ride_construction_construct()
 * Only uses part that deals with construction state
 */

void game_command_callback_ride_construct_placed_back(sint32 eax, sint32 ebx, sint32 ecx, sint32 edx, sint32 esi, sint32 edi, sint32 ebp)
{
    sint32 trackDirection, x, y, z;
    track_begin_end trackBeginEnd;

    trackDirection = _currentTrackPieceDirection ^ 2;
    x = _currentTrackBeginX;
    y = _currentTrackBeginY;
    z = _currentTrackBeginZ;
    if (!(trackDirection & 4)) {
        x += TileDirectionDelta[trackDirection].x;
        y += TileDirectionDelta[trackDirection].y;
    }

    if (track_block_get_previous_from_zero(x, y, z, _currentRideIndex, trackDirection, &trackBeginEnd)) {
        _currentTrackBeginX = trackBeginEnd.begin_x;
        _currentTrackBeginY = trackBeginEnd.begin_y;
        _currentTrackBeginZ = trackBeginEnd.begin_z;
        _currentTrackPieceDirection = trackBeginEnd.begin_direction;
        _currentTrackPieceType = track_element_get_type(trackBeginEnd.begin_element);
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
        ride_select_previous_section();
    }
    else {
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_0;
    }

    window_ride_construction_do_station_check();
    window_ride_construction_update_active_elements();
}

void game_command_callback_ride_construct_placed_front(sint32 eax, sint32 ebx, sint32 ecx, sint32 edx, sint32 esi, sint32 edi, sint32 ebp)
{
    sint32 trackDirection, x, y, z;

    trackDirection = _currentTrackPieceDirection;
    x = _currentTrackBeginX;
    y = _currentTrackBeginY;
    z = _currentTrackBeginZ;
    if (!(trackDirection & 4)) {
        x -= TileDirectionDelta[trackDirection].x;
        y -= TileDirectionDelta[trackDirection].y;
    }

    CoordsXYE next_track;
    if (track_block_get_next_from_zero(x, y, z, _currentRideIndex, trackDirection, &next_track, &z, &trackDirection)) {
        _currentTrackBeginX = next_track.x;
        _currentTrackBeginY = next_track.y;
        _currentTrackBeginZ = z;
        _currentTrackPieceDirection = tile_element_get_direction(next_track.element);
        _currentTrackPieceType = track_element_get_type(next_track.element);
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
        ride_select_next_section();
    }
    else {
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_0;
    }

    window_ride_construction_do_station_check();
    window_ride_construction_do_entrance_exit_check();
    window_ride_construction_update_active_elements();
}

/**
*
* Network client callback when removing ride pieces
*   Client does execute placing the piece on the same tick as mouse_up - waits for server command
* Re-executes function from ride_construction - window_ride_construction_mouseup_demolish()
* Only uses part that deals with construction state
*/

void game_command_callback_ride_remove_track_piece(sint32 eax, sint32 ebx, sint32 ecx, sint32 edx, sint32 esi, sint32 edi, sint32 ebp)
{
    sint32 x, y, z, direction, type;

    x = gRideRemoveTrackPieceCallbackX;
    y = gRideRemoveTrackPieceCallbackY;
    z = gRideRemoveTrackPieceCallbackZ;
    direction = gRideRemoveTrackPieceCallbackDirection;
    type = gRideRemoveTrackPieceCallbackType;

    window_ride_construction_mouseup_demolish_next_piece(x, y, z, direction, type);
}

/**
 *
 *  rct2: 0x006B2FC5
 */
void game_command_set_ride_appearance(sint32 *eax, sint32 *ebx, sint32 *ecx, sint32 *edx, sint32 *esi, sint32 *edi, sint32 *ebp)
{
    bool apply = (*ebx & GAME_COMMAND_FLAG_APPLY);

    uint8 ride_id = *edx;
    if (ride_id >= MAX_RIDES)
    {
        log_warning("Invalid game command for ride %u", ride_id);
        *ebx = MONEY32_UNDEFINED;
        return;
    }
    uint8 type = *ebx >> 8;
    uint8 value = *edx >> 8;
    uint32 index = (uint32)*edi;

    if (*edi < 0) {
        log_warning("Invalid game command, index %d out of bounds", index);
        *ebx = MONEY32_UNDEFINED;
        return;
    }

    Ride *ride = get_ride(ride_id);
    if (ride->type == RIDE_TYPE_NULL) {
        log_warning("Invalid game command, ride_id = %u", ride_id);
        *ebx = MONEY32_UNDEFINED;
        return;
    }

    if (apply && gGameCommandNestLevel == 1) {
        if (ride->overall_view.xy != RCT_XY8_UNDEFINED) {
            LocationXYZ16 coord;
            coord.x = ride->overall_view.x * 32 + 16;
            coord.y = ride->overall_view.y * 32 + 16;
            coord.z = tile_element_height(coord.x, coord.y);
            network_set_player_last_action_coord(network_get_player_index(game_command_playerid), coord);
        }
    }

    *ebx = 0;
    switch(type) {
    case 0:
        if (index >= Util::CountOf(ride->track_colour_main)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        }
        if (apply) {
            ride->track_colour_main[index] = value;
            gfx_invalidate_screen();
        }
        break;
    case 1:
        if (index >= Util::CountOf(ride->track_colour_additional)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        }
        if (apply) {
            ride->track_colour_additional[index] = value;
            gfx_invalidate_screen();
        }
        break;
    case 2:
        if (index >= Util::CountOf(ride->vehicle_colours)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        }
        if (apply) {
            *((uint8*)(&ride->vehicle_colours[index])) = value;
            ride_update_vehicle_colours(ride_id);
        }
        break;
    case 3:
        if (index >= Util::CountOf(ride->vehicle_colours)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        }
        if (apply) {
            *((uint8*)(&ride->vehicle_colours[index]) + 1) = value;
            ride_update_vehicle_colours(ride_id);
        }
        break;
    case 4:
        if (index >= Util::CountOf(ride->track_colour_supports)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        }
        if (apply) {
            ride->track_colour_supports[index] = value;
            gfx_invalidate_screen();
        }
        break;
    case 5:
        if (apply) {
            ride->colour_scheme_type &= ~(RIDE_COLOUR_SCHEME_DIFFERENT_PER_TRAIN | RIDE_COLOUR_SCHEME_DIFFERENT_PER_CAR);
            ride->colour_scheme_type |= value;
            for (uint32 i = 1; i < Util::CountOf(ride->vehicle_colours); i++) {
                ride->vehicle_colours[i] = ride->vehicle_colours[0];
                ride->vehicle_colours_extended[i] = ride->vehicle_colours_extended[0];
            }
            ride_update_vehicle_colours(ride_id);
        }
        break;
    case 6:
        if (apply) {
            ride->entrance_style = value;
            gLastEntranceStyle = value;
            gfx_invalidate_screen();
        }
        break;
    case 7:
        if (index >= Util::CountOf(ride->vehicle_colours_extended)) {
            log_warning("Invalid game command, index %d out of bounds", index);
            *ebx = MONEY32_UNDEFINED;
            return;
        } else {
            if (apply) {
                ride->vehicle_colours_extended[index] = value;
                ride_update_vehicle_colours(ride_id);
            }
        }
        break;
    }
    window_invalidate_by_number(WC_RIDE, ride_id);
}

/**
 *
 *  rct2: 0x006B53E9
 */
void game_command_set_ride_price(sint32 *eax, sint32 *ebx, sint32 *ecx, sint32 *edx, sint32 *esi, sint32 *edi, sint32 *ebp)
{
    uint32 flags = *ebx;
    uint8 ride_number = (*edx & 0xFF);
    if (ride_number >= MAX_RIDES)
    {
        log_warning("Invalid game command for ride %u", ride_number);
        *ebx = MONEY32_UNDEFINED;
        return;
    }
    Ride *ride = get_ride(ride_number);
    if (ride->type == RIDE_TYPE_NULL)
    {
        log_warning("Invalid game command for ride %u", ride_number);
        *ebx = MONEY32_UNDEFINED;
        return;
    }
    rct_ride_entry * rideEntry = get_ride_entry(ride->subtype);
    money16 price = *edi;
    bool secondary_price = (*edx >> 8);

    if (rideEntry == nullptr)
    {
        log_warning("Invalid game command for ride %u", ride_number);
        *ebx = MONEY32_UNDEFINED;
        return;
    }

    //eax
    //ebx flags
    //ecx ecx
    //edx ride_number
    //ebp rideEntry

    *ebx = 0; // for cost check - changing ride price does not cost anything

    gCommandExpenditureType = RCT_EXPENDITURE_TYPE_PARK_RIDE_TICKETS;
    if (flags & GAME_COMMAND_FLAG_APPLY) {
        uint32 shop_item;

        if (ride->overall_view.xy != RCT_XY8_UNDEFINED) {
            LocationXYZ16 coord;
            coord.x = ride->overall_view.x * 32 + 16;
            coord.y = ride->overall_view.y * 32 + 16;
            coord.z = tile_element_height(coord.x, coord.y);
            network_set_player_last_action_coord(network_get_player_index(game_command_playerid), coord);
        }

        if (!secondary_price) {
            shop_item = SHOP_ITEM_ADMISSION;
            if (ride->type != RIDE_TYPE_TOILETS) {
                shop_item = rideEntry->shop_item;
                if (shop_item == SHOP_ITEM_NONE) {
                    ride->price = price;
                    window_invalidate_by_class(WC_RIDE);
                    return;
                }
            }
            // Check same price in park flags
            if (!shop_item_has_common_price(shop_item)) {
                ride->price = price;
                window_invalidate_by_class(WC_RIDE);
                return;
            }
        }
        else {
            shop_item = rideEntry->shop_item_secondary;
            if (shop_item == SHOP_ITEM_NONE) {
                shop_item = RidePhotoItems[ride->type];
                if ((ride->lifecycle_flags & RIDE_LIFECYCLE_ON_RIDE_PHOTO) == 0) {
                    ride->price_secondary = price;
                    window_invalidate_by_class(WC_RIDE);
                    return;
                }
            }
            // Check same price in park flags
            if (!shop_item_has_common_price(shop_item)) {
                ride->price_secondary = price;
                window_invalidate_by_class(WC_RIDE);
                return;
            }
        }
        ride = get_ride(0);

        for (uint8 rideId = 0; rideId < MAX_RIDES; rideId++, ride++) {
            // Unplaced rides have a type of NULL
            if (ride->type == RIDE_TYPE_NULL)
                continue;

            rideEntry = get_ride_entry(ride->subtype);

            if (ride->type != RIDE_TYPE_TOILETS || shop_item != SHOP_ITEM_ADMISSION) {
                if (rideEntry->shop_item == shop_item) {
                    ride->price = price;
                    window_invalidate_by_number(WC_RIDE, rideId);
                }
            }
            else {
                ride->price = price;
                window_invalidate_by_number(WC_RIDE, rideId);
            }
            // If the shop item is the same or an on-ride photo
            if (rideEntry->shop_item_secondary == shop_item ||
                (rideEntry->shop_item_secondary == SHOP_ITEM_NONE && shop_item_is_photo(shop_item))) {

                ride->price_secondary = price;
                window_invalidate_by_number(WC_RIDE, rideId);
            }
        }
    }
}

bool ride_type_has_flag(sint32 rideType, sint32 flag)
{
    return (RideProperties[rideType].flags & flag) != 0;
}

/*
 * The next eight functions are helpers to access ride data at the offset 10E &
 * 110. Known as the turn counts. There are 3 different types (default, banked, sloped)
 * and there are 4 counts as follows:
 *
 * 1 element turns: low 5 bits
 * 2 element turns: bits 6-8
 * 3 element turns: bits 9-11
 * 4 element or more turns: bits 12-15
 *
 * 4 plus elements only possible on sloped type. Falls back to 3 element
 * if by some miracle you manage 4 element none sloped.
 */

void increment_turn_count_1_element(Ride* ride, uint8 type){
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return;
    }
    uint16 value = (*turn_count & TURN_MASK_1_ELEMENT) + 1;
    *turn_count &= ~TURN_MASK_1_ELEMENT;

    if (value > TURN_MASK_1_ELEMENT)
        value = TURN_MASK_1_ELEMENT;
    *turn_count |= value;
}

void increment_turn_count_2_elements(Ride* ride, uint8 type){
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return;
    }
    uint16 value = (*turn_count & TURN_MASK_2_ELEMENTS) + 0x20;
    *turn_count &= ~TURN_MASK_2_ELEMENTS;

    if (value > TURN_MASK_2_ELEMENTS)
        value = TURN_MASK_2_ELEMENTS;
    *turn_count |= value;
}

void increment_turn_count_3_elements(Ride* ride, uint8 type){
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return;
    }
    uint16 value = (*turn_count & TURN_MASK_3_ELEMENTS) + 0x100;
    *turn_count &= ~TURN_MASK_3_ELEMENTS;

    if (value > TURN_MASK_3_ELEMENTS)
        value = TURN_MASK_3_ELEMENTS;
    *turn_count |= value;
}

void increment_turn_count_4_plus_elements(Ride* ride, uint8 type){
    uint16* turn_count;
    switch (type){
    case 0:
    case 1:
        // Just in case fallback to 3 element turn
        increment_turn_count_3_elements(ride, type);
        return;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return;
    }
    uint16 value = (*turn_count & TURN_MASK_4_PLUS_ELEMENTS) + 0x800;
    *turn_count &= ~TURN_MASK_4_PLUS_ELEMENTS;

    if (value > TURN_MASK_4_PLUS_ELEMENTS)
        value = TURN_MASK_4_PLUS_ELEMENTS;
    *turn_count |= value;
}

sint32 get_turn_count_1_element(Ride* ride, uint8 type) {
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return 0;
    }

    return (*turn_count) & TURN_MASK_1_ELEMENT;
}

sint32 get_turn_count_2_elements(Ride* ride, uint8 type) {
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return 0;
    }

    return ((*turn_count) & TURN_MASK_2_ELEMENTS) >> 5;
}

sint32 get_turn_count_3_elements(Ride* ride, uint8 type) {
    uint16* turn_count;
    switch (type){
    case 0:
        turn_count = &ride->turn_count_default;
        break;
    case 1:
        turn_count = &ride->turn_count_banked;
        break;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return 0;
    }

    return ((*turn_count) & TURN_MASK_3_ELEMENTS) >> 8;
}

sint32 get_turn_count_4_plus_elements(Ride* ride, uint8 type) {
    uint16* turn_count;
    switch (type){
    case 0:
    case 1:
        return 0;
    case 2:
        turn_count = &ride->turn_count_sloped;
        break;
    default:
        return 0;
    }

    return ((*turn_count) & TURN_MASK_4_PLUS_ELEMENTS) >> 11;
}

bool ride_has_spinning_tunnel(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_TUNNEL_SPLASH_OR_RAPIDS;
}

bool ride_has_water_splash(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_TUNNEL_SPLASH_OR_RAPIDS;
}

bool ride_has_rapids(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_TUNNEL_SPLASH_OR_RAPIDS;
}

bool ride_has_log_reverser(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_REVERSER_OR_WATERFALL;
}

bool ride_has_waterfall(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_REVERSER_OR_WATERFALL;
}

bool ride_has_whirlpool(Ride *ride) {
    return ride->special_track_elements & RIDE_ELEMENT_WHIRLPOOL;
}

uint8 ride_get_helix_sections(Ride *ride) {
    // Helix sections stored in the low 5 bits.
    return ride->special_track_elements & 0x1F;
}

bool ride_is_powered_launched(Ride *ride)
{
    return
        ride->mode == RIDE_MODE_POWERED_LAUNCH_PASSTROUGH ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED;
}

bool ride_is_block_sectioned(Ride *ride)
{
    return
        ride->mode == RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED ||
        ride->mode == RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED;
}

bool ride_has_any_track_elements(sint32 rideIndex)
{
    tile_element_iterator it;

    tile_element_iterator_begin(&it);
    while (tile_element_iterator_next(&it)) {
        if (tile_element_get_type(it.element) != TILE_ELEMENT_TYPE_TRACK)
            continue;
        if (track_element_get_ride_index(it.element) != rideIndex)
            continue;
        if (it.element->flags & TILE_ELEMENT_FLAG_GHOST)
            continue;

        return true;
    }

    return false;
}

void ride_all_has_any_track_elements(bool *rideIndexArray)
{
    tile_element_iterator it;

    memset(rideIndexArray, 0, MAX_RIDES * sizeof(bool));

    tile_element_iterator_begin(&it);
    while (tile_element_iterator_next(&it)) {
        if (tile_element_get_type(it.element) != TILE_ELEMENT_TYPE_TRACK)
            continue;
        if (it.element->flags & TILE_ELEMENT_FLAG_GHOST)
            continue;

        rideIndexArray[track_element_get_ride_index(it.element)] = true;
    }
}

/**
 *
 *  rct2: 0x006847BA
 */
void set_vehicle_type_image_max_sizes(rct_ride_entry_vehicle* vehicle_type, sint32 num_images){
    uint8 bitmap[200][200] = { 0 };

    rct_drawpixelinfo dpi = {
        /*.bits = */(uint8*)bitmap,
        /*.x = */-100,
        /*.y = */-100,
        /*.width = */200,
        /*.height = */200,
        /*.pitch = */0,
        /*.zoom_level = */0
    };

    for (sint32 i = 0; i < num_images; ++i){
        gfx_draw_sprite_software(&dpi, vehicle_type->base_image_id + i, 0, 0, 0);
    }
    sint32 al = -1;
    for (sint32 i = 99; i != 0; --i){
        for (sint32 j = 0; j < 200; j++){
            if (bitmap[j][100 - i] != 0){
                al = i;
                break;
            }
        }

        if (al != -1)
            break;

        for (sint32 j = 0; j < 200; j++){
            if (bitmap[j][100 + i] != 0){
                al = i;
                break;
            }
        }

        if (al != -1)
            break;
    }

    al++;
    sint32 bl = -1;

    for (sint32 i = 99; i != 0; --i){
        for (sint32 j = 0; j < 200; j++){
            if (bitmap[100 - i][j] != 0){
                bl = i;
                break;
            }
        }

        if (bl != -1)
            break;
    }
    bl++;

    sint32 bh = -1;

    for (sint32 i = 99; i != 0; --i){
        for (sint32 j = 0; j < 200; j++){
            if (bitmap[100 + i][j] != 0){
                bh = i;
                break;
            }
        }

        if (bh != -1)
            break;
    }
    bh++;

    // Moved from object paint

    if (vehicle_type->flags & VEHICLE_ENTRY_FLAG_13){
        bl += 16;
    }

    vehicle_type->sprite_width = al;
    vehicle_type->sprite_height_negative = bl;
    vehicle_type->sprite_height_positive = bh;
}

static sint32 loc_6CD18E(sint16 mapX, sint16 mapY, sint16 entranceMinX, sint16 entranceMinY, sint16 entranceMaxX, sint16 entranceMaxY)
{
    sint32 direction = 0;
    if (mapX == entranceMinX) {
        if (mapY > entranceMinY && mapY < entranceMaxY) {
            return direction;
        }
    }
    direction = 1;
    if (mapY == entranceMaxY) {
        if (mapX > entranceMinX && mapX < entranceMaxX) {
            return direction;
        }
    }
    direction = 2;
    if (mapX == entranceMaxX) {
        if (mapY > entranceMinY && mapY < entranceMaxY) {
            return direction;
        }
    }
    direction = 3;
    if (mapY == entranceMinY) {
        if (mapX > entranceMinX && mapX < entranceMaxX) {
            return direction;
        }
    }
    return -1;
}


/**
 *
 *  rct2: 0x006CCF70
 */
void ride_get_entrance_or_exit_position_from_screen_position(sint32 screenX, sint32 screenY, sint32 * outX, sint32 * outY, sint32 * outDirection)
{
    sint16            mapX, mapY;
    sint16            entranceMinX, entranceMinY, entranceMaxX, entranceMaxY, word_F4418C, word_F4418E;
    sint32            interactionType, direction, stationHeight, stationDirection;
    rct_tile_element * tileElement;
    rct_viewport    * viewport;
    Ride            * ride;

    gRideEntranceExitPlaceDirection = 255;
    get_map_coordinates_from_pos(screenX, screenY, 0xFFFB, &mapX, &mapY, &interactionType, &tileElement, &viewport);
    if (interactionType != 0)
    {
        if (tile_element_get_type(tileElement) == TILE_ELEMENT_TYPE_TRACK)
        {
            if (track_element_get_ride_index(tileElement) == gRideEntranceExitPlaceRideIndex)
            {
                if (TrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN)
                {
                    if (track_element_get_type(tileElement) == TRACK_ELEM_MAZE)
                    {
                        gRideEntranceExitPlaceStationIndex = 0;
                    }
                    else
                    {
                        gRideEntranceExitPlaceStationIndex = tile_element_get_station(tileElement);
                    }
                }
            }
        }
    }

    ride          = get_ride(gRideEntranceExitPlaceRideIndex);
    stationHeight = ride->station_heights[gRideEntranceExitPlaceStationIndex];

    screen_get_map_xy_with_z(screenX, screenY, stationHeight * 8, &mapX, &mapY);
    if (mapX == LOCATION_NULL)
    {
        *outX = 0x8000;
        return;
    }

    word_F4418C = mapX;
    word_F4418E = mapY;
    _unkF44188.x = floor2(mapX, 32);
    _unkF44188.y = floor2(mapY, 32);
    *outX = _unkF44188.x;
    *outY = _unkF44188.y;

    if (ride->type == RIDE_TYPE_NULL)
        return;

    LocationXY8 stationStart = ride->station_starts[gRideEntranceExitPlaceStationIndex];
    if (stationStart.xy == RCT_XY8_UNDEFINED)
        return;

    _unkF44188.z = stationHeight;

    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_3))
    {
        mapX = (word_F4418C & 0x1F) - 16;
        mapY = (word_F4418E & 0x1F) - 16;
        if (std::abs(mapX) < std::abs(mapY))
        {
            direction = mapY < 0 ? 3 : 1;
        }
        else
        {
            direction = mapX < 0 ? 0 : 2;
        }

        for (sint32 i = 0; i < MAX_STATIONS; i++)
        {
            mapX = _unkF44188.x + TileDirectionDelta[direction].x;
            mapY = _unkF44188.y + TileDirectionDelta[direction].y;
            if (mapX >= 0 && mapY >= 0 && mapX < (256 * 32) && mapY < (256 * 32))
            {
                tileElement = map_get_first_element_at(mapX >> 5, mapY >> 5);
                do
                {
                    if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
                        continue;
                    if (tileElement->base_height != stationHeight)
                        continue;
                    if (track_element_get_ride_index(tileElement) != gRideEntranceExitPlaceRideIndex)
                        continue;
                    if (track_element_get_type(tileElement) == TRACK_ELEM_INVERTED_90_DEG_UP_TO_FLAT_QUARTER_LOOP)
                    {
                        gRideEntranceExitPlaceDirection = direction ^ 2;
                        *outDirection = direction ^ 2;
                        return;
                    }
                    if (tile_element_get_station(tileElement) != gRideEntranceExitPlaceStationIndex)
                        continue;

                    sint32 eax = (direction + 2 - tile_element_get_direction(tileElement)) & TILE_ELEMENT_DIRECTION_MASK;
                    if (FlatRideTrackSequenceProperties[track_element_get_type(tileElement)][tile_element_get_track_sequence(tileElement)] & (1 << eax))
                    {
                        gRideEntranceExitPlaceDirection = direction ^ 2;
                        *outDirection = direction ^ 2;
                        return;
                    }
                }
                while (!tile_element_is_last_for_tile(tileElement++));
            }
            direction = (direction + 1) & 3;
        }
        gRideEntranceExitPlaceDirection = 0xFF;
    }
    else
    {
        mapX         = stationStart.x * 32;
        mapY         = stationStart.y * 32;
        entranceMinX = mapX;
        entranceMinY = mapY;

        tileElement = ride_get_station_start_track_element(ride, gRideEntranceExitPlaceStationIndex);
        if (tileElement == nullptr)
        {
            *outX = 0x8000;
            return;
        }
        direction        = tile_element_get_direction(tileElement);
        stationDirection = direction;

        while (true)
        {
            entranceMaxX = mapX;
            entranceMaxY = mapY;
            mapX -= TileDirectionDelta[direction].x;
            mapY -= TileDirectionDelta[direction].y;
            tileElement = map_get_first_element_at(mapX >> 5, mapY >> 5);
            bool goToNextTile = false;

            do
            {
                if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
                    continue;
                if (track_element_get_ride_index(tileElement) != gRideEntranceExitPlaceRideIndex)
                    continue;
                if (tile_element_get_station(tileElement) != gRideEntranceExitPlaceStationIndex)
                    continue;

                switch (track_element_get_type(tileElement))
                {
                case TRACK_ELEM_END_STATION:
                case TRACK_ELEM_BEGIN_STATION:
                case TRACK_ELEM_MIDDLE_STATION:
                    goToNextTile = true;
                }
            }
            while (!goToNextTile && !tile_element_is_last_for_tile(tileElement++));

            if (!goToNextTile)
                break;
        }

        mapX = entranceMinX;
        if (mapX > entranceMaxX)
        {
            entranceMinX = entranceMaxX;
            entranceMaxX = mapX;
        }

        mapY = entranceMinY;
        if (mapY > entranceMaxY)
        {
            entranceMinY = entranceMaxY;
            entranceMaxY = mapY;
        }

        direction = loc_6CD18E(*outX, *outY, entranceMinX - 32, entranceMinY - 32, entranceMaxX + 32, entranceMaxY + 32);
        if (direction != -1 && direction != stationDirection && direction != (stationDirection ^ 2))
        {
            gRideEntranceExitPlaceDirection = direction;
            *outDirection = direction;
            return;
        }
    }
}

bool ride_select_backwards_from_front()
{
    track_begin_end trackBeginEnd;

    ride_construction_invalidate_current_track();
    if (track_block_get_previous_from_zero(_currentTrackBeginX, _currentTrackBeginY, _currentTrackBeginZ, _currentRideIndex, _currentTrackPieceDirection, &trackBeginEnd)) {
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
        _currentTrackBeginX = trackBeginEnd.begin_x;
        _currentTrackBeginY = trackBeginEnd.begin_y;
        _currentTrackBeginZ = trackBeginEnd.begin_z;
        _currentTrackPieceDirection = trackBeginEnd.begin_direction;
        _currentTrackPieceType = track_element_get_type(trackBeginEnd.begin_element);
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
        return true;
    } else {
        return false;
    }
}

bool ride_select_forwards_from_back()
{
    sint32 x, y, z, direction;

    ride_construction_invalidate_current_track();

    x = _currentTrackBeginX;
    y = _currentTrackBeginY;
    z = _currentTrackBeginZ;
    direction = _currentTrackPieceDirection ^ 2;
    CoordsXYE next_track;

    if (track_block_get_next_from_zero(x, y, z, _currentRideIndex, direction, &next_track, &z, &direction)) {
        _rideConstructionState = RIDE_CONSTRUCTION_STATE_SELECTED;
        _currentTrackBeginX = next_track.x;
        _currentTrackBeginY = next_track.y;
        _currentTrackBeginZ = z;
        _currentTrackPieceDirection = tile_element_get_direction(next_track.element);
        _currentTrackPieceType = track_element_get_type(next_track.element);
        _currentTrackSelectionFlags = 0;
        _rideConstructionArrowPulseTime = 0;
        return true;
    } else {
        return false;
    }
}

money32 ride_remove_track_piece(sint32 x, sint32 y, sint32 z, sint32 direction, sint32 type, uint8 flags)
{
    gGameCommandErrorTitle = STR_RIDE_CONSTRUCTION_CANT_REMOVE_THIS;
    if (network_get_mode() == NETWORK_MODE_CLIENT)
    {
        game_command_callback = game_command_callback_ride_remove_track_piece;
    }
    return game_do_command(x, flags | ((direction & 3) << 8), y, type, GAME_COMMAND_REMOVE_TRACK, z, 0);
}

/**
 *
 *  rct2: 0x006B58EF
 */
bool ride_are_all_possible_entrances_and_exits_built(Ride *ride)
{
    if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_IS_SHOP))
        return true;

    for (sint32 i = 0; i < MAX_STATIONS; i++) {
        if (ride->station_starts[i].xy == RCT_XY8_UNDEFINED)
        {
            continue;
        }
        if (ride_get_entrance_location(ride, i).isNull())
        {
            gGameCommandErrorText = STR_ENTRANCE_NOT_YET_BUILT;
            return false;
        }
        if (ride_get_exit_location(ride, i).isNull()) {
            gGameCommandErrorText = STR_EXIT_NOT_YET_BUILT;
            return false;
        }
    }
    return true;
}

/**
 *
 *  rct2: 0x006B59C6
 */
void invalidate_test_results(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);

    ride_measurement_clear(ride);
    ride->excitement = RIDE_RATING_UNDEFINED;
    ride->lifecycle_flags &= ~RIDE_LIFECYCLE_TESTED;
    ride->lifecycle_flags &= ~RIDE_LIFECYCLE_TEST_IN_PROGRESS;
    if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK) {
        for (sint32 i = 0; i < ride->num_vehicles; i++) {
            uint16 spriteIndex = ride->vehicles[i];
            if (spriteIndex != SPRITE_INDEX_NULL) {
                rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
                vehicle->update_flags &= ~VEHICLE_UPDATE_FLAG_TESTING;
            }
        }
    }
    window_invalidate_by_number(WC_RIDE, rideIndex);
}

/**
 *
 *  rct2: 0x006B7481
 *
 * @param rideIndex (dl)
 * @param reliabilityIncreaseFactor (ax)
 */
void ride_fix_breakdown(sint32 rideIndex, sint32 reliabilityIncreaseFactor)
{
    Ride *ride = get_ride(rideIndex);

    ride->lifecycle_flags &= ~RIDE_LIFECYCLE_BREAKDOWN_PENDING;
    ride->lifecycle_flags &= ~RIDE_LIFECYCLE_BROKEN_DOWN;
    ride->lifecycle_flags &= ~RIDE_LIFECYCLE_DUE_INSPECTION;
    ride->window_invalidate_flags |= RIDE_INVALIDATE_RIDE_MAIN | RIDE_INVALIDATE_RIDE_LIST | RIDE_INVALIDATE_RIDE_MAINTENANCE;

    if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK) {
        for (sint32 i = 0; i < ride->num_vehicles; i++) {
            uint16 spriteIndex = ride->vehicles[i];
            while (spriteIndex != SPRITE_INDEX_NULL) {
                rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
                vehicle->update_flags &= ~VEHICLE_UPDATE_FLAG_ZERO_VELOCITY;
                vehicle->update_flags &= ~VEHICLE_UPDATE_FLAG_BROKEN_CAR;
                vehicle->update_flags &= ~VEHICLE_UPDATE_FLAG_BROKEN_TRAIN;
                spriteIndex = vehicle->next_vehicle_on_train;
            }
        }
    }

    uint8 unreliability = 100 - ride->reliability_percentage;
    ride->reliability += reliabilityIncreaseFactor * (unreliability / 2);
}

/**
 *
 *  rct2: 0x006DE102
 */
static void ride_update_vehicle_colours(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);
    if (ride->type == RIDE_TYPE_SPACE_RINGS || ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_16)) {
        gfx_invalidate_screen();
    }

    for (sint32 i = 0; i < MAX_VEHICLES_PER_RIDE; i++) {
        sint32 carIndex = 0;
        uint16 spriteIndex = ride->vehicles[i];
        rct_vehicle_colour colours = { 0 };
        uint8 coloursExtended = 0;

        while (spriteIndex != SPRITE_INDEX_NULL) {
            rct_vehicle *vehicle = GET_VEHICLE(spriteIndex);
            switch (ride->colour_scheme_type & 3) {
            case RIDE_COLOUR_SCHEME_ALL_SAME:
                colours = ride->vehicle_colours[0];
                coloursExtended = ride->vehicle_colours_extended[0];
                break;
            case RIDE_COLOUR_SCHEME_DIFFERENT_PER_TRAIN:
                colours = ride->vehicle_colours[i];
                coloursExtended = ride->vehicle_colours_extended[i];
                break;
            case RIDE_COLOUR_SCHEME_DIFFERENT_PER_CAR:
                colours = ride->vehicle_colours[Math::Min(carIndex, MAX_CARS_PER_TRAIN - 1)];
                coloursExtended = ride->vehicle_colours_extended[Math::Min(carIndex, MAX_CARS_PER_TRAIN - 1)];
                break;
            }

            vehicle->colours = colours;
            vehicle->colours_extended = coloursExtended;
            invalidate_sprite_2((rct_sprite*)vehicle);
            spriteIndex = vehicle->next_vehicle_on_train;
            carIndex++;
        }
    }
}

/**
 *
 *  rct2: 0x006DE4CD
 * trainLayout: Originally fixed to 0x00F64E38. This no longer postfixes with 255.
 */
void ride_entry_get_train_layout(sint32 rideEntryIndex, sint32 numCarsPerTrain, uint8 *trainLayout)
{
    for (sint32 i = 0; i < numCarsPerTrain; i++)
    {
        trainLayout[i] = ride_entry_get_vehicle_at_position(rideEntryIndex, numCarsPerTrain, i);
    }
}

uint8 ride_entry_get_vehicle_at_position(sint32 rideEntryIndex,sint32 numCarsPerTrain,sint32 position)
{
    rct_ride_entry *rideEntry = get_ride_entry(rideEntryIndex);
    if (position == 0 && rideEntry->front_vehicle != 255) {
        return rideEntry->front_vehicle;
    } else if (position == 1 && rideEntry->second_vehicle != 255) {
        return rideEntry->second_vehicle;
    } else if (position == 2 && rideEntry->third_vehicle != 255) {
        return rideEntry->third_vehicle;
    } else if (position == numCarsPerTrain - 1 && rideEntry->rear_vehicle != 255) {
        return rideEntry->rear_vehicle;
    } else {
        return rideEntry->default_vehicle;
    }
}

// Finds track pieces that a given ride entry has sprites for
uint64 ride_entry_get_supported_track_pieces(const rct_ride_entry * rideEntry)
{
    uint64 supportedPieces = 0xFFFFFFFFFFFFFFFFULL;
    uint16 trackPieceRequiredSprites[55] = {
        0x0001u, 0x0001u, 0x0001u, 0x0000u, 0x0006u, 0x0002u, 0x0020u, 0x000E,
        0x0003u, 0x0006u, 0x0007u, 0x0002u, 0x0004u, 0x0001u, 0x0001u, 0x0001u,
        0x0001u, 0x0061u, 0x000E, 0x1081u, 0x0001u, 0x0020u, 0x0020u, 0x0001u,
        0x0001u, 0x0000u, 0x0001u, 0x0001u, 0x000C, 0x0061u, 0x0002u, 0x000E,
        0x0480u, 0x0001u, 0x0061u, 0x0001u, 0x0001u, 0x000Fu, 0x0001u, 0x0200u,
        0x0007u, 0x0008u, 0x0000u, 0x0000u, 0x4000u, 0x0008u, 0x0001u, 0x0001u,
        0x0061u, 0x0061u, 0x0008u, 0x0008u, 0x0001u, 0x000Eu, 0x000Eu
    };

    //Only check default vehicle; it's assumed the others will have correct sprites if this one does (I've yet to find an exception, at least)
    for (sint32 j = 0; j < 55; j++) {
        if ((rideEntry->vehicles[rideEntry->default_vehicle].sprite_flags & trackPieceRequiredSprites[j]) != trackPieceRequiredSprites[j])
            supportedPieces &= ~(1ULL << j);
    }

    return supportedPieces;
}

static sint32 ride_get_smallest_station_length(Ride *ride)
{
    uint32 result = -1;
    for (sint32 i = 0; i < MAX_STATIONS; i++) {
        if (ride->station_starts[i].xy != RCT_XY8_UNDEFINED) {
            result = Math::Min(result, (uint32)(ride->station_length[i]));
        }
    }
    return (sint32)result;
}

/**
 *
 *  rct2: 0x006CB3AA
 */
static sint32 ride_get_track_length(Ride * ride)
{
    rct_window             * w;
    rct_tile_element        * tileElement = nullptr;
    track_circuit_iterator   it, slowIt;
    sint32                   x = 0, y = 0, z, trackType, rideIndex, result;
    bool                     foundTrack = false;

    for (sint32 i = 0; i < MAX_STATIONS && !foundTrack; i++)
    {
        LocationXY8 location = ride->station_starts[i];
        if (location.xy == RCT_XY8_UNDEFINED)
            continue;

        x = location.x * 32;
        y = location.y * 32;
        z = ride->station_heights[i];

        tileElement = map_get_first_element_at(x >> 5, y >> 5);
        do
        {
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK)
                continue;

            trackType = track_element_get_type(tileElement);
            if (!(TrackSequenceProperties[trackType][0] & TRACK_SEQUENCE_FLAG_ORIGIN))
                continue;

            if (tileElement->base_height != z)
                continue;

            foundTrack = true;
        }
        while (!foundTrack && !tile_element_is_last_for_tile(tileElement++));
    }

    if (foundTrack)
    {
        rideIndex = track_element_get_ride_index(tileElement);

        w = window_find_by_class(WC_RIDE_CONSTRUCTION);
        if (w != nullptr && _rideConstructionState != RIDE_CONSTRUCTION_STATE_0 && _currentRideIndex == rideIndex)
        {
            ride_construction_invalidate_current_track();
        }

        bool moveSlowIt = true;
        result = 0;
        track_circuit_iterator_begin(&it, {x, y, tileElement});
        slowIt = it;
        while (track_circuit_iterator_next(&it))
        {
            trackType = track_element_get_type(it.current.element);
            result += TrackPieceLengths[trackType];

            moveSlowIt = !moveSlowIt;
            if (moveSlowIt)
            {
                track_circuit_iterator_next(&slowIt);
                if (track_circuit_iterators_match(&it, &slowIt))
                {
                    return 0;
                }
            }
        }
        return result;
    }
    else
    {
        return 0;
    }
}

/**
 *
 *  rct2: 0x006DD57D
 */
void ride_update_max_vehicles(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);
    if (ride->subtype == RIDE_ENTRY_INDEX_NULL)
        return;

    rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
    if (rideEntry == nullptr)
    {
        return;
    }
    rct_ride_entry_vehicle *vehicleEntry;
    uint8 numCarsPerTrain, numVehicles;
    sint32 maxNumTrains;

    if (rideEntry->cars_per_flat_ride == 0xFF) {
        sint32 trainLength;
        ride->num_cars_per_train = Math::Max(rideEntry->min_cars_in_train, ride->num_cars_per_train);
        ride->min_max_cars_per_train = rideEntry->max_cars_in_train | (rideEntry->min_cars_in_train << 4);

        // Calculate maximum train length based on smallest station length
        sint32 stationLength = ride_get_smallest_station_length(ride);
        if (stationLength == -1)
            return;

        stationLength = (stationLength * 0x44180) - 0x16B2A;
        sint32 maxMass = RideData5[ride->type].max_mass << 8;
        sint32 maxCarsPerTrain = 1;
        for (sint32 numCars = rideEntry->max_cars_in_train; numCars > 0; numCars--) {
            trainLength = 0;
            sint32 totalMass = 0;
            for (sint32 i = 0; i < numCars; i++) {
                vehicleEntry = &rideEntry->vehicles[ride_entry_get_vehicle_at_position(ride->subtype, numCars, i)];
                trainLength += vehicleEntry->spacing;
                totalMass += vehicleEntry->car_mass;
            }

            if (trainLength <= stationLength && totalMass <= maxMass) {
                maxCarsPerTrain = numCars;
                break;
            }
        }
        sint32 newCarsPerTrain = Math::Max(ride->proposed_num_cars_per_train, rideEntry->min_cars_in_train);
        maxCarsPerTrain = Math::Max(maxCarsPerTrain, (sint32)rideEntry->min_cars_in_train);
        if (!gCheatsDisableTrainLengthLimit) {
            newCarsPerTrain = Math::Min(maxCarsPerTrain, newCarsPerTrain);
        }
        ride->min_max_cars_per_train = maxCarsPerTrain | (rideEntry->min_cars_in_train << 4);

        switch (ride->mode) {
        case RIDE_MODE_CONTINUOUS_CIRCUIT_BLOCK_SECTIONED:
        case RIDE_MODE_POWERED_LAUNCH_BLOCK_SECTIONED:
            maxNumTrains = Math::Clamp(1, ride->num_stations + ride->num_block_brakes - 1, 31);
            break;
        case RIDE_MODE_REVERSE_INCLINE_LAUNCHED_SHUTTLE:
        case RIDE_MODE_POWERED_LAUNCH_PASSTROUGH:
        case RIDE_MODE_SHUTTLE:
        case RIDE_MODE_LIM_POWERED_LAUNCH:
        case RIDE_MODE_POWERED_LAUNCH:
            maxNumTrains = 1;
            break;
        default:
            // Calculate maximum number of trains
            trainLength = 0;
            for (sint32 i = 0; i < newCarsPerTrain; i++) {
                vehicleEntry = &rideEntry->vehicles[ride_entry_get_vehicle_at_position(ride->subtype, newCarsPerTrain, i)];
                trainLength += vehicleEntry->spacing;
            }

            sint32 totalLength = trainLength / 2;
            if (newCarsPerTrain != 1)
                totalLength /= 2;

            maxNumTrains = 0;
            do {
                maxNumTrains++;
                totalLength += trainLength;
            } while (totalLength <= stationLength);

            if (
                (ride->mode != RIDE_MODE_STATION_TO_STATION && ride->mode != RIDE_MODE_CONTINUOUS_CIRCUIT) ||
                !(RideData4[ride->type].flags & RIDE_TYPE_FLAG4_ALLOW_MORE_VEHICLES_THAN_STATION_FITS)
            ) {
                maxNumTrains = Math::Min(maxNumTrains, 31);
            } else {
                vehicleEntry = &rideEntry->vehicles[ride_entry_get_vehicle_at_position(ride->subtype, newCarsPerTrain, 0)];
                sint32 speed = vehicleEntry->powered_max_speed;

                sint32 totalSpacing = 0;
                for (sint32 i = 0; i < newCarsPerTrain; i++) {
                    vehicleEntry = &rideEntry->vehicles[ride_entry_get_vehicle_at_position(ride->subtype, newCarsPerTrain, i)];
                    totalSpacing += vehicleEntry->spacing;
                }

                totalSpacing >>= 13;
                sint32 trackLength = ride_get_track_length(ride) / 4;
                if (speed > 10) trackLength = (trackLength * 3) / 4;
                if (speed > 25) trackLength = (trackLength * 3) / 4;
                if (speed > 40) trackLength = (trackLength * 3) / 4;

                maxNumTrains = 0;
                sint32 length = 0;
                do {
                    maxNumTrains++;
                    length += totalSpacing;
                } while (maxNumTrains < 31 && length < trackLength);
            }
            break;
        }
        ride->max_trains = maxNumTrains;

        numCarsPerTrain = Math::Min(ride->proposed_num_cars_per_train, (uint8)newCarsPerTrain);
    } else {
        ride->max_trains = rideEntry->cars_per_flat_ride;
        ride->min_max_cars_per_train = rideEntry->max_cars_in_train | (rideEntry->min_cars_in_train << 4);
        numCarsPerTrain = rideEntry->max_cars_in_train;
        maxNumTrains = rideEntry->cars_per_flat_ride;
    }

    if (gCheatsDisableTrainLengthLimit) {
        maxNumTrains = 31;
    }
    numVehicles = Math::Min(ride->proposed_num_vehicles, (uint8)maxNumTrains);

    // Refresh new current num vehicles / num cars per vehicle
    if (numVehicles != ride->num_vehicles || numCarsPerTrain != ride->num_cars_per_train) {
        ride->num_cars_per_train = numCarsPerTrain;
        ride->num_vehicles = numVehicles;
        window_invalidate_by_number(WC_RIDE, rideIndex);
    }
}

void ride_set_ride_entry(sint32 rideIndex, sint32 rideEntry)
{
    gGameCommandErrorTitle = STR_RIDE_SET_VEHICLE_TYPE_FAIL;
    game_do_command(
        0,
        GAME_COMMAND_FLAG_APPLY | (RIDE_SET_VEHICLES_COMMAND_TYPE_RIDE_ENTRY << 8),
        0,
        (rideEntry << 8) | rideIndex,
        GAME_COMMAND_SET_RIDE_VEHICLES,
        0,
        0
    );
}

void ride_set_num_vehicles(sint32 rideIndex, sint32 numVehicles)
{
    gGameCommandErrorTitle = STR_RIDE_SET_VEHICLE_SET_NUM_TRAINS_FAIL;
    game_do_command(
        0,
        GAME_COMMAND_FLAG_APPLY | (RIDE_SET_VEHICLES_COMMAND_TYPE_NUM_TRAINS << 8),
        0,
        (numVehicles << 8) | rideIndex,
        GAME_COMMAND_SET_RIDE_VEHICLES,
        0,
        0
    );
}

void ride_set_num_cars_per_vehicle(sint32 rideIndex, sint32 numCarsPerVehicle)
{
    gGameCommandErrorTitle = STR_RIDE_SET_VEHICLE_SET_NUM_CARS_PER_TRAIN_FAIL;
    game_do_command(
        0,
        GAME_COMMAND_FLAG_APPLY | (RIDE_SET_VEHICLES_COMMAND_TYPE_NUM_CARS_PER_TRAIN << 8),
        0,
        (numCarsPerVehicle << 8) | rideIndex,
        GAME_COMMAND_SET_RIDE_VEHICLES,
        0,
        0
    );
}

static bool ride_is_vehicle_type_valid(Ride *ride, uint8 inputRideEntryIndex)
{
    bool selectionShouldBeExpanded;
    sint32 rideTypeIterator, rideTypeIteratorMax;

    if (gCheatsShowVehiclesFromOtherTrackTypes &&
        !(ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_FLAT_RIDE) || ride->type == RIDE_TYPE_MAZE || ride->type == RIDE_TYPE_MINI_GOLF)
    ) {
        selectionShouldBeExpanded = true;
        rideTypeIterator = 0;
        rideTypeIteratorMax = RIDE_TYPE_COUNT - 1;
    } else {
        selectionShouldBeExpanded = false;
        rideTypeIterator = ride->type;
        rideTypeIteratorMax = ride->type;
    }

    for (; rideTypeIterator <= rideTypeIteratorMax; rideTypeIterator++) {
        if (selectionShouldBeExpanded) {
            if (ride_type_has_flag(rideTypeIterator, RIDE_TYPE_FLAG_FLAT_RIDE)) continue;
            if (rideTypeIterator == RIDE_TYPE_MAZE || rideTypeIterator == RIDE_TYPE_MINI_GOLF) continue;
        }

        uint8 *rideEntryIndexPtr = get_ride_entry_indices_for_ride_type(rideTypeIterator);
        for (uint8 *currentRideEntryIndex = rideEntryIndexPtr; *currentRideEntryIndex != RIDE_ENTRY_INDEX_NULL; currentRideEntryIndex++) {
            uint8 rideEntryIndex = *currentRideEntryIndex;
            if (rideEntryIndex == inputRideEntryIndex) {
                if (!ride_entry_is_invented(rideEntryIndex) && !gCheatsIgnoreResearchStatus) {
                    return false;
                }

                return true;
            }
        }
    }

    return false;
}

static money32 ride_set_vehicles(uint8 rideIndex, uint8 setting, uint8 value, uint32 flags, uint8 ex)
{
    rct_ride_entry *rideEntry;

    Ride *ride = get_ride(rideIndex);
    if (ride == nullptr || ride->type == RIDE_TYPE_NULL) {
        log_warning("Invalid game command for ride #%u", rideIndex);
        return MONEY32_UNDEFINED;
    }

    gCommandExpenditureType = RCT_EXPENDITURE_TYPE_RIDE_RUNNING_COSTS;

    if (ride->lifecycle_flags & RIDE_LIFECYCLE_BROKEN_DOWN) {
        gGameCommandErrorText = STR_HAS_BROKEN_DOWN_AND_REQUIRES_FIXING;
        return MONEY32_UNDEFINED;
    }

    if (ride->status != RIDE_STATUS_CLOSED) {
        gGameCommandErrorText = STR_MUST_BE_CLOSED_FIRST;
        return MONEY32_UNDEFINED;
    }

    switch (setting) {
    case RIDE_SET_VEHICLES_COMMAND_TYPE_NUM_TRAINS:
        if (!(flags & GAME_COMMAND_FLAG_APPLY)) {
            return 0;
        }

        ride_clear_for_construction(rideIndex);
        ride_remove_peeps(rideIndex);
        ride->vehicle_change_timeout = 100;

        ride->proposed_num_vehicles = value;
        break;
    case RIDE_SET_VEHICLES_COMMAND_TYPE_NUM_CARS_PER_TRAIN:
        if (!(flags & GAME_COMMAND_FLAG_APPLY)) {
            return 0;
        }

        ride_clear_for_construction(rideIndex);
        ride_remove_peeps(rideIndex);
        ride->vehicle_change_timeout = 100;

        invalidate_test_results(rideIndex);
        rideEntry = get_ride_entry(ride->subtype);
        if (!gCheatsDisableTrainLengthLimit) {
            value = Math::Clamp(rideEntry->min_cars_in_train, value, rideEntry->max_cars_in_train);
        }
        ride->proposed_num_cars_per_train = value;
        break;
    case RIDE_SET_VEHICLES_COMMAND_TYPE_RIDE_ENTRY:
        {
            if (!ride_is_vehicle_type_valid(ride, value)) {
                log_error("Invalid vehicle type.");
                return MONEY32_UNDEFINED;
            }

            if (!(flags & GAME_COMMAND_FLAG_APPLY)) {
                return 0;
            }

            ride_clear_for_construction(rideIndex);
            ride_remove_peeps(rideIndex);
            ride->vehicle_change_timeout = 100;

            invalidate_test_results(rideIndex);
            ride->subtype = value;
            rideEntry = get_ride_entry(ride->subtype);

            uint8 preset = ex;
            if (!(flags & GAME_COMMAND_FLAG_NETWORKED)) {
                preset = ride_get_unused_preset_vehicle_colour(ride->type, ride->subtype);
            }

            // Validate preset
            vehicle_colour_preset_list *presetList = rideEntry->vehicle_preset_list;
            if (preset >= presetList->count) {
                log_error("Unknown vehicle colour preset.");
                return MONEY32_UNDEFINED;
            }

            ride_set_vehicle_colours_to_random_preset(ride, preset);
            if (!gCheatsDisableTrainLengthLimit) {
                ride->proposed_num_cars_per_train = Math::Clamp(rideEntry->min_cars_in_train, ride->proposed_num_cars_per_train, rideEntry->max_cars_in_train);
            }
            break;
        }

    default:
        log_error("Unknown vehicle command.");
        return MONEY32_UNDEFINED;
    }

    ride->num_circuits = 1;
    ride_update_max_vehicles(rideIndex);

    if (ride->overall_view.xy != RCT_XY8_UNDEFINED) {
        LocationXYZ16 coord;
        coord.x = ride->overall_view.x * 32 + 16;
        coord.y = ride->overall_view.y * 32 + 16;
        coord.z = tile_element_height(coord.x, coord.y);
        network_set_player_last_action_coord(network_get_player_index(game_command_playerid), coord);
    }

    rct_window *w = window_find_by_number(WC_RIDE, rideIndex);
    if (w != nullptr) {
        if (w->page == 4) { // WINDOW_RIDE_PAGE_COLOUR
            w->vehicleIndex = 0;
        }
        window_invalidate(w);
    }

    gfx_invalidate_screen();
    return 0;
}

/**
 *
 *  rct2: 0x006B52D4
 */
void game_command_set_ride_vehicles(sint32 *eax, sint32 *ebx, sint32 *ecx, sint32 *edx, sint32 *esi, sint32 *edi, sint32 *ebp)
{
    uint8 rideIndex = *edx & 0xFF;
    uint8 setting = (*ebx >> 8) & 0xFF;
    uint8 value = (*edx >> 8) & 0xFF;
    uint32 flags = *ebx;
    uint8 ex = *eax & 0xFF;
    *ebx = ride_set_vehicles(rideIndex, setting, value, flags, ex);
}

/**
 *
 *  rct2: 0x006CB945
 */
void sub_6CB945(sint32 rideIndex)
{
    Ride* ride = get_ride(rideIndex);
    if (ride->type != RIDE_TYPE_MAZE) {
        for (uint8 stationId = 0; stationId < MAX_STATIONS; ++stationId) {
            if (ride->station_starts[stationId].xy == RCT_XY8_UNDEFINED)
                continue;

            LocationXYZ16 location = {
                (sint16)(ride->station_starts[stationId].x * 32),
                (sint16)(ride->station_starts[stationId].y * 32),
                (ride->station_heights[stationId])
            };
            uint8 direction = 0xFF;

            bool specialTrack = false;
            rct_tile_element* tileElement = nullptr;

            while (true) {
                if (direction != 0xFF) {
                    location.x -= TileDirectionDelta[direction].x;
                    location.y -= TileDirectionDelta[direction].y;
                }
                tileElement = map_get_first_element_at(location.x >> 5, location.y >> 5);

                bool trackFound = false;
                do {
                    if (tileElement->base_height != location.z) continue;
                    if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
                    if (track_element_get_ride_index(tileElement) != rideIndex) continue;
                    if (tile_element_get_track_sequence(tileElement) != 0) continue;
                    if (!(TrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN)) continue;

                    trackFound = true;
                    break;
                } while (!tile_element_is_last_for_tile(tileElement++));

                if (trackFound == false) {
                    break;
                }

                tile_element_set_station(tileElement, stationId);
                direction = tile_element_get_direction(tileElement);

                if (ride_type_has_flag(ride->type, RIDE_TYPE_FLAG_3)) {
                    specialTrack = true;
                    break;
                }
            }

            if (specialTrack == false) {
                continue;
            }

            const rct_preview_track *trackBlock = get_track_def_from_ride(ride, track_element_get_type(tileElement));
            while ((++trackBlock)->index != 0xFF) {
                LocationXYZ16 blockLocation = location;
                map_offset_with_rotation(&blockLocation.x, &blockLocation.y, trackBlock->x, trackBlock->y, direction);

                bool trackFound = false;
                tileElement = map_get_first_element_at(blockLocation.x >> 5, blockLocation.y >> 5);
                do {
                    if (blockLocation.z != tileElement->base_height) continue;
                    if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
                    if (!(TrackSequenceProperties[track_element_get_type(tileElement)][0] & TRACK_SEQUENCE_FLAG_ORIGIN)) continue;

                    trackFound = true;
                    break;
                } while (!tile_element_is_last_for_tile(tileElement++));

                if (!trackFound) {
                    break;
                }

                tile_element_set_station(tileElement, stationId);
            }
        }
    }

    // Needs room for an entrance and an exit per station, plus one position for the list terminator.
    TileCoordsXYZD locations[(MAX_STATIONS * 2) + 1];
    TileCoordsXYZD *locationList = locations;
    for (uint8 stationId = 0; stationId < MAX_STATIONS; ++stationId)
    {
        TileCoordsXYZD entrance = ride_get_entrance_location(rideIndex, stationId);
        if (!entrance.isNull())
        {
            *locationList++ = entrance;
            ride_clear_entrance_location(ride, stationId);
        }

        TileCoordsXYZD exit = ride_get_exit_location(rideIndex, stationId);
        if (!exit.isNull())
        {
            *locationList++ = exit;
            ride_clear_exit_location(ride, stationId);
        }
    }
    (*locationList++).x = COORDS_NULL;

    locationList = locations;
    for (; !(*locationList).isNull(); locationList++) {
        TileCoordsXYZD * locationList2 = locationList;
        locationList2++;

        bool duplicateLocation = false;
        do {
            if ((*locationList).x == (*locationList2).x &&
                (*locationList).y == (*locationList2).y)
            {
                duplicateLocation = true;
                break;
            }
        } while (!(*locationList2++).isNull());

        if (duplicateLocation)
        {
            continue;
        }

        CoordsXY location = { (*locationList).x * 32, (*locationList).y * 32 };

        rct_tile_element *tileElement = map_get_first_element_at(location.x >> 5, location.y >> 5);
        do {
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_ENTRANCE) continue;
            if (tileElement->properties.entrance.ride_index != rideIndex) continue;
            if (tileElement->properties.entrance.type > ENTRANCE_TYPE_RIDE_EXIT) continue;

            CoordsXY nextLocation = location;
            nextLocation.x += TileDirectionDelta[tile_element_get_direction(tileElement)].x;
            nextLocation.y += TileDirectionDelta[tile_element_get_direction(tileElement)].y;

            bool shouldRemove = true;
            rct_tile_element *trackElement = map_get_first_element_at(nextLocation.x >> 5, nextLocation.y >> 5);
            do {
                if (tile_element_get_type(trackElement) != TILE_ELEMENT_TYPE_TRACK) continue;
                if (track_element_get_ride_index(trackElement) != rideIndex) continue;
                if (trackElement->base_height != tileElement->base_height) continue;

                uint8 trackType = track_element_get_type(trackElement);
                uint8 trackSequence = tile_element_get_track_sequence(trackElement);

                uint8 direction = (tile_element_get_direction(tileElement) - tile_element_get_direction(trackElement) + 2) & 3;

                if (!(TrackSequenceProperties[trackType][trackSequence] & (1 << direction))) {
                    continue;
                }

                uint8 stationId = 0;
                if (trackType != TRACK_ELEM_INVERTED_90_DEG_UP_TO_FLAT_QUARTER_LOOP) {
                    stationId = tile_element_get_station(trackElement);
                }

                if (tileElement->properties.entrance.type == ENTRANCE_TYPE_RIDE_EXIT)
                {
                    if (!ride_get_exit_location(ride, stationId).isNull())
                        break;

                    ride_set_exit_location(ride, stationId,
                        { location.x / 32, location.y / 32, ride->station_heights[stationId], (uint8)tile_element_get_direction(tileElement) });
                }
                else
                {
                    if (!ride_get_entrance_location(ride, stationId).isNull())
                        break;

                    ride_set_entrance_location(ride, stationId,
                        { location.x / 32, location.y / 32, ride->station_heights[stationId], (uint8)tile_element_get_direction(tileElement) });
                }

                tileElement->properties.entrance.index &= 0x8F;
                tileElement->properties.entrance.index |= stationId << 4;
                shouldRemove = false;
            } while (!tile_element_is_last_for_tile(trackElement++));

            if (shouldRemove == true) {
                footpath_queue_chain_reset();
                maze_entrance_hedge_replacement(location.x, location.y, tileElement);
                footpath_remove_edges_at(location.x, location.y, tileElement);
                footpath_update_queue_chains();
                map_invalidate_tile_full(location.x, location.y);
                tile_element_remove(tileElement);
                tileElement--;
            }
        } while (!tile_element_is_last_for_tile(tileElement++));
    }
}


void ride_set_to_default_inspection_interval(sint32 rideIndex)
{
    Ride *ride = get_ride(rideIndex);
    uint8 defaultInspectionInterval = gConfigGeneral.default_inspection_interval;
    if (ride->inspection_interval != defaultInspectionInterval) {
        if (defaultInspectionInterval <= RIDE_INSPECTION_NEVER) {
            gGameCommandErrorTitle = STR_CANT_CHANGE_OPERATING_MODE;
            game_do_command(0, (defaultInspectionInterval << 8) | 1, 0, (5 << 8) | rideIndex, GAME_COMMAND_SET_RIDE_SETTING, 0, 0);
        }
    }
}

/**
 *
 *  rct2: 0x006B752C
 */
void ride_crash(uint8 rideIndex, uint8 vehicleIndex)
{
    Ride *ride = get_ride(rideIndex);
    rct_vehicle *vehicle = GET_VEHICLE(ride->vehicles[vehicleIndex]);

    if (!(gScreenFlags & SCREEN_FLAGS_TITLE_DEMO)) {
        // Open ride window for crashed vehicle
        auto intent = Intent(WD_VEHICLE);
        intent.putExtra(INTENT_EXTRA_VEHICLE, vehicle);
        rct_window * w = context_open_intent(&intent);

        rct_viewport * viewport = window_get_viewport(w);
        if (w != nullptr && viewport != nullptr) {
            viewport->flags |= VIEWPORT_FLAG_SOUND_ON;
        }
    }

    set_format_arg(0, rct_string_id, ride->name);
    set_format_arg(2, uint32, ride->name_arguments);
    if (gConfigNotifications.ride_crashed) {
        news_item_add_to_queue(NEWS_ITEM_RIDE, STR_RIDE_HAS_CRASHED, rideIndex);
    }
}

bool shop_item_is_food_or_drink(sint32 shopItem)
{
    switch (shopItem) {
    case SHOP_ITEM_DRINK:
    case SHOP_ITEM_BURGER:
    case SHOP_ITEM_CHIPS:
    case SHOP_ITEM_ICE_CREAM:
    case SHOP_ITEM_CANDYFLOSS:
    case SHOP_ITEM_PIZZA:
    case SHOP_ITEM_POPCORN:
    case SHOP_ITEM_HOT_DOG:
    case SHOP_ITEM_TENTACLE:
    case SHOP_ITEM_TOFFEE_APPLE:
    case SHOP_ITEM_DOUGHNUT:
    case SHOP_ITEM_COFFEE:
    case SHOP_ITEM_CHICKEN:
    case SHOP_ITEM_LEMONADE:
    case SHOP_ITEM_PRETZEL:
    case SHOP_ITEM_CHOCOLATE:
    case SHOP_ITEM_ICED_TEA:
    case SHOP_ITEM_FUNNEL_CAKE:
    case SHOP_ITEM_BEEF_NOODLES:
    case SHOP_ITEM_FRIED_RICE_NOODLES:
    case SHOP_ITEM_WONTON_SOUP:
    case SHOP_ITEM_MEATBALL_SOUP:
    case SHOP_ITEM_FRUIT_JUICE:
    case SHOP_ITEM_SOYBEAN_MILK:
    case SHOP_ITEM_SUJEONGGWA:
    case SHOP_ITEM_SUB_SANDWICH:
    case SHOP_ITEM_COOKIE:
    case SHOP_ITEM_ROAST_SAUSAGE:
        return true;
    default:
        return false;
    }
}

bool shop_item_is_food(sint32 shopItem)
{
    switch (shopItem) {
    case SHOP_ITEM_BURGER:
    case SHOP_ITEM_CHIPS:
    case SHOP_ITEM_ICE_CREAM:
    case SHOP_ITEM_CANDYFLOSS:
    case SHOP_ITEM_PIZZA:
    case SHOP_ITEM_POPCORN:
    case SHOP_ITEM_HOT_DOG:
    case SHOP_ITEM_TENTACLE:
    case SHOP_ITEM_TOFFEE_APPLE:
    case SHOP_ITEM_DOUGHNUT:
    case SHOP_ITEM_CHICKEN:
    case SHOP_ITEM_PRETZEL:
    case SHOP_ITEM_FUNNEL_CAKE:
    case SHOP_ITEM_BEEF_NOODLES:
    case SHOP_ITEM_FRIED_RICE_NOODLES:
    case SHOP_ITEM_WONTON_SOUP:
    case SHOP_ITEM_MEATBALL_SOUP:
    case SHOP_ITEM_SUB_SANDWICH:
    case SHOP_ITEM_COOKIE:
    case SHOP_ITEM_ROAST_SAUSAGE:
        return true;
    default:
        return false;
    }
}

bool shop_item_is_drink(sint32 shopItem)
{
    switch (shopItem) {
    case SHOP_ITEM_DRINK:
    case SHOP_ITEM_COFFEE:
    case SHOP_ITEM_LEMONADE:
    case SHOP_ITEM_CHOCOLATE:
    case SHOP_ITEM_ICED_TEA:
    case SHOP_ITEM_FRUIT_JUICE:
    case SHOP_ITEM_SOYBEAN_MILK:
    case SHOP_ITEM_SUJEONGGWA:
        return true;
    default:
        return false;
    }
}

bool shop_item_is_souvenir(sint32 shopItem)
{
    switch (shopItem) {
    case SHOP_ITEM_BALLOON:
    case SHOP_ITEM_TOY:
    case SHOP_ITEM_MAP:
    case SHOP_ITEM_PHOTO:
    case SHOP_ITEM_UMBRELLA:
    case SHOP_ITEM_HAT:
    case SHOP_ITEM_TSHIRT:
    case SHOP_ITEM_PHOTO2:
    case SHOP_ITEM_PHOTO3:
    case SHOP_ITEM_PHOTO4:
    case SHOP_ITEM_SUNGLASSES:
        return true;
    default:
        return false;
    }
}

void ride_reset_all_names()
{
    sint32 i;
    Ride *ride;
    char rideNameBuffer[256];
    ride_name_args name_args;

    FOR_ALL_RIDES(i, ride)
    {
        ride->name = STR_NONE;

        name_args.type_name = RideNaming[ride->type].name;
        name_args.number = 0;
        do {
            name_args.number++;
            format_string(rideNameBuffer, 256, 1, &name_args);
        } while (ride_name_exists(rideNameBuffer));

        ride->name = 1;
        ride->name_arguments_type_name = name_args.type_name;
        ride->name_arguments_number = name_args.number;
    }
}

const uint8* ride_seek_available_modes(Ride *ride)
{
    const uint8* availableModes;

    if (!gCheatsShowAllOperatingModes) {
        availableModes = RideAvailableModes;

        for (sint32 i = 0; i < ride->type; i++) {
            while (*(availableModes++) != 255) { }
        }
    }
    else
    {
        availableModes = AllRideModesAvailable;
    }

    return availableModes;
}

// Gets the approximate value of customers per hour for this ride. Multiplies ride_customers_in_last_5_minutes() by 12.
uint32 ride_customers_per_hour(const Ride *ride) {
    return ride_customers_in_last_5_minutes(ride) * 12;
}

// Calculates the number of customers for this ride in the last 5 minutes (or more correctly 9600 game ticks)
uint32 ride_customers_in_last_5_minutes(const Ride *ride)
{
    uint32 sum = 0;

    for (sint32 i = 0; i < CUSTOMER_HISTORY_SIZE; i++)
    {
        sum += ride->num_customers[i];
    }

    return sum;
}

rct_vehicle *ride_get_broken_vehicle(Ride *ride) {
    uint16 vehicleIndex = ride->vehicles[ride->broken_vehicle];

    if (vehicleIndex == SPRITE_INDEX_NULL) {
        return nullptr;
    }

    rct_vehicle *vehicle = GET_VEHICLE(vehicleIndex);
    for (uint8 i = 0; i < ride->broken_car; i++) {
        vehicle = GET_VEHICLE(vehicle->next_vehicle_on_train);
    }

    return vehicle;
}

/**
 *
 *  rct2: 0x006D235B
 */
void ride_delete(uint8 rideIndex)
{
    Ride *ride = get_ride(rideIndex);
    user_string_free(ride->name);
    ride->type = RIDE_TYPE_NULL;
}

void ride_renew(Ride * ride)
{
    // Set build date to current date (so the ride is brand new)
    ride->build_date = gDateMonthsElapsed;
    ride->reliability = RIDE_INITIAL_RELIABILITY;
}

static bool ride_is_ride(Ride * ride)
{
    switch (ride->type) {
    case RIDE_TYPE_FOOD_STALL:
    case RIDE_TYPE_1D:
    case RIDE_TYPE_DRINK_STALL:
    case RIDE_TYPE_1F:
    case RIDE_TYPE_SHOP:
    case RIDE_TYPE_22:
    case RIDE_TYPE_INFORMATION_KIOSK:
    case RIDE_TYPE_TOILETS:
    case RIDE_TYPE_CASH_MACHINE:
    case RIDE_TYPE_FIRST_AID:
        return false;
    default:
        return true;
    }
}

money16 ride_get_price(Ride * ride)
{
    if (gParkFlags & PARK_FLAGS_NO_MONEY) return 0;
    if (ride_is_ride(ride)) {
        if (!park_ride_prices_unlocked()) {
            return 0;
        }
    }
    return ride->price;
}

/**
 * Return the tile_element of an adjacent station at x,y,z(+-2).
 * Returns nullptr if no suitable tile_element is found.
 */
rct_tile_element *get_station_platform(sint32 x, sint32 y, sint32 z, sint32 z_tolerance) {
    bool foundTileElement = false;
    rct_tile_element *tileElement = map_get_first_element_at(x >> 5, y >> 5);
    if (tileElement != nullptr) {
        do {
            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_TRACK) continue;
            /* Check if tileElement is a station platform. */
            if (!track_element_is_station(tileElement)) continue;

            if (z - z_tolerance > tileElement->base_height ||
                z + z_tolerance < tileElement->base_height
            ) {
                /* The base height if tileElement is not within
                 * the z tolerance. */
                continue;
            }

            foundTileElement = true;
            break;
        } while (!tile_element_is_last_for_tile(tileElement++));
    }
    if (!foundTileElement) {
        return nullptr;
    }

    return tileElement;
}

/**
 * Check for an adjacent station to x,y,z in direction.
 */
static bool check_for_adjacent_station(sint32 x, sint32 y, sint32 z, uint8 direction)
{
    bool found = false;
    sint32 adjX = x;
    sint32 adjY = y;
    for (uint32 i = 0; i <= RIDE_ADJACENCY_CHECK_DISTANCE; i++)
    {
        adjX += TileDirectionDelta[direction].x;
        adjY += TileDirectionDelta[direction].y;
        rct_tile_element * stationElement = get_station_platform(adjX, adjY, z, 2);
        if (stationElement != nullptr)
        {
            sint32 rideIndex = track_element_get_ride_index(stationElement);
            Ride * ride = get_ride(rideIndex);
            if (ride->depart_flags & RIDE_DEPART_SYNCHRONISE_WITH_ADJACENT_STATIONS)
            {
                found = true;
            }
        }
    }
    return found;
}

/**
 * Return whether ride has at least one adjacent station to it.
 */
bool ride_has_adjacent_station(Ride *ride)
{
    bool found = false;

    /* Loop through all of the ride stations, checking for an
     * adjacent station on either side. */
    for (sint32 stationNum = 0; stationNum < MAX_STATIONS; stationNum++) {
        if (ride->station_starts[stationNum].xy != RCT_XY8_UNDEFINED) {
            /* Get the map element for the station start. */
            uint16 stationX = ride->station_starts[stationNum].x * 32;
            uint16 stationY = ride->station_starts[stationNum].y * 32;
            uint8 stationZ = ride->station_heights[stationNum];

            rct_tile_element *stationElement = get_station_platform(stationX, stationY, stationZ, 0);
            if (stationElement == nullptr) {
                continue;
            }
            /* Check the first side of the station */
            sint32 direction = tile_element_get_direction_with_offset(stationElement, 1);
            found = check_for_adjacent_station(stationX, stationY, stationZ, direction);
            if (found) break;
            /* Check the other side of the station */
            direction ^= 2;
            found = check_for_adjacent_station(stationX, stationY, stationZ, direction);
            if (found) break;
        }
    }
    return found;
}

bool ride_has_ratings(const Ride * ride)
{
    return ride->excitement != RIDE_RATING_UNDEFINED;
}

const char * ride_type_get_enum_name(sint32 rideType)
{
    static constexpr const char * RideTypeEnumNames[RIDE_TYPE_COUNT] =
    {
        nameof(RIDE_TYPE_SPIRAL_ROLLER_COASTER),
        nameof(RIDE_TYPE_STAND_UP_ROLLER_COASTER),
        nameof(RIDE_TYPE_SUSPENDED_SWINGING_COASTER),
        nameof(RIDE_TYPE_INVERTED_ROLLER_COASTER),
        nameof(RIDE_TYPE_JUNIOR_ROLLER_COASTER),
        nameof(RIDE_TYPE_MINIATURE_RAILWAY),
        nameof(RIDE_TYPE_MONORAIL),
        nameof(RIDE_TYPE_MINI_SUSPENDED_COASTER),
        nameof(RIDE_TYPE_BOAT_HIRE),
        nameof(RIDE_TYPE_WOODEN_WILD_MOUSE),
        nameof(RIDE_TYPE_STEEPLECHASE),
        nameof(RIDE_TYPE_CAR_RIDE),
        nameof(RIDE_TYPE_LAUNCHED_FREEFALL),
        nameof(RIDE_TYPE_BOBSLEIGH_COASTER),
        nameof(RIDE_TYPE_OBSERVATION_TOWER),
        nameof(RIDE_TYPE_LOOPING_ROLLER_COASTER),
        nameof(RIDE_TYPE_DINGHY_SLIDE),
        nameof(RIDE_TYPE_MINE_TRAIN_COASTER),
        nameof(RIDE_TYPE_CHAIRLIFT),
        nameof(RIDE_TYPE_CORKSCREW_ROLLER_COASTER),
        nameof(RIDE_TYPE_MAZE),
        nameof(RIDE_TYPE_SPIRAL_SLIDE),
        nameof(RIDE_TYPE_GO_KARTS),
        nameof(RIDE_TYPE_LOG_FLUME),
        nameof(RIDE_TYPE_RIVER_RAPIDS),
        nameof(RIDE_TYPE_DODGEMS),
        nameof(RIDE_TYPE_SWINGING_SHIP),
        nameof(RIDE_TYPE_SWINGING_INVERTER_SHIP),
        nameof(RIDE_TYPE_FOOD_STALL),
        nameof(RIDE_TYPE_1D),
        nameof(RIDE_TYPE_DRINK_STALL),
        nameof(RIDE_TYPE_1F),
        nameof(RIDE_TYPE_SHOP),
        nameof(RIDE_TYPE_MERRY_GO_ROUND),
        nameof(RIDE_TYPE_22),
        nameof(RIDE_TYPE_INFORMATION_KIOSK),
        nameof(RIDE_TYPE_TOILETS),
        nameof(RIDE_TYPE_FERRIS_WHEEL),
        nameof(RIDE_TYPE_MOTION_SIMULATOR),
        nameof(RIDE_TYPE_3D_CINEMA),
        nameof(RIDE_TYPE_TOP_SPIN),
        nameof(RIDE_TYPE_SPACE_RINGS),
        nameof(RIDE_TYPE_REVERSE_FREEFALL_COASTER),
        nameof(RIDE_TYPE_LIFT),
        nameof(RIDE_TYPE_VERTICAL_DROP_ROLLER_COASTER),
        nameof(RIDE_TYPE_CASH_MACHINE),
        nameof(RIDE_TYPE_TWIST),
        nameof(RIDE_TYPE_HAUNTED_HOUSE),
        nameof(RIDE_TYPE_FIRST_AID),
        nameof(RIDE_TYPE_CIRCUS),
        nameof(RIDE_TYPE_GHOST_TRAIN),
        nameof(RIDE_TYPE_TWISTER_ROLLER_COASTER),
        nameof(RIDE_TYPE_WOODEN_ROLLER_COASTER),
        nameof(RIDE_TYPE_SIDE_FRICTION_ROLLER_COASTER),
        nameof(RIDE_TYPE_STEEL_WILD_MOUSE),
        nameof(RIDE_TYPE_MULTI_DIMENSION_ROLLER_COASTER),
        nameof(RIDE_TYPE_MULTI_DIMENSION_ROLLER_COASTER_ALT),
        nameof(RIDE_TYPE_FLYING_ROLLER_COASTER),
        nameof(RIDE_TYPE_FLYING_ROLLER_COASTER_ALT),
        nameof(RIDE_TYPE_VIRGINIA_REEL),
        nameof(RIDE_TYPE_SPLASH_BOATS),
        nameof(RIDE_TYPE_MINI_HELICOPTERS),
        nameof(RIDE_TYPE_LAY_DOWN_ROLLER_COASTER),
        nameof(RIDE_TYPE_SUSPENDED_MONORAIL),
        nameof(RIDE_TYPE_LAY_DOWN_ROLLER_COASTER_ALT),
        nameof(RIDE_TYPE_REVERSER_ROLLER_COASTER),
        nameof(RIDE_TYPE_HEARTLINE_TWISTER_COASTER),
        nameof(RIDE_TYPE_MINI_GOLF),
        nameof(RIDE_TYPE_GIGA_COASTER),
        nameof(RIDE_TYPE_ROTO_DROP),
        nameof(RIDE_TYPE_FLYING_SAUCERS),
        nameof(RIDE_TYPE_CROOKED_HOUSE),
        nameof(RIDE_TYPE_MONORAIL_CYCLES),
        nameof(RIDE_TYPE_COMPACT_INVERTED_COASTER),
        nameof(RIDE_TYPE_WATER_COASTER),
        nameof(RIDE_TYPE_AIR_POWERED_VERTICAL_COASTER),
        nameof(RIDE_TYPE_INVERTED_HAIRPIN_COASTER),
        nameof(RIDE_TYPE_MAGIC_CARPET),
        nameof(RIDE_TYPE_SUBMARINE_RIDE),
        nameof(RIDE_TYPE_RIVER_RAFTS),
        nameof(RIDE_TYPE_50),
        nameof(RIDE_TYPE_ENTERPRISE),
        nameof(RIDE_TYPE_52),
        nameof(RIDE_TYPE_53),
        nameof(RIDE_TYPE_54),
        nameof(RIDE_TYPE_55),
        nameof(RIDE_TYPE_INVERTED_IMPULSE_COASTER),
        nameof(RIDE_TYPE_MINI_ROLLER_COASTER),
        nameof(RIDE_TYPE_MINE_RIDE),
        nameof(RIDE_TYPE_59),
        nameof(RIDE_TYPE_LIM_LAUNCHED_ROLLER_COASTER),
    };

    return RideTypeEnumNames[rideType];
}

/**
 *  Searches for a non-null ride type in a ride entry.
 *  If none is found, it will still return RIDE_TYPE_NULL.
 */
uint8 ride_entry_get_first_non_null_ride_type(const rct_ride_entry * rideEntry)
{
    for (uint8 i = 0; i < MAX_RIDE_TYPES_PER_RIDE_ENTRY; i++)
    {
        if (rideEntry->ride_type[i] != RIDE_TYPE_NULL)
        {
            return rideEntry->ride_type[i];
        }
    }
   return RIDE_TYPE_NULL;
}

bool ride_type_supports_boosters(uint8 rideType)
{
    if (rideType == RIDE_TYPE_LOOPING_ROLLER_COASTER ||
        rideType == RIDE_TYPE_CORKSCREW_ROLLER_COASTER ||
        rideType == RIDE_TYPE_TWISTER_ROLLER_COASTER ||
        rideType == RIDE_TYPE_VERTICAL_DROP_ROLLER_COASTER ||
        rideType == RIDE_TYPE_GIGA_COASTER ||
        rideType == RIDE_TYPE_JUNIOR_ROLLER_COASTER)
    {
        return true;
    }
    return false;
}

sint32 get_booster_speed(uint8 rideType, sint32 rawSpeed)
{
    sint8 shiftFactor = RideProperties[rideType].booster_speed_factor;
    if (shiftFactor == 0)
    {
        return rawSpeed;
    }
    else if (shiftFactor > 0)
    {
        return (rawSpeed << shiftFactor);
    }
    else
    {
        // Workaround for an issue with older compilers (GCC 6, Clang 4) which would fail the build
        sint8 shiftFactorAbs = std::abs(shiftFactor);
        return (rawSpeed >> shiftFactorAbs);
    }
}

void fix_invalid_vehicle_sprite_sizes()
{
    Ride *ride;
    uint16 i;
    FOR_ALL_RIDES(i, ride)
    {
        for (uint16 j = 0; j < MAX_VEHICLES_PER_RIDE; j++) {
            uint16 rideSpriteIndex = ride->vehicles[j];
            while (rideSpriteIndex != SPRITE_INDEX_NULL) {
                rct_vehicle * vehicle = try_get_vehicle(rideSpriteIndex);
                if (vehicle == nullptr) {
                    break;
                }

                rct_ride_entry_vehicle * vehicleEntry = vehicle_get_vehicle_entry(vehicle);
                if (vehicleEntry == nullptr) {
                    break;
                }

                if (vehicle->sprite_width == 0)
                {
                    vehicle->sprite_width = vehicleEntry->sprite_width;
                }
                if (vehicle->sprite_height_negative == 0)
                {
                    vehicle->sprite_height_negative = vehicleEntry->sprite_height_negative;
                }
                if (vehicle->sprite_height_positive == 0)
                {
                    vehicle->sprite_height_positive = vehicleEntry->sprite_height_positive;
                }
                rideSpriteIndex = vehicle->next_vehicle_on_train;
            }
        }
    }
}

bool ride_entry_has_category(const rct_ride_entry * rideEntry, uint8 category)
{
    for (sint32 i = 0; i < MAX_CATEGORIES_PER_RIDE; i++)
    {
        if (rideEntry->category[i] == category)
        {
            return true;
        }
    }

    return false;
}

sint32 ride_get_entry_index(sint32 rideType, sint32 rideSubType)
{
    sint32 subType = rideSubType;

    if (subType == RIDE_ENTRY_INDEX_NULL)
    {
        uint8 *availableRideEntries = get_ride_entry_indices_for_ride_type(rideType);
        for (uint8 *rideEntryIndex = availableRideEntries; *rideEntryIndex != RIDE_ENTRY_INDEX_NULL; rideEntryIndex++)
        {
            rct_ride_entry *rideEntry = get_ride_entry(*rideEntryIndex);
            if (rideEntry == nullptr)
            {
                return RIDE_ENTRY_INDEX_NULL;
            }

            // Can happen in select-by-track-type mode
            if (!ride_entry_is_invented(*rideEntryIndex) && !gCheatsIgnoreResearchStatus)
            {
                continue;
            }

            if (!RideGroupManager::RideTypeIsIndependent(rideType))
            {
                subType = *rideEntryIndex;
                break;
            }
        }
        if (subType == RIDE_ENTRY_INDEX_NULL)
        {
            subType = availableRideEntries[0];
        }
    }

    return subType;
}

LocationXY16 ride_get_rotated_coords(sint16 x, sint16 y, sint16 z)
{
    LocationXY16 rotatedCoords = { 0, 0 };

    switch (get_current_rotation())
    {
    case 0:
        rotatedCoords.x = y - x;
        rotatedCoords.y = ((y + x) / 2) - z;
        break;
    case 1:
        rotatedCoords.x = -x - y;
        rotatedCoords.y = ((y - x) / 2) - z;
        break;
    case 2:
        rotatedCoords.x = x - y;
        rotatedCoords.y = ((-y - x) / 2) - z;
        break;
    case 3:
        rotatedCoords.x = y + x;
        rotatedCoords.y = ((x - y) / 2) - z;
        break;
    }

    return rotatedCoords;
}

// Normally, a station has at most one entrance and one exit, which are at the same height
// as the station. But in hacked parks, neither can be taken for granted. This code ensures
// that the ride->entrances and ride->exits arrays will point to one of them. There is
// an ever-so-slight chance two entrances/exits for the same station reside on the same tile.
// In cases like this, the one at station height will be considered the "true" one.
// If none exists at that height, newer and higher placed ones take precedence.
void determine_ride_entrance_and_exit_locations()
{
    log_verbose("Inspecting ride entrance / exit locations");

    sint32 rideIndex;
    Ride * ride;
    FOR_ALL_RIDES(rideIndex, ride)
    {
        for (sint32 stationIndex = 0; stationIndex < MAX_STATIONS; stationIndex++)
        {
            TileCoordsXYZD          entranceLoc        = ride->entrances[stationIndex];
            TileCoordsXYZD          exitLoc            = ride->exits[stationIndex];
            bool                    fixEntrance        = false;
            bool                    fixExit            = false;
            const rct_tile_element * tileElement;

            // Skip if the station has no entrance
            if (!entranceLoc.isNull())
            {
                tileElement = map_get_ride_entrance_element_at(entranceLoc.x * 32, entranceLoc.y * 32, entranceLoc.z, false);

                if (tileElement == nullptr || tileElement->properties.entrance.ride_index != rideIndex || tile_element_get_station(tileElement) != stationIndex)
                {
                    fixEntrance = true;
                }
                else
                {
                    ride->entrances[stationIndex].direction = (uint8)tile_element_get_direction(tileElement);
                }
            }

            if (!exitLoc.isNull())
            {
                tileElement = map_get_ride_exit_element_at(exitLoc.x * 32, exitLoc.y * 32, entranceLoc.z, false);

                if (tileElement == nullptr || tileElement->properties.entrance.ride_index != rideIndex || tile_element_get_station(tileElement) != stationIndex)
                {
                    fixExit = true;
                }
                else
                {
                    ride->exits[stationIndex].direction = (uint8)tile_element_get_direction(tileElement);
                }
            }

            if (!fixEntrance && !fixExit)
            {
                continue;
            }

            // At this point, we know we have a disconnected entrance or exit.
            // Search the map to find it. Skip the outer ring of invisible tiles.
            bool alreadyFoundEntrance = false;
            bool alreadyFoundExit     = false;
            for (uint8 x = 1; x < MAXIMUM_MAP_SIZE_TECHNICAL - 1; x++)
            {
                for (uint8 y = 1; y < MAXIMUM_MAP_SIZE_TECHNICAL - 1; y++)
                {
                    tileElement = map_get_first_element_at(x, y);

                    if (tileElement != nullptr)
                    {
                        do
                        {
                            if (tile_element_get_type(tileElement) != TILE_ELEMENT_TYPE_ENTRANCE)
                            {
                                continue;
                            }
                            if (tileElement->properties.entrance.ride_index != rideIndex)
                            {
                                continue;
                            }
                            if (tile_element_get_station(tileElement) != stationIndex)
                            {
                                continue;
                            }

                            // The expected height is where entrances and exit reside in non-hacked parks.
                            const uint8 expectedHeight = ride->station_heights[stationIndex];

                            if (fixEntrance && tileElement->properties.entrance.type == ENTRANCE_TYPE_RIDE_ENTRANCE)
                            {
                                if (alreadyFoundEntrance)
                                {
                                    if (ride->entrances[stationIndex].z == expectedHeight)
                                        continue;
                                    if (ride->entrances[stationIndex].z > tileElement->base_height)
                                        continue;
                                }

                                // Found our entrance
                                ride_set_entrance_location(ride, stationIndex, { x, y, tileElement->base_height, (uint8)tile_element_get_direction(tileElement) });
                                alreadyFoundEntrance = true;

                                log_verbose("Fixed disconnected entrance of ride %d, station %d to x = %d, y = %d and z = %d.", rideIndex, stationIndex, x, y, tileElement->base_height);
                            }
                            else if (fixExit && tileElement->properties.entrance.type == ENTRANCE_TYPE_RIDE_EXIT)
                            {
                                if (alreadyFoundExit)
                                {
                                    if (ride->exits[stationIndex].z == expectedHeight)
                                        continue;
                                    if (ride->exits[stationIndex].z > tileElement->base_height)
                                        continue;
                                }

                                // Found our exit
                                ride_set_exit_location(ride, stationIndex, { x, y, tileElement->base_height, (uint8)tile_element_get_direction(tileElement) });
                                alreadyFoundExit = true;

                                log_verbose("Fixed disconnected exit of ride %d, station %d to x = %d, y = %d and z = %d.", rideIndex, stationIndex, x, y, tileElement->base_height);
                            }
                        }
                        while (!tile_element_is_last_for_tile(tileElement++));
                    }
                }
            }

            if (fixEntrance && !alreadyFoundEntrance)
            {
                ride_clear_entrance_location(ride, stationIndex);
                log_verbose("Cleared disconnected entrance of ride %d, station %d.", rideIndex, stationIndex);
            }
            if (fixExit && !alreadyFoundExit)
            {
                ride_clear_exit_location(ride, stationIndex);
                log_verbose("Cleared disconnected exit of ride %d, station %d.", rideIndex, stationIndex);
            }
        }
    }
}
