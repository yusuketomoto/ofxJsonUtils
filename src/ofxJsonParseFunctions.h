//
//  ofxJsonParseFunctions.h
//
//  Created by ISHII 2bit on 2016/05/19.
//
//

#pragma once

#include "ofMain.h"
#include "json_compatible.h"

namespace ofx {
    namespace JsonUtils {
        namespace detail {
            template <typename T>
            class has_loadJson {
            private:
                template <typename U, U> struct really_has;
                template <typename C>
                static std::true_type Test(really_has<void (C::*)(const ofJson &), &C::loadJson> *);
                template <typename>
                static std::false_type Test(...);
                
            public:
                static bool const value = decltype(Test<T>(0))::value;
            };
            
            static bool has_keys(const ofJson &json, const std::vector<std::string> &keys) {
                auto end = json.end();
                bool all = true;
                for(const auto &key : keys) {
                    all &= (json.find(key) != end);
                }
                return all;
            }
        };
        
        static constexpr char const skip_invalid_format[] = "skip: invalid format.";
        static constexpr char const skip_json_isnt_string[] = "skip: json isn't string.";
        static constexpr char const skip_json_isnt_boolean[] = "skip: json isn't boolean.";
        static constexpr char const skip_json_isnt_number[] = "skip: json isn't number.";
        static constexpr char const skip_json_isnt_array[] = "skip: json isn't array.";
        static constexpr char const skip_json_isnt_object[] = "skip: json isn't object.";
        static constexpr char const skip_json_isnt_object_or_array[] = "skip: json isn't object or array.";
        
        inline void parse(const ofJson &json, std::string &value) {
            if(!json.is_string()) return ofLogVerbose("ofxJsonUtils::parse string") << skip_json_isnt_string;
            value = json;
        }
        
        inline void parse(const ofJson &json, bool &value) {
            if(!json.is_boolean()) return ofLogVerbose("ofxJsonUtils::parse boolean") << skip_json_isnt_boolean;
            value = json;
        }
        
        template <typename T>
        auto parse(const ofJson &json, T &value)
        -> typename std::enable_if<std::is_arithmetic<T>::value>::type {
            if(!json.is_number()) return ofLogVerbose("ofxJsonUtils::parse number") << skip_json_isnt_number;
            value = json;
        }
        
        template <typename T>
        auto parse(const ofJson &json, T &value)
        -> typename std::enable_if<detail::has_loadJson<T>::value>::type {
            value.loadJson(json);
        }
        
