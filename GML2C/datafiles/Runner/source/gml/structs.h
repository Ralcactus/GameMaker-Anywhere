#include <stdio.h>
#include <string.h>

enum CollisionTypes{
    AUTOMATIC,
    FULL_IMAGE,
    MANUAL
};

enum BBoxTypes{
    RECTANGLE,
    RECTANGLE_WITH_ROTATION,
    ELLIPSE,
    PRECISE,
    PRECISE_BY_FRAME,
    SPINE_COLLISION_MESH
};

typedef struct{
    const char* frames;
} Frames;

typedef struct{
    const char *location;

    //General metadata
    int width, height,
        originX, originY,
        frameSpd; //0 -> Frames per second, 1 -> Frames per game frame
    
    float fps;

    //Collision Mask
    int maskLeft,
    maskTop,
    maskRight,
    maskBottom,
    type,
    bboxType;
    
    Frames* frames;
} GMSprite;

typedef struct{
    GMSprite* sprite;
    bool persistent, solid, visible;
} Object;

enum LayerType {
    LAYER_BACKGROUND = 0,
    LAYER_INSTANCE = 1,
    LAYER_TILE = 2,
    LAYER_PATH = 3,
    LAYER_ASSET = 4,
    LAYER_EFFECTS = 5,
    LAYER_UI = 6
};

typedef struct{
    float x, y, rotation;
    float scaleX, scaleY;
    Object* object;
} LayerInstances;

typedef struct{
    float x, y, rotation;
    float scaleX, scaleY;
    GMSprite* sprite;
} LayerAssets;

typedef struct{

    //Properties
    GMSprite* sprite;
    int color;
    bool hTile, vTile, stretch;
    int offX, offY;
    float spdX, spdY;
    float animSpd;
    int timeUnits;

    //Filters & Effects
    int effectType, tintCol;
    
} GMLayerBackground;

typedef struct {
    LayerInstances* instances;
    int instanceCount;
} GMLayerInstance;

typedef struct {
    LayerAssets* assets;
    int assetCount;
} GMLayerAsset;

typedef struct{
    int type, depth;
    const void* data;
} GMLayer;

typedef struct{
    int camXPos, camYPos,
    camWidth, camHeight,
    viewXPos, viewYPos,
    viewWidth, viewHeight,
    objFHBorder, objFVBorder,
    hSpd, vSpd;

    bool inherit, visible;

    Object* object;
} GMViewPorts;

typedef struct{
    
    //Properties
    int id;
    const char* name;
    int width, height;
    bool persistent, inheritRmSettings;

    //Viewports and Camera
    bool clearDisplayBuffer,
    clearViewBackground,
    enableViews,
    inheritViewSettings;
    GMViewPorts views[7];
    int viewCount;

    //Physics
    bool inheritPhySettings, phyEnabled;
    float gravX, gravY;
    float pix2met;
    
    //Layers
    GMLayer* layers;
    size_t layerCount;
} GMRoom;