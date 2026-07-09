#include <vector>
#include <algorithm>
#include <variant>
#include <cstdint>
#include <memory>

#pragma once

struct GMvar{
    std::variant<const char*, bool, float, int, std::shared_ptr<std::vector<GMvar>>> value;

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
            else if constexpr (std::is_same_v<T, bool>)
                return val ? 1.0f : 0.0f;
            else if constexpr (std::is_same_v<T, const char*>)
                return 0.0f;
            else if constexpr (std::is_same_v<T, std::shared_ptr<std::vector<GMvar>>>)
                return 0.0f; // arrays don't convert to a number
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
            else if constexpr (std::is_same_v<T, int>)
                return val != 0;
            else if constexpr (std::is_same_v<T, float>)
                return val != 0.0f;
            else if constexpr (std::is_same_v<T, const char*>)
                return val != nullptr;
            else // array
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
    // +
    GMvar operator+ (float o) const { return GMvar{(float)*this + o}; } 
    GMvar operator+(const GMvar& o) const { return GMvar {(float)*this + (float)o}; }

    // -
    GMvar operator- (float o) const { return GMvar{(float)*this - o}; } 
    GMvar operator-(const GMvar& o) const { return GMvar {(float)*this - (float)o}; }

    // *
    GMvar operator* (float o) const { return GMvar{(float)*this * o}; } 
    GMvar operator*(const GMvar& o) const { return GMvar {(float)*this * (float)o}; }

    // /
    GMvar operator/ (float o) const { return GMvar{(float)*this / o}; } 
    GMvar operator/(const GMvar& o) const { return GMvar {(float)*this / (float)o}; }

    GMvar operator++ (int) { GMvar tmp(*this); value = (float)*this + 1; return tmp; } // ++
    GMvar operator-- (int) { GMvar tmp(*this); value = (float)*this - 1; return tmp; } // --
    GMvar& operator+= (float o) { value = (float)*this + o; return *this; } // +=
    GMvar& operator-= (float o) { value = (float)*this - o; return *this; } // -=
    GMvar& operator*= (float o) { value = (float)*this * o; return *this; } // *=
    GMvar& operator/= (float o) { value = (float)*this / o; return *this; } // /=

    //checking the variable
    bool operator== (float o) const { return (float)*this == o; }
    bool operator!= (float o) const { return (float)*this != o; }
    bool operator< (float o) const { return (float)*this < o; }
    bool operator> (float o) const { return (float)*this > o; }
    bool operator<= (float o) const { return (float)*this <= o; }
    bool operator>= (float o) const { return (float)*this >= o; }

    //array access — lazily promotes scalar -> array on first write, like GML
    GMvar& operator[](int i) {
        if (!std::holds_alternative<std::shared_ptr<std::vector<GMvar>>>(value))
            value = std::make_shared<std::vector<GMvar>>();

        auto& arr = *std::get<std::shared_ptr<std::vector<GMvar>>>(value);
        if ((int)arr.size() <= i) arr.resize(i + 1);
        return arr[i];
    }

    const GMvar& operator[](int i) const {
        static const GMvar undefined;
        if (!std::holds_alternative<std::shared_ptr<std::vector<GMvar>>>(value))
            return undefined;

        auto& arr = *std::get<std::shared_ptr<std::vector<GMvar>>>(value);
        if (i < 0 || i >= (int)arr.size()) return undefined;
        return arr[i];
    }
};

inline GMvar operator+ (float o, const GMvar& v) { return v + o; }
inline GMvar operator- (float o, const GMvar& v) { return GMvar{o - (float)v}; }
inline GMvar operator* (float o, const GMvar& v) { return v * o; }
inline GMvar operator/ (float o, const GMvar& v) { return GMvar{o / (float)v}; }

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

#define var GMvar