        static void parse(const ofJson &json, ofVec2f &v) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"x", "y"})) return ofLogVerbose("ofxJsonUtils::parse ofVec2f") << skip_invalid_format;
                v.set(json["x"], json["y"]);
            } else if(json.is_array()) {
                if(json.size() < 2) return ofLogVerbose("ofxJsonUtils::parse ofVec2f") << skip_invalid_format;
                v.set(json[0], json[1]);
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofVec2f") << skip_json_isnt_object_or_array;
            }
        }
        
        static void parse(const ofJson &json, ofVec3f &v) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"x", "y", "z"})) return ofLogVerbose("ofxJsonUtils::parse ofVec3f") << skip_invalid_format;
                v.set(json["x"], json["y"], json["z"]);
            } else if(json.is_array()) {
                if(json.size() < 3) return ofLogVerbose("ofxJsonUtils::parse ofVec3f") << skip_invalid_format;
                v.set(json[0], json[1], json[2]);
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofVec3f") << skip_json_isnt_object_or_array;
            }
        }
        
        static void parse(const ofJson &json, ofVec4f &v) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"x", "y", "z", "w"})) return ofLogVerbose("ofxJsonUtils::parse ofVec4f") << skip_invalid_format;
                v.set(json["x"], json["y"], json["z"], json["w"]);
            } else if(json.is_array()) {
                if(json.size() < 3) return ofLogVerbose("ofxJsonUtils::parse ofVec3f") << skip_invalid_format;
                v.set(json[0], json[1], json[2], json[3]);
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofVec4f") << skip_json_isnt_object_or_array;
            }
        }
    
        static void parse(const ofJson &json, ofRectangle &rect) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"x", "y", "width", "height"})) return ofLogVerbose("ofxJsonUtils::parse ofRectangle") << skip_invalid_format;
                rect.set(json["x"], json["y"], json["width"], json["height"]);
            } else if(json.is_array()) {
                if(json.size() < 4) return ofLogVerbose("ofxJsonUtils::parse ofRectangle") << skip_invalid_format;
                rect.set(json[0], json[1], json[2], json[3]);
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofRectangle") << skip_json_isnt_object_or_array;
            }
        }
        
        static void parse(const ofJson &json, ofMatrix4x4 &mat) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"value0", "value1", "value2", "value3"})) return
                    ofLogVerbose("ofxJsonUtils::parse ofMatrix4x4") << skip_invalid_format;
                parse(json["value0"].get<ofJson::object_t>(), mat._mat[0]);
                parse(json["value1"].get<ofJson::object_t>(), mat._mat[1]);
                parse(json["value2"].get<ofJson::object_t>(), mat._mat[2]);
                parse(json["value3"].get<ofJson::object_t>(), mat._mat[3]);
            } else if(json.is_array()) {
                if(json.size() < 16) return ofLogVerbose("ofxJsonUtils::parse ofMatrix4x4") << skip_invalid_format;
                mat.set(json[0], json[1], json[2], json[3], json[4], json[5], json[6], json[7], json[8], json[9], json[10], json[11], json[12], json[13], json[14], json[15]);
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofMatrix4x4") << skip_json_isnt_object_or_array;
            }
        }
        
        template <typename PixelType>
        void parse(const ofJson &json, ofColor_<PixelType> &c) {
            if(json.is_object()) {
                auto end = json.end();
                if(!detail::has_keys(json, {"r", "g", "b"})) return ofLogVerbose("ofxJsonUtils::parse ofColor") << skip_invalid_format;
                c.set(json["r"], json["g"], json["b"]);
                if(json.find("a") != json.end()) c.a = json["a"];
            } else if(json.is_array()) {
                if(json.size() < 3) return ofLogVerbose("ofxJsonUtils::parse ofColor") << skip_invalid_format;
                c.set(json[0], json[1], json[2]);
                if(3 < json.size()) c.a = json[3];
            } else {
                ofLogVerbose("ofxJsonUtils::parse ofColor") << skip_json_isnt_object_or_array;
            }
        }
        
        template <typename T>
        void parse(const ofJson &json, std::vector<T> &vec) {
            if(!json.is_array()) {
                return ofLogVerbose("ofxJsonUtils::parse vector") << skip_json_isnt_array;
            }
            vec.resize(json.size());
            for(std::size_t i = 0; i < json.size(); ++i) {
                parse(json[i], vec[i]);
            }
        }
        
        template <typename T>
        void parse(const ofJson &json, std::deque<T> &vec) {
            if(!json.is_array()) {
                return ofLogVerbose("ofxJsonUtils::parse deque") << skip_json_isnt_array;
            }
            vec.resize(json.size());
            for(std::size_t i = 0; i < json.size(); ++i) {
                parse(json[i], vec[i]);
            }
        }
        
        template <typename T, std::size_t size>
        void parse(const ofJson &json, std::array<T, size> &arr) {
            if(!json.is_array()) {
                return ofLogVerbose("ofxJsonUtils::parse array") << skip_json_isnt_array;
            }
            for(std::size_t i = 0; i < json.size(); ++i) {
                if(size <= i) break;
                parse(json[i], arr[i]);
            }
        }

        template <typename T>
        void parse(const ofJson &json, std::map<std::string, T> &table) {
            if(!json.is_object()) {
                return ofLogVerbose("ofxJsonUtils::parse map") << skip_json_isnt_object;
            }
            table.clear();
            for(ofJson::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
                T t;
                parse(it.value(), t);
                table.emplace(it.key(), std::move(t));
            }
        }
    };
};