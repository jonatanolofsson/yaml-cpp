#ifndef NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)) // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/null.h"
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <vector>

namespace YAML
{
    namespace conversion {
        inline bool IsInfinity(const std::string& input) {
            return input == ".inf" || input == ".Inf" || input == ".INF" || input == "+.inf" || input == "+.Inf" || input == "+.INF";
        }
        
        inline bool IsNegativeInfinity(const std::string& input) {
            return input == "-.inf" || input == "-.Inf" || input == "-.INF";
        }
        
        inline bool IsNaN(const std::string& input) {
            return input == ".nan" || input == ".NaN" || input == ".NAN";
        }
    }
    
	// std::string
	template<>
	struct convert<std::string> {
		static Node encode(const std::string& rhs) {
			return Node(rhs);
		}
		
		static bool decode(const Node& node, std::string& rhs) {
			if(!node.IsScalar())
				return false;
			rhs = node.Scalar();
			return true;
		}
	};
	
	template<>
	struct convert<_Null> {
		static Node encode(const _Null& /* rhs */) {
			return Node();
		}
		
		static bool decode(const Node& node, _Null& /* rhs */) {
			return node.IsNull();
		}
	};
	
#define YAML_DEFINE_CONVERT_STREAMABLE(type)\
	template<>\
	struct convert<type> {\
		static Node encode(const type& rhs) {\
			std::stringstream stream;\
			stream << rhs;\
			return Node(stream.str());\
		}\
		\
		static bool decode(const Node& node, type& rhs) {\
			if(node.Type() != NodeType::Scalar)\
				return false;\
            const std::string& input = node.Scalar();\
			std::stringstream stream(input);\
            stream.unsetf(std::ios::dec);\
			if((stream >> rhs) && (stream >> std::ws).eof())\
                return true;\
            if(std::numeric_limits<type>::has_infinity) {\
                if(conversion::IsInfinity(input)) {\
                    rhs = std::numeric_limits<type>::infinity();\
                    return true;\
                } else if(conversion::IsNegativeInfinity(input)) {\
                    rhs = -std::numeric_limits<type>::infinity();\
                    return true;\
                }\
            }\
            \
            if(std::numeric_limits<type>::has_quiet_NaN && conversion::IsNaN(input)) {\
                rhs = std::numeric_limits<type>::quiet_NaN();\
                return true;\
            }\
            \
            return false;\
		}\
	}
	
	YAML_DEFINE_CONVERT_STREAMABLE(int);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned);
	YAML_DEFINE_CONVERT_STREAMABLE(short);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned short);
	YAML_DEFINE_CONVERT_STREAMABLE(long);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned long);
	YAML_DEFINE_CONVERT_STREAMABLE(long long);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned long long);
	
	YAML_DEFINE_CONVERT_STREAMABLE(char);
	YAML_DEFINE_CONVERT_STREAMABLE(unsigned char);
	
	YAML_DEFINE_CONVERT_STREAMABLE(float);
	YAML_DEFINE_CONVERT_STREAMABLE(double);
	YAML_DEFINE_CONVERT_STREAMABLE(long double);
	
#undef YAML_DEFINE_CONVERT_STREAMABLE
	
	// bool
	template<>
	struct convert<bool> {
		static Node encode(bool rhs) {
			return rhs ? Node("true") : Node("false");
		}
		
		static bool decode(const Node& node, bool& rhs);
	};

	// std::map
	template<typename K, typename V>
	struct convert<std::map<K, V> > {
		static Node encode(const std::map<K, V>& rhs) {
			Node node(NodeType::Map);
			for(typename std::map<K, V>::const_iterator it=rhs.begin();it!=rhs.end();++it)
				node[it->first] = it->second;
			return node;
		}
		
		static bool decode(const Node& node, std::map<K, V>& rhs) {
			if(!node.IsMap())
				return false;

			rhs.clear();
			for(const_iterator it=node.begin();it!=node.end();++it)
				rhs[it->first.as<K>()] = it->second.as<V>();
			return true;
		}
	};
	
	// std::vector
	template<typename T>
	struct convert<std::vector<T> > {
		static Node encode(const std::vector<T>& rhs) {
			Node node(NodeType::Sequence);
			for(typename std::vector<T>::const_iterator it=rhs.begin();it!=rhs.end();++it)
				node.append(*it);
			return node;
		}
		
		static bool decode(const Node& node, std::vector<T>& rhs) {
			if(!node.IsSequence())
				return false;
			
			rhs.clear();
			for(const_iterator it=node.begin();it!=node.end();++it)
				rhs.push_back(it->as<T>());
			return true;
		}
	};
	
	// std::list
	template<typename T>
	struct convert<std::list<T> > {
		static Node encode(const std::list<T>& rhs) {
			Node node(NodeType::Sequence);
			for(typename std::list<T>::const_iterator it=rhs.begin();it!=rhs.end();++it)
				node.append(*it);
			return node;
		}
		
		static bool decode(const Node& node, std::list<T>& rhs) {
			if(!node.IsSequence())
				return false;
			
			rhs.clear();
			for(const_iterator it=node.begin();it!=node.end();++it)
				rhs.push_back(it->as<T>());
			return true;
		}
	};
}

#endif // NODE_CONVERT_H_62B23520_7C8E_11DE_8A39_0800200C9A66
