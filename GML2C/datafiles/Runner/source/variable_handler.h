#include <vector>
#include <algorithm>
#include <variant>
#include <cstdint>

#pragma once

struct GMvar{
    std::variant<const char*, bool, float, int> value;

    //type init
    GMvar() : value(0) {};
    GMvar(int i) :value(i) {};
    GMvar(float f) : value(f) {};
    GMvar(bool b) : value(b) {};
    GMvar(const char* s) : value(s) {};
    GMvar(const GMvar& other) : value(other.value) {};

    #pragma region //converting variable types
    //convert to float
    operator float () const{
        return std::visit([](auto && val) -> float {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, int>)
                return static_cast<float>(val);
            else if constexpr (std::is_same_v<T, bool>){
                if (val)
                    return 1.0f;
                else
                    return 0.0f;
            }
            else if constexpr (std::is_same_v<T, const char*>)
                return 0.0f;
            else
                return val;
        }, value);
    }

    //convert to int
    operator int() const{
        return (int)(float)*this;
    }

    //convert to bool
    operator bool() const{
        return std::visit([](auto&& val) -> bool {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, bool>)
                return val;
            if constexpr (std::is_same_v<T, int>)
                return val != 0;
            if constexpr (std::is_same_v<T, float>)
                return val != 0.0f;
            if constexpr (std::is_same_v<T, const char*>)
                return val != nullptr;
        }, value);
    }

    //convert to const char*
    operator const char*() const {
        if (auto* s = std::get_if<const char*>(&value)) 
            return *s;

        return nullptr;
    }
    #pragma endregion

    //writing to the var
    GMvar& operator= (int i) { value = i; return *this; }
    GMvar& operator= (float f) { value = f; return *this; }
    GMvar& operator= (bool b) { value = b; return *this; }
    GMvar& operator= (const char* s) { value = s; return *this; }
    GMvar& operator= (const GMvar& other) { value = other.value; return *this; }

    //adding, subtracting, multiplying and dividing
    GMvar operator+ (float o) const { return GMvar{(float)*this + o}; }
    GMvar operator- (float o) const { return GMvar{(float)*this - o}; }
    GMvar operator* (float o) const { return GMvar{(float)*this * o}; }
    GMvar operator/ (float o) const { return GMvar{(float)*this / o}; }
    GMvar& operator+= (float o) { value = (float)*this + o; return *this; }
    GMvar& operator-= (float o) { value = (float)*this - o; return *this; }
    GMvar& operator*= (float o) { value = (float)*this * o; return *this; }
    GMvar& operator/= (float o) { value = (float)*this / o; return *this; }

    //checking the variable
    bool operator== (float o) const { return (float)*this == o; }
    bool operator!= (float o) const { return (float)*this != o; }
    bool operator< (float o) const { return (float)*this < o; }
    bool operator> (float o) const { return (float)*this > o; }
    bool operator<= (float o) const { return (float)*this <= o; }
    bool operator>= (float o) const { return (float)*this >= o; }
};

struct VarNode{
    uint16_t vId;
    GMvar value;
};

struct Object{
    std::vector<VarNode> vars;

    GMvar& GetVar(uint16_t index){

        // find var in sorted vector using binary search
        auto it = std::lower_bound(vars.begin(), vars.end(), index, [](const VarNode& node, uint16_t id){
            return node.vId < id;
        });

        // var already exists
        if (it != vars.end() && it->vId == index)
            return it->value;

        // create var while keeping vector sorted
        it = vars.insert(it, {index, GMvar()});

        return it->value;
    }
};

inline VarNode globVar_x = {0, 0};
#define varId_x globVar_x.vId
inline VarNode globVar_y = {1, 0};
#define varId_y globVar_y.vId
inline VarNode globVar_sprite_index = {2, -1};
#define varId_sprite_index globVar_sprite_index.vId
inline VarNode globVar_image_xscale = {3, 1};
#define varId_image_xscale globVar_image_xscale.vId
inline VarNode globVar_image_yscale = {4, 1};
#define varId_image_yscale globVar_image_yscale.vId
inline VarNode globVar_id = {5, -4};
#define varId_id globVar_id.vId
inline VarNode globVar_visible = {6, false};
#define varId_visible globVar_visible.vId
inline VarNode globVar_solid = {7, false};
#define varId_solid globVar_solid.vId
inline VarNode globVar_persistent = {8, false};
#define varId_persistent globVar_persistent.vId
inline VarNode globVar_depth = {9, 0};
#define varId_depth globVar_depth.vId
inline VarNode globVar_layer = {10, -4};
#define varId_layer globVar_layer.vId
inline VarNode globVar_on_ui_layer = {11, false};
#define varId_on_ui_layer globVar_on_ui_layer.vId
inline VarNode globVar_collision_space = {12, -4};
#define varId_collision_space globVar_collision_space.vId
inline VarNode globVar_direction = {13, 0};
#define varId_direction globVar_direction.vId
inline VarNode globVar_friction = {14, 0};
#define varId_friction globVar_friction.vId
inline VarNode globVar_gravity = {15, 0};
#define varId_gravity globVar_gravity.vId
inline VarNode globVar_gravity_direction = {16, 0};
#define varId_gravity_direction globVar_gravity_direction.vId
inline VarNode globVar_hspeed = {17, 0};
#define varId_hspeed globVar_hspeed.vId
inline VarNode globVar_vspeed = {18, 0};
#define varId_vspeed globVar_vspeed.vId
inline VarNode globVar_speed = {19, 0};
#define varId_speed globVar_speed.vId
inline VarNode globVar_xstart = {20, 0};
#define varId_xstart globVar_xstart.vId
inline VarNode globVar_ystart = {21, 0};
#define varId_ystart globVar_ystart.vId
inline VarNode globVar_xprevious = {22, 0};
#define varId_xprevious globVar_xprevious.vId
inline VarNode globVar_yprevious = {23, 0};
#define varId_yprevious globVar_yprevious.vId
inline VarNode globVar_object_index = {24, 0};
#define varId_object_index globVar_object_index.vId
inline VarNode globVar_sprite_width = {25, 0};
#define varId_sprite_width globVar_sprite_width.vId
inline VarNode globVar_sprite_height = {26, 0};
#define varId_sprite_height globVar_sprite_height.vId
inline VarNode globVar_sprite_xoffset = {27, 0};
#define varId_sprite_xoffset globVar_sprite_xoffset.vId
inline VarNode globVar_sprite_yoffset = {28, 0};
#define varId_sprite_yoffset globVar_sprite_yoffset.vId
inline VarNode globVar_image_alpha = {29, 0};
#define varId_image_alpha globVar_image_alpha.vId
inline VarNode globVar_image_angle = {30, 0};
#define varId_image_angle globVar_image_angle.vId
inline VarNode globVar_image_blend = {31, 0};
#define varId_image_blend globVar_image_blend.vId
inline VarNode globVar_image_index = {32, 0};
#define varId_image_index globVar_image_index.vId
inline VarNode globVar_image_number = {33, 0};
#define varId_image_number globVar_image_number.vId
inline VarNode globVar_image_speed = {34, 0};
#define varId_image_speed globVar_image_speed.vId
